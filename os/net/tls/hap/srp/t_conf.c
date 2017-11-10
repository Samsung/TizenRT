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

#include <stdio.h>

#include <tls/hap/t_defines.h>
#include <tls/hap/t_pwd.h>
#include <tls/hap/t_read.h>
#include <tls/hap/srp_aux.h>

/* Master builtin parameter storage object */

static struct pre_struct {
	struct t_preconf preconf;
	int state;					/* 0 == uninitialized/first time */
	cstr *modbuf;
	cstr *genbuf;
} pre_params[] = {
	/*
	   { { "HMujfBWu4LfBFA0j3PpN7UbgUYfv.rMoMNuVRMoekpZ",
	   "2",
	   NULL }, 0 },
	   { { "W2KsCfRxb3/ELBvnVWufMA0gbdBlLXbJihgZkgp3xLTKwtPCUhSOHNZ5VLb9pBGR",
	   "2",
	   NULL }, 0 },
	 */
	{ {
			"3Kn/YYiomHkFkfM1x4kayR125MGkzpLUDy3y14FlTMwYnhZkjrMXnoC2TcFAecNlU5kFzgcpKYUbBOPZFRtyf3", "2", NULL
		}, 0
	}, { {
			"CbDP.jR6YD6wAj2ByQWxQxQZ7.9J9xkn2.Uqb3zVm16vQyizprhBw9hi80psatZ8k54vwZfiIeEHZVsDnyqeWSSIpWso.wh5GD4OFgdhVI3", "2", NULL
		}, 0
	}, { {
			"iqJ7nFZ4bGCRjE1F.FXEwL085Zb0kLM2TdHDaVVCdq0cKxvnH/0FLskJTKlDtt6sDl89dc//aEULTVFGtcbA/tDzc.bnFE.DWthQOu2n2JwKjgKfgCR2lZFWXdnWmoOh", "2", NULL
		}, 0
	}, { {
			"///////////93zgY8MZ2DCJ6Oek0t1pHAG9E28fdp7G22xwcEnER8b5A27cED0JTxvKPiyqwGnimAmfjybyKDq/XDMrjKS95v8MrTc9UViRqJ4BffZes8F//////////", "7", "oakley prime 1"
		}, 0
	}, { {
			"Ewl2hcjiutMd3Fu2lgFnUXWSc67TVyy2vwYCKoS9MLsrdJVT9RgWTCuEqWJrfB6uE3LsE9GkOlaZabS7M29sj5TnzUqOLJMjiwEzArfiLr9WbMRANlF68N5AVLcPWvNx6Zjl3m5Scp0BzJBz9TkgfhzKJZ.WtP3Mv/67I/0wmRZ", "2", NULL
		}, 0
	}, { {
			"F//////////oG/QeY5emZJ4ncABWDmSqIa2JWYAPynq0Wk.fZiJco9HIWXvZZG4tU.L6RFDEaCRC2iARV9V53TFuJLjRL72HUI5jNPYNdx6z4n2wQOtxMiB/rosz0QtxUuuQ/jQYP.bhfya4NnB7.P9A6PHxEPJWV//////////", "5", "oakley prime 2"
		}, 0
	},
#ifdef DRPRIMES
	{ {
			"F///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////m3C47", "5", "1036-bit DR prime"
		}, 0
	},
#endif
	{ {
			"3NUKQ2Re4P5BEK0TLg2dX3gETNNNECPoe92h4OVMaDn3Xo/0QdjgG/EvM.hiVV1BdIGklSI14HA38Mpe5k04juR5/EXMU0r1WtsLhNXwKBlf2zEfoOh0zVmDvqInpU695f29Iy7sNW3U5RIogcs740oUp2Kdv5wuITwnIx84cnO.e467/IV1lPnvMCr0pd1dgS0a.RV5eBJr03Q65Xy61R", "2", NULL
		}, 0
	}, { {
			"dUyyhxav9tgnyIg65wHxkzkb7VIPh4o0lkwfOKiPp4rVJrzLRYVBtb76gKlaO7ef5LYGEw3G.4E0jbMxcYBetDy2YdpiP/3GWJInoBbvYHIRO9uBuxgsFKTKWu7RnR7yTau/IrFTdQ4LY/q.AvoCzMxV0PKvD9Odso/LFIItn8PbTov3VMn/ZEH2SqhtpBUkWtmcIkEflhX/YY/fkBKfBbe27/zUaKUUZEUYZ2H2nlCL60.JIPeZJSzsu/xHDVcx", "2", NULL
		}, 0
	}, { {
			"2iQzj1CagQc/5ctbuJYLWlhtAsPHc7xWVyCPAKFRLWKADpASkqe9djWPFWTNTdeJtL8nAhImCn3Sr/IAdQ1FrGw0WvQUstPx3FO9KNcXOwisOQ1VlL.gheAHYfbYyBaxXL.NcJx9TUwgWDT0hRzFzqSrdGGTN3FgSTA1v4QnHtEygNj3eZ.u0MThqWUaDiP87nqha7XnT66bkTCkQ8.7T8L4KZjIImrNrUftedTTBi.WCi.zlrBxDuOM0da0JbUkQlXqvp0yvJAPpC11nxmmZOAbQOywZGmu9nhZNuwTlxjfIro0FOdthaDTuZRL9VL7MRPUDo/DQEyW.d4H.UIlzp", "2", NULL
		}, 0
	},
#ifdef DRPRIMES
	{ {
			"3////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////m8MPh", "2", "2072-bit DR prime"
		}, 0
	},
#endif
	{ {
			"///////////93zgY8MZ2DCJ6Oek0t1pHAG9E28fdp7G22xwcEnER8b5A27cED0JTxvKPiyqwGnimAmfjybyKDq/XDMrjKS95v8MrTc9UViRqJ4BffZVjQml/NBRq1hVjxZXh.rg9dwMkdoGHV4iVvaaePb7iv5izmW1ykA5ZlmMOsaWs75NJccaMFwZz9CzVWsLT8zoZhPOSOlDM88LIkvxLAGTmbfPjPmmrJagyc0JnT6m8oXWXV3AGNaOkDiuxuvvtB1WEXWER9uEYx0UYZxN5NV1lJ5B9tYlBzfLO5nWvbKbywfLgvHNI9XYO.WKG5NAEMeggn2sjCnSD151wCwXL8QlV7BfaxFk515ZRxmgAwd5NNGOCVREN3uMcuUJ7g/MkZDi9CzSUZ9JWIYLXdSxZqYOQqkvhyI/w1jcA26JOTW9pFiXgP58VAnWNUo0Ck.4NLtfXNMnt2OZ0kjb6uWZYJw1qvQinGzjR/E3z48vBWj4WgJhIol//////////", "5", "3072-bit MODP prime"
		}, 0
	}, { {
			"F//////////oG/QeY5emZJ4ncABWDmSqIa2JWYAPynq0Wk.fZiJco9HIWXvZZG4tU.L6RFDEaCRC2iARV9V53TFuJLjRL72HUI5jNPYNdx6z4n2wQOtxMiB/rosz0QtxUuuQ/jQYP.bhfya4NnB7.P9A6PHxEHRFS80VBYXOxy5cDf8DXnLqvff5Z.e/IJFNuDbNIFSewsM76BpLY25KhkUrIa7S9QMRMSCDKvAl9W4yNHi2CeO8Nmoa5v6BZREE.EUTomO3eO3coU3ekm7ee.rnLtmRqnIoTuho/QLM1SOEPL9VEgLQkKLqYOOcFe541LoZbgAgiGjhJCN3GHGUZEeLI6htnowPEpxXGHOs.yAYkfnLrq637spbm.5fk7anwlrhepR2JFN7eoKu4ebOPtEuz8c6jBkQ/4l.WRPYWXas7O2Spx8QcHI7oiO5tiW3BlX5rTwOLriTmc8mBhPHk88ua.WTEMhCKFRM/pW/H2EIuBH8AaX204QSZmIfuVcruXncX2zkbiccSCd66hquZmQb6WqjXKBsYM3wSegr4pesxl2smJUZlakZlmK7xxAfYXyMKTEQy1TcRAMJw2Gmw8ZEw66KLldxHzXAN3EujUlk1lTTY5mI1pG1f4drR1QgPEqwfYDZzt1Xl.tt92cm8zDz3N9D0OncV//////////", "5", "4096-bit MODP prime"
		}, 0
	},
#ifdef DRPRIMES
	{ {
			"/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////m8pOF", "B", "4116-bit DR prime"
		}, 0
	},
#endif
	{ {
			"///////////93zgY8MZ2DCJ6Oek0t1pHAG9E28fdp7G22xwcEnER8b5A27cED0JTxvKPiyqwGnimAmfjybyKDq/XDMrjKS95v8MrTc9UViRqJ4BffZVjQml/NBRq1hVjxZXh.rg9dwMkdoGHV4iVvaaePb7iv5izmW1ykA5ZlmMOsaWs75NJccaMFwZz9CzVWsLT8zoZhPOSOlDM88LIkvxLAGTmbfPjPmmrJagyc0JnT6m8oXWXV3AGNaOkDiuxuvvtB1WEXWER9uEYx0UYZxN5NV1lJ5B9tYlBzfLO5nWvbKbywfLgvHNI9XYO.WKG5NAEMeggn2sjCnSD151wCwXL8QlV7BfaxFk515ZRxmgAwd5NNGOCVREN3uMcuUJ7g/MkZDi9CzSUZ9JWIYLXdSxZqYOQqkvhyI/w1jcA26JOTW9pFiXgP58VAnWNUo0Ck.4NLtfXNMnt2OZ0kjb6uWZYJw1qvQinGzjR/E3z48vBWj4WgI480HfoF1AdX.RNY76Q4BswMoQPmoSOQlJYF1gKQ3Is5GlQ9OFfoYhKJEZRkyBR1DwE.IwE/1GVligcA7nPHqvhm5sPifPFe933eYCxeOPHM.Vj7s4fSCxYrw.u6zrs8N187D06aIVLi5gfaxJgc8sFtS66/xVSaAR0ZqtqDSaq0eIIDiFwjD9yS2R1rDooO2P6tibr7dOzkZUz.F.K1gsUKmxbspWlGJ01hh1gK.sG9zWn5vSdi8PQYHZQFjlhpviKxKJERBhErBiRstyKH.RC9Kip855HAzUlGc.uz04tZDA/MOFA0SPBakpmCkeLqN8T0/I2ryvkTFxsrLvmBqQO38Arg40na0iSdbddoN..n.Zp8wbwVZRCYBuF7KMt/rXQnKlK1x8hGLIgp8zjVhz8uTWKp5xI3u0tuAUN5Uxoc.Ce1g7LYxV5sdRrKAezYX./yEiPpB6Z4zLSsbR9x2xobZ8uV.ZNRZm4Q0G.ZsO/I63k4hyjMmjqTDRcbFaURSn09F//////////",
			"5", "6144-bit MODP prime"
		}, 0
	}, { {
			"3//////////yaFsg8XQC8qnCPYYu3S7D4f0au8YcVCT08BlgOx4viYKKe8UOuq1DtlbHcppJf36p0h2ctoNnGtJ.4rRMrHmaNaXRLsObv.nlHCGkccD.rh2/zSjlG6j.tkE6lxMecVfQwV915yIn/cIIXcKUpaMpt207oueME/1PZQI3OSLTEQQHO/gFqapr.3PLqZtAEjbXnYyrOWXLAxdjKf1t2Mbcrd33LEIhoO1F5qR0ZA625yCf1UHYuspZlZddSi60w60vidWwBi1wAFjSLTy6zCKidUAylsbLWN63cLINpgbMhb5T8c69Zw1H0LSevQYgh4BQqp5mq4K7epg5KXgzySkcJi.uK4MDll2ehgSLTT1WnzivSFXQRXvCUhzQwCsmaprnwCbE1A9M6TpkFI9XhIxclnB/e6sOe8PDXs0dC.o6faKXyh61Tx80oxuHTNUc5TR7S9YC2wsKRY2E9Fe7Jbgp53srlyuFqGZak2qI2f8GW16d8y4gU7vjU8SPeGlRfR9fd39nXgzE8y6fHeDBOL2zebW.dAAjHCwDkxmji4texvBexy51..ogOeV5b7Jcl0NPcoba.WaCEY8pkXXb5Rv.qVOIbmpkBNhxWRtNOXS4WSq0QH9zMmMgcJjEgOZO/TmOR/jzoGfi2FJVGroJG2X98sm/gqqdnm9i7KtB9W9aRUoNKUTZswDxtu/vG6hPvJ3kNRE2z1C06ki6fJxP0ds34NboUmXbg96De.s.lFcnJjHCvikixKknlRVnH7vimbIpCWKL4hrwz2RxZq0JUCqhzPWye1nakIxF0owXNHSXq3z8BNpcvq/lRLNd0lHfWCWhMeG36G2noUMUV9Vxx7wFCZgNf.Dio8lWyTHRV/M5h5IzG7iYj1LAhCZsr.lqZXs1JCNj8FW3VWfvSLxlARuoW6eTMBjyNQTlLGgZsA7x/mwndCiQCJrLpQLidiBlAMCZX/wDTkF0He13wFPZz8OEuIlorR2tHqrkQK.HvjlX5PTAEIRnB.vUGuTtosgJBVZDY.nD1pkJ6wEyWojesTqm1q7wU/Yln7xILszfDhf2HcEgjZd5hazMWq8xHqA/79U2EF5ilZdMKju/sullo4YjaY8Yu4f0Dy1nFhLwWQ8/37D7FyP6pgC6jBoyY6BuE5tVgTIt.Ym8VeUMWp0.rRtJe6Appriw9ufcqg4/W/HFWjtp4Eu7IhQZP5b.YPe2LTmMJp7CK8HeKT.Qj86LtjVg6nrH2zVkTDS/hpQyCUpw9eDP16zEk7dv902KEBI1niruYQ02xLxZWhoHaDflm2RaULMEH7LdVfgfumKE9sLfJVo1zMw82vRd5WoO3TcEtJt///////////",
			"J", "8192-bit MODP prime"
		}, 0
	}
};

