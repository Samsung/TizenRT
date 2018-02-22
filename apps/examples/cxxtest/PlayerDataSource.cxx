#include "PlayerDataSource.hpp"

namespace Media
{
	PlayerDataSource::PlayerDataSource()
	{
	}

	PlayerDataSource::PlayerDataSource(int _channels, int _sampleRate, int _pcmFormat, std::string &_dataPath)
		: DataSource(_channels, _sampleRate, _pcmFormat, _dataPath)
	{
	}

	PlayerDataSource::~PlayerDataSource()
	{
	}
}
