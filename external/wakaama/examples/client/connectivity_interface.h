


/*Util Functions*/

/****************************************************************************
 * Name: get_interface_name
 *
 * Description:
 *   Get the underlined interface name
 *   
 *
 * Returned Value:
 *
 ****************************************************************************/
void get_interface_name(char *mac);


/*Read APIs*/
/****************************************************************************
 * Name: get_ip_address
 *
 * Description:
 *   Fetch IPv4/IPv6 address
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
void get_ip_address(char *ipAddr);

/****************************************************************************
 * Name: get_router_ip_address
 *
 * Description:
 *   Get the router IPv4/IPv6 address
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
 void get_router_ip_address(char *routerIPAddr);

/****************************************************************************
 * Name: get_signal_strength
 *
 * Description:
 *  Get Signal Strength value (RSSI in case of IEEE 802.15.4) 
 *
 * Returned Value:
 *   Return the appropriate Callback flags
 *
 ****************************************************************************/
void get_signal_strength(int *rssi_value);

/****************************************************************************
 * Name: get_link_utilization
 *
 * Description:
 *   Get link utilization statictics from MAC/PHY
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
 int get_link_utilization(void);

/****************************************************************************
 * Name: get_lqi
 *
 * Description:
 *   Get link quality index value from MAC/PHY
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
int get_lqi(void);

/****************************************************************************
 * Name: get_tx_data
 *
 * Description:
 *   Get tx data value from MAC/PHY
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
 
int get_tx_data(void);

/****************************************************************************
 * Name: get_rx_data
 *
 * Description:
 *   Get rx data value from MAC/PHY
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
 
int get_rx_data(void);

/****************************************************************************
 * Name: get_max_message_size
 *
 * Description:
 *   Get maximum message size
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
 int get_max_message_size(void);


/****************************************************************************
 * Name: get_bitrate
 *
 * Description:
 *   Get bitrate
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/ 
 void get_bitrate(int *bitrate);


/****************************************************************************
 * Name: get_network_bearer
 *
 * Description:
 *   Get Network Bearer
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/ 
void get_network_bearer(int *nwbearer);

/****************************************************************************
 * Name: get_avl_network_bearer
 *
 * Description:
 *   Get Available Network Bearer
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
void get_avl_network_bearer(int *nwbearer);