_TYPE(int) t_getprecount(void)
{
	return (sizeof(pre_params) / sizeof(struct pre_struct));
}

_TYPE(struct t_preconf *)
/*t_getpreparam(idx)
     int idx;*/
t_getpreparam(int idx)
{
	if (idx >= t_getprecount()) {
		return NULL;
	}

	if (pre_params[idx].state == 0) {
		/* Wire up storage */
		pre_params[idx].modbuf = cstr_new();
		pre_params[idx].genbuf = cstr_new();

		/* Convert from b64 to t_num */
		pre_params[idx].preconf.modulus.len = t_cstrfromb64(pre_params[idx].modbuf, pre_params[idx].preconf.mod_b64);
		pre_params[idx].preconf.generator.len = t_cstrfromb64(pre_params[idx].genbuf, pre_params[idx].preconf.gen_b64);

		pre_params[idx].preconf.modulus.data = (unsigned char *)(pre_params[idx].modbuf->data);
		pre_params[idx].preconf.generator.data = (unsigned char *)(pre_params[idx].genbuf->data);

		pre_params[idx].state = 1;
	}
	return &(pre_params[idx].preconf);
}

_TYPE(struct t_conf *)
/*t_openconf(fp)
     FILE * fp;*/
t_openconf(FILE *fp)
{
	struct t_conf *tc;
	char close_flag = 0;

	if (fp == NULL) {			/* NULL means to open the system default file */
		if ((fp = fopen(DEFAULT_CONF, "r")) == NULL) {
			return NULL;
		}
		close_flag = 1;
	} else {
		close_flag = 0;    /* If it's a real fd, don't close it automatically */
	}

	if ((tc = malloc(sizeof(struct t_conf))) == NULL) {
		return NULL;
	}
	tc->instream = fp;
	tc->close_on_exit = close_flag;
	tc->modbuf = cstr_new();
	tc->genbuf = cstr_new();

	return tc;
}

