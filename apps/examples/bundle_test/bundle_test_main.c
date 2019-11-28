/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <app/bundle.h>

#define TEST(b)                                            \
	do {                                                   \
		if ((b) == false) {                                \
			printf("%s[%d] failed\n", __func__, __LINE__); \
		}                                                  \
	} while (0)

void test_bundle_create(void)
{
	bundle *b;

	b = bundle_create();
	TEST(NULL != b);

	bundle_free(b);
	printf("test_bundle_create done\n");
}

void test_bundle_add_get(void)
{
	bundle *b = NULL;

	/* new bundle */
	b = bundle_create();
	TEST(0 == bundle_add(b, "key1", "val1"));
	TEST(0 == bundle_add(b, "key2", "val2"));

	TEST(0 == strcmp("val2", bundle_get_val(b, "key2")));
	TEST(2 == bundle_get_count(b));

	/* key duplication check 
	 * If key is already exist, raise error code, and ignore new val.
	 * old value is preserved.
	 */
	TEST(0 != bundle_add(b, "key1", "val123"));
	TEST(EPERM == get_errno());
	/* still has "val1" for "key1" */
	TEST(0 == strcmp("val1", bundle_get_val(b, "key1")));
	/* still count is 2 */
	TEST(2 == bundle_get_count(b));

	/* 3rd val */
	bundle_add(b, "key3", "val3");
	TEST(3 == bundle_get_count(b)); /* now 3 */

	bundle_free(b);
	printf("test_bundle_add_get done\n");
}

void test_bundle_add_invalid(void)
{
	bundle *b;

	TEST(0 != bundle_add(NULL, "a", "b"));
	TEST(EINVAL == get_errno());

	b = bundle_create();
	TEST(-1 == bundle_add(b, NULL, "val1"));
	TEST(EKEYREJECTED == get_errno());

	TEST(0 != bundle_add(b, "key1", NULL));
	TEST(EINVAL == get_errno());

	TEST(0 != bundle_add(b, "", "aaa"));
	TEST(EKEYREJECTED == get_errno());

	bundle_free(b);
	printf("test_bundle_add_invalid done\n");
}

void test_bundle_get_invalid(void)
{
	bundle *b;

	TEST(NULL == bundle_get_val(NULL, "key1"));
	TEST(EINVAL == get_errno());

	b = bundle_create();

	TEST(NULL == bundle_get_val(b, "key4"));
	TEST(ENOKEY == get_errno());

	TEST(NULL == bundle_get_val(NULL, "aaa"));
	TEST(EINVAL == get_errno());

	TEST(NULL == bundle_get_val(b, NULL));
	TEST(EKEYREJECTED == get_errno());

	bundle_free(b);
	printf("test_bundle_get_invalid done\n");
}

void iter_f(const char *key, const char *val, void *data)
{
	char *keys[] = {"1", "2", "3"};
	char *vals[] = {"a", "b", "c"};
	int i;

	for (i = 0; i < 3; i++) {
		if (0 == strcmp(key, keys[i])) {
			TEST(0 == strcmp(val, vals[i]));
			break;
		}
	}
}

void test_bundle_del(void)
{
	bundle *b;

	TEST(0 != bundle_del(NULL, "key"));
	TEST(EINVAL == get_errno());

	b = bundle_create();

	TEST(0 != bundle_del(b, NULL));
	TEST(EKEYREJECTED == get_errno());

	TEST(0 != bundle_del(b, ""));
	TEST(EKEYREJECTED == get_errno());

	TEST(0 != bundle_del(b, "key"));
	TEST(ENOKEY == get_errno());

	TEST(0 == bundle_get_count(b));

	bundle_add(b, "k1", "v1");
	const char *v = bundle_get_val(b, "k1");
	TEST(NULL != v && 0 == strcmp("v1", v));

	TEST(1 == bundle_get_count(b));

	TEST(0 == bundle_del(b, "k1"));
	TEST(NULL == bundle_get_val(b, "k1"));
	TEST(ENOKEY == get_errno());

	TEST(0 == bundle_get_count(b));

	bundle_free(b);
	printf("test_bundle_del done\n");
}

void test_bundle_iterate(void)
{
	bundle *b;

	b = bundle_create();

	bundle_add(b, "1", "a");
	bundle_add(b, "2", "b");
	bundle_add(b, "3", "c");

	bundle_iterate(b, iter_f, NULL);

	bundle_free(b);
	printf("test_bundle_iterate done\n");
}

void test_bundle_encode_decode(void)
{
	bundle *b1, *b2;
	bundle_raw *r;
	int size_r;

	b1 = bundle_create();
	bundle_add(b1, "k1", "v1");
	bundle_add(b1, "k2", "v2");
	bundle_encode(b1, &r, &size_r);

	/* show serialized data */
	/*
	char *c = r;
	for(; c < r+size_r; c++) {
		printf("%c", *c);
		if ('\0' == *c) printf("\n");
	}
	*/

	b2 = bundle_decode(r, size_r);

	TEST(0 == strcmp(bundle_get_val(b1, "k1"), bundle_get_val(b2, "k1")));
	TEST(0 == strcmp(bundle_get_val(b1, "k2"), bundle_get_val(b2, "k2")));

	bundle_free(b1);
	bundle_free(b2);
	free(r);
	printf("test_bundle_encode_decode done\n");
}

static void _dup_check_cb(const char *k, const char *v, void *data)
{
	bundle *b = (bundle *)data;

	TEST(0 == strcmp(bundle_get_val(b, k), v));
}

void test_bundle_dup(void)
{
	bundle *b1, *b2;
	b1 = bundle_create();
	bundle_add(b1, "k1", "v1");
	bundle_add(b1, "k2", "v2");

	b2 = bundle_dup(b1);

	TEST(NULL != b2);
	bundle_iterate(b2, _dup_check_cb, b1);
	TEST(bundle_get_count(b1) == bundle_get_count(b2));

	bundle_free(b1);
	bundle_free(b2);
	printf("test_bundle_dup done\n");
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int bundle_test_main(int argc, char *argv[])
#endif
{
	test_bundle_create();
	test_bundle_add_get();
	test_bundle_add_invalid();
	test_bundle_get_invalid();
	test_bundle_del();
	test_bundle_iterate();
	test_bundle_encode_decode();
	test_bundle_dup();

	printf("All Bundle test done\n");
	return 0;
}
