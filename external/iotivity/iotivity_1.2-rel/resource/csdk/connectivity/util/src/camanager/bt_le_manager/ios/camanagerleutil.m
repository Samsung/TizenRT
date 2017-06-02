/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include "cacommon.h"
#include "logger.h"
#include "cacommonutil.h"
#include "camanagerleutil.h"
#include "caleclient.h"
#include "caleutils.h"

#define TAG "OIC_CA_MANAGER_UTIL"

static const NSString *NSUSERDEFULATS_KEY = @"acList_dictionary";
static NSMutableDictionary *g_connectedDeviceSet = NULL;
static NSMutableDictionary *g_peripherals = NULL;

NSUserDefaults* CAManagerGetSharedPreference()
{
    return [NSUserDefaults standardUserDefaults];
}

bool CAManagerUpdatePrefStringSet(NSMutableDictionary *set)
{
    OIC_LOG(DEBUG, TAG, "IN-CAManagerUpdatePrefStringSet");
    VERIFY_NON_NULL_RET(set, TAG, "set", false);

    NSUserDefaults *userDefaults = CAManagerGetSharedPreference();
    @synchronized (userDefaults) {
        NSData *contentData = [NSKeyedArchiver archivedDataWithRootObject:set];
        [userDefaults setObject:contentData forKey:NSUSERDEFULATS_KEY];
    }

    return [userDefaults synchronize];
}

NSMutableDictionary* CAManagerGetPrefStringSet()
{
    OIC_LOG(DEBUG, TAG, "CAManagerGetPrefStringSet");

    NSUserDefaults *userDefaults = CAManagerGetSharedPreference();

    NSMutableDictionary *set = nil;

    if (userDefaults) {
        NSData *contentData = [[userDefaults objectForKey:NSUSERDEFULATS_KEY] mutableCopy];
        if (contentData) {
            set = (NSMutableDictionary*) [NSKeyedUnarchiver unarchiveObjectWithData:contentData];
        }
    }

    return [set copy];
}

bool CAManagerContainsPrefStringSet()
{
    OIC_LOG(DEBUG, TAG, "CAManagerContainsPrefStringSet");

    NSUserDefaults *userDefaults = CAManagerGetSharedPreference();

    NSData *data = nil;

    if (userDefaults) {
        data = [userDefaults dataForKey:NSUSERDEFULATS_KEY];
    }

    return data != nil ? true : false;
}

bool CAManagerRemovePrefStringSet()
{
    OIC_LOG(DEBUG, TAG, "CAManagerRemovePrefStringSet");

    NSUserDefaults *userDefaults = CAManagerGetSharedPreference();
    @synchronized (userDefaults) {
        [userDefaults removeObjectForKey:NSUSERDEFULATS_KEY];
    }

    return [userDefaults synchronize];
}

CBPeripheral* CAManagerGetValueConnectedDeviceAddress(const char* address)
{
    OIC_LOG(DEBUG, TAG, "IN-CAManagerGetValueConnectedDeviceAddress");

    VERIFY_NON_NULL_RET(address, TAG, "address", nil);

    NSString *ns_address = [NSString stringWithUTF8String:address];

    if (![g_connectedDeviceSet objectForKey:ns_address] || ![g_peripherals objectForKey:ns_address])
    {
        OIC_LOG(DEBUG, TAG, "it's un-registered one");
        return false;
    }

    return (CBPeripheral *)g_peripherals[ns_address];
}

bool CAManagerSetValueConnectedDeviceAddress(CBPeripheral *peripheral, const char* address)
{
    OIC_LOG(DEBUG, TAG, "IN-CAManagerSetValueConnectedDeviceAddress");

    VERIFY_NON_NULL_RET(address, TAG, "address", false);

    NSString *ns_address = [NSString stringWithUTF8String:address];

    if ([g_connectedDeviceSet objectForKey:ns_address] && [g_peripherals objectForKey:ns_address])
    {
        OIC_LOG(DEBUG, TAG, "it's already done");
        return true;
    }

    NSUUID *peri_identifier =[[NSUUID alloc] initWithUUIDString:
                              [[peripheral identifier] UUIDString]];

    [g_connectedDeviceSet setObject:peri_identifier forKey:ns_address];
    [g_peripherals setObject:peripheral forKey:ns_address];

    return true;
    //return CAManagerUpdatePrefStringSet(g_connectedDeviceSet);
}