_TYPE(struct t_conf *)
/*t_openconfbyname(confname)
     const char * confname;*/
t_openconfbyname(const char *confname)
{
	FILE *fp;
	struct t_conf *t;

	if (confname == NULL) {
		return t_openconf(NULL);
	}

	if ((fp = fopen(confname, "r")) == NULL) {
		return NULL;
	}

	t = t_openconf(fp);
	t->close_on_exit = 1;		/* Since we opened it, we should close it */
	return t;
}

_TYPE(void)
/*t_closeconf(tc)
     struct t_conf * tc;*/
t_closeconf(struct t_conf *tc)
{
	if (tc->close_on_exit) {
		fclose(tc->instream);
	}
	if (tc->modbuf) {
		cstr_clear_free(tc->modbuf);
	}
	if (tc->genbuf) {
		cstr_clear_free(tc->genbuf);
	}
	free(tc);
}

_TYPE(void)
/*t_rewindconf(tc)
     struct t_conf * tc;*/
t_rewindconf(struct t_conf *tc)
{
	rewind(tc->instream);
}

_TYPE(struct t_confent *)
/*t_getconfent(tc)
     struct t_conf * tc;*/
t_getconfent(struct t_conf *tc)
{
	char indexbuf[16];
	cstr *b64buf = cstr_new();

	while (1) {
		if (t_nextfield(tc->instream, indexbuf, 16) > 0 && (tc->tcbuf.index = atoi(indexbuf)) > 0 && t_nextcstrfield(tc->instream, b64buf) > 0 && (tc->tcbuf.modulus.len = t_cstrfromb64(tc->modbuf, b64buf->data)) > 0 && t_nextcstrfield(tc->instream, b64buf) > 0 && (tc->tcbuf.generator.len = t_cstrfromb64(tc->genbuf, b64buf->data)) > 0) {
			tc->tcbuf.modulus.data = (unsigned char *)(tc->modbuf->data);
			tc->tcbuf.generator.data = (unsigned char *)(tc->genbuf->data);
			t_nextline(tc->instream);
			cstr_clear_free(b64buf);
			return &tc->tcbuf;
		} else if (t_nextline(tc->instream) < 0) {
			cstr_clear_free(b64buf);
			return NULL;
		}
	}
}

