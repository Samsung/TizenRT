#ifndef __MEDIA_DATASOURCE_HPP
#define __MEDIA_DATASOURCE_HPP

#include <iostream>

namespace Media 
{
	class DataSource
	{
	public:
		DataSource();
		DataSource(int, int, int, std::string &);
		~DataSource();

	public:
		int getChannels();
		int getSampleRate();
		int getPcmFormat();
		const std::string& getDataPath() const;

		void setChannels(int);
		void setSampleRate(int);
		void setPcmFormat(int);
		void setDataPath(std::string &);

	private:
		int channels;
		int sampleRate;
		int pcmFormat;
		std::string dataPath;
	};
}

#endif