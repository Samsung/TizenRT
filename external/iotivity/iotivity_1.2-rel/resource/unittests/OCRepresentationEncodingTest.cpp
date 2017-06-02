//******************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <gtest/gtest.h>
#include <OCApi.h>
#include <OCRepresentation.h>
#include <octypes.h>
#include <ocstack.h>
#include <ocpayload.h>
#include <ocpayloadcbor.h>
#include <oic_malloc.h>
#include <oic_string.h>
#include "payload_logging.h"

bool operator==(const OCByteString& lhs, const OCByteString& rhs)
{
    bool result = (lhs.len == rhs.len);
    if (result)
    {
        result = (memcmp(lhs.bytes, rhs.bytes, lhs.len) == 0);
    }
    return result;
}

namespace OC
{
    bool operator==(const OC::NullType&, const OC::NullType&)
    {
        return true;
    }

    bool operator==(const OC::OCRepresentation& lhs, const OC::OCRepresentation& rhs)
    {
        return lhs.getUri() == rhs.getUri() &&
            lhs.getChildren() == rhs.getChildren() &&
            lhs.getResourceInterfaces() == rhs.getResourceInterfaces() &&
            lhs.getResourceTypes() == rhs.getResourceTypes() &&
            lhs.m_values == rhs.m_values;
    }
}
// these tests validate the OCRepresentation->OCPayload, OCPayload->CBOR,
// CBOR->OCPayload and OCPayload->OCRepresentation conversions
namespace OCRepresentationEncodingTest
{
    static const char *sid1;
    // Device Payloads
    TEST(DeviceDiscoveryEncoding, Normal)
    {
        sid1 = OCGetServerInstanceIDString();
        const char devicename1[] = "device name";
        OCRepPayload *device = OCRepPayloadCreate();
        EXPECT_TRUE(device);
        EXPECT_TRUE(OCRepPayloadAddResourceType(device, OC_RSRVD_RESOURCE_TYPE_DEVICE));
        EXPECT_TRUE(OCRepPayloadAddResourceType(device, "oic.d.tv"));
        EXPECT_TRUE(OCRepPayloadSetPropString(device, OC_RSRVD_DEVICE_ID, sid1));
        EXPECT_TRUE(OCRepPayloadSetPropString(device, OC_RSRVD_DEVICE_NAME, devicename1));
        EXPECT_TRUE(OCRepPayloadSetPropString(device, OC_RSRVD_SPEC_VERSION, OC_SPEC_VERSION));
        EXPECT_TRUE(OCRepPayloadSetPropString(device, "x.org.iotivity.newproperty", "value"));

        size_t dim[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};
        char **dt = (char **)OICMalloc(sizeof(char *) * 1);
        EXPECT_TRUE(dt);
        dt[0] = OICStrdup(OC_DATA_MODEL_VERSION);
        EXPECT_TRUE(OCRepPayloadSetStringArray(device, OC_RSRVD_DATA_MODEL_VERSION, (const char **)dt, dim));
        OICFree(dt[0]);
        OICFree(dt);

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* parsedDevice;
        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)device, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&parsedDevice, PAYLOAD_TYPE_REPRESENTATION, cborData, cborSize));
        OICFree(cborData);

        OCRepPayload *parsedRep = (OCRepPayload *)parsedDevice;
        char *value = NULL;
        EXPECT_TRUE(OCRepPayloadGetPropString(parsedRep, OC_RSRVD_DEVICE_ID, &value));
        EXPECT_STREQ(sid1, value);
        OICFree(value);
        EXPECT_TRUE(OCRepPayloadGetPropString(parsedRep, OC_RSRVD_DEVICE_NAME, &value));
        EXPECT_STREQ(devicename1, value);
        OICFree(value);
        EXPECT_TRUE(OCRepPayloadGetPropString(parsedRep, OC_RSRVD_SPEC_VERSION, &value));
        EXPECT_STREQ(OC_SPEC_VERSION, value);
        OICFree(value);
        EXPECT_TRUE(OCRepPayloadGetPropString(device, "x.org.iotivity.newproperty", &value));
        EXPECT_STREQ("value", value);
        OICFree(value);
        char **dmv = NULL;
        EXPECT_TRUE(OCRepPayloadGetStringArray(parsedRep, OC_RSRVD_DATA_MODEL_VERSION, &dmv, dim));
        EXPECT_STREQ(OC_DATA_MODEL_VERSION, dmv[0]);
        OICFree(dmv[0]);
        OICFree(dmv);
        EXPECT_STREQ("oic.wk.d", parsedRep->types->value);
        EXPECT_STREQ("oic.d.tv", parsedRep->types->next->value);
        EXPECT_EQ(device->base.type, parsedRep->base.type);

        OCPayloadDestroy((OCPayload*)device);

        OC::MessageContainer mc1;
        mc1.setPayload(parsedDevice);
        EXPECT_EQ(1u, mc1.representations().size());
        const OC::OCRepresentation &r1 = mc1.representations()[0];
        EXPECT_STREQ(sid1, r1.getValue<std::string>(OC_RSRVD_DEVICE_ID).c_str());
        EXPECT_STREQ(devicename1, r1.getValue<std::string>(OC_RSRVD_DEVICE_NAME).c_str());
        EXPECT_STREQ(OC_SPEC_VERSION, r1.getValue<std::string>(OC_RSRVD_SPEC_VERSION).c_str());
        EXPECT_STREQ("value", r1.getValue<std::string>("x.org.iotivity.newproperty").c_str());
        std::vector<std::string> dmv2 = r1.getValue<std::vector<std::string>>(OC_RSRVD_DATA_MODEL_VERSION);
        EXPECT_STREQ(OC_DATA_MODEL_VERSION, dmv2[0].c_str());

        OCPayloadDestroy(parsedDevice);

        device = OCRepPayloadCreate();
        EXPECT_TRUE(device);
        EXPECT_TRUE(OCRepPayloadAddResourceType(device, OC_RSRVD_RESOURCE_TYPE_DEVICE));
        EXPECT_TRUE(OCRepPayloadAddResourceType(device, "oic.d.tv"));
        EXPECT_TRUE(OCRepPayloadSetPropString(device, OC_RSRVD_DEVICE_NAME, devicename1));
        EXPECT_TRUE(OCRepPayloadSetPropString(device, OC_RSRVD_DEVICE_ID, sid1));
        EXPECT_TRUE(OCRepPayloadSetPropString(device, OC_RSRVD_SPEC_VERSION, OC_SPEC_VERSION));
        size_t dim1[MAX_REP_ARRAY_DEPTH] = {2, 0, 0};
        char **dt1 = (char **)OICMalloc(sizeof(char *) * 2);
        EXPECT_TRUE(dt1);
        dt1[0] = OICStrdup("res.1.1.0");
        dt1[1] = OICStrdup("sh.1.1.0");
        OCRepPayloadSetStringArray(device, OC_RSRVD_DATA_MODEL_VERSION, (const char**)dt1, dim1);
        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload *)device, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&parsedDevice, PAYLOAD_TYPE_REPRESENTATION, cborData, cborSize));
        OICFree(cborData);
        OICFree(dt1[0]);
        OICFree(dt1[1]);
        OICFree(dt1);
        char **dmv1 = NULL;
        parsedRep = (OCRepPayload *)parsedDevice;
        EXPECT_TRUE(OCRepPayloadGetStringArray(parsedRep, OC_RSRVD_DATA_MODEL_VERSION, &dmv1, dim));
        EXPECT_STREQ("res.1.1.0", dmv1[0]);
        EXPECT_STREQ("sh.1.1.0", dmv1[1]);
        OICFree(dmv1[0]);
        OICFree(dmv1[1]);
        OICFree(dmv1);
        OCPayloadDestroy((OCPayload *)device);
        OC::MessageContainer mc2;
        mc2.setPayload(parsedDevice);
        EXPECT_EQ(1u, mc2.representations().size());
        const OC::OCRepresentation r2 = mc2.representations()[0];
        std::vector<std::string> dmv3 = r2.getValue<std::vector<std::string>>(OC_RSRVD_DATA_MODEL_VERSION);
        EXPECT_STREQ("res.1.1.0", dmv3[0].c_str());
        EXPECT_STREQ("sh.1.1.0", dmv3[1].c_str());
        OCPayloadDestroy(parsedDevice);
    }

    // Platform Payloads
    TEST(PlatformDiscoveryEncoding, Normal)
    {
        static char pfid1[] = "646F6F72-4465-7669-6365-555549443030";
        static char mfgnm1[] = "mfgnm";
        static char mfgurl1[] = "http://www.iotivity.org";
        static char modelnum1[] = "modelnum";
        static char dom1[] = "dom";
        static char pfver1[] = "pfver";
        static char osver1[] = "osver";
        static char hwver1[] = "hwver";
        static char fwver1[] = "fwver";
        static char url1[] = "http://www.iotivity.org";
        static char time1[] = "20161122T143938Z";
        static char vid1[] = "Manufacturer Freeform Text";
        OCRepPayload *platform = OCRepPayloadCreate();
        EXPECT_TRUE(platform != NULL);
        EXPECT_EQ(PAYLOAD_TYPE_REPRESENTATION, ((OCPayload*)platform)->type);
        EXPECT_TRUE(OCRepPayloadAddResourceType(platform, (char *)OC_RSRVD_RESOURCE_TYPE_PLATFORM));
        EXPECT_TRUE(OCRepPayloadAddInterface(platform, (char *)OC_RSRVD_INTERFACE_DEFAULT));
        EXPECT_TRUE(OCRepPayloadAddInterface(platform, (char *)OC_RSRVD_INTERFACE_READ));
        EXPECT_TRUE(OCRepPayloadSetPropString(platform, OC_RSRVD_PLATFORM_ID, pfid1));
        EXPECT_TRUE(OCRepPayloadSetPropString(platform, OC_RSRVD_MFG_NAME, mfgnm1));
        EXPECT_TRUE(OCRepPayloadSetPropString(platform, OC_RSRVD_MFG_URL, mfgurl1));
        EXPECT_TRUE(OCRepPayloadSetPropString(platform, OC_RSRVD_MODEL_NUM, modelnum1));
        EXPECT_TRUE(OCRepPayloadSetPropString(platform, OC_RSRVD_MFG_DATE, dom1));
        EXPECT_TRUE(OCRepPayloadSetPropString(platform, OC_RSRVD_OS_VERSION, osver1));
        EXPECT_TRUE(OCRepPayloadSetPropString(platform, OC_RSRVD_PLATFORM_VERSION, pfver1));
        EXPECT_TRUE(OCRepPayloadSetPropString(platform, OC_RSRVD_HARDWARE_VERSION, hwver1));
        EXPECT_TRUE(OCRepPayloadSetPropString(platform, OC_RSRVD_FIRMWARE_VERSION, fwver1));
        EXPECT_TRUE(OCRepPayloadSetPropString(platform, OC_RSRVD_SUPPORT_URL, url1));
        EXPECT_TRUE(OCRepPayloadSetPropString(platform, OC_RSRVD_SYSTEM_TIME, time1));
        EXPECT_TRUE(OCRepPayloadSetPropString(platform, OC_RSRVD_VID, vid1));


        uint8_t* cborData;
        size_t cborSize;
        OCPayload* parsedPlatform = NULL;
        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)platform, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&parsedPlatform, PAYLOAD_TYPE_REPRESENTATION, cborData, cborSize));
        OICFree(cborData);

        OCRepPayload *platform1 = (OCRepPayload *)parsedPlatform;
        EXPECT_EQ(platform->base.type, platform1->base.type);
        char *value = NULL;
        EXPECT_TRUE(OCRepPayloadGetPropString(platform1, OC_RSRVD_PLATFORM_ID, &value));
        EXPECT_STREQ(pfid1, value);
        OICFree(value);

        EXPECT_TRUE(OCRepPayloadGetPropString(platform1, OC_RSRVD_MFG_NAME, &value));
        EXPECT_STREQ(mfgnm1, value);
        OICFree(value);

        EXPECT_TRUE(OCRepPayloadGetPropString(platform1, OC_RSRVD_MFG_URL, &value));
        EXPECT_STREQ(mfgurl1, value);
        OICFree(value);

        EXPECT_TRUE(OCRepPayloadGetPropString(platform1, OC_RSRVD_MODEL_NUM, &value));
        EXPECT_STREQ(modelnum1, value);
        OICFree(value);

        EXPECT_TRUE(OCRepPayloadGetPropString(platform1, OC_RSRVD_MFG_DATE, &value));
        EXPECT_STREQ(dom1, value);
        OICFree(value);

        EXPECT_TRUE(OCRepPayloadGetPropString(platform1, OC_RSRVD_OS_VERSION, &value));
        EXPECT_STREQ(osver1, value);
        OICFree(value);

        EXPECT_TRUE(OCRepPayloadGetPropString(platform1, OC_RSRVD_PLATFORM_VERSION, &value));
        EXPECT_STREQ(pfver1, value);
        OICFree(value);

        EXPECT_TRUE(OCRepPayloadGetPropString(platform1, OC_RSRVD_HARDWARE_VERSION, &value));
        EXPECT_STREQ(hwver1, value);
        OICFree(value);

        EXPECT_TRUE(OCRepPayloadGetPropString(platform1, OC_RSRVD_FIRMWARE_VERSION, &value));
        EXPECT_STREQ(fwver1, value);
        OICFree(value);

        EXPECT_TRUE(OCRepPayloadGetPropString(platform1, OC_RSRVD_SUPPORT_URL, &value));
        EXPECT_STREQ(url1, value);
        OICFree(value);

        EXPECT_TRUE(OCRepPayloadGetPropString(platform1, OC_RSRVD_SYSTEM_TIME, &value));
        EXPECT_STREQ(time1, value);
        OICFree(value);

        EXPECT_TRUE(OCRepPayloadGetPropString(platform1, OC_RSRVD_VID, &value));
        EXPECT_STREQ(vid1, value);
        OICFree(value);

        EXPECT_STREQ(platform1->types->value, OC_RSRVD_RESOURCE_TYPE_PLATFORM);
        EXPECT_STREQ(platform1->interfaces->value, OC_RSRVD_INTERFACE_DEFAULT);
        EXPECT_STREQ(platform1->interfaces->next->value, OC_RSRVD_INTERFACE_READ);

        OC::MessageContainer mc;
        mc.setPayload(parsedPlatform);
        EXPECT_EQ(1u, mc.representations().size());
        const OC::OCRepresentation& r = mc.representations()[0];
        EXPECT_STREQ(pfid1, r.getValue<std::string>(OC_RSRVD_PLATFORM_ID).c_str());
        EXPECT_STREQ(mfgnm1, r.getValue<std::string>(OC_RSRVD_MFG_NAME).c_str());
        EXPECT_STREQ(mfgurl1, r.getValue<std::string>(OC_RSRVD_MFG_URL).c_str());
        EXPECT_STREQ(modelnum1, r.getValue<std::string>(OC_RSRVD_MODEL_NUM).c_str());
        EXPECT_STREQ(dom1, r.getValue<std::string>(OC_RSRVD_MFG_DATE).c_str());
        EXPECT_STREQ(pfver1, r.getValue<std::string>(OC_RSRVD_PLATFORM_VERSION).c_str());
        EXPECT_STREQ(osver1, r.getValue<std::string>(OC_RSRVD_OS_VERSION).c_str());
        EXPECT_STREQ(hwver1, r.getValue<std::string>(OC_RSRVD_HARDWARE_VERSION).c_str());
        EXPECT_STREQ(fwver1, r.getValue<std::string>(OC_RSRVD_FIRMWARE_VERSION).c_str());
        EXPECT_STREQ(url1, r.getValue<std::string>(OC_RSRVD_SUPPORT_URL).c_str());
        EXPECT_STREQ(time1, r.getValue<std::string>(OC_RSRVD_SYSTEM_TIME).c_str());
        EXPECT_STREQ(vid1, r.getValue<std::string>(OC_RSRVD_VID).c_str());

        OCPayloadDestroy((OCPayload *)platform);
        OCPayloadDestroy((OCPayload *)platform1);
    }

    TEST(PresencePayload, Normal)
    {
        static const char uri1[] = "/testuri";
        uint32_t maxAge = 0;
        uint32_t sequenceNumber = 0;
        OCPresenceTrigger trigger = OC_PRESENCE_TRIGGER_CREATE;
        OCPresencePayload *presence = OCPresencePayloadCreate(sequenceNumber, maxAge, trigger, uri1);

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;
        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)presence, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_PRESENCE,
                    cborData, cborSize));
        OCPayloadDestroy((OCPayload*)presence);
        OICFree(cborData);

        OCPresencePayload* parsed = ((OCPresencePayload*)cparsed);
        EXPECT_EQ(sequenceNumber, parsed->sequenceNumber);
        EXPECT_EQ(maxAge, parsed->maxAge);
        EXPECT_EQ(trigger, parsed->trigger);
        EXPECT_STREQ(uri1, parsed->resourceType);

        OCPayloadDestroy(cparsed);
    }
    // Representation Payloads
    TEST(RepresentationEncoding, BaseAttributeTypes)
    {
        OC::OCRepresentation startRep;
        startRep.setNULL("NullAttr");
        startRep.setValue("IntAttr", 77);
        startRep.setValue("DoubleAttr", 3.333);
        startRep.setValue("BoolAttr", true);
        startRep.setValue("StringAttr", std::string("String attr"));

        uint8_t binval[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8,
                            0x9, 0x0, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF
                           };
        OCByteString byteString = {binval, sizeof(binval)};
        startRep.setValue("ByteStringAttr", byteString);

        OC::MessageContainer mc1;
        mc1.addRepresentation(startRep);

        OCRepPayload* cstart = mc1.getPayload();
        EXPECT_EQ(PAYLOAD_TYPE_REPRESENTATION, cstart->base.type);

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;
        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)cstart, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_REPRESENTATION,
                    cborData, cborSize));
        OCPayloadDestroy((OCPayload*)cstart);
        OICFree(cborData);

        OC::MessageContainer mc2;
        mc2.setPayload(cparsed);
        EXPECT_EQ(1u, mc2.representations().size());
        const OC::OCRepresentation& r = mc2.representations()[0];

        EXPECT_TRUE(r.isNULL("NullAttr"));
        EXPECT_EQ(77, r.getValue<int>("IntAttr"));
        EXPECT_EQ(3.333, r.getValue<double>("DoubleAttr"));
        EXPECT_EQ(true, r.getValue<bool>("BoolAttr"));
        EXPECT_STREQ("String attr", r.getValue<std::string>("StringAttr").c_str());
        const char *expectedByteString = "\\x1\\x2\\x3\\x4\\x5\\x6\\x7\\x8\\x9\\x0\\xa\\xb\\xc\\xd\\xe\\xf";
        EXPECT_STREQ(expectedByteString, r.getValueToString("ByteStringAttr").c_str());

        OCPayloadDestroy(cparsed);
    }

    TEST(RepresentationEncoding, RepAttributeEmpty)
    {
        OC::OCRepresentation startRep;
        std::vector<int> iarr {};
        std::vector<double> darr {};
        std::vector<bool> barr {};
        std::vector<std::string> strarr {};
        std::vector<OC::OCRepresentation> objarr {};
        std::vector<OCByteString> bytestrarr {{NULL, 0}};
        startRep.setValue("StringAttr", std::string(""));
        startRep["iarr"] = iarr;
        startRep["darr"] = darr;
        startRep["barr"] = barr;
        startRep["strarr"] = strarr;
        startRep["objarr"] = objarr;
        startRep["bytestrarr"] = bytestrarr;
        startRep.setValue("StringAttr2", std::string("String attr"));

        OC::MessageContainer mc1;
        mc1.addRepresentation(startRep);
        OCRepPayload* cstart = mc1.getPayload();
        EXPECT_EQ(PAYLOAD_TYPE_REPRESENTATION, cstart->base.type);

        uint8_t *cborData = NULL;
        size_t cborSize = 0;
        OCPayload *cparsed = NULL;
        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)cstart, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_REPRESENTATION,
                    cborData, cborSize));
        OCPayloadDestroy((OCPayload*)cstart);
        OICFree(cborData);

        OC::MessageContainer mc2;
        mc2.setPayload(cparsed);
        EXPECT_EQ(1u, mc2.representations().size());
        const OC::OCRepresentation& r = mc2.representations()[0];

        EXPECT_STREQ("", r.getValue<std::string>("StringAttr").c_str());
        std::vector<int> iarr2 = r["iarr"];
        EXPECT_EQ(iarr, iarr2);
        std::vector<double> darr2 = r["darr"];
        EXPECT_EQ(darr, darr2);
        std::vector<bool> barr2 = r["barr"];
        EXPECT_EQ(barr, barr2);
        std::vector<std::string> strarr2 = r["strarr"];
        EXPECT_EQ(strarr, strarr2);
        std::vector<OC::OCRepresentation> objarr2 = r["objarr"];
        EXPECT_EQ(objarr, objarr2);
        std::vector<uint8_t> binAttr = r.getValue<std::vector<uint8_t>>("BinaryAttr");
        EXPECT_EQ(bytestrarr[0].len, binAttr.size());
        EXPECT_STREQ("String attr", r.getValue<std::string>("StringAttr2").c_str());
        OIC_LOG_PAYLOAD(DEBUG, cparsed);
        OCPayloadDestroy(cparsed);
    }

    TEST(RepresentationEncoding, RepAttribute)
    {
        OC::OCRepresentation startRep;
        OC::OCRepresentation subRep;
        subRep.setNULL("NullAttr");
        subRep.setValue("IntAttr", 77);
        subRep.setValue("DoubleAttr", 3.333);
        subRep.setValue("BoolAttr", true);
        subRep.setValue("StringAttr", std::string("String attr"));
        std::vector<uint8_t> bin_data {5,3,4,5,6,0,34,2,4,5,6,3};
        subRep.setValue("BinaryAttr", bin_data);
        startRep.setValue("Sub", subRep);

        OC::MessageContainer mc1;
        mc1.addRepresentation(startRep);

        OCRepPayload* cstart = mc1.getPayload();
        EXPECT_EQ(PAYLOAD_TYPE_REPRESENTATION, cstart->base.type);

        uint8_t* cborData;
        size_t cborSize;
        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)cstart, &cborData, &cborSize));
        OCPayload* cparsed;
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_REPRESENTATION,
                    cborData, cborSize));
        OCPayloadDestroy((OCPayload*)cstart);
        OICFree(cborData);

        OC::MessageContainer mc2;
        mc2.setPayload(cparsed);
        EXPECT_EQ(1u, mc2.representations().size());
        const OC::OCRepresentation& r = mc2.representations()[0];

        OC::OCRepresentation newSubRep = r["Sub"];

        EXPECT_TRUE(newSubRep.isNULL("NullAttr"));
        EXPECT_EQ(77, newSubRep.getValue<int>("IntAttr"));
        EXPECT_EQ(3.333, newSubRep.getValue<double>("DoubleAttr"));
        EXPECT_EQ(true, newSubRep.getValue<bool>("BoolAttr"));
        EXPECT_STREQ("String attr", newSubRep.getValue<std::string>("StringAttr").c_str());
        EXPECT_EQ(bin_data,
                newSubRep.getValue<std::vector<uint8_t>>("BinaryAttr"));
        OCPayloadDestroy(cparsed);
    }

    TEST(RepresentationEncoding, OneDVectors)
    {
        // Setup
        OC::OCRepresentation startRep;

        OC::OCRepresentation subRep1;
        OC::OCRepresentation subRep2;
        OC::OCRepresentation subRep3;
        subRep1.setNULL("NullAttr");
        subRep1.setValue("IntAttr", 77);
        subRep2.setValue("DoubleAttr", 3.333);
        subRep2.setValue("BoolAttr", true);
        subRep3.setValue("StringAttr", std::string("String attr"));

        std::vector<int> iarr {1,2,3,4,5,6,7,8,9};
        std::vector<double> darr {1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9};
        std::vector<bool> barr {false, true, false, false, true, true};
        std::vector<std::string> strarr {"item1", "item2", "item3", "item4"};
        std::vector<OC::OCRepresentation> objarr {subRep1, subRep2, subRep3};

        uint8_t binval1[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8};
        OCByteString byteStringRef1 {binval1, sizeof(binval1)};
        OCByteString byteString1 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString1, &byteStringRef1));
        uint8_t binval2[] = {0x9, 0x0, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};
        OCByteString byteStringRef2 {binval2, sizeof(binval2)};
        OCByteString byteString2 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString2, &byteStringRef2));
        std::vector<OCByteString> bytestrarrRef {byteStringRef1, byteStringRef2 };
        std::vector<OCByteString> bytestrarr {byteString1, byteString2 };

        startRep["iarr"] = iarr;
        startRep["darr"] = darr;
        startRep["barr"] = barr;
        startRep["strarr"] = strarr;
        startRep["objarr"] = objarr;
        startRep["bytestrarr"] = bytestrarr;

        // Encode/decode
        OC::MessageContainer mc1;
        mc1.addRepresentation(startRep);

        OCRepPayload* cstart = mc1.getPayload();
        EXPECT_EQ(PAYLOAD_TYPE_REPRESENTATION, cstart->base.type);

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;
        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload *)cstart, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_REPRESENTATION,
                                              cborData, cborSize));
        OCPayloadDestroy((OCPayload *)cstart);
        OICFree(cborData);

        OC::MessageContainer mc2;
        mc2.setPayload(cparsed);
        EXPECT_EQ(1u, mc2.representations().size());
        const OC::OCRepresentation &r = mc2.representations()[0];

        // Test
        std::vector<int> iarr2 = r["iarr"];
        std::vector<double> darr2 = r["darr"];
        std::vector<bool> barr2 = r["barr"];
        std::vector<std::string> strarr2 = r["strarr"];
        std::vector<OC::OCRepresentation> objarr2 = r["objarr"];
        std::vector<OCByteString> bytestrarr2 = r["bytestrarr"];

        EXPECT_EQ(iarr, iarr2);
        EXPECT_EQ(darr, darr2);
        EXPECT_EQ(barr, barr2);
        EXPECT_EQ(strarr, strarr2);
        EXPECT_EQ(objarr, objarr2);

        EXPECT_EQ(bytestrarrRef, bytestrarr2);
        OCPayloadDestroy(cparsed);
    }

    TEST(RepresentationEncoding, TwoDVectors)
    {
        // Setup
        OC::OCRepresentation startRep;

        OC::OCRepresentation subRep1;
        OC::OCRepresentation subRep2;
        OC::OCRepresentation subRep3;
        subRep1.setNULL("NullAttr");
        subRep1.setValue("IntAttr", 77);
        subRep2.setValue("DoubleAttr", 3.333);
        subRep2.setValue("BoolAttr", true);
        subRep3.setValue("StringAttr", std::string("String attr"));

        std::vector<std::vector<int>> iarr {{1,2,3},{4,5,6},{7,8,9}};
        std::vector<std::vector<double>> darr {{1.1,2.2,3.3},{4.4,5.5,6.6},{7.7,8.8,9.9}};
        std::vector<std::vector<bool>> barr {{false, true}, {false, false}, {true, true}};
        std::vector<std::vector<std::string>> strarr {{"item1", "item2"}, {"item3", "item4"}};
        std::vector<std::vector<OC::OCRepresentation>> objarr
        {{subRep1, subRep2, subRep3}, {subRep3, subRep2, subRep1}};

        uint8_t binval1[] = {0x1, 0x2, 0x3, 0x4};
        OCByteString byteStringRef1 {binval1, sizeof(binval1) };
        OCByteString byteString1 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString1, &byteStringRef1));
        uint8_t binval2[] = {0x5, 0x6, 0x7, 0x8};
        OCByteString byteStringRef2 {binval2, sizeof(binval2) };
        OCByteString byteString2 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString2, &byteStringRef2));
        uint8_t binval3[] = {0x9, 0x0, 0xA, 0xB};
        OCByteString byteStringRef3 {binval3, sizeof(binval3) };
        OCByteString byteString3 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString3, &byteStringRef3));
        uint8_t binval4[] = {0xC, 0xD, 0xE, 0xF};
        OCByteString byteStringRef4 {binval4, sizeof(binval4) };
        OCByteString byteString4 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString4, &byteStringRef4));
        std::vector<std::vector<OCByteString>> bytestrarrRef
        {
            {byteStringRef1, byteStringRef2}, {byteStringRef3, byteStringRef4}
        };
        std::vector<std::vector<OCByteString>> bytestrarr
        {
            {byteString1, byteString2}, {byteString3, byteString4}
        };

        startRep["iarr"] = iarr;
        startRep["darr"] = darr;
        startRep["barr"] = barr;
        startRep["strarr"] = strarr;
        startRep["objarr"] = objarr;
        startRep["bytestrarr"] = bytestrarr;

        // Encode/decode
        OC::MessageContainer mc1;
        mc1.addRepresentation(startRep);

        OCRepPayload *cstart = mc1.getPayload();
        EXPECT_EQ(PAYLOAD_TYPE_REPRESENTATION, cstart->base.type);

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;
        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload *)cstart, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_REPRESENTATION,
                                              cborData, cborSize));
        OCPayloadDestroy((OCPayload *)cstart);
        OICFree(cborData);

        OC::MessageContainer mc2;
        mc2.setPayload(cparsed);
        EXPECT_EQ(1u, mc2.representations().size());
        const OC::OCRepresentation &r = mc2.representations()[0];

        // Test
        std::vector<std::vector<int>> iarr2 = r["iarr"];
        std::vector<std::vector<double>> darr2 = r["darr"];
        std::vector<std::vector<bool>> barr2 = r["barr"];
        std::vector<std::vector<std::string>> strarr2 = r["strarr"];
        std::vector<std::vector<OC::OCRepresentation>> objarr2 = r["objarr"];
        std::vector<std::vector<OCByteString>> bytestrarr2 = r["bytestrarr"];

        EXPECT_EQ(iarr, iarr2);
        EXPECT_EQ(darr, darr2);
        EXPECT_EQ(barr, barr2);
        EXPECT_EQ(strarr, strarr2);
        EXPECT_EQ(objarr, objarr2);

        EXPECT_EQ(bytestrarrRef, bytestrarr2);

        OCPayloadDestroy(cparsed);
    }

    TEST(RepresentationEncoding, TwoDVectorsJagged)
    {
        // Setup
        OC::OCRepresentation startRep;

        OC::OCRepresentation subRep1;
        OC::OCRepresentation subRep2;
        OC::OCRepresentation subRep3;
        subRep1.setNULL("NullAttr");
        subRep1.setValue("IntAttr", 77);
        subRep2.setValue("DoubleAttr", 3.333);
        subRep2.setValue("BoolAttr", true);
        subRep3.setValue("StringAttr", std::string("String attr"));

        std::vector<std::vector<int>> iarr {{1,2,3},{4,6},{7,8,9}};
        std::vector<std::vector<double>> darr {{1.1,2.2,3.3},{4.4,5.5,6.6},{8.8,9.9}};
        std::vector<std::vector<bool>> barr {{false, true}, {false}, {true, true}};
        std::vector<std::vector<std::string>> strarr {{"item1"}, {"item3", "item4"}};
        std::vector<std::vector<OC::OCRepresentation>> objarr
        {{subRep1, subRep3}, {subRep3, subRep2, subRep1}};

        uint8_t binval1[] = {0x1};
        OCByteString byteStringRef1 {binval1, sizeof(binval1) };
        OCByteString byteString1 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString1, &byteStringRef1));
        uint8_t binval3[] = {0x2, 0x3, 0x4};
        OCByteString byteStringRef3 {binval3, sizeof(binval3) };
        OCByteString byteString3 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString3, &byteStringRef3));
        uint8_t binval4[] = {0x5, 0x6, 0x7, 0x8};
        OCByteString byteStringRef4 {binval4, sizeof(binval4) };
        OCByteString byteString4 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString4, &byteStringRef4));

        std::vector<std::vector<OCByteString>> bytestrarrRef
        {
            {byteStringRef1}, {byteStringRef3, byteStringRef4}
        };

        std::vector<std::vector<OCByteString>> bytestrarr
        {
            {byteString1}, {byteString3, byteString4}
        };

        startRep["iarr"] = iarr;
        startRep["darr"] = darr;
        startRep["barr"] = barr;
        startRep["strarr"] = strarr;
        startRep["objarr"] = objarr;

        startRep["bytestrarr"] = bytestrarr;

        EXPECT_STREQ("[[\\x1 ] [\\x2\\x3\\x4 \\x5\\x6\\x7\\x8 ] ]",
                     startRep.getValueToString("bytestrarr").c_str());
        // Encode/decode
        OC::MessageContainer mc1;
        mc1.addRepresentation(startRep);

        OCRepPayload *cstart = mc1.getPayload();
        EXPECT_EQ(PAYLOAD_TYPE_REPRESENTATION, cstart->base.type);

        uint8_t *cborData;
        size_t cborSize;
        OCPayload *cparsed;
        OCStackResult result = OCConvertPayload((OCPayload *)cstart, &cborData, &cborSize);
        EXPECT_EQ(OC_STACK_OK, result);
        result = OCParsePayload(&cparsed, PAYLOAD_TYPE_REPRESENTATION,
                                cborData, cborSize);
        EXPECT_EQ(OC_STACK_OK, result);

        OCPayloadDestroy((OCPayload *)cstart);
        OICFree(cborData);

        OC::MessageContainer mc2;
        mc2.setPayload(cparsed);
        EXPECT_EQ(1u, mc2.representations().size());
        const OC::OCRepresentation &r = mc2.representations()[0];

        // Test
        std::vector<std::vector<int>> iarr2 = r["iarr"];
        std::vector<std::vector<double>> darr2 = r["darr"];
        std::vector<std::vector<bool>> barr2 = r["barr"];
        std::vector<std::vector<std::string>> strarr2 = r["strarr"];
        std::vector<std::vector<OC::OCRepresentation>> objarr2 = r["objarr"];

        std::vector<std::vector<OCByteString>> bytestrarr2 = r["bytestrarr"];

        // Note: due to the way that the CSDK works, all 2d arrays need to be rectangular.
        // Since std::vector doesn't require this, items received on the other side end up
        // being backfilled.  This section removes the backfilling
        iarr2[1].pop_back();
        darr2[2].pop_back();
        barr2[1].pop_back();
        strarr2[0].pop_back();
        objarr2[0].pop_back();
        bytestrarr2[0].pop_back();

        EXPECT_EQ(iarr, iarr2);
        EXPECT_EQ(darr, darr2);
        EXPECT_EQ(barr, barr2);
        EXPECT_EQ(strarr, strarr2);
        EXPECT_EQ(objarr, objarr2);
        EXPECT_EQ(bytestrarr.size(), bytestrarr2.size());
        EXPECT_EQ(bytestrarrRef, bytestrarr2);

        OCPayloadDestroy(cparsed);
    }

    TEST(RepresentationEncoding, ThreeDVectors)
    {
        // Setup
        OC::OCRepresentation startRep;

        OC::OCRepresentation subRep1;
        OC::OCRepresentation subRep2;
        OC::OCRepresentation subRep3;
        subRep1.setNULL("NullAttr");
        subRep1.setValue("IntAttr", 77);
        subRep2.setValue("DoubleAttr", 3.333);
        subRep2.setValue("BoolAttr", true);
        subRep3.setValue("StringAttr", std::string("String attr"));

        std::vector<std::vector<std::vector<int>>> iarr
            {{{1,2,3},{4,5,6}},{{7,8,9},{10,11,12}},{{13,14,15},{16,17,18}}};
        std::vector<std::vector<std::vector<double>>> darr
            {{{1.1,2.2,3.3},{4.4,5.5,6.6}},
                {{7.7,8.7,9.7},{10.7,11.7,12.7}},
                {{13.7,14.7,15.7},{16.7,17.7,18.7}}};
        std::vector<std::vector<std::vector<bool>>> barr
            {{{false, true},{true, false}},{{false, true},{true, false}}};
        std::vector<std::vector<std::vector<std::string>>> strarr
            {
                {{"item1", "item2"},{"item3", "item4"}},
                {{"item5", "item6"},{"item7", "item8"}},
                {{"item9", "item10"},{"item11", ""}}
            };
        std::vector<std::vector<std::vector<OC::OCRepresentation>>> objarr
            {
                {{subRep1, subRep2},{subRep3, subRep1}},
                {{subRep2, subRep3},{subRep2, subRep1}},
                {{subRep3, subRep2},{subRep1, subRep2}}
            };

        uint8_t binval1[] = {0x1, 0x2, 0x3, 0x4};
        OCByteString byteStringRef1 {binval1, sizeof(binval1)};
        OCByteString byteString1 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString1, &byteStringRef1));
        uint8_t binval2[] = {0x5, 0x6, 0x7, 0x8};
        OCByteString byteStringRef2 {binval2, sizeof(binval2)};
        OCByteString byteString2 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString2, &byteStringRef2));
        uint8_t binval3[] = {0x9, 0x0, 0xA, 0xB};
        OCByteString byteStringRef3 {binval3, sizeof(binval3)};
        OCByteString byteString3 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString3, &byteStringRef3));
        uint8_t binval4[] = {0xC, 0xD, 0xE, 0xF};
        OCByteString byteStringRef4 {binval4, sizeof(binval4)};
        OCByteString byteString4 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString4, &byteStringRef4));
        uint8_t binval5[] = {0x11, 0x12, 0x13, 0x14};
        OCByteString byteStringRef5 {binval5, sizeof(binval5)};
        OCByteString byteString5 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString5, &byteStringRef5));
        uint8_t binval6[] = {0x15, 0x16, 0x17, 0x18};
        OCByteString byteStringRef6 {binval6, sizeof(binval6)};
        OCByteString byteString6 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString6, &byteStringRef6));
        uint8_t binval7[] = {0x19, 0x10, 0x1A, 0x1B};
        OCByteString byteStringRef7 {binval7, sizeof(binval7)};
        OCByteString byteString7 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString7, &byteStringRef7));
        uint8_t binval8[] = {0x1C, 0x1D, 0x1E, 0x1F};
        OCByteString byteStringRef8 {binval8, sizeof(binval8)};
        OCByteString byteString8 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString8, &byteStringRef8));
        uint8_t binval9[] = {0x21, 0x22, 0x23, 0x24};
        OCByteString byteStringRef9 {binval9, sizeof(binval9)};
        OCByteString byteString9 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString9, &byteStringRef9));
        uint8_t binval10[] = {0x25, 0x26, 0x27, 0x28};
        OCByteString byteStringRef10 {binval10, sizeof(binval10)};
        OCByteString byteString10 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString10, &byteStringRef10));
        uint8_t binval11[] = {0x29, 0x20, 0x2A, 0x2B};
        OCByteString byteStringRef11 {binval11, sizeof(binval11)};
        OCByteString byteString11 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString11, &byteStringRef11));
        uint8_t binval12[] = {0xFF};
        OCByteString byteStringRef12 {binval12, sizeof(binval12)};
        OCByteString byteString12 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString12, &byteStringRef12));

        std::vector<std::vector<std::vector<OCByteString>>> bytestrarrRef
        {
            {{byteStringRef1, byteStringRef2}, {byteStringRef3, byteStringRef4}},
            {{byteStringRef5, byteStringRef6}, {byteStringRef7, byteStringRef8}},
            {{byteStringRef9, byteStringRef10}, {byteStringRef11, byteStringRef12}}
        };

        std::vector<std::vector<std::vector<OCByteString>>> bytestrarr
        {
            {{byteString1, byteString2}, {byteString3, byteString4}},
            {{byteString5, byteString6}, {byteString7, byteString8}},
            {{byteString9, byteString10}, {byteString11, byteString12}}
        };

        startRep["iarr"] = iarr;
        startRep["darr"] = darr;
        startRep["barr"] = barr;
        startRep["strarr"] = strarr;
        startRep["objarr"] = objarr;
        startRep["bytestrarr"] = bytestrarr;

        // Encode/decode
        OC::MessageContainer mc1;
        mc1.addRepresentation(startRep);

        OCRepPayload *cstart = mc1.getPayload();
        EXPECT_EQ(PAYLOAD_TYPE_REPRESENTATION, cstart->base.type);

        uint8_t *cborData;
        size_t cborSize;
        OCPayload *cparsed;
        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload *)cstart, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_REPRESENTATION,
                                              cborData, cborSize));
        OCPayloadDestroy((OCPayload *)cstart);
        OICFree(cborData);

        OC::MessageContainer mc2;
        mc2.setPayload(cparsed);
        EXPECT_EQ(1u, mc2.representations().size());
        const OC::OCRepresentation &r = mc2.representations()[0];

        // Test
        std::vector<std::vector<std::vector<int>>> iarr2 = r["iarr"];
        std::vector<std::vector<std::vector<double>>> darr2 = r["darr"];
        std::vector<std::vector<std::vector<bool>>> barr2 = r["barr"];
        std::vector<std::vector<std::vector<std::string>>> strarr2 = r["strarr"];
        std::vector<std::vector<std::vector<OC::OCRepresentation>>> objarr2 = r["objarr"];
        std::vector<std::vector<std::vector<OCByteString>>> bytestrarr2 = r["bytestrarr"];

        EXPECT_EQ(iarr, iarr2);
        EXPECT_EQ(darr, darr2);
        EXPECT_EQ(barr, barr2);
        EXPECT_EQ(strarr, strarr2);
        EXPECT_EQ(objarr, objarr2);
        EXPECT_EQ(bytestrarrRef, bytestrarr2);
        OCPayloadDestroy(cparsed);
    }

    TEST(RepresentationEncoding, ThreeDVectorsJagged)
    {
        // Setup
        OC::OCRepresentation startRep;

        OC::OCRepresentation subRep1;
        OC::OCRepresentation subRep2;
        OC::OCRepresentation subRep3;
        subRep1.setNULL("NullAttr");
        subRep1.setValue("IntAttr", 77);
        subRep2.setValue("DoubleAttr", 3.333);
        subRep2.setValue("BoolAttr", true);
        subRep3.setValue("StringAttr", std::string("String attr"));

        std::vector<std::vector<std::vector<int>>> iarr
            {
                {{1,2,3},{4,5,6}},
                {{7,8,9},{10,12}},
                {{13,14,15},{16,17,18}}
            };
        std::vector<std::vector<std::vector<double>>> darr
            {
                {{1.1,2.2,3.3},{4.4,5.5,6.6}},
                {{7.7,8.7,9.7},{10.7,12.7}},
                {{13.7,14.7,15.7},{16.7,17.7,18.7}}
            };
        std::vector<std::vector<std::vector<bool>>> barr
            {
                {{false, true},{true}},
                {{false, true},{true, false}}
            };
        std::vector<std::vector<std::vector<std::string>>> strarr
            {
                {{"item1", "item2"},{"item3", "item4"}},
                {{"item5", "item6"},{"item8"}},
                {{"item9", "item10"},{"item11", ""}}
            };
        std::vector<std::vector<std::vector<OC::OCRepresentation>>> objarr
            {
                {{subRep1, subRep2},{subRep3, subRep1}},
                {{subRep2, subRep3},{subRep2}},
                {{subRep3, subRep2}}
            };

        uint8_t binval1[] = {0x1, 0x2, 0x3, 0x4};
        OCByteString byteStringRef1 {binval1, sizeof(binval1)};
        OCByteString byteString1 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString1, &byteStringRef1));
        uint8_t binval2[] = {0x5, 0x6, 0x7, 0x8};
        OCByteString byteStringRef2 {binval2, sizeof(binval2)};
        OCByteString byteString2 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString2, &byteStringRef2));
        uint8_t binval3[] = {0x9, 0x0, 0xA, 0xB};
        OCByteString byteStringRef3 {binval3, sizeof(binval3)};
        OCByteString byteString3 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString3, &byteStringRef3));
        uint8_t binval4[] = {0xC, 0xD, 0xE, 0xF};
        OCByteString byteStringRef4 {binval4, sizeof(binval4)};
        OCByteString byteString4 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString4, &byteStringRef4));
        uint8_t binval5[] = {0x11, 0x12, 0x13, 0x14};
        OCByteString byteStringRef5 {binval5, sizeof(binval5)};
        OCByteString byteString5 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString5, &byteStringRef5));
        uint8_t binval6[] = {0x15, 0x16, 0x17, 0x18};
        OCByteString byteStringRef6 {binval6, sizeof(binval6)};
        OCByteString byteString6 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString6, &byteStringRef6));
        uint8_t binval8[] = {0x1C, 0x1D, 0x1E, 0x1F};
        OCByteString byteStringRef8 {binval8, sizeof(binval8)};
        OCByteString byteString8 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString8, &byteStringRef8));
        uint8_t binval9[] = {0x21, 0x22, 0x23, 0x24};
        OCByteString byteStringRef9 {binval9, sizeof(binval9)};
        OCByteString byteString9 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString9, &byteStringRef9));
        uint8_t binval10[] = {0x25, 0x26, 0x27, 0x28};
        OCByteString byteStringRef10 {binval10, sizeof(binval10)};
        OCByteString byteString10 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString10, &byteStringRef10));
        uint8_t binval11[] = {0x29, 0x20, 0x2A, 0x2B};
        OCByteString byteStringRef11 {binval11, sizeof(binval11)};
        OCByteString byteString11 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString11, &byteStringRef11));
        uint8_t binval12[] = {0xFF};
        OCByteString byteStringRef12 {binval12, sizeof(binval12)};
        OCByteString byteString12 {NULL,0};
        EXPECT_TRUE(OCByteStringCopy(&byteString12, &byteStringRef12));

        std::vector<std::vector<std::vector<OCByteString>>> bytestrarrRef
        {
            {{byteStringRef1, byteStringRef2}, {byteStringRef3, byteStringRef4}},
            {{byteStringRef5, byteStringRef6}, {byteStringRef8}},
            {{byteStringRef9, byteStringRef10}, {byteStringRef11, byteStringRef12}}
        };

        std::vector<std::vector<std::vector<OCByteString>>> bytestrarr
        {
            {{byteString1, byteString2}, {byteString3, byteString4}},
            {{byteString5, byteString6}, {byteString8}},
            {{byteString9, byteString10}, {byteString11, byteString12}}
        };

        startRep["iarr"] = iarr;
        startRep["darr"] = darr;
        startRep["barr"] = barr;
        startRep["strarr"] = strarr;
        startRep["objarr"] = objarr;
        startRep["bytestrarr"] = bytestrarr;

        // Encode/decode
        OC::MessageContainer mc1;
        mc1.addRepresentation(startRep);

        OCRepPayload *cstart = mc1.getPayload();
        EXPECT_EQ(PAYLOAD_TYPE_REPRESENTATION, cstart->base.type);

        uint8_t *cborData;
        size_t cborSize;
        OCPayload *cparsed;
        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload *)cstart, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_REPRESENTATION,
                                              cborData, cborSize));
        OCPayloadDestroy((OCPayload *)cstart);
        OICFree(cborData);

        OC::MessageContainer mc2;
        mc2.setPayload(cparsed);
        EXPECT_EQ(1u, mc2.representations().size());
        const OC::OCRepresentation &r = mc2.representations()[0];

        // Test
        std::vector<std::vector<std::vector<int>>> iarr2 = r["iarr"];
        std::vector<std::vector<std::vector<double>>> darr2 = r["darr"];
        std::vector<std::vector<std::vector<bool>>> barr2 = r["barr"];
        std::vector<std::vector<std::vector<std::string>>> strarr2 = r["strarr"];
        std::vector<std::vector<std::vector<OC::OCRepresentation>>> objarr2 = r["objarr"];
        std::vector<std::vector<std::vector<OCByteString>>> bytestrarr2 = r["bytestrarr"];

        // Note: due to the way that the CSDK works, all 3d arrays need to be cuboidal.
        // Since std::vector doesn't require this, items received on the other side end up
        // being backfilled.  This section removes the backfilling
        iarr2[1][1].pop_back();
        darr2[1][1].pop_back();
        barr2[0][1].pop_back();
        strarr2[1][1].pop_back();
        objarr2[1][1].pop_back();
        objarr2[2].pop_back();
        bytestrarr2[1][1].pop_back();

        EXPECT_EQ(iarr, iarr2);
        EXPECT_EQ(darr, darr2);
        EXPECT_EQ(barr, barr2);
        EXPECT_EQ(strarr, strarr2);
        EXPECT_EQ(objarr, objarr2);
        EXPECT_EQ(bytestrarrRef, bytestrarr2);
        OCPayloadDestroy(cparsed);
    }

    TEST(DiscoveryRTandIF, SingleItemNormal)
    {
        OCDiscoveryPayload* payload = OCDiscoveryPayloadCreate();
        OCResourcePayload* resource = (OCResourcePayload*)OICCalloc(1, sizeof(OCResourcePayload));
        payload->sid = OICStrdup(sid1);
        payload->resources = resource;

        EXPECT_TRUE(OCResourcePayloadAddStringLL(&resource->types, "rt.singleitem"));
        EXPECT_TRUE(OCResourcePayloadAddStringLL(&resource->interfaces, "if.singleitem"));

        resource->uri = OICStrdup("/uri/thing");

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;

        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)payload, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_DISCOVERY,
                    cborData, cborSize));

        EXPECT_EQ(1u, OCDiscoveryPayloadGetResourceCount((OCDiscoveryPayload*)cparsed));
        OCResourcePayload* parsedResource = ((OCDiscoveryPayload*)cparsed)->resources;

        EXPECT_STREQ(sid1, ((OCDiscoveryPayload*)cparsed)->sid);
        EXPECT_EQ(NULL, parsedResource->next);
        EXPECT_STREQ("rt.singleitem", parsedResource->types->value);
        EXPECT_EQ(NULL, parsedResource->types->next);
        EXPECT_EQ(NULL, parsedResource->interfaces->next);
        EXPECT_STREQ("if.singleitem", parsedResource->interfaces->value);

        OICFree(cborData);
        OCPayloadDestroy(cparsed);
        OCDiscoveryPayloadDestroy(payload);
    }

    TEST(DiscoveryRTandIF, SingleItemFrontTrim)
    {
        OCDiscoveryPayload* payload = OCDiscoveryPayloadCreate();
        payload->sid = OICStrdup(sid1);
        OCResourcePayload* resource = (OCResourcePayload*)OICCalloc(1, sizeof(OCResourcePayload));
        payload->resources = resource;

        EXPECT_TRUE(OCResourcePayloadAddStringLL(&resource->types, "    rt.singleitem"));
        EXPECT_TRUE(OCResourcePayloadAddStringLL(&resource->interfaces, "    if.singleitem"));
        resource->uri = OICStrdup("/uri/thing");

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;

        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)payload, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_DISCOVERY,
                    cborData, cborSize));

        EXPECT_EQ(1u, OCDiscoveryPayloadGetResourceCount((OCDiscoveryPayload*)cparsed));
        OCResourcePayload* parsedResource = ((OCDiscoveryPayload*)cparsed)->resources;

        EXPECT_EQ(NULL, parsedResource->next);
        EXPECT_STREQ(sid1, ((OCDiscoveryPayload*)cparsed)->sid);
        EXPECT_EQ(NULL, parsedResource->types->next);
        EXPECT_STREQ("rt.singleitem", parsedResource->types->value);
        EXPECT_EQ(NULL, parsedResource->interfaces->next);
        EXPECT_STREQ("if.singleitem", parsedResource->interfaces->value);

        OICFree(cborData);
        OCPayloadDestroy(cparsed);
        OCDiscoveryPayloadDestroy(payload);
    }

    TEST(DiscoveryRTandIF, SingleItemBackTrim)
    {
        OCDiscoveryPayload* payload = OCDiscoveryPayloadCreate();
        payload->sid = OICStrdup(sid1);
        OCResourcePayload* resource = (OCResourcePayload*)OICCalloc(1, sizeof(OCResourcePayload));
        payload->resources = resource;

        OCResourcePayloadAddStringLL(&resource->types, "rt.singleitem    ");
        OCResourcePayloadAddStringLL(&resource->interfaces, "if.singleitem    ");
        resource->uri = OICStrdup("/uri/thing");

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;

        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)payload, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_DISCOVERY,
                    cborData, cborSize));

        EXPECT_EQ(1u, OCDiscoveryPayloadGetResourceCount((OCDiscoveryPayload*)cparsed));
        OCResourcePayload* parsedResource = ((OCDiscoveryPayload*)cparsed)->resources;

        EXPECT_EQ(NULL, parsedResource->next);
        EXPECT_STREQ(sid1, ((OCDiscoveryPayload*)cparsed)->sid);
        EXPECT_EQ(NULL, parsedResource->types->next);
        EXPECT_STREQ("rt.singleitem", parsedResource->types->value);
        EXPECT_EQ(NULL, parsedResource->interfaces->next);
        EXPECT_STREQ("if.singleitem", parsedResource->interfaces->value);

        OICFree(cborData);
        OCPayloadDestroy(cparsed);
        OCDiscoveryPayloadDestroy(payload);
    }
    TEST(DiscoveryRTandIF, SingleItemBothTrim)
    {
        OCDiscoveryPayload* payload = OCDiscoveryPayloadCreate();
        payload->sid = OICStrdup(sid1);
        OCResourcePayload* resource = (OCResourcePayload*)OICCalloc(1, sizeof(OCResourcePayload));
        payload->resources = resource;

        OCResourcePayloadAddStringLL(&resource->types, "    rt.singleitem    ");
        OCResourcePayloadAddStringLL(&resource->interfaces, "    if.singleitem     ");
        resource->uri = OICStrdup("/uri/thing");

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;

        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)payload, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_DISCOVERY,
                    cborData, cborSize));

        EXPECT_EQ(1u, OCDiscoveryPayloadGetResourceCount((OCDiscoveryPayload*)cparsed));
        OCResourcePayload* parsedResource = ((OCDiscoveryPayload*)cparsed)->resources;

        EXPECT_EQ(NULL, parsedResource->next);
        EXPECT_STREQ(sid1, ((OCDiscoveryPayload*)cparsed)->sid);
        EXPECT_EQ(NULL, parsedResource->types->next);
        EXPECT_STREQ("rt.singleitem", parsedResource->types->value);
        EXPECT_EQ(NULL, parsedResource->interfaces->next);
        EXPECT_STREQ("if.singleitem", parsedResource->interfaces->value);

        OICFree(cborData);
        OCPayloadDestroy(cparsed);
        OCDiscoveryPayloadDestroy(payload);
    }
    TEST(DiscoveryRTandIF, MultiItemsNormal)
    {
        OCDiscoveryPayload* payload = OCDiscoveryPayloadCreate();
        payload->sid = OICStrdup(sid1);
        OCResourcePayload* resource = (OCResourcePayload*)OICCalloc(1, sizeof(OCResourcePayload));
        payload->resources = resource;

        OCResourcePayloadAddStringLL(&resource->types, "rt.firstitem");
        OCResourcePayloadAddStringLL(&resource->types, "rt.seconditem");
        OCResourcePayloadAddStringLL(&resource->interfaces, "if.firstitem");
        OCResourcePayloadAddStringLL(&resource->interfaces, "if.seconditem");
        resource->uri = OICStrdup("/uri/thing");

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;

        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)payload, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_DISCOVERY,
                    cborData, cborSize));

        EXPECT_EQ(1u, OCDiscoveryPayloadGetResourceCount((OCDiscoveryPayload*)cparsed));
        OCResourcePayload* parsedResource = ((OCDiscoveryPayload*)cparsed)->resources;

        EXPECT_EQ(NULL, parsedResource->next);
        EXPECT_STREQ(sid1, ((OCDiscoveryPayload*)cparsed)->sid);
        EXPECT_EQ(NULL, parsedResource->types->next->next);
        EXPECT_STREQ("rt.firstitem", parsedResource->types->value);
        EXPECT_STREQ("rt.seconditem", parsedResource->types->next->value);
        EXPECT_EQ(NULL, parsedResource->interfaces->next->next);
        EXPECT_STREQ("if.firstitem", parsedResource->interfaces->value);
        EXPECT_STREQ("if.seconditem", parsedResource->interfaces->next->value);

        OICFree(cborData);
        OCPayloadDestroy(cparsed);
        OCDiscoveryPayloadDestroy(payload);
    }
    TEST(DiscoveryRTandIF, MultiItemExtraLeadSpaces)
    {
        OCDiscoveryPayload* payload = OCDiscoveryPayloadCreate();
        payload->sid = OICStrdup(sid1);
        OCResourcePayload* resource = (OCResourcePayload*)OICCalloc(1, sizeof(OCResourcePayload));
        payload->resources = resource;

        OCResourcePayloadAddStringLL(&resource->types, "  rt.firstitem");
        OCResourcePayloadAddStringLL(&resource->types, "  rt.seconditem");
        OCResourcePayloadAddStringLL(&resource->interfaces, "  if.firstitem");
        OCResourcePayloadAddStringLL(&resource->interfaces, "  if.seconditem");
        resource->uri = OICStrdup("/uri/thing");

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;

        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)payload, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_DISCOVERY,
                    cborData, cborSize));

        EXPECT_EQ(1u, OCDiscoveryPayloadGetResourceCount((OCDiscoveryPayload*)cparsed));
        OCResourcePayload* parsedResource = ((OCDiscoveryPayload*)cparsed)->resources;

        EXPECT_EQ(NULL, parsedResource->next);
        EXPECT_STREQ(sid1, ((OCDiscoveryPayload*)cparsed)->sid);
        EXPECT_EQ(NULL, parsedResource->types->next->next);
        EXPECT_STREQ("rt.firstitem", parsedResource->types->value);
        EXPECT_STREQ("rt.seconditem", parsedResource->types->next->value);
        EXPECT_EQ(NULL, parsedResource->interfaces->next->next);
        EXPECT_STREQ("if.firstitem", parsedResource->interfaces->value);
        EXPECT_STREQ("if.seconditem", parsedResource->interfaces->next->value);

        OICFree(cborData);
        OCPayloadDestroy(cparsed);
        OCDiscoveryPayloadDestroy(payload);
    }
    TEST(DiscoveryRTandIF, MultiItemExtraTrailSpaces)
    {
        OCDiscoveryPayload* payload = OCDiscoveryPayloadCreate();
        payload->sid = OICStrdup(sid1);
        OCResourcePayload* resource = (OCResourcePayload*)OICCalloc(1, sizeof(OCResourcePayload));
        payload->resources = resource;

        OCResourcePayloadAddStringLL(&resource->types, "rt.firstitem  ");
        OCResourcePayloadAddStringLL(&resource->types, "rt.seconditem  ");
        OCResourcePayloadAddStringLL(&resource->interfaces, "if.firstitem  ");
        OCResourcePayloadAddStringLL(&resource->interfaces, "if.seconditem  ");
        resource->uri = OICStrdup("/uri/thing");

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;

        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)payload, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_DISCOVERY,
                    cborData, cborSize));

        EXPECT_EQ(1u, OCDiscoveryPayloadGetResourceCount((OCDiscoveryPayload*)cparsed));
        OCResourcePayload* parsedResource = ((OCDiscoveryPayload*)cparsed)->resources;

        EXPECT_EQ(NULL, parsedResource->next);
        EXPECT_STREQ(sid1, ((OCDiscoveryPayload*)cparsed)->sid);
        EXPECT_EQ(NULL, parsedResource->types->next->next);
        EXPECT_STREQ("rt.firstitem", parsedResource->types->value);
        EXPECT_STREQ("rt.seconditem", parsedResource->types->next->value);
        EXPECT_EQ(NULL, parsedResource->interfaces->next->next);
        EXPECT_STREQ("if.firstitem", parsedResource->interfaces->value);
        EXPECT_STREQ("if.seconditem", parsedResource->interfaces->next->value);

        OICFree(cborData);
        OCPayloadDestroy(cparsed);
        OCDiscoveryPayloadDestroy(payload);
    }
    TEST(DiscoveryRTandIF, MultiItemBothSpaces)
    {
        OCDiscoveryPayload* payload = OCDiscoveryPayloadCreate();
        payload->sid = OICStrdup(sid1);
        OCResourcePayload* resource = (OCResourcePayload*)OICCalloc(1, sizeof(OCResourcePayload));
        payload->resources = resource;

        OCResourcePayloadAddStringLL(&resource->types, "  rt.firstitem  ");
        OCResourcePayloadAddStringLL(&resource->types, "  rt.seconditem  ");
        OCResourcePayloadAddStringLL(&resource->interfaces, "  if.firstitem  ");
        OCResourcePayloadAddStringLL(&resource->interfaces, "  if.seconditem  ");
        resource->uri = OICStrdup("/uri/thing");

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;

        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)payload, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_DISCOVERY,
                    cborData, cborSize));

        EXPECT_EQ(1u, OCDiscoveryPayloadGetResourceCount((OCDiscoveryPayload*)cparsed));
        OCResourcePayload* parsedResource = ((OCDiscoveryPayload*)cparsed)->resources;

        EXPECT_EQ(NULL, parsedResource->next);
        EXPECT_STREQ(sid1, ((OCDiscoveryPayload*)cparsed)->sid);
        EXPECT_EQ(NULL, parsedResource->types->next->next);
        EXPECT_STREQ("rt.firstitem", parsedResource->types->value);
        EXPECT_STREQ("rt.seconditem", parsedResource->types->next->value);
        EXPECT_EQ(NULL, parsedResource->interfaces->next->next);
        EXPECT_STREQ("if.firstitem", parsedResource->interfaces->value);
        EXPECT_STREQ("if.seconditem", parsedResource->interfaces->next->value);

        OICFree(cborData);
        OCPayloadDestroy(cparsed);
        OCDiscoveryPayloadDestroy(payload);
    }
    TEST(RepresentationEncodingRTandIF, SingleItemNormal)
    {
        OCRepPayload* payload = OCRepPayloadCreate();
        OCRepPayloadAddResourceType(payload, "rt.firstitem");
        OCRepPayloadAddInterface(payload, "if.firstitem");

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;

        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)payload, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_REPRESENTATION,
                    cborData, cborSize));

        OCRepPayload* parsedPayload = (OCRepPayload*)cparsed;

        EXPECT_STREQ("rt.firstitem", parsedPayload->types->value);
        EXPECT_EQ(NULL, parsedPayload->types->next);
        EXPECT_STREQ("if.firstitem", parsedPayload->interfaces->value);
        EXPECT_EQ(NULL, parsedPayload->interfaces->next);

        OICFree(cborData);
        OCRepPayloadDestroy(payload);
        OCPayloadDestroy(cparsed);
    }
    TEST(RepresentationEncodingRTandIF, SingleItemFrontTrim)
    {
        OCRepPayload* payload = OCRepPayloadCreate();
        OCRepPayloadAddResourceType(payload, "  rt.firstitem");
        OCRepPayloadAddInterface(payload, "  if.firstitem");

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;

        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)payload, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_REPRESENTATION,
                    cborData, cborSize));

        OCRepPayload* parsedPayload = (OCRepPayload*)cparsed;

        EXPECT_STREQ("rt.firstitem", parsedPayload->types->value);
        EXPECT_EQ(NULL, parsedPayload->types->next);
        EXPECT_STREQ("if.firstitem", parsedPayload->interfaces->value);
        EXPECT_EQ(NULL, parsedPayload->interfaces->next);


        OICFree(cborData);
        OCRepPayloadDestroy(payload);
        OCPayloadDestroy(cparsed);
    }
    TEST(RepresentationEncodingRTandIF, SingleItemBackTrim)
    {
        OCRepPayload* payload = OCRepPayloadCreate();
        OCRepPayloadAddResourceType(payload, "rt.firstitem  ");
        OCRepPayloadAddInterface(payload, "if.firstitem  ");

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;

        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)payload, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_REPRESENTATION,
                    cborData, cborSize));

        OCRepPayload* parsedPayload = (OCRepPayload*)cparsed;

        EXPECT_STREQ("rt.firstitem", parsedPayload->types->value);
        EXPECT_EQ(NULL, parsedPayload->types->next);
        EXPECT_STREQ("if.firstitem", parsedPayload->interfaces->value);
        EXPECT_EQ(NULL, parsedPayload->interfaces->next);


        OICFree(cborData);
        OCRepPayloadDestroy(payload);
        OCPayloadDestroy(cparsed);
    }
    TEST(RepresentationEncodingRTandIF, SingleItemBothTrim)
    {
        OCRepPayload* payload = OCRepPayloadCreate();
        OCRepPayloadAddResourceType(payload, "  rt.firstitem  ");
        OCRepPayloadAddInterface(payload, "  if.firstitem  ");

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;

        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)payload, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_REPRESENTATION,
                    cborData, cborSize));

        OCRepPayload* parsedPayload = (OCRepPayload*)cparsed;

        EXPECT_STREQ("rt.firstitem", parsedPayload->types->value);
        EXPECT_EQ(NULL, parsedPayload->types->next);
        EXPECT_STREQ("if.firstitem", parsedPayload->interfaces->value);
        EXPECT_EQ(NULL, parsedPayload->interfaces->next);


        OICFree(cborData);
        OCRepPayloadDestroy(payload);
        OCPayloadDestroy(cparsed);
    }
    TEST(RepresentationEncodingRTandIF, MultiItemsNormal)
    {
        OCRepPayload* payload = OCRepPayloadCreate();
        OCRepPayloadAddResourceType(payload, "rt.firstitem");
        OCRepPayloadAddResourceType(payload, "rt.seconditem");
        OCRepPayloadAddInterface(payload, "if.firstitem");
        OCRepPayloadAddInterface(payload, "if.seconditem");

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;

        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)payload, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_REPRESENTATION,
                    cborData, cborSize));

        OCRepPayload* parsedPayload = (OCRepPayload*)cparsed;

        EXPECT_STREQ("rt.firstitem", parsedPayload->types->value);
        EXPECT_STREQ("rt.seconditem", parsedPayload->types->next->value);
        EXPECT_EQ(NULL, parsedPayload->types->next->next);
        EXPECT_STREQ("if.firstitem", parsedPayload->interfaces->value);
        EXPECT_STREQ("if.seconditem", parsedPayload->interfaces->next->value);
        EXPECT_EQ(NULL, parsedPayload->interfaces->next->next);


        OICFree(cborData);
        OCRepPayloadDestroy(payload);
        OCPayloadDestroy(cparsed);
    }
    TEST(RepresentationEncodingRTandIF, MultiItemExtraLeadSpaces)
    {
        OCRepPayload* payload = OCRepPayloadCreate();
        OCRepPayloadAddResourceType(payload, "  rt.firstitem");
        OCRepPayloadAddResourceType(payload, "  rt.seconditem");
        OCRepPayloadAddInterface(payload, "  if.firstitem");
        OCRepPayloadAddInterface(payload, "  if.seconditem");

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;

        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)payload, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_REPRESENTATION,
                    cborData, cborSize));

        OCRepPayload* parsedPayload = (OCRepPayload*)cparsed;

        EXPECT_STREQ("rt.firstitem", parsedPayload->types->value);
        EXPECT_STREQ("rt.seconditem", parsedPayload->types->next->value);
        EXPECT_EQ(NULL, parsedPayload->types->next->next);
        EXPECT_STREQ("if.firstitem", parsedPayload->interfaces->value);
        EXPECT_STREQ("if.seconditem", parsedPayload->interfaces->next->value);
        EXPECT_EQ(NULL, parsedPayload->interfaces->next->next);


        OICFree(cborData);
        OCRepPayloadDestroy(payload);
        OCPayloadDestroy(cparsed);
    }
    TEST(RepresentationEncodingRTandIF, MultiItemExtraTrailSpaces)
    {
        OCRepPayload* payload = OCRepPayloadCreate();
        OCRepPayloadAddResourceType(payload, "rt.firstitem  ");
        OCRepPayloadAddResourceType(payload, "rt.seconditem  ");
        OCRepPayloadAddInterface(payload, "if.firstitem  ");
        OCRepPayloadAddInterface(payload, "if.seconditem  ");

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;

        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)payload, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_REPRESENTATION,
                    cborData, cborSize));

        OCRepPayload* parsedPayload = (OCRepPayload*)cparsed;

        EXPECT_STREQ("rt.firstitem", parsedPayload->types->value);
        EXPECT_STREQ("rt.seconditem", parsedPayload->types->next->value);
        EXPECT_EQ(NULL, parsedPayload->types->next->next);
        EXPECT_STREQ("if.firstitem", parsedPayload->interfaces->value);
        EXPECT_STREQ("if.seconditem", parsedPayload->interfaces->next->value);
        EXPECT_EQ(NULL, parsedPayload->interfaces->next->next);


        OICFree(cborData);
        OCRepPayloadDestroy(payload);
        OCPayloadDestroy(cparsed);
    }
    TEST(RepresentationEncodingRTandIF, MultiItemExtraMiddleSpaces)
    {
        OCRepPayload* payload = OCRepPayloadCreate();
        OCRepPayloadAddResourceType(payload, "  rt.firstitem  ");
        OCRepPayloadAddResourceType(payload, "  rt.seconditem  ");
        OCRepPayloadAddInterface(payload, "  if.firstitem  ");
        OCRepPayloadAddInterface(payload, "  if.seconditem  ");

        uint8_t* cborData;
        size_t cborSize;
        OCPayload* cparsed;

        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)payload, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_REPRESENTATION,
                    cborData, cborSize));

        OCRepPayload* parsedPayload = (OCRepPayload*)cparsed;

        EXPECT_STREQ("rt.firstitem", parsedPayload->types->value);
        EXPECT_STREQ("rt.seconditem", parsedPayload->types->next->value);
        EXPECT_EQ(NULL, parsedPayload->types->next->next);
        EXPECT_STREQ("if.firstitem", parsedPayload->interfaces->value);
        EXPECT_STREQ("if.seconditem", parsedPayload->interfaces->next->value);
        EXPECT_EQ(NULL, parsedPayload->interfaces->next->next);


        OICFree(cborData);
        OCRepPayloadDestroy(payload);
        OCPayloadDestroy(cparsed);
    }
    TEST(RepresentationEncodingRTandIF, TestPayloadContents)
    {
        OC::OCRepresentation subRep1;
        std::vector<std::string> types;
        types.push_back("rt.firstitem");
        std::vector<std::string> interfaces;
        interfaces.push_back("if.firstitem");
        subRep1.setResourceTypes(types);
        subRep1.setResourceInterfaces(interfaces);
        subRep1.setNULL("NullAttr");
        subRep1.setValue("IntAttr", 77);
        subRep1.setValue("DoubleAttr", 3.333);
        subRep1.setValue("BoolAttr", true);
        subRep1.setValue("StringAttr", std::string("String attr"));

        OC::MessageContainer mc1;
        mc1.addRepresentation(subRep1);

        OCRepPayload *repPayload = mc1.getPayload();
        EXPECT_EQ(PAYLOAD_TYPE_REPRESENTATION, repPayload->base.type);

        uint8_t *cborData = NULL;
        size_t cborSize = 0;
        OCPayload *cparsed = NULL;

        EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*)repPayload, &cborData, &cborSize));
        EXPECT_EQ(OC_STACK_OK, OCParsePayload(&cparsed, PAYLOAD_TYPE_REPRESENTATION,
                    cborData, cborSize));

        OCRepPayload *parsedPayload = (OCRepPayload *)cparsed;
        EXPECT_EQ(NULL, parsedPayload->uri);
        EXPECT_STREQ("rt.firstitem", parsedPayload->types->value);
        EXPECT_EQ(NULL, parsedPayload->types->next);
        EXPECT_STREQ("if.firstitem", parsedPayload->interfaces->value);
        EXPECT_EQ(NULL, parsedPayload->interfaces->next);

        // To make sure rt and if are not duplicated.
        EXPECT_STREQ("BoolAttr", parsedPayload->values->name);
        EXPECT_EQ(true, parsedPayload->values->b);
        EXPECT_EQ(OCREP_PROP_BOOL, parsedPayload->values->type);
        parsedPayload->values = parsedPayload->values->next;

        EXPECT_STREQ("DoubleAttr", parsedPayload->values->name);
        EXPECT_EQ(OCREP_PROP_DOUBLE, parsedPayload->values->type);
        EXPECT_EQ(3.3330000000000002, parsedPayload->values->d);
        parsedPayload->values = parsedPayload->values->next;

        EXPECT_STREQ("IntAttr", parsedPayload->values->name);
        EXPECT_EQ(77, parsedPayload->values->i);
        EXPECT_EQ(OCREP_PROP_INT, parsedPayload->values->type);
        parsedPayload->values = parsedPayload->values->next;

        EXPECT_STREQ("NullAttr", parsedPayload->values->name);
        EXPECT_EQ(OCREP_PROP_NULL, parsedPayload->values->type);
        parsedPayload->values = parsedPayload->values->next;

        EXPECT_STREQ("StringAttr", parsedPayload->values->name);
        EXPECT_STREQ("String attr", parsedPayload->values->str);
        EXPECT_EQ(OCREP_PROP_STRING, parsedPayload->values->type);
        parsedPayload->values = parsedPayload->values->next;

        EXPECT_EQ(NULL, parsedPayload->values);

        OICFree(cborData);
        OCRepPayloadDestroy(repPayload);
        OCPayloadDestroy(cparsed);
    }
}