_TYPE(struct t_confent *)
/*t_getconflast(tc)
     struct t_conf * tc;*/
t_getconflast(struct t_conf *tc)
{
	int valid = 0;

	t_rewindconf(tc);
	while (t_getconfent(tc) != NULL) {
		valid = 1;
	}
	if (valid) {
		return &tc->tcbuf;
	} else {
		return NULL;
	}
}

_TYPE(struct t_confent *)
/*t_getconfbyindex(tc, index)
     struct t_conf * tc;
     int index;*/
t_getconfbyindex(struct t_conf *tc, int index)
{
	char indexbuf[16];
	cstr *b64buf = cstr_new();
	int tindex;

	t_rewindconf(tc);

	while (t_nextfield(tc->instream, indexbuf, 16) > 0) {
		if ((tindex = atoi(indexbuf)) == index)
			if (t_nextcstrfield(tc->instream, b64buf) > 0 && (tc->tcbuf.modulus.len = t_cstrfromb64(tc->modbuf, b64buf->data)) > 0 && t_nextcstrfield(tc->instream, b64buf) > 0 && (tc->tcbuf.generator.len = t_cstrfromb64(tc->genbuf, b64buf->data)) > 0) {
				tc->tcbuf.index = tindex;
				tc->tcbuf.modulus.data = (unsigned char *)(tc->modbuf->data);
				tc->tcbuf.generator.data = (unsigned char *)(tc->genbuf->data);
				t_nextline(tc->instream);
				cstr_clear_free(b64buf);
				return &tc->tcbuf;
			}
		if (t_nextline(tc->instream) < 0) {
			cstr_clear_free(b64buf);
			return NULL;
		}
	}
	cstr_clear_free(b64buf);
	return NULL;
}

