#include "RecorderDataSource.hpp"

namespace Media
{
	RecorderDataSource::RecorderDataSource()
	{
	}

	RecorderDataSource::RecorderDataSource(int _channels, int _sampleRate, int _pcmFormat)
		: DataSource(_channels, _sampleRate, _pcmFormat)
	{
	}

	RecorderDataSource::~RecorderDataSource()
	{
	}
}