bool CAManagerAddConnectedDeviceAddress(const char* address)
{
    OIC_LOG(DEBUG, TAG, "IN-CAManagerAddConnectedDeviceAddress");

    VERIFY_NON_NULL_RET(address, TAG, "address", false);

    NSString *ns_address = [NSString stringWithUTF8String:address];

    if ([g_connectedDeviceSet objectForKey:ns_address] && [g_peripherals objectForKey:ns_address])
    {
        OIC_LOG(DEBUG, TAG, "it's already done");
        return true;
    }

    [g_connectedDeviceSet setObject:[NSNull null] forKey:ns_address];
    [g_peripherals setObject:[NSNull null] forKey:ns_address];

    return true;
    //return CAManagerUpdatePrefStringSet(g_connectedDeviceSet);
}

bool CAManagerIsConnectedDeviceAddress(const char* address)
{
    OIC_LOG(DEBUG, TAG, "CAManagerIsConnectedDeviceAddress");

    VERIFY_NON_NULL_RET(address, TAG, "address", false);

    NSString *ns_address = [NSString stringWithUTF8String:address];

    return ([g_connectedDeviceSet objectForKey:ns_address] != nil &&
            [g_peripherals objectForKey:ns_address] != nil) ? true : false;
}

bool CAManagerGetConnectedDeviceAddress()
{
    OIC_LOG(DEBUG, TAG, "CAManagerGetConnectedDeviceAddress");

    return false;
}

bool CAManagerRemoveConnectedDeviceAddress(const char* address)
{
    OIC_LOG(DEBUG, TAG, "IN-CAManagerRemoveConnectedDeviceAddress");

    VERIFY_NON_NULL_RET(address, TAG, "address", false);

    NSString *ns_address = [NSString stringWithUTF8String:address];

    if (![g_connectedDeviceSet objectForKey:ns_address])
    {
        OIC_LOG(ERROR, TAG, "it's already done");
        return true;
    }

    if (![g_peripherals objectForKey:ns_address])
    {
        OIC_LOG(ERROR, TAG, "identifier OR peripheral set is wrong");
        return false;
    }

    [g_peripherals removeObjectForKey:ns_address];
    [g_connectedDeviceSet removeObjectForKey:ns_address];

    return true;
}

static bool ishexdigit(char var)
{
    if(var <= '9')
    {
        if(var >= '0') return true;
        else           return false;
    }
    else if(var <= 'F')
    {
        if(var >= 'A') return true;
        else           return false;
    }
    else if(var <= 'f')
    {
        if(var >= 'a') return true;
        else           return false;
    }

    return false;
}

bool CAManagerCheckBTAddress(const char *address)
{
    OIC_LOG(DEBUG, TAG, "CAManagerCheckBTAddress");

    VERIFY_NON_NULL_RET(address, TAG, "address is null", false);

    /*
    * Check if the provided MAC address is valid.
    * 1. The length of MAC address should always be 17.
    * 2. Hyphens are expected at positions {3, 6, 9, 12, 15}.
    * 3. The rest characters should be simple xdigits.
    */
    int hyphens[5] = {3, 6, 9, 12, 15};
    if (strlen(address) != 17)
    {
        return false;//Oops. The lenth doesn't match.
    }

    for (int i = 0, counter = 0; i < 17; i ++)
    {
        char var = address[i];
        if (i == hyphens[counter] - 1)// Check if a hyphen is expected here.
        {
            // Yep. We need a hyphen here.
            if (var != ':')
            {
                return false;// Oops. The character is not a hyphen.
            }
            else
            {
                counter++;// Move on to the next expected hyphen position.
            }
        }
        else
        {
            // Nope. The character here should be a simple xdigit
            if (ishexdigit(var) == 0)
            {
                return false;// Oops. The current character is not a hyphen.
            }
        }
    }
    return true;// Seen'em all!
}

bool CAManagerCreateSetString()
{
    OIC_LOG(DEBUG, TAG, "CAManagerCreateSetString");

    g_connectedDeviceSet = [[NSMutableDictionary alloc] init];
    g_peripherals = [[NSMutableDictionary alloc] init];

    return (g_connectedDeviceSet != nil && g_peripherals != nil) ? true : false;
}

CAResult_t CAManagerTerminateSetString()
{
    OIC_LOG(DEBUG, TAG, "CAManagerTerminateSetString");

    [g_connectedDeviceSet removeAllObjects];
    [g_connectedDeviceSet release];

    [g_peripherals removeAllObjects];
    [g_peripherals release];

    g_connectedDeviceSet = nil;
    g_peripherals = nil;

    return (g_connectedDeviceSet == nil && g_peripherals == nil) ? CA_STATUS_OK : CA_STATUS_FAILED;
}