/*
 * This is the safe prime generation logic.
 * To generate a safe prime p (where p = 2q+1 and q is prime), we start
 * with a random odd q that is one bit shorter than the desired length
 * of p.  We use a simple 30-element sieve to filter the values of q
 * and consider only those that are 11, 23, or 29 (mod 30).  (If q were
 * anything else, either q or p would be divisible by 2, 3, or 5).
 * For the values of q that are left, we apply the following tests in
 * this order:
 *
 *   trial divide q
 *   let p = 2q + 1
 *   trial divide p
 *   apply Fermat test to q (2^q == 2 (mod q))
 *   apply Fermat test to p (2^p == 2 (mod p))
 *   apply real probablistic primality test to q
 *   apply real probablistic primality test to p
 *
 * A number that passes all these tests is considered a safe prime for
 * our purposes.  The tests are ordered this way for efficiency; the
 * slower tests are run rarely if ever at all.
 */

static int
/*trialdiv(x)
     const BigInteger x;*/
trialdiv(const BigInteger x)
{
	static int primes[] = {		/* All odd primes < 256 */
		3, 5, 7, 11, 13, 17, 19, 23, 29,
		31, 37, 41, 43, 47, 53, 59, 61, 67,
		71, 73, 79, 83, 89, 97, 101, 103,
		107, 109, 113, 127, 131, 137, 139, 149, 151,
		157, 163, 167, 173, 179, 181, 191, 193, 197,
		199, 211, 223, 227, 229, 233, 239, 241, 251
	};
	static int nprimes = sizeof(primes) / sizeof(int);
	int i;

	for (i = 0; i < nprimes; ++i) {
		if (BigIntegerModInt(x, primes[i], NULL) == 0) {
			return primes[i];
		}
	}
	return 1;
}

