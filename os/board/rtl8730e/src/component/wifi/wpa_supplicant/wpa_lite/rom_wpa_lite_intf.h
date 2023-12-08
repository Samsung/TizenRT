#ifndef _ROM_WPA_LITE_INTF_H_
#define _ROM_WPA_LITE_INTF_H_

#define PMK_LEN					32
#define PMKID_LEN				16

#define LIB1X_ETHER_EAPOL_TYPE	0x888E

#define LIB1X_EAPOL_VER				1	//0000 0001B
#define LIB1X_EAPOL_EAPPKT			0       //0000 0000B
#define LIB1X_EAPOL_START			1	//0000 0001B
#define LIB1X_EAPOL_LOGOFF			2       //0000 0010B
#define LIB1X_EAPOL_KEY				3	//0000 0011B
#define LIB1X_EAPOL_ENCASFALERT			4	//0000 0100B


enum {
	PSK_WPA		= 1,
	PSK_WPA2	= 2
};

union pn48	{

	u64	val;
	struct {
		u8 TSC0;
		u8 TSC1;
		u8 TSC2;
		u8 TSC3;
		u8 TSC4;
		u8 TSC5;
		u8 TSC6;
		u8 TSC7;
	} _byte_;
};

struct lib1x_eapol {
	unsigned char		protocol_version;
	unsigned char		packet_type;			// This makes it odd in number !
	unsigned short		packet_body_length;
};


#endif // _ROM_WPA_LITE_INTF_H_

