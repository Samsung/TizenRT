/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#include <cstdio>
#include <vector>
#include <iostream>
#include <debug.h>

#include <tinyara/init.h>

#include <apps/platform/cxxinitialize.h>

#include <ctime>
#include <fstream>
#include <google/protobuf/util/time_util.h>
#include <string>

#include "addressbook.pb.h"

using namespace std;

using google::protobuf::util::TimeUtil;

//***************************************************************************
// Definitions
//***************************************************************************
// Configuration ************************************************************
// C++ initialization requires CXX initializer support

#if !defined(CONFIG_HAVE_CXX) || !defined(CONFIG_HAVE_CXXINITIALIZE)
#undef CONFIG_EXAMPLES_PROTOBUF_CXXINITIALIZE
#endif

// Debug ********************************************************************
// Non-standard debug that may be enabled just for testing the constructors

#ifndef CONFIG_DEBUG
#undef CONFIG_DEBUG_CXX
#endif

#ifdef CONFIG_DEBUG_CXX
#define cxxdbg dbg
#define cxxlldbg lldbg
#ifdef CONFIG_DEBUG_VERBOSE
#define cxxvdbg vdbg
#define cxxllvdbg llvdbg
#else
#define cxxvdbg(...)
#define cxxllvdbg(...)
#endif
#else
#define cxxdbg(...)
#define cxxlldbg(...)
#define cxxvdbg(...)
#define cxxllvdbg(...)
#endif

//***************************************************************************
// Private Data
//***************************************************************************

#ifdef CONFIG_HAVE_CXXINITIALIZE
static tutorial::AddressBook address_book;
#endif

//***************************************************************************
// Private Functions
//***************************************************************************

void PromptForAddress(tutorial::Person* person)
{
	person->set_id(100);
	*person->mutable_name() = "abc";
	// person->set_name("abc");
	string email = "abc@samsung.com";
	if (!email.empty()) {
		person->set_email(email);
	}
	tutorial::Person::PhoneNumber* phone_number = person->add_phones();
	phone_number->set_number("1234");
	phone_number->set_type(tutorial::Person::MOBILE);
	*person->mutable_last_updated() = TimeUtil::SecondsToTimestamp(time(NULL));
#if 0
	cout << "Enter person ID number: ";
	int id;
	cin >> id;
	person->set_id(id);
	cin.ignore(256, '\n');

	cout << "Enter name: ";
	getline(cin, *person->mutable_name());

	cout << "Enter email address (blank for none): ";
	string email;
	getline(cin, email);
	if (!email.empty()) {
		person->set_email(email);
	}

	while (true) {
		cout << "Enter a phone number (or leave blank to finish): ";
		string number;
		getline(cin, number);
		if (number.empty()) {
			break;
		}

		tutorial::Person::PhoneNumber *phone_number = person->add_phones();
		phone_number->set_number(number);

		cout << "Is this a mobile, home, or work phone? ";
		string type;
		getline(cin, type);
		if (type == "mobile") {
			phone_number->set_type(tutorial::Person::MOBILE);
		} else if (type == "home") {
			phone_number->set_type(tutorial::Person::HOME);
		} else if (type == "work") {
			phone_number->set_type(tutorial::Person::WORK);
		} else {
			cout << "Unknown phone type.  Using default." << endl;
		}
	}
	*person->mutable_last_updated() = TimeUtil::SecondsToTimestamp(time(NULL));
#endif
}

// Iterates though all people in the AddressBook and prints info about them.
void ListPeople(const tutorial::AddressBook& address_book)
{
	for (int i = 0; i < address_book.people_size(); i++) {
		const tutorial::Person& person = address_book.people(i);

		cout << "Person ID: " << person.id() << endl;
		cout << "  Name: " << person.name() << endl;
		if (person.email() != "") {
			cout << "  E-mail address: " << person.email() << endl;
		}

		for (int j = 0; j < person.phones_size(); j++) {
			const tutorial::Person::PhoneNumber& phone_number = person.phones(j);

			switch (phone_number.type()) {
			case tutorial::Person::MOBILE:
				cout << "  Mobile phone #: ";
				break;
			case tutorial::Person::HOME:
				cout << "  Home phone #: ";
				break;
			case tutorial::Person::WORK:
				cout << "  Work phone #: ";
				break;
			default:
				cout << "  Unknown phone #: ";
				break;
			}
			cout << phone_number.number() << endl;
		}
		if (person.has_last_updated()) {
			cout << "  Updated: " << TimeUtil::ToString(person.last_updated()) << endl;
		}
	}
}

void DoStream(void)
{
	{
		// Read the existing address book.
		fstream input("/mnt/addressbook.dat", ios::in | ios::binary);
		if (!input) {
			cout << "/mnt/addressbook.dat"
				 << ": File not found.  Creating a new file." << endl;
		} else if (!address_book.ParseFromIstream(&input)) {
			cerr << "Failed to parse address book." << endl;
			return;
		}
	}

	// Add an address.
	PromptForAddress(address_book.add_people());

	{
		// Write the new address book back to disk.
		fstream output("/mnt/addressbook.dat", ios::out | ios::trunc | ios::binary);
		if (!address_book.SerializeToOstream(&output)) {
			cerr << "Failed to write address book." << endl;
			return;
		}
	}

	{
		// Read the existing address book.
		fstream input("/mnt/addressbook.dat", ios::in | ios::binary);
		if (!address_book.ParseFromIstream(&input)) {
			cerr << "Failed to parse address book." << endl;
			return;
		}
	}

	ListPeople(address_book);
}

void DoString(void)
{
	string data;

	PromptForAddress(address_book.add_people());

	address_book.SerializeToString(&data);
	address_book.ParseFromString(data);

	ListPeople(address_book);
}

/****************************************************************************
 * Name: addressbook_main
 ****************************************************************************/

extern "C" {
int addressbook_main(int argc, char* argv[])
{
// If C++ initialization for static constructors is supported, then do
// that first

#ifdef CONFIG_EXAMPLES_PROTOBUF_CXXINITIALIZE
	up_cxxinitialize();
#endif

	GOOGLE_PROTOBUF_VERIFY_VERSION;

	DoStream();
	DoString();

	// Optional:  Delete all global objects allocated by libprotobuf.
	// google::protobuf::ShutdownProtobufLibrary();

	return 0;
}
}