/* x + sieve30[x%30] == 11, 23, or 29 (mod 30) */

static int sieve30[] = { 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,
						 1, 12, 11, 10, 9, 8, 7, 6, 5, 4,
						 3, 2, 1, 6, 5, 4, 3, 2, 1, 12
					   };

/* Find a Sophie-Germain prime between "lo" and "hi".  NOTE: this is not
   a "safe prime", but the smaller prime.  Take 2q+1 to get the safe prime. */

static void
//sophie_germain(q, lo, hi)
//     BigInteger q;        /* assumed initialized */
//     const BigInteger lo;
//     const BigInteger hi;*/
sophie_germain(BigInteger q /* assumed initialized */ , const BigInteger lo, const BigInteger hi)
{
	BigInteger m, p, r;
	cstr *parambuf;
	int foundprime = 0;
	int i, mod30;

	m = BigIntegerFromInt(0);
	BigIntegerSub(m, hi, lo);
	i = BigIntegerByteLen(m);
	parambuf = cstr_new();
	cstr_set_length(parambuf, i);
	t_random((unsigned char *)(parambuf->data), i);
	r = BigIntegerFromBytes((const unsigned char *)(parambuf->data), i);
	cstr_clear_free(parambuf);
	BigIntegerMod(r, r, m, NULL);

	BigIntegerAdd(q, r, lo);
	if (BigIntegerModInt(q, 2, NULL) == 0) {
		BigIntegerAddInt(q, q, 1);    /* make q odd */
	}

	mod30 = BigIntegerModInt(q, 30, NULL);	/* mod30 = q % 30 */

	BigIntegerFree(m);
	m = BigIntegerFromInt(2);	/* m = 2 */
	p = BigIntegerFromInt(0);

	while (BigIntegerCmp(q, hi) < 0) {
		if (trialdiv(q) < 2) {
			BigIntegerMulInt(p, q, 2, NULL);	/* p = 2 * q */
			BigIntegerAddInt(p, p, 1);	/* p += 1 */
			if (trialdiv(p) < 2) {
				BigIntegerModExp(r, m, q, q, NULL, NULL);	/* r = 2^q % q */
				if (BigIntegerCmpInt(r, 2) == 0) {	/* if(r == 2) */
					BigIntegerModExp(r, m, p, p, NULL, NULL);	/* r = 2^p % p */
					if (BigIntegerCmpInt(r, 2) == 0) {	/* if(r == 2) */
						if (BigIntegerCheckPrime(q, NULL) && BigIntegerCheckPrime(p, NULL)) {
							++foundprime;
							break;
						}
					}
				}
			}
		}

		i = sieve30[mod30];
		BigIntegerAddInt(q, q, i);	/* q += i */
		mod30 = (mod30 + i) % 30;
	}

	/* should wrap around on failure */
	if (!foundprime) {
		fprintf(stderr, "Prime generation failed!\n");
		exit(1);
	}

	BigIntegerFree(r);
	BigIntegerFree(m);
	BigIntegerFree(p);
}

_TYPE(struct t_confent *)
/*t_makeconfent(tc, nsize)
     struct t_conf * tc;
     int nsize;*/
