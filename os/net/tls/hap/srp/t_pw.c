/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/*
 * Copyright (c) 1997-2007  The Stanford SRP Authentication Project
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT SHALL STANFORD BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Redistributions in source or binary form must retain an intact copy
 * of this copyright notice.
 */

#include <tls/hap/t_defines.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif							/* HAVE_UNISTD_H */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef USE_HOMEDIR
#include <pwd.h>
#endif
#ifdef WIN32
#include <io.h>
#endif

#include <tls/hap/t_pwd.h>
#include <tls/hap/t_read.h>
#include <tls/hap/t_sha.h>
#include <tls/hap/srp_aux.h>

static struct t_pw *syspw = NULL;
static struct t_passwd tpass;

_TYPE(struct t_pw *) t_newpw()
{
	struct t_pw *tpw;
	if ((tpw = malloc(sizeof(struct t_pw))) == NULL) {
		return NULL;
	}
	tpw->instream = NULL;
	tpw->close_on_exit = 0;
	tpw->state = FILE_ONLY;
	tpw->pwbuf = cstr_new();

	return tpw;
}

/* t_openpw(NULL) is deprecated - use settpent()/gettpnam() instead */

_TYPE(struct t_pw *)
t_openpw(fp)
FILE *fp;
{
	struct t_pw *tpw;
	char close_flag = 0;

	if (fp == NULL) {			/* Deprecated */
		if ((fp = fopen(DEFAULT_PASSWD, "r")) == NULL) {
			return NULL;
		}
		close_flag = 1;
	} else {
		close_flag = 0;
	}

	if ((tpw = malloc(sizeof(struct t_pw))) == NULL) {
		return NULL;
	}
	tpw->instream = fp;
	tpw->close_on_exit = close_flag;
	tpw->state = FILE_ONLY;
	tpw->pwbuf = cstr_new();

	return tpw;
}

_TYPE(struct t_pw *)
t_openpwbyname(pwname)
const char *pwname;
{
	FILE *fp;
	struct t_pw *t;

	if (pwname == NULL) {		/* Deprecated */
		return t_openpw(NULL);
	}

	if ((fp = fopen(pwname, "r")) == NULL) {
		return NULL;
	}

	t = t_openpw(fp);
	t->close_on_exit = 1;
	t->pwbuf = cstr_new();
	return t;
}

_TYPE(void)
t_closepw(tpw)
struct t_pw *tpw;
{
	if (tpw->close_on_exit) {
		fclose(tpw->instream);
	}
	if (tpw->pwbuf) {
		cstr_clear_free(tpw->pwbuf);
	}
	free(tpw);
}

_TYPE(void) t_rewindpw(tpw)
struct t_pw *tpw;
{
#ifdef ENABLE_YP
	if (tpw->state == IN_NIS) {
		tpw->state = FILE_NIS;
	}
#endif
	rewind(tpw->instream);
}

#ifdef ENABLE_YP
static void savepwent(tpw, pwent)
struct t_pw *tpw;
struct t_pwent *pwent;
{
	tpw->pebuf.name = tpw->userbuf;
	tpw->pebuf.salt.data = tpw->saltbuf;
	strcpy(tpw->pebuf.name, pwent->name);
	cstr_setn(tpw->pwbuf, pwent->password.data, pwent->password.len);
	tpw->pebuf.password.data = tpw->pwbuf->data;
	tpw->pebuf.password.len = tpw->pwbuf->length;
	tpw->pebuf.salt.len = pwent->salt.len;
	memcpy(tpw->pebuf.salt.data, pwent->salt.data, pwent->salt.len);
	tpw->pebuf.index = pwent->index;
}
#endif							/* ENABLE_YP */

_TYPE(struct t_pwent *) t_getpwent(tpw)
struct t_pw *tpw;
{
	char indexbuf[16];
	cstr *passbuf = NULL;
	char saltstr[MAXB64SALTLEN];

#ifdef ENABLE_YP
	struct t_passwd *nisent;
	/* FIXME: should tell caller to get conf entry from NIS also */

	if (tpw->state == IN_NIS) {
		nisent = _yp_gettpent();
		if (nisent != NULL) {
			savepwent(tpw, &nisent->tp);
			return &tpw->pebuf;
		}
		tpw->state = FILE_NIS;
	}
#endif

