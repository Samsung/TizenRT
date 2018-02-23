#include <media/DataSource.hpp>

namespace Media 
{
	DataSource::DataSource() : channels(2), sampleRate(16000), pcmFormat(0), dataPath("/mnt/record")
	{
		std::cout << "DataSource constructor" << std::endl;		
	}

	DataSource::DataSource(int _channels, int _sampleRate, int _pcmFormat, std::string& _dataPath)
		: channels(_channels), sampleRate(_sampleRate), pcmFormat(_pcmFormat), dataPath(_dataPath)
	{
		std::cout << "DataSource(int _channels, int _sampleRate, int _pcmFormat) constructor" << std::endl;
	}

	int DataSource::getChannels()
	{
		return channels;
	}

	int DataSource::getSampleRate()
	{
		return sampleRate;
	}

	int DataSource::getPcmFormat()
	{
		return pcmFormat;
	}

	const std::string& DataSource::getDataPath() const
	{
		return dataPath;
	}

	void DataSource::setChannels(int _channels)
	{
		channels = _channels;
	}

	void DataSource::setSampleRate(int _sampleRate)
	{
		sampleRate = _sampleRate;
	}

	void DataSource::setPcmFormat(int _pcmFormat)
	{
		pcmFormat = _pcmFormat;
	}

	DataSource::~DataSource()
	{
	}
}