t_makeconfent(struct t_conf *tc, int nsize)
{
	BigInteger n, g, q, t, u;

	t = BigIntegerFromInt(0);
	u = BigIntegerFromInt(1);	/* u = 1 */
	BigIntegerLShift(t, u, nsize - 2);	/* t = 2^(nsize-2) */
	BigIntegerMulInt(u, t, 2, NULL);	/* u = 2^(nsize-1) */

	q = BigIntegerFromInt(0);
	sophie_germain(q, t, u);

	n = BigIntegerFromInt(0);
	BigIntegerMulInt(n, q, 2, NULL);
	BigIntegerAddInt(n, n, 1);

	/* Look for a generator mod n */
	g = BigIntegerFromInt(2);
	while (1) {
		BigIntegerModExp(t, g, q, n, NULL, NULL);	/* t = g^q % n */
		if (BigIntegerCmpInt(t, 1) == 0) {	/* if(t == 1) */
			BigIntegerAddInt(g, g, 1);    /* ++g */
		} else {
			break;
		}
	}
	BigIntegerFree(t);
	BigIntegerFree(u);
	BigIntegerFree(q);

	BigIntegerToCstr(n, tc->modbuf);
	tc->tcbuf.modulus.data = (unsigned char *)(tc->modbuf->data);
	tc->tcbuf.modulus.len = tc->modbuf->length;
	BigIntegerFree(n);

	BigIntegerToCstr(g, tc->genbuf);
	tc->tcbuf.generator.data = (unsigned char *)(tc->genbuf->data);
	tc->tcbuf.generator.len = tc->genbuf->length;
	BigIntegerFree(g);

	tc->tcbuf.index = 1;
	return &tc->tcbuf;
}

_TYPE(struct t_confent *)
/*t_makeconfent_c(tc, nsize)
     struct t_conf * tc;
     int nsize;*/
t_makeconfent_c(struct t_conf *tc, int nsize)
{
	BigInteger g, n, p, q, j, k, t, u;
	int psize, qsize;

	psize = nsize / 2;
	qsize = nsize - psize;

	t = BigIntegerFromInt(1);	/* t = 1 */
	u = BigIntegerFromInt(0);
	BigIntegerLShift(u, t, psize - 3);	/* u = t*2^(psize-3) = 2^(psize-3) */
	BigIntegerMulInt(t, u, 3, NULL);	/* t = 3*u = 1.5*2^(psize-2) */
	BigIntegerAdd(u, u, t);		/* u += t [u = 2^(psize-1)] */
	j = BigIntegerFromInt(0);
	sophie_germain(j, t, u);

	k = BigIntegerFromInt(0);
	if (qsize != psize) {
		BigIntegerFree(t);
		t = BigIntegerFromInt(1);	/* t = 1 */
		BigIntegerLShift(u, t, qsize - 3);	/* u = t*2^(qsize-3) = 2^(qsize-3) */
		BigIntegerMulInt(t, u, 3, NULL);	/* t = 3*u = 1.5*2^(qsize-2) */
		BigIntegerAdd(u, u, t);	/* u += t [u = 2^(qsize-1)] */
	}
	sophie_germain(k, t, u);

	p = BigIntegerFromInt(0);
	BigIntegerMulInt(p, j, 2, NULL);	/* p = 2 * j */
	BigIntegerAddInt(p, p, 1);	/* p += 1 */

	q = BigIntegerFromInt(0);
	BigIntegerMulInt(q, k, 2, NULL);	/* q = 2 * k */
	BigIntegerAddInt(q, q, 1);	/* q += 1 */

	n = BigIntegerFromInt(0);
	BigIntegerMul(n, p, q, NULL);	/* n = p * q */
	BigIntegerMul(u, j, k, NULL);	/* u = j * k */

	BigIntegerFree(p);
	BigIntegerFree(q);
	BigIntegerFree(j);
	BigIntegerFree(k);

	g = BigIntegerFromInt(2);	/* g = 2 */

	/* Look for a generator mod n */
	while (1) {
		BigIntegerModExp(t, g, u, n, NULL, NULL);	/* t = g^u % n */
		if (BigIntegerCmpInt(t, 1) == 0) {
			BigIntegerAddInt(g, g, 1);    /* ++g */
		} else {
			break;
		}
	}

	BigIntegerFree(u);
	BigIntegerFree(t);

	BigIntegerToCstr(n, tc->modbuf);
	tc->tcbuf.modulus.data = (unsigned char *)(tc->modbuf->data);
	tc->tcbuf.modulus.len = tc->modbuf->length;
	BigIntegerFree(n);

	BigIntegerToCstr(g, tc->genbuf);
	tc->tcbuf.generator.data = (unsigned char *)(tc->genbuf->data);
	tc->tcbuf.generator.len = tc->genbuf->length;
	BigIntegerFree(g);

	tc->tcbuf.index = 1;
	return &tc->tcbuf;
}

_TYPE(struct t_confent *)
/*t_newconfent(tc)
    struct t_conf * tc;*/
