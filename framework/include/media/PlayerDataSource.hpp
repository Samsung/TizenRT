#ifndef __MEDIA_PLAYERDATASOURCE_HPP
#define __MEDIA_PLAYERDATASOURCE_HPP

#include "DataSource.hpp"

namespace Media
{
	class PlayerDataSource : public DataSource
	{
	public:
		PlayerDataSource();
		PlayerDataSource(int _channels, int _sampleRate, int _pcmFormat, std::string &);
		~PlayerDataSource();
	};
}

#endif
