#ifndef _TIZENRT_WIRELESS_WIFI_H__
#define _TIZENRT_WIRELESS_WIFI_H__

#define TRWIFI_MACADDR_LEN			  6
#define TRWIFI_MACADDR_STR_LEN		  17
#define TRWIFI_SSID_LEN				  32
#define TRWIFI_PASSPHRASE_LEN		  64

typedef enum {
	TRWIFI_FAIL = -1,
	TRWIFI_SUCCESS,
	TRWIFI_INVALID_ARGS,
	TRWIFI_TIMEOUT,
	TRWIFI_BUSY,
	TRWIFI_FILE_ERROR,
	TRWIFI_ALREADY_CONNECTED,
} trwifi_result_e;

typedef enum {
	TRWIFI_IEEE_80211_LEGACY,			  /**<	IEEE 802.11a/g/b		  */
	TRWIFI_IEEE_80211_A,				  /**<	IEEE 802.11a			  */
	TRWIFI_IEEE_80211_B,				  /**<	IEEE 802.11b			  */
	TRWIFI_IEEE_80211_G,				  /**<	IEEE 802.11g			  */
	TRWIFI_IEEE_80211_N,				  /**<	IEEE 802.11n			  */
	TRWIFI_IEEE_80211_AC,				  /**<	IEEE 802.11ac			  */
	TRWIFI_NOT_SUPPORTED,				  /**<	Driver does not report	  */
} trwifi_standard_type_e;

typedef enum {
	TRWIFI_AUTH_OPEN,					 /**<  open mode								 */
	TRWIFI_AUTH_WEP_SHARED,				 /**<  use shared key (wep key)					 */
	TRWIFI_AUTH_WPA_PSK,					 /**<  WPA_PSK mode								 */
	TRWIFI_AUTH_WPA2_PSK,				 /**<  WPA2_PSK mode							 */
	TRWIFI_AUTH_WPA3_PSK,				 /**<  WPA3_PSK mode							 */
	TRWIFI_AUTH_WPA_AND_WPA2_PSK,		 /**<  WPA_PSK and WPA_PSK mixed mode			 */
	TRWIFI_AUTH_WPA_PSK_ENT,				 /**<  Enterprise WPA_PSK mode					 */
	TRWIFI_AUTH_WPA2_PSK_ENT,			 /**<  Enterprise WPA2_PSK mode					 */
	TRWIFI_AUTH_WPA_AND_WPA2_PSK_ENT,	 /**<  Enterprise WPA_PSK and WPA_PSK mixed mode */
	TRWIFI_AUTH_IBSS_OPEN,				  /**<	IBSS ad-hoc mode						  */
	TRWIFI_AUTH_WPS,						 /**<  WPS mode									 */
	TRWIFI_AUTH_UNKNOWN,					 /**<  unknown type								 */
} trwifi_ap_auth_type_e;

typedef enum {
	TRWIFI_CRYPTO_NONE,					 /**<  none encryption							 */
	TRWIFI_CRYPTO_WEP_64,				 /**<  WEP encryption wep-40					 */
	TRWIFI_CRYPTO_WEP_128,				 /**<  WEP encryption wep-104					 */
	TRWIFI_CRYPTO_AES,					 /**<  AES encryption							 */
	TRWIFI_CRYPTO_TKIP,					 /**<  TKIP encryption							 */
	TRWIFI_CRYPTO_TKIP_AND_AES,			 /**<  TKIP and AES mixed encryption			 */
	TRWIFI_CRYPTO_AES_ENT,				 /**<  Enterprise AES encryption				 */
	TRWIFI_CRYPTO_TKIP_ENT,				 /**<  Enterprise TKIP encryption				 */
	TRWIFI_CRYPTO_TKIP_AND_AES_ENT,		 /**<  Enterprise TKIP and AES mixed encryption	 */
	TRWIFI_CRYPTO_UNKNOWN,				 /**<  unknown encryption						 */
} trwifi_ap_crypto_type_e;

typedef enum {
	TRWIFI_DISCONNECTED,			/**<  wifi is disconnected	*/
	TRWIFI_CONNECTED,			/**<  connected				*/
	TRWIFI_SOFTAP_MODE,			/**<  soft ap mode			*/
} trwifi_status_e;


typedef struct {
	char ssid[TRWIFI_SSID_LEN + 1];				 /**<  Service Set Identification		  */
	unsigned int ssid_length;						/**<  Service Set Identification Length	 */
	char passphrase[TRWIFI_PASSPHRASE_LEN + 1];	 /**<  ap passphrase(password)			  */
	unsigned int passphrase_length;					/**<  ap passphrase length				 */
	trwifi_ap_auth_type_e ap_auth_type;			 /**<  @ref trwifi_ap_auth_type		   */
	trwifi_ap_crypto_type_e ap_crypto_type;		 /**<  @ref trwifi_ap_crypto_type	   */
} trwifi_ap_config_s;

