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
/****************************************************************************
 * examples/route_client/route_guide_client.cc
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include <tinyara/config.h>
#include <iostream>
#include <memory>
#include <string>
#include <chrono>
#include <random>
#include <string>
#include <thread>

#include <grpc++/grpc++.h>
#include <tinyara/init.h>

#include "helper.h"
#include "route_guide.grpc.pb.h"

#include <apps/platform/cxxinitialize.h>

struct arg_holder {
	int argc;
	char** argv;
};

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientAsyncResponseReader;
using grpc::ClientAsyncWriter;
using grpc::ClientAsyncReader;
using grpc::ClientAsyncReaderWriter;
using grpc::ClientWriter;
using grpc::CompletionQueue;
using grpc::Status;
using grpc::ChannelCredentials;
using routeguide::Point;
using routeguide::Feature;
using routeguide::Rectangle;
using routeguide::RouteSummary;
using routeguide::RouteNote;
using routeguide::RouteGuide;

Point MakePoint(long latitude, long longitude)
{
	Point p;
	p.set_latitude(latitude);
	p.set_longitude(longitude);
	return p;
}

Feature MakeFeature(const std::string& name, long latitude, long longitude)
{
	Feature f;
	f.set_name(name);
	f.mutable_location()->CopyFrom(MakePoint(latitude, longitude));
	return f;
}

RouteNote MakeRouteNote(const std::string& message, long latitude, long longitude)
{
	RouteNote n;
	n.set_message(message);
	n.mutable_location()->CopyFrom(MakePoint(latitude, longitude));
	return n;
}

class RouteGuideClient
{
public:
	RouteGuideClient(std::shared_ptr<Channel> channel, const std::string& db) : stub_(RouteGuide::NewStub(channel))
	{
		routeguide::ParseDb(db, &feature_list_);
	}

	void GetFeature()
	{
		Point point;
		Feature feature;
		point = MakePoint(409146138, -746188906);
		if (!GetOneFeature(point, &feature)) {
			std::cout << "############### GetFeature FAIL ###############" << std::endl;
			return;
		}
		point = MakePoint(0, 0);
		if (!GetOneFeature(point, &feature)) {
			std::cout << "############### GetFeature FAIL ###############" << std::endl;
			return;
		}
		std::cout << "############### GetFeature SUCCESS ###############" << std::endl;
	}
	virtual bool GetOneFeature(const Point&, Feature*) = 0;
	virtual void ListFeatures() = 0;
	virtual void RecordRoute() = 0;
	virtual void RouteChat() = 0;

	const float kCoordFactor_ = 10000000.0;
	std::unique_ptr<RouteGuide::Stub> stub_;
	std::vector<Feature> feature_list_;
};

class SyncRouteGuideClient : public RouteGuideClient
{
public:
	SyncRouteGuideClient(std::shared_ptr<Channel> channel, const std::string& db) : RouteGuideClient(channel, db){};

	void ListFeatures()
	{
		routeguide::Rectangle rect;
		Feature feature;
		ClientContext context;

		rect.mutable_lo()->set_latitude(400000000);
		rect.mutable_lo()->set_longitude(-750000000);
		rect.mutable_hi()->set_latitude(420000000);
		rect.mutable_hi()->set_longitude(-730000000);
		std::cout << "Looking for features between 40, -75 and 42, -73" << std::endl;

		std::unique_ptr<ClientReader<Feature>> reader(stub_->ListFeatures(&context, rect));
		while (reader->Read(&feature)) {
			std::cout << "Found feature called " << feature.name() << " at "
					  << feature.location().latitude() / kCoordFactor_ << ", "
					  << feature.location().longitude() / kCoordFactor_ << std::endl;
		}
		Status status = reader->Finish();
		if (status.ok()) {
			std::cout << "############### ListFeatures SUCCESS ###############" << std::endl;
		} else {
			std::cout << "############### ListFeatures FAIL ###############" << std::endl;
		}
	}

	void RecordRoute()
	{
		Point point;
		RouteSummary stats;
		ClientContext context;
		const int kPoints = 10;
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

		std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> feature_distribution(0, feature_list_.size() - 1);
		std::uniform_int_distribution<int> delay_distribution(500, 1500);

		std::unique_ptr<ClientWriter<Point>> writer(stub_->RecordRoute(&context, &stats));
		for (int i = 0; i < kPoints; i++) {
			const Feature& f = feature_list_[feature_distribution(generator)];
			std::cout << "Visiting point " << f.location().latitude() / kCoordFactor_ << ", "
					  << f.location().longitude() / kCoordFactor_ << std::endl;
			if (!writer->Write(f.location())) {
				// Broken stream.
				break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(delay_distribution(generator)));
		}
		writer->WritesDone();
		Status status = writer->Finish();
		if (status.ok()) {
			std::cout << "Finished trip with " << stats.point_count() << " points\n"
					  << "Passed " << stats.feature_count() << " features\n"
					  << "Travelled " << stats.distance() << " meters\n"
					  << "It took " << stats.elapsed_time() << " seconds" << std::endl;
			std::cout << "############### RecordRoute SUCCESS ###############" << std::endl;
		} else {
			std::cout << "############### RecordRoute FAIL ###############" << std::endl;
		}
	}

	void RouteChat()
	{
		ClientContext context;

		std::shared_ptr<ClientReaderWriter<RouteNote, RouteNote>> stream(stub_->RouteChat(&context));

		std::thread writer([stream]() {
			std::vector<RouteNote> notes{MakeRouteNote("First message", 0, 0), MakeRouteNote("Second message", 0, 1),
										 MakeRouteNote("Third message", 1, 0), MakeRouteNote("Fourth message", 0, 0)};
			for (const RouteNote& note : notes) {
				std::cout << "Sending message " << note.message() << " at " << note.location().latitude() << ", "
						  << note.location().longitude() << std::endl;
				stream->Write(note);
			}
			stream->WritesDone();
		});

		RouteNote server_note;
		while (stream->Read(&server_note)) {
			std::cout << "Got message " << server_note.message() << " at " << server_note.location().latitude() << ", "
					  << server_note.location().longitude() << std::endl;
		}
		writer.join();
		Status status = stream->Finish();
		if (status.ok()) {
			std::cout << "############### RouteChat SUCCESS ###############" << std::endl;
		} else {
			std::cout << "############### RouteChat FAIL ###############" << std::endl;
		}
	}

private:
	bool GetOneFeature(const Point& point, Feature* feature)
	{
		ClientContext context;
		Status status = stub_->GetFeature(&context, point, feature);
		if (!status.ok()) {
			std::cout << "GetFeature rpc Failed." << std::endl;
			return false;
		}
		if (!feature->has_location()) {
			std::cout << "Server returns incomplete feature." << std::endl;
			return false;
		}
		if (feature->name().empty()) {
			std::cout << "Found no feature at " << feature->location().latitude() / kCoordFactor_ << ", "
					  << feature->location().longitude() / kCoordFactor_ << std::endl;
		} else {
			std::cout << "Found feature called " << feature->name() << " at "
					  << feature->location().latitude() / kCoordFactor_ << ", "
					  << feature->location().longitude() / kCoordFactor_ << std::endl;
		}
		return true;
	}
};

class AsyncRouteGuideClient : public RouteGuideClient
{
public:
	AsyncRouteGuideClient(std::shared_ptr<Channel> channel, const std::string& db) : RouteGuideClient(channel, db){};

	void ListFeatures()
	{
		routeguide::Rectangle rect;
		Feature feature;
		CompletionQueue cq;
		Status status;
		ClientContext context;

		rect.mutable_lo()->set_latitude(400000000);
		rect.mutable_lo()->set_longitude(-750000000);
		rect.mutable_hi()->set_latitude(420000000);
		rect.mutable_hi()->set_longitude(-730000000);
		std::cout << "Looking for features between 40, -75 and 42, -73" << std::endl;

		std::unique_ptr<ClientAsyncReader<Feature>> reader(stub_->AsyncListFeatures(&context, rect, &cq, (void*)1));

		void* got_tag;
		bool ok = false;
		bool ret;

		ret = cq.Next(&got_tag, &ok);
		if (!ret || !ok || got_tag != (void*)1) {
			std::cout << "RPC initialization Failed" << std::endl;
			return;
		}

		while (1) {
			reader->Read(&feature, got_tag);
			ok = false;
			bool ret = cq.Next(&got_tag, &ok);
			if (!ret || !ok || got_tag != (void*)1) {
				break;
			}
			std::cout << "Found feature called " << feature.name() << " at "
					  << feature.location().latitude() / kCoordFactor_ << ", "
					  << feature.location().longitude() / kCoordFactor_ << std::endl;
		}
		reader->Finish(&status, (void*)1);
		ret = cq.Next(&got_tag, &ok);
		if (!ret || !ok || got_tag != (void*)1) {
			std::cout << "reader Finish is Failed" << std::endl;
			return;
		}

		if (status.ok()) {
			std::cout << "############### Async ListFeatures SUCCESS ###############" << std::endl;
		} else {
			std::cout << "############### Async ListFeatures FAIL ###############" << std::endl;
		}
	}

	void RecordRoute()
	{
		Point point;
		RouteSummary stats;
		ClientContext context;
		CompletionQueue cq;
		const int kPoints = 10;
		Status status;
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

		std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> feature_distribution(0, feature_list_.size() - 1);
		std::uniform_int_distribution<int> delay_distribution(500, 1500);

		std::unique_ptr<ClientAsyncWriter<Point>> writer(stub_->AsyncRecordRoute(&context, &stats, &cq, (void*)1));

		void* got_tag;
		bool ok = false;
		bool ret;

		ret = cq.Next(&got_tag, &ok);
		if (!ret || !ok || got_tag != (void*)1) {
			std::cout << "RPC initialization Failed" << std::endl;
			return;
		}

		for (int i = 0; i < kPoints; i++) {
			const Feature& f = feature_list_[feature_distribution(generator)];
			std::cout << "Visiting point " << f.location().latitude() / kCoordFactor_ << ", "
					  << f.location().longitude() / kCoordFactor_ << std::endl;
			writer->Write(f.location(), got_tag);
			ret = cq.Next(&got_tag, &ok);
			if (!ret || !ok || got_tag != (void*)1) {
				std::cout << "Write Failed" << std::endl;
				break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(delay_distribution(generator)));
		}

		writer->WritesDone(got_tag);
		ret = cq.Next(&got_tag, &ok);
		if (!ret || !ok || got_tag != (void*)1) {
			std::cout << "WritesDone Failed" << std::endl;
			return;
		}

		writer->Finish(&status, (void*)1);
		ret = cq.Next(&got_tag, &ok);
		if (!ret || !ok || got_tag != (void*)1) {
			std::cout << "writer Finish Failed" << std::endl;
			return;
		}

		if (status.ok()) {
			std::cout << "Finished trip with " << stats.point_count() << " points\n"
					  << "Passed " << stats.feature_count() << " features\n"
					  << "Travelled " << stats.distance() << " meters\n"
					  << "It took " << stats.elapsed_time() << " seconds" << std::endl;
			std::cout << "############### Async RecordRoute SUCCESS ###############" << std::endl;
		} else {
			std::cout << "############### Async RecordRoute FAIL ###############" << std::endl;
		}
	}

	void RouteChat()
	{
		ClientContext context;
		CompletionQueue cq;
		Status status;

		std::shared_ptr<ClientAsyncReaderWriter<RouteNote, RouteNote>> stream(
			stub_->AsyncRouteChat(&context, &cq, (void*)1));

		void* got_tag_read;
		bool ok = false;
		bool ret;

		ret = cq.Next(&got_tag_read, &ok);
		if (!ret || !ok || got_tag_read != (void*)1) {
			std::cout << "RPC initialization Failed" << std::endl;
			return;
		}

		std::thread writer([stream, &cq]() {
			bool thd_ok = false;
			bool thd_ret;
			void* got_tag_write;

			std::vector<RouteNote> notes{MakeRouteNote("First message", 0, 0), MakeRouteNote("Second message", 0, 1),
										 MakeRouteNote("Third message", 1, 0), MakeRouteNote("Fourth message", 0, 0)};
			for (const RouteNote& note : notes) {
				std::cout << "Sending message " << note.message() << " at " << note.location().latitude() << ", "
						  << note.location().longitude() << std::endl;
				stream->Write(note, (void*)1);
				thd_ret = cq.Next(&got_tag_write, &thd_ok);
				if (!thd_ret || !thd_ok || got_tag_write != (void*)1) {
					std::cout << "Write Failed" << std::endl;
					break;
				}
			}
			stream->WritesDone(got_tag_write);
			thd_ret = cq.Next(&got_tag_write, &thd_ok);
			if (!thd_ret || !thd_ok || got_tag_write != (void*)1) {
				std::cout << "WriteDone Failed" << std::endl;
			}
		});

		RouteNote server_note;
		while (1) {
			stream->Read(&server_note, (void*)1);
			ret = cq.Next(&got_tag_read, &ok);
			if (!ret || !ok || got_tag_read != (void*)1) {
				break;
			}
			std::cout << "Got message " << server_note.message() << " at " << server_note.location().latitude() << ", "
					  << server_note.location().longitude() << std::endl;
		}
		writer.join();

		stream->Finish(&status, (void*)1);
		ret = cq.Next(&got_tag_read, &ok);
		if (!ret || !ok || got_tag_read != (void*)1) {
			std::cout << "stream Finish Failed" << std::endl;
			return;
		}

		if (status.ok()) {
			std::cout << "############### Async RouteChat SUCCESS ###############" << std::endl;
		} else {
			std::cout << "############### Async RouteChat FAIL ###############" << std::endl;
		}
	}

private:
	bool GetOneFeature(const Point& point, Feature* feature)
	{
		ClientContext context;
		CompletionQueue cq;
		Status status;

		std::unique_ptr<ClientAsyncResponseReader<Feature>> rpc(stub_->AsyncGetFeature(&context, point, &cq));

		rpc->Finish(feature, &status, (void*)1);

		void* got_tag;
		bool ok = false;
		bool ret;

		ret = cq.Next(&got_tag, &ok);
		if (!ret || !ok || got_tag != (void*)1) {
			std::cout << "RPC Failed" << std::endl;
			return false;
		}

		if (!status.ok()) {
			std::cout << "GetFeature Failed." << std::endl;
			return false;
		}

		if (!feature->has_location()) {
			std::cout << "Server returns incomplete feature." << std::endl;
			return false;
		}

		if (feature->name().empty()) {
			std::cout << "Found no feature at " << feature->location().latitude() / kCoordFactor_ << ", "
					  << feature->location().longitude() / kCoordFactor_ << std::endl;
		} else {
			std::cout << "Found feature called " << feature->name() << " at "
					  << feature->location().latitude() / kCoordFactor_ << ", "
					  << feature->location().longitude() / kCoordFactor_ << std::endl;
		}
		return true;
	}
};

int route_client_guide_cb(void* args)
{
	// Expect only arg: --db_path=path/to/route_guide_db.json.
	bool secure_mode = false;
	int sync_mode;
	char* p;
	char* addr;

	struct arg_holder* arg = (struct arg_holder*)args;
	int argc = arg->argc;
	char** argv = arg->argv;

	if (argc < 3 || argc > 4) {
		printf("Usage: \tgrpc_client [MODE] <option> [IP:PORT]\n");
		printf("\tMODE\t\t 1 : sync mode\n");
		printf("\t	  \t 2 : async mode\n");
		printf("\t<option>\t-s : secure mode\n");
		printf("\t<example>\tgrpc_client 1 -s 192.168.1.2:50051\n");
		return 0;
	}

	sync_mode = atoi(argv[1]);

	if (sync_mode != 1 && sync_mode != 2) {
		printf("%s : invalid mode\n", sync_mode);
		return 0;
	}

	for (int i = 2; i < argc; i++) {
		p = argv[i];
		if (strcmp(p, "-s") == 0) {
			secure_mode = true;
			printf("SECURE ON!\n");
		} else {
			addr = p;
			printf("ADDR : %s\n", addr);
		}
	}

	std::shared_ptr<Channel> myChannel;
#ifdef CONFIG_FS_ROMFS
	std::string db = routeguide::GetDbFileContent(0, 0);
#else
	std::string db = JSON_LOCATION_DATA;
#endif
	if (secure_mode) {
		std::shared_ptr<ChannelCredentials> creds;
		grpc::SslCredentialsOptions ssl_opts;
		ssl_opts.pem_root_certs = test_root_cert;
		grpc::ChannelArguments channel_args = grpc::ChannelArguments();
		channel_args.SetSslTargetNameOverride("foo.test.google.fr");
		creds = grpc::SslCredentials(grpc::SslCredentialsOptions(ssl_opts));

		myChannel = grpc::CreateCustomChannel(addr, creds, channel_args);
	} else {
		myChannel = grpc::CreateChannel(addr, grpc::InsecureChannelCredentials());
	}

	RouteGuideClient* client;

	if (sync_mode == 1) {
		printf("Sync Client!\n");
		client = new SyncRouteGuideClient(myChannel, db);
	} else {
		printf("Async Client!\n");
		client = new AsyncRouteGuideClient(myChannel, db);
	}

	std::cout << "-------------- GetFeature --------------" << std::endl;
	client->GetFeature();
	std::cout << "-------------- ListFeatures --------------" << std::endl;
	client->ListFeatures();
	std::cout << "-------------- RecordRoute --------------" << std::endl;
	client->RecordRoute();
	std::cout << "-------------- RouteChat --------------" << std::endl;
	client->RouteChat();

	delete client;

	return 0;
}

/*
 * Definition for handling pthread
 */
