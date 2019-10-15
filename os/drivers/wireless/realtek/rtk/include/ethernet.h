/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
/*! \file */
#ifndef __INC_ETHERNET_H
#define __INC_ETHERNET_H

#define ETHERNET_ADDRESS_LENGTH 6		  //!< Ethernet Address Length
#define ETHERNET_HEADER_SIZE 14			  //!< Ethernet Header Length
#define LLC_HEADER_SIZE 6				  //!< LLC Header Length
#define TYPE_LENGTH_FIELD_SIZE 2		  //!< Type/Length Size
#define MINIMUM_ETHERNET_PACKET_SIZE 60   //!< Minimum Ethernet Packet Size
#define MAXIMUM_ETHERNET_PACKET_SIZE 1514 //!< Maximum Ethernet Packet Size

#define RT_ETH_IS_MULTICAST(_pAddr) ((((u8 *)(_pAddr))[0] & 0x01) != 0) //!< Is Multicast Address?
#define RT_ETH_IS_BROADCAST(_pAddr)(\
	((u8 *)(_pAddr))[0] == 0xff &&  \
	((u8 *)(_pAddr))[1] == 0xff &&  \
	((u8 *)(_pAddr))[2] == 0xff &&  \
	((u8 *)(_pAddr))[3] == 0xff &&  \
	((u8 *)(_pAddr))[4] == 0xff &&  \
	((u8 *)(_pAddr))[5] == 0xff) //!< Is Broadcast Address?

#endif // #ifndef __INC_ETHERNET_H