	passbuf = cstr_new();

	while (1) {
		if (t_nextfield(tpw->instream, tpw->userbuf, MAXUSERLEN) > 0) {
#ifdef ENABLE_YP
			if (tpw->state == FILE_NIS && *tpw->userbuf == '+') {
				t_nextline(tpw->instream);
				if (strlen(tpw->userbuf) > 1) {	/* +name:... */
					nisent = _yp_gettpnam(tpw->userbuf + 1);
					if (nisent != NULL) {
						savepwent(tpw, nisent);
						cstr_clear_free(passbuf);
						return &tpw->pebuf;
					}
				} else {		/* +:... */
					tpw->state = IN_NIS;
					_yp_settpent();
					cstr_clear_free(passbuf);
					return t_getpwent(tpw);
				}
			}
#endif
			if (t_nextcstrfield(tpw->instream, passbuf) > 0 && (tpw->pebuf.password.len = t_cstrfromb64(tpw->pwbuf, passbuf->data)) > 0 && t_nextfield(tpw->instream, saltstr, MAXB64SALTLEN) > 0 && (tpw->pebuf.salt.len = t_fromb64((char *)(tpw->saltbuf), saltstr)) > 0 && t_nextfield(tpw->instream, indexbuf, 16) > 0 && (tpw->pebuf.index = atoi(indexbuf)) > 0) {
				tpw->pebuf.name = tpw->userbuf;
				tpw->pebuf.password.data = (unsigned char *)(tpw->pwbuf->data);
				tpw->pebuf.salt.data = tpw->saltbuf;
				t_nextline(tpw->instream);
				cstr_clear_free(passbuf);
				return &tpw->pebuf;
			}
		}
		if (t_nextline(tpw->instream) < 0) {
			cstr_clear_free(passbuf);
			return NULL;
		}
	}
}

_TYPE(struct t_pwent *)
t_getpwbyname(tpw, user)
struct t_pw *tpw;
const char *user;
{
	char indexbuf[16];
	cstr *passbuf = NULL;
	char saltstr[MAXB64SALTLEN];
	char username[MAXUSERLEN];
#ifdef ENABLE_YP
	struct t_passwd *nisent;
#endif

	t_rewindpw(tpw);
	passbuf = cstr_new();

	while (t_nextfield(tpw->instream, username, MAXUSERLEN) > 0) {
#ifdef ENABLE_YP
		if (tpw->state == FILE_NIS && *username == '+') {
			if (strlen(username) == 1 || strcmp(user, username + 1) == 0) {
				nisent = _yp_gettpnam(user);	/* Entry is +username or + */
				if (nisent != NULL) {
					savepwent(tpw, &nisent->tp);
					cstr_clear_free(passbuf);
					return &tpw->pebuf;
				}
			}
		}
#endif
		if (strcmp(user, username) == 0)
			if (t_nextcstrfield(tpw->instream, passbuf) > 0 && (tpw->pebuf.password.len = t_cstrfromb64(tpw->pwbuf, passbuf->data)) > 0 && t_nextfield(tpw->instream, saltstr, MAXB64SALTLEN) > 0 && (tpw->pebuf.salt.len = t_fromb64((char *)(tpw->saltbuf), saltstr)) > 0 && t_nextfield(tpw->instream, indexbuf, 16) > 0 && (tpw->pebuf.index = atoi(indexbuf)) > 0) {
				strcpy(tpw->userbuf, username);
				tpw->pebuf.name = tpw->userbuf;
				tpw->pebuf.password.data = (unsigned char *)(tpw->pwbuf->data);
				tpw->pebuf.salt.data = tpw->saltbuf;
				t_nextline(tpw->instream);
				cstr_clear_free(passbuf);
				return &tpw->pebuf;
			}
		if (t_nextline(tpw->instream) < 0) {
			cstr_clear_free(passbuf);
			return NULL;
		}
	}
	cstr_clear_free(passbuf);
	return NULL;
}

