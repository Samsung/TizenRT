/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

//***************************************************************************
// Included Files
//***************************************************************************

#include <tinyara/config.h>
#include <apps/platform/cxxinitialize.h>
#include "messagetype.pb.h"

using namespace std;

static int total_success, total_fails;

#define SIMPLE_COMPARE(a, b) ((a) == (b))
#define FUNC_TEST_ARG(func, type, input, compare)				\
	void func(void) {											\
		string data;											\
		tizenrt::type tc_input, tc_output;						\
		tc_input.set_data(input);								\
		tc_input.SerializeToString(&data);						\
		tc_output.ParseFromString(data);						\
		if (compare(tc_input.data(), tc_output.data()) != 0) {	\
			cout << "Test(" << __FUNCTION__ << "):\tsuccess\n";	\
			total_success++;									\
		}														\
		else {													\
			cout << "Test(" << __FUNCTION__ << "):\tfails\n";	\
			total_fails++;										\
		}														\
	};

#define FUNC_TEST(func, type, input)					\
	FUNC_TEST_ARG(func, type, input, SIMPLE_COMPARE)

extern "C" {

	static int string_compare(string src, string dest) {
		if (src.compare(dest) == 0) {
			return 1;
		} else {
			return -1;
		}
	}

	FUNC_TEST(TestDouble, MessageDouble, 23.3f)
	FUNC_TEST(TestFloat, MessageFloat, 23.3f)
	FUNC_TEST(TestInt32, MessageInt32, 23)
	FUNC_TEST(TestInt64, MessageInt64, 1111155555)
	FUNC_TEST(TestUint32, MessageUint32, 11111)
	FUNC_TEST(TestUint64, MessageUint64, 1111155555)
	FUNC_TEST(TestSint32, MessageSint32, -11111)
	FUNC_TEST(TestSint64, MessageSint64, -1111155555)
	FUNC_TEST(TestFixed32, MessageFixed32, 11111)
	FUNC_TEST(TestFixed64, MessageFixed64, 1111155555)
	FUNC_TEST(TestSfixed32, MessageSfixed32, 11111)
	FUNC_TEST(TestSfixed64, MessageSfixed64, 1111155555)
	FUNC_TEST(TestBool, MessageBool, 1)
	FUNC_TEST_ARG(TestString, MessageString, "hello world", string_compare)
	FUNC_TEST_ARG(TestBytes, MessageBytes, "hello bytes", string_compare)
	FUNC_TEST(TestEnum, MessageEnum, tizenrt::MessageEnum::MOBILE)

	void TestNested(void) {
		string data;
		tizenrt::MessageNested nested_input, nested_output;
		tizenrt::MessageNested::MessageInner *inner_input = nested_input.mutable_inner();

		nested_input.set_data(23);
		inner_input->set_data(34);

		nested_input.SerializeToString(&data);
		nested_output.ParseFromString(data);
		tizenrt::MessageNested::MessageInner *inner_output = nested_output.mutable_inner();

		if ((nested_input.data() == nested_output.data()) &&
			(inner_input->data() == inner_output->data())) {
			cout << "Test(" << __FUNCTION__ << "):\tsuccess\n";
			total_success++;
		} else {
			cout << "Test(" << __FUNCTION__ << "):\tfails\n";
			total_fails++;
		}
	}


	void TestResult(void)
	{
		cout << "+++++++++++++++++++++++++++++++++++++\n";
		cout << "Total Success: " << total_success << endl;
		cout << "Total fails: " << total_fails << endl;
		cout << "+++++++++++++++++++++++++++++++++++++\n\n";
	}


	int prototest_main(int argc, char *argv[])
	{
		up_cxxinitialize();

		GOOGLE_PROTOBUF_VERIFY_VERSION;

		total_fails = total_success = 0;

		TestDouble();
		TestFloat();

		TestInt32();
		TestInt64();

		TestUint32();
		TestUint64();

		TestSint32();
		TestSint64();

		TestFixed32();
		TestFixed64();

		TestSfixed32();
		TestSfixed64();

		TestBool();
		TestBytes();
		TestString();

		TestEnum();
		TestNested();

		// Result
		TestResult();
		// Optional:  Delete all global objects allocated by libprotobuf.
		//google::protobuf::ShutdownProtobufLibrary();
		return 0;
	}
}

