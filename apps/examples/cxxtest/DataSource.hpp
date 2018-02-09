#ifndef __MEIDAFRAMEWORK_DATASOURCE_HPP
#define __MEIDAFRAMEWORK_DATASOURCE_HPP

namespace Media 
{
	class DataSource
	{
	public:
		DataSource();
		DataSource(int, int, int);
		~DataSource();

	public:
		int getChannels();
		int getSampleRate();
		int getPcmFormat();
		void setChannels(int);
		void setSampleRate(int);
		void setPcmFormat(int);

	private:
		int channels;
		int sampleRate;
		int pcmFormat;
	};
}

#endif