_TYPE(struct t_pwent *)
t_makepwent(tpw, user, pass, salt, confent)
struct t_pw *tpw;
const char *user;
const char *pass;
const struct t_num *salt;
const struct t_confent *confent;
{
	BigInteger x, v, n, g;
	unsigned char dig[SHA_DIGESTSIZE];
	SHACTX ctxt;

	tpw->pebuf.name = tpw->userbuf;
	tpw->pebuf.salt.data = tpw->saltbuf;

	strncpy(tpw->pebuf.name, user, MAXUSERLEN);
	tpw->pebuf.index = confent->index;

	if (salt) {
		tpw->pebuf.salt.len = salt->len;
		memcpy(tpw->pebuf.salt.data, salt->data, salt->len);
	} else {
		memset(dig, 0, SALTLEN);	/* salt is 80 bits */
		tpw->pebuf.salt.len = SALTLEN;
		do {
			t_random(tpw->pebuf.salt.data, SALTLEN);
		} while (memcmp(tpw->pebuf.salt.data, dig, SALTLEN) == 0);
		if (tpw->pebuf.salt.data[0] == 0) {
			tpw->pebuf.salt.data[0] = 0xff;
		}
	}

	n = BigIntegerFromBytes(confent->modulus.data, confent->modulus.len);
	g = BigIntegerFromBytes(confent->generator.data, confent->generator.len);
	v = BigIntegerFromInt(0);

	SHAInit(&ctxt);
	SHAUpdate(&ctxt, (const unsigned char *)user, strlen(user));
	SHAUpdate(&ctxt, (const unsigned char *)":", 1);
	SHAUpdate(&ctxt, (const unsigned char *)pass, strlen(pass));
	SHAFinal(dig, &ctxt);

	SHAInit(&ctxt);
	SHAUpdate(&ctxt, tpw->pebuf.salt.data, tpw->pebuf.salt.len);
	SHAUpdate(&ctxt, dig, sizeof(dig));
	SHAFinal(dig, &ctxt);

	/* x = H(s, H(u, ':', p)) */
	x = BigIntegerFromBytes(dig, sizeof(dig));

	BigIntegerModExp(v, g, x, n, NULL, NULL);
	BigIntegerToCstr(v, tpw->pwbuf);
	tpw->pebuf.password.data = (unsigned char *)(tpw->pwbuf->data);
	tpw->pebuf.password.len = tpw->pwbuf->length;

	BigIntegerFree(v);
	BigIntegerFree(x);
	BigIntegerFree(g);
	BigIntegerFree(n);

	return &tpw->pebuf;
}

_TYPE(void)
/*t_putpwent(ent, fp)
     const struct t_pwent * ent;
     FILE * fp;*/
t_putpwent(const struct t_pwent *ent, FILE *fp)
{
	cstr *strbuf = cstr_new();
	char saltbuf[MAXB64SALTLEN];

	fprintf(fp, "%s:%s:%s:%d\n", ent->name, t_tob64cstr(strbuf, (const char *)(ent->password.data), ent->password.len), t_tob64(saltbuf, (const char *)(ent->salt.data), ent->salt.len), ent->index);
	cstr_clear_free(strbuf);
}

static int
/*t_pwcopy(pwdest, pwsrc, diff)
     FILE * pwdest;
     FILE * pwsrc;
     struct t_pwent * diff;*/
t_pwcopy(FILE *pwdest, FILE *pwsrc, struct t_pwent *diff)
{
	struct t_pw *src;
	struct t_pwent *ent;

	if ((src = t_openpw(pwsrc)) == NULL) {
		return -1;
	}

	while ((ent = t_getpwent(src)) != NULL)
		if (diff && strcmp(diff->name, ent->name) == 0) {
			t_putpwent(diff, pwdest);
			diff = NULL;
		} else {
			t_putpwent(ent, pwdest);
		}

	if (diff) {
		t_putpwent(diff, pwdest);
	}

	return 0;
}

static int
/*t_pwremove(pwdest, pwsrc, user)
     FILE * pwdest;
     FILE * pwsrc;
     const char * user;*/
t_pwremove(FILE *pwdest, FILE *pwsrc, const char *user)
{
	struct t_pw *src;
	struct t_pwent *ent;

	if ((src = t_openpw(pwsrc)) == NULL) {
		return -1;
	}

	while ((ent = t_getpwent(src)) != NULL)
		if (strcmp(user, ent->name) != 0) {
			t_putpwent(ent, pwdest);
		}

	return 0;
}