t_newconfent(struct t_conf *tc)
{
	tc->tcbuf.index = 0;
	tc->tcbuf.modulus.data = (unsigned char *)(tc->modbuf->data);
	tc->tcbuf.modulus.len = 0;
	tc->tcbuf.generator.data = (unsigned char *)(tc->genbuf->data);
	tc->tcbuf.generator.len = 0;
	return &tc->tcbuf;
}

_TYPE(int)
/*t_cmpconfent(cf1, cf2)
     const struct t_confent * cf1;
     const struct t_confent * cf2;*/
t_cmpconfent(const struct t_confent *cf1, const struct t_confent *cf2)
{
	int diff;

	diff = cf1->modulus.len - cf2->modulus.len;
	if (diff != 0) {
		return diff;
	}
	diff = cf1->generator.len - cf2->generator.len;
	if (diff != 0) {
		return diff;
	}
	diff = memcmp(cf1->modulus.data, cf2->modulus.data, cf1->modulus.len);
	if (diff != 0) {
		return diff;
	}
	return memcmp(cf1->generator.data, cf2->generator.data, cf1->generator.len);
}

_TYPE(void)
/*t_putconfent(ent, fp)
     const struct t_confent * ent;
     FILE * fp;*/
t_putconfent(const struct t_confent *ent, FILE *fp)
{
	cstr *strbuf = cstr_new();

	fprintf(fp, "%d:%s:", ent->index, t_tob64cstr(strbuf, (const char *)(ent->modulus.data), ent->modulus.len));
	fprintf(fp, "%s\n", t_tob64cstr(strbuf, (const char *)(ent->generator.data), ent->generator.len));
	cstr_clear_free(strbuf);
}

int
/*t_isprime(x)
     BigInteger x;*/
t_isprime(BigInteger x)
{
	BigInteger t;
	int iscomp;

	if (trialdiv(x) > 1) {
		return 0;
	}

	t = BigIntegerFromInt(2);
	BigIntegerModExp(t, t, x, x, NULL, NULL);
	iscomp = (BigIntegerCmpInt(t, 2) != 0);
	BigIntegerFree(t);
	if (iscomp) {
		return 0;
	} else {
		return BigIntegerCheckPrime(x, NULL);
	}
}

/* TJW: Why is this here?  Nobody seems to call it...
_TYPE( int )
t_checkprime(num)
     const struct t_num * num;
{
  BigInteger x, halfx;
  int retval;

  retval = NUM_SAFE;
  x = BigIntegerFromBytes(num->data, num->len);
  if(!t_isprime(x))
    retval = NUM_NOTPRIME;
  else {
    halfx = BigIntegerFromInt(0);
    BigIntegerSubInt(x, x, 1);
    BigIntegerDivInt(halfx, x, 2, NULL);
    if(!t_isprime(halfx))
      retval = NUM_NOTSAFE;
    BigIntegerFree(halfx);
  }
  BigIntegerFree(x);
  return retval;
}
*/

/* System conf file accessors */

#include <tls/hap/nys_config.h>

static struct t_conf *sysconf = NULL;

static int confinit(void)
{
	if (sysconf == NULL) {
		if ((sysconf = t_openconfbyname(DEFAULT_CONF)) == NULL) {
			return -1;
		}
	}
	return 0;
}

#ifdef ENABLE_NSW
struct t_confent *_gettcent
#else
_TYPE(struct t_confent *)
gettcent
#endif
(void)
{
	if (confinit() < 0) {
		return NULL;
	}
	return t_getconfent(sysconf);
}

#ifdef ENABLE_NSW
struct t_confent *_gettcid
#else
_TYPE(struct t_confent *)
gettcid
#endif
/*(id)
     int id;*/
(int id)
{
	if (confinit() < 0) {
		return NULL;
	}
	return t_getconfbyindex(sysconf, id);
}

#ifdef ENABLE_NSW
void
_settcent
#else
_TYPE(void)
settcent
#endif
(void)
{
	if (confinit() < 0) {
		return;
	}
	t_rewindconf(sysconf);
}

#ifdef ENABLE_NSW
void
_endtcent
#else
_TYPE(void)
endtcent
#endif
(void)
{
	if (sysconf != NULL) {
		t_closeconf(sysconf);
		sysconf = NULL;
	}
}
