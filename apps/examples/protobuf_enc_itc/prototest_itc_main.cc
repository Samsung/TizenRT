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
#define NEG_SIMPLE_COMPARE(a, b) ((a) != (b))
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

#define NEG_FUNC_TEST(func, type, input)					\
	NEG_FUNC_TEST_ARG(func, type, input, SIMPLE_COMPARE)

#define NEG_FUNC_TEST_ARG(func, type, input, compare)				\
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



extern "C" {

	static int string_compare(string src, string dest) {
		if (src.compare(dest) == 0) {
			return 1;
		} else {
			return -1;
		}
	}

	NEG_FUNC_TEST(ITCNegTestUint32f, MessageUint32, 11.8f)
	NEG_FUNC_TEST(ITCNegTestBool, MessageBool, 1.8f)
	NEG_FUNC_TEST(ITCNegTestInt32, MessageInt32, 23.2f)
	NEG_FUNC_TEST(ITCNegTestInt64, MessageInt64, 1111155555.5f)
	NEG_FUNC_TEST(ITCNegTestUint32, MessageUint32, -11)
	void ITCTestNested(void) {
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

	void ITCTestResult(void)
	{
		cout << "+++++++++++++++++++++++++++++++++++++\n";
		cout << "Total Success: " << total_success << endl;
		cout << "Total fails: " << total_fails << endl;
		cout << "+++++++++++++++++++++++++++++++++++++\n\n";
	}

	int prototest_itc_main(int argc, char *argv[])
	{
		up_cxxinitialize();

		GOOGLE_PROTOBUF_VERIFY_VERSION;

		total_fails = total_success = 0;
		
		ITCNegTestUint32();
		ITCNegTestUint32f();
		ITCNegTestBool();
		ITCNegTestInt32();
		ITCNegTestInt64();

		ITCTestResult();
		return 0;
	}
}