#define ROUTE_CLIENT_PRIORITY 100
#define ROUTE_CLIENT_STACK_SIZE 16384
#define ROUTE_CLIENT_SCHED_POLICY SCHED_RR

extern "C" {
int route_client_main(int argc, char** argv)
{
// If C++ initialization for static constructors is supported, then do
// that first
#ifdef CONFIG_EXAMPLES_ROUTE_CLIENT_CXXINITIALIZE
	up_cxxinitialize();
#endif
	setenv("GRPC_VERBOSITY", "DEBUG", 1);
	pthread_t tid;
	pthread_attr_t attr;
	struct sched_param sparam;
	struct arg_holder args;

	args.argc = argc;
	args.argv = argv;

	int r;

	/* 1. set a priority */
	sparam.sched_priority = ROUTE_CLIENT_PRIORITY;
	if ((r = pthread_attr_setschedparam(&attr, &sparam)) != 0) {
		printf("%s: pthread_attr_setschedparam failed, status=%d\n", __func__, r);
		return -1;
	}

	if ((r = pthread_attr_setschedpolicy(&attr, ROUTE_CLIENT_SCHED_POLICY)) != 0) {
		printf("%s: pthread_attr_setschedpolicy failed, status=%d\n", __func__, r);
		return -1;
	}

	/* 2. set a stacksize */
	if ((r = pthread_attr_setstacksize(&attr, ROUTE_CLIENT_STACK_SIZE)) != 0) {
		printf("%s: pthread_attr_setstacksize failed, status=%d\n", __func__, r);
		return -1;
	}

	/* 3. create pthread with entry function */
	if ((r = pthread_create(&tid, &attr, (pthread_startroutine_t)route_client_guide_cb, &args)) != 0) {
		printf("%s: pthread_create failed, status=%d\n", __func__, r);
		return -1;
	}

	/* Wait for the threads to stop */
	pthread_join(tid, NULL);

	return 0;
}
}
