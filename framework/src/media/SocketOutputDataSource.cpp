/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <debug.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <media/SocketOutputDataSource.h>

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

namespace media {
namespace stream {

SocketOutputDataSource::SocketOutputDataSource(const std::string& ipAddr, const uint16_t port)
	: OutputDataSource(), mIpAddr(ipAddr), mPort(port), mSockFd(INVALID_SOCKET)
{
}

SocketOutputDataSource::SocketOutputDataSource(unsigned int channels, unsigned int sampleRate, audio_format_type_t pcmFormat, const std::string& ipAddr, const uint16_t port)
	: OutputDataSource(channels, sampleRate, pcmFormat), mIpAddr(ipAddr), mPort(port), mSockFd(INVALID_SOCKET)
{
}

SocketOutputDataSource::SocketOutputDataSource(const SocketOutputDataSource& source) :
	OutputDataSource(source), mIpAddr(source.mIpAddr), mPort(source.mPort), mSockFd(source.mSockFd)
{
}

SocketOutputDataSource& SocketOutputDataSource::operator=(const SocketOutputDataSource& source)
{
	OutputDataSource::operator=(source);
	return *this;
}

bool SocketOutputDataSource::open()
{
	socklen_t addrlen;
	struct sockaddr_in serveraddr;

	if (mSockFd != INVALID_SOCKET) {
		meddbg("Error: Socket is already open\n");
		return false;
	}

	mSockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSockFd == INVALID_SOCKET) {
		meddbg("Error: Fail to open socket\n");
		return false;
	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(mPort);
	serveraddr.sin_addr.s_addr = inet_addr(mIpAddr.c_str());
	addrlen = sizeof(struct sockaddr);

	if (connect(mSockFd, (struct sockaddr *)&serveraddr, addrlen) < 0) {
		meddbg("Errro: Fail to connect socket (errno=%d)\n", errno);
		::close(mSockFd);
		return false;
	}

	medvdbg("Connected to the server, fd = %d\n", mSockFd);

	return true;
}

bool SocketOutputDataSource::close()
{
	if ((mSockFd != INVALID_SOCKET) && ::close(mSockFd) != EOF) {
		mSockFd = INVALID_SOCKET;
		return true;
	}

	return false;
}

bool SocketOutputDataSource::isPrepare()
{
	return (mSockFd != INVALID_SOCKET);
}

ssize_t SocketOutputDataSource::write(unsigned char* buf, size_t size)
{
	if (size == 0) {
		return 0;
	}

	if (!buf) {
		return EOF;
	}

	return send(mSockFd, buf, size, 0);
}

SocketOutputDataSource::~SocketOutputDataSource()
{
}

} // namespace stream
} // namespace media
