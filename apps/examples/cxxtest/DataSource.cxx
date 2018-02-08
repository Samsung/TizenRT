#include "DataSource.hpp"

namespace Media 
{
	DataSource::DataSource() : channels(2), sampleRate(16000), pcmFormat(0)
	{
	}

	DataSource::DataSource(int _channels, int _sampleRate, int _pcmFormat)
		: channels(_channels), sampleRate(_sampleRate), pcmFormat(_pcmFormat)
	{

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
