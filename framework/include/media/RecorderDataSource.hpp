#ifndef __MEDIA_RECORDDATASOURCE_HPP
#define __MEDIA_RECORDDATASOURCE_HPP

#include "DataSource.hpp"

namespace Media
{
	class RecorderDataSource : public DataSource
	{
	public:
		RecorderDataSource();
		RecorderDataSource(int _channels, int _sampleRate, int _pcmFormat, std::string &);
		~RecorderDataSource();
	};
}

#endif