typedef struct {
	unsigned int channel;								   /**<	 Radio channel that the AP beacon was received on		*/
	char ssid[TRWIFI_SSID_LEN + 1];						/**<  Service Set Identification (i.e. Name of Access Point) */
	unsigned int ssid_length;							   /**<	 The length of Service Set Identification				*/
	unsigned char bssid[TRWIFI_MACADDR_STR_LEN + 1];	/**<  MAC address (xx:xx:xx:xx:xx:xx) of Access Point		 */
	unsigned int max_rate;								   /**<	 Maximum data rate in kilobits/s						*/
	int rssi;											   /**<	 Receive Signal Strength Indication in dBm				*/
	trwifi_standard_type_e phy_mode;					/**< Supported MAC/PHY standard								 */
	trwifi_ap_auth_type_e ap_auth_type;					/**<  @ref trwifi_ap_auth_type							  */
	trwifi_ap_crypto_type_e ap_crypto_type;				/**<  @ref trwifi_ap_crypto_type						  */
} trwifi_ap_scan_info_s;

typedef struct {
	unsigned int channel;								   /**<	 soft ap wifi channel				*/
	char ssid[TRWIFI_SSID_LEN + 1];						/**<  Service Set Identification		 */
	unsigned int ssid_length;							   /**<	 Service Set Identification Length	*/
	char passphrase[TRWIFI_PASSPHRASE_LEN + 1];			/**<  ap passphrase(password)			 */
	unsigned int passphrase_length;						   /**<	 ap passphrase length				*/
	trwifi_ap_auth_type_e ap_auth_type;					/**<  @ref trwifi_ap_auth_type		  */
	trwifi_ap_crypto_type_e ap_crypto_type;				/**<  @ref trwifi_ap_crypto_type	  */
	void (*inform_new_sta_join)(void);						/**< @ref new station joining softAP	*/
} trwifi_softap_config_s;

typedef struct trwifi_scan_list {
	trwifi_ap_scan_info_s ap_info;
	struct trwifi_scan_list *next;
} trwifi_scan_list_s;

typedef struct {
	uint32_t ip4_address;								   /**<	 ip4 address							   */
	unsigned char mac_address[TRWIFI_MACADDR_LEN];		/**<  MAC address of wifi interface				*/
	int rssi;											   /**<	 Receive Signal Strength Indication in dBm */
	trwifi_status_e wifi_status;						/**<  @ref trwifi_status					 */
} trwifi_info;

struct netdev;
/*	block call */
typedef trwifi_result_e (*trwifi_init)(struct netdev *dev);
/*	block call */
typedef trwifi_result_e (*trwifi_deinit)(struct netdev *dev);
/*	non-block */
typedef trwifi_result_e (*trwifi_scan_ap)(struct netdev *dev, trwifi_ap_config_s *config);
/*	non-block */
typedef trwifi_result_e (*trwifi_connect_ap)(struct netdev *dev, trwifi_ap_config_s *config, void *arg);
/*	non-block */
typedef trwifi_result_e (*trwifi_disconnect_ap)(struct netdev *dev, void *arg);
/*	block */
typedef trwifi_result_e (*trwifi_get_info)(struct netdev *dev, trwifi_info *info);
/*	block */
typedef trwifi_result_e (*trwifi_start_sta)(struct netdev *dev);
/*	block */
typedef trwifi_result_e (*trwifi_start_softap)(struct netdev *dev, trwifi_softap_config_s *config);
/*	block */
typedef trwifi_result_e (*trwifi_stop_softap)(struct netdev *dev);
/*	block */
typedef trwifi_result_e (*trwifi_set_autoconnect)(struct netdev *dev, uint8_t chk);
/*	block */
typedef trwifi_result_e (*trwifi_drv_ioctl)(struct netdev *dev, int cmd, unsigned long arg);

struct trwifi_ops {
	trwifi_init init;
	trwifi_deinit deinit;
	trwifi_scan_ap scan_ap;
	trwifi_connect_ap connect_ap;
	trwifi_disconnect_ap disconnect_ap;
	trwifi_get_info get_info;
	trwifi_start_sta start_sta;
	trwifi_start_softap start_softap;
	trwifi_stop_softap stop_softap;
	trwifi_set_autoconnect set_autoconnect;
	trwifi_drv_ioctl drv_ioctl;
};

#endif // _TIZENRT_WIRELESS_WIFI_H__
