#include <media/RecorderDataSource.hpp>

namespace Media
{
	RecorderDataSource::RecorderDataSource()
		: DataSource()
	{
	}

	RecorderDataSource::RecorderDataSource(int _channels, int _sampleRate, int _pcmFormat, std::string &_dataPath)
		: DataSource(_channels, _sampleRate, _pcmFormat, _dataPath)
	{
	}

	RecorderDataSource::~RecorderDataSource()
	{
	}
}

