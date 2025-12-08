#ifndef _WPA_LITE_H_
#define _WPA_LITE_H_

#include "rom_secure_common.h"
#include "wpa_lite_intf.h"
#include "rom_wpa_lite_intf.h"
#include "rom_wpa_lite_psk.h"
#include "rom_wpa_lite_sae.h"
#include "wpa_lite_psk.h"
#include "wpa_lite_sae.h"

#define WPA_ERR(...) printf("[WPA][ERROR]" __VA_ARGS__);
#define WPA_WARN(...) printf("[WPA][WARNING]" __VA_ARGS__);
#define WPA_DBG(...) printf("[WPA][DBG]" __VA_ARGS__);


#endif // _WPA_LITE_H_