/* TODO: Implement a more general method to handle delete/change */

_TYPE(int)
/*t_changepw(pwname, diff)
     const char * pwname;
     const struct t_pwent * diff;*/
t_changepw(const char *pwname, const struct t_pwent *diff)
{
	char *bakfile;
	char *bakfile2;
	struct stat st;
	FILE *passfp;
	FILE *bakfp;
	int rv;

	if (pwname == NULL) {
		pwname = DEFAULT_PASSWD;
	}

	if ((passfp = fopen(pwname, "rb")) == NULL) {
		return -1;
	}
	if (fstat(fileno(passfp), &st) < 0) {
		fclose(passfp);
		return -1;
	}
	if ((bakfile = malloc(strlen(pwname) + 5)) == NULL) {
		fclose(passfp);
		return -1;
	} else if ((bakfile2 = malloc(strlen(pwname) + 5)) == NULL) {
		fclose(passfp);
		free(bakfile);
		return -1;
	}

	sprintf(bakfile, "%s.bak", pwname);
	sprintf(bakfile2, "%s.sav", pwname);

	if ((bakfp = fopen(bakfile2, "wb")) == NULL && (unlink(bakfile2) < 0 || (bakfp = fopen(bakfile2, "wb")) == NULL)) {
		fclose(passfp);
		fclose(bakfp);
		free(bakfile);
		free(bakfile2);
		return -1;
	}

	rv = 0;

	if (syspw != NULL) {
		t_closepw(syspw);
		syspw = NULL;
	}
#ifdef NO_FCHMOD
	chmod(bakfile2, st.st_mode & 0777);
#else
	fchmod(fileno(bakfp), st.st_mode & 0777);
#endif

	t_pwcopy(bakfp, passfp, (struct t_pwent *)diff);

	fclose(bakfp);
	fclose(passfp);

#ifdef USE_RENAME
	unlink(bakfile);
	if (rename(pwname, bakfile) < 0) {
		rv = -1;
	} else {
		if (rename(bakfile2, pwname) < 0) {
			rv = -1;
		}
	}
#else
	unlink(bakfile);
	link(pwname, bakfile);
	unlink(pwname);
	link(bakfile2, pwname);
	unlink(bakfile2);
#endif
	free(bakfile);
	free(bakfile2);

	return rv;
}

_TYPE(int)
/*t_deletepw(pwname, user)
     const char * pwname;
     const char * user;*/
t_deletepw(const char *pwname, const char *user)
{
	char *bakfile;
	char *bakfile2;
	struct stat st;
	FILE *passfp;
	FILE *bakfp;

	if (pwname == NULL) {
		pwname = DEFAULT_PASSWD;
	}

	if ((passfp = fopen(pwname, "rb")) == NULL || fstat(fileno(passfp), &st) < 0) {
		return -1;
	}

	if ((bakfile = malloc(strlen(pwname) + 5)) == NULL) {
		fclose(passfp);
		return -1;
	} else if ((bakfile2 = malloc(strlen(pwname) + 5)) == NULL) {
		fclose(passfp);
		free(bakfile);
		return -1;
	}

	sprintf(bakfile, "%s.bak", pwname);
	sprintf(bakfile2, "%s.sav", pwname);

	if ((bakfp = fopen(bakfile2, "wb")) == NULL && (unlink(bakfile2) < 0 || (bakfp = fopen(bakfile2, "wb")) == NULL)) {
		if(bakfile != NULL) free(bakfile);
		if(bakfile2 != NULL) free(bakfile2);
		fclose(passfp);
		fclose(bakfp);
		return -1;
	}

	if (syspw != NULL) {
		t_closepw(syspw);
		syspw = NULL;
	}
#ifdef NO_FCHMOD
	chmod(bakfile2, st.st_mode & 0777);
#else
	fchmod(fileno(bakfp), st.st_mode & 0777);
#endif

	t_pwremove(bakfp, passfp, user);

	fclose(bakfp);
	fclose(passfp);

#ifdef USE_RENAME
	unlink(bakfile);
	if (rename(pwname, bakfile) < 0) {
		if(bakfile != NULL) free(bakfile);
		if(bakfile2 != NULL) free(bakfile2);
		return -1;
	}
	if (rename(bakfile2, pwname) < 0) {
		if(bakfile != NULL) free(bakfile);
		if(bakfile2 != NULL) free(bakfile2);
		return -1;
	}
#else
	unlink(bakfile);
	link(pwname, bakfile);
	unlink(pwname);
	link(bakfile2, pwname);
	unlink(bakfile2);
#endif
	free(bakfile);
	free(bakfile2);

	return 0;
}

_TYPE(int)
/*t_verifypw(user, pass)
     const char * user;
     const char * pass;*/
t_verifypw(const char *user, const char *pass)
{
	struct t_passwd *ent;
	struct t_pw *temp_pw;
	struct t_pwent *testent;
	int rval;

	if ((ent = gettpnam(user)) == NULL) {
		return -1;
	}

	temp_pw = t_newpw();
	testent = t_makepwent(temp_pw, user, pass, &ent->tp.salt, &ent->tc);

	if (ent->tp.password.len == testent->password.len && memcmp(ent->tp.password.data, testent->password.data, ent->tp.password.len) == 0) {
		rval = 1;
	} else {
		rval = 0;
	}

	t_closepw(temp_pw);
	return rval;
}

/* System password file accessors */

#include <tls/hap/nys_config.h>

static int pwinit(void)
{
	if (syspw == NULL) {
		if ((syspw = t_openpwbyname(DEFAULT_PASSWD)) == NULL) {
			return -1;
		}
		syspw->state = FILE_NIS;
	}
	return 0;
}

static void
/*pwsetup(out, tpwd, tcnf)
     struct t_passwd * out;
     struct t_pwent * tpwd;
     struct t_confent * tcnf;*/
pwsetup(struct t_passwd *out, struct t_pwent *tpwd, struct t_confent *tcnf)
{
	out->tp.name = tpwd->name;
	out->tp.password.len = tpwd->password.len;
	out->tp.password.data = tpwd->password.data;
	out->tp.salt.len = tpwd->salt.len;
	out->tp.salt.data = tpwd->salt.data;
	out->tp.index = tpwd->index;

	out->tc.index = tcnf->index;
	out->tc.modulus.len = tcnf->modulus.len;
	out->tc.modulus.data = tcnf->modulus.data;
	out->tc.generator.len = tcnf->generator.len;
	out->tc.generator.data = tcnf->generator.data;
}

#ifdef ENABLE_NSW
struct t_passwd *_gettpent
#else
_TYPE(struct t_passwd *) gettpent
#endif
(void)
{
	struct t_pwent *tpptr;
	struct t_confent *tcptr;

	if (pwinit() < 0) {
		return NULL;
	}
	tpptr = t_getpwent(syspw);
	if (tpptr == NULL) {
		return NULL;
	}
	tcptr =
#ifdef ENABLE_NSW
		_gettcid
#else
		gettcid
#endif
		(tpptr->index);
	if (tcptr == NULL) {
		return NULL;
	}
	pwsetup(&tpass, tpptr, tcptr);
	return &tpass;
}

#ifdef ENABLE_NSW
struct t_passwd *_gettpnam
#else
_TYPE(struct t_passwd *)
gettpnam
#endif
/*(user)*/
(const char *user)
{
	struct t_pwent *tpptr;
	struct t_confent *tcptr;

	if (pwinit() < 0) {
		return NULL;
	}
	tpptr = t_getpwbyname(syspw, user);
	if (tpptr == NULL) {
		return NULL;
	}
	tcptr =
#ifdef ENABLE_NSW
		_gettcid
#else
		gettcid
#endif
		(tpptr->index);
	if (tcptr == NULL) {
		return NULL;
	}
	pwsetup(&tpass, tpptr, tcptr);
	return &tpass;
}

#ifdef ENABLE_NSW
void
_settpent
#else
_TYPE(void)
settpent
#endif
(void)
{
	if (pwinit() < 0) {
		return;
	}
	t_rewindpw(syspw);
}

#ifdef ENABLE_NSW
void
_endtpent
#else
_TYPE(void)
endtpent
#endif
(void)
{
	if (syspw != NULL) {
		t_closepw(syspw);
		syspw = NULL;
	}
}
