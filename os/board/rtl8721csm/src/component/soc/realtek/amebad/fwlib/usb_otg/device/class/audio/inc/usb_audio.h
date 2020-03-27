#ifndef __USB_AUDIO_H
#define __USB_AUDIO_H

#include <platform_opts.h>

#ifdef CONFIG_USBD_AUDIO

#include "usb.h"
#include "usb_gadget.h"
#include "usb_composite.h"
#include "audio/inc/usb_audio_8763.h"
#include "ameba_otg.h"

#define USB_ISO_OUT_ENDPOINT_ADDRESS    0x02
#define USB_ISO_IN_ENDPOINT_ADDRESS     0x81

struct usb_audio_control {
    //struct list_head list;
    dwc_list_link_t list;
    const char *name;
    uint8_t type;
    int data[5];
    int (*set)(struct usb_audio_control *con, uint8_t cmd, int value);
    int (*get)(struct usb_audio_control *con, uint8_t cmd);
};

struct usb_audio_control_selector {
    //struct list_head list;
    dwc_list_link_t list;
    //struct list_head control;
    dwc_list_link_t control;
    uint8_t id;
    const char *name;
    uint8_t type;
    struct usb_descriptor_header *desc;
};

#define UAC_VERSION_1                   0x00
#define UAC_VERSION_2                   0x20

/* A.2 Audio Interface Subclass Codes */
#define USB_SUBCLASS_AUDIOCONTROL       0x01
#define USB_SUBCLASS_AUDIOSTREAMING     0x02
#define USB_SUBCLASS_MIDISTREAMING      0x03

#define F_AUDIO_NUM_INTERFACES          2

/* A.5 Audio Class-Specific AC Interface Descriptor Subtypes */
#define UAC_HEADER                      0x01
#define UAC_INPUT_TERMINAL              0x02
#define UAC_OUTPUT_TERMINAL             0x03
#define UAC_MIXER_UNIT                  0x04
#define UAC_SELECTOR_UNIT               0x05
#define UAC_FEATURE_UNIT                0x06
#define UAC1_PROCESSING_UNIT            0x07
#define UAC1_EXTENSION_UNIT             0x08


/* A.6 Audio Class-Specific AS Interface Descriptor Subtypes */
#define UAC_AS_GENERAL                  0x01
#define UAC_FORMAT_TYPE                 0x02
#define UAC_FORMAT_SPECIFIC             0x03

/* A.7 Processing Unit Process Types */
#define UAC_PROCESS_UNDEFINED           0x00
#define UAC_PROCESS_UP_DOWNMIX          0x01
#define UAC_PROCESS_DOLBY_PROLOGIC      0x02
#define UAC_PROCESS_STEREO_EXTENDER     0x03
#define UAC_PROCESS_REVERB              0x04
#define UAC_PROCESS_CHORUS              0x05
#define UAC_PROCESS_DYN_RANGE_COMP      0x06

/* A.8 Audio Class-Specific Endpoint Descriptor Subtypes */
#define UAC_EP_GENERAL                  0x01

/* A.9 Audio Class-Specific Request Codes */
#define UAC_SET_                        0x00
#define UAC_GET_                        0x80

#define UAC__CUR                        0x1
#define UAC__MIN                        0x2
#define UAC__MAX                        0x3
#define UAC__RES                        0x4
#define UAC__MEM                        0x5

#define UAC_SET_CUR                     (UAC_SET_ | UAC__CUR)
#define UAC_GET_CUR                     (UAC_GET_ | UAC__CUR)
#define UAC_SET_MIN                     (UAC_SET_ | UAC__MIN)
#define UAC_GET_MIN                     (UAC_GET_ | UAC__MIN)
#define UAC_SET_MAX                     (UAC_SET_ | UAC__MAX)
#define UAC_GET_MAX                     (UAC_GET_ | UAC__MAX)
#define UAC_SET_RES                     (UAC_SET_ | UAC__RES)
#define UAC_GET_RES                     (UAC_GET_ | UAC__RES)
#define UAC_SET_MEM                     (UAC_SET_ | UAC__MEM)
#define UAC_GET_MEM                     (UAC_GET_ | UAC__MEM)

#define UAC_GET_STAT                    0xff


/* A.10 Control Selector Codes */

/* A.10.1 Terminal Control Selectors */
#define UAC_TERM_COPY_PROTECT           0x01

/* A.10.2 Feature Unit Control Selectors */
#define UAC_FU_MUTE                     0x01
#define UAC_FU_VOLUME                   0x02
#define UAC_FU_BASS                     0x03
#define UAC_FU_MID                      0x04
#define UAC_FU_TREBLE                   0x05
#define UAC_FU_GRAPHIC_EQUALIZER        0x06
#define UAC_FU_AUTOMATIC_GAIN           0x07
#define UAC_FU_DELAY                    0x08
#define UAC_FU_BASS_BOOST               0x09
#define UAC_FU_LOUDNESS                 0x0a

#define UAC_CONTROL_BIT(CS)     (1 << ((CS) - 1))

/* A.10.3.1 Up/Down-mix Processing Unit Controls Selectors */
#define UAC_UD_ENABLE                   0x01
#define UAC_UD_MODE_SELECT              0x02

/* A.10.3.2 Dolby Prologic (tm) Processing Unit Controls Selectors */
#define UAC_DP_ENABLE                   0x01
#define UAC_DP_MODE_SELECT              0x02

/* A.10.3.3 3D Stereo Extender Processing Unit Control Selectors */
#define UAC_3D_ENABLE                   0x01
#define UAC_3D_SPACE                    0x02

/* A.10.3.4 Reverberation Processing Unit Control Selectors */
#define UAC_REVERB_ENABLE               0x01
#define UAC_REVERB_LEVEL                0x02
#define UAC_REVERB_TIME                 0x03
#define UAC_REVERB_FEEDBACK             0x04

/* A.10.3.5 Chorus Processing Unit Control Selectors */
#define UAC_CHORUS_ENABLE               0x01
#define UAC_CHORUS_LEVEL                0x02
#define UAC_CHORUS_RATE                 0x03
#define UAC_CHORUS_DEPTH                0x04

/* A.10.3.6 Dynamic Range Compressor Unit Control Selectors */
#define UAC_DCR_ENABLE                  0x01
#define UAC_DCR_RATE                    0x02
#define UAC_DCR_MAXAMPL                 0x03
#define UAC_DCR_THRESHOLD               0x04
#define UAC_DCR_ATTACK_TIME             0x05
#define UAC_DCR_RELEASE_TIME            0x06

/* A.10.4 Extension Unit Control Selectors */
#define UAC_XU_ENABLE                   0x01

/* MIDI - A.1 MS Class-Specific Interface Descriptor Subtypes */
#define UAC_MS_HEADER                   0x01
#define UAC_MIDI_IN_JACK                0x02
#define UAC_MIDI_OUT_JACK               0x03

/* MIDI - A.1 MS Class-Specific Endpoint Descriptor Subtypes */
#define UAC_MS_GENERAL                  0x01

/* Terminals - 2.1 USB Terminal Types */
#define UAC_TERMINAL_UNDEFINED          0x100
#define UAC_TERMINAL_STREAMING          0x101
#define UAC_TERMINAL_VENDOR_SPEC        0x1FF

#pragma pack(push)
#pragma pack(1)

/* Terminal Control Selectors */
/* 4.3.2  Class-Specific AC Interface Descriptor */

struct uac1_ac_header_descriptor {
    uint8_t  bLength;                  /* 8 + n */
    uint8_t  bDescriptorType;          /* USB_DT_CS_INTERFACE */
    uint8_t  bDescriptorSubtype;       /* UAC_MS_HEADER */
    uint16_t bcdADC;                  /* 0x0100 */
    uint16_t wTotalLength;            /* includes Unit and Terminal desc. */
    uint8_t  bInCollection;            /* n */
    uint8_t  baInterfaceNr[F_AUDIO_NUM_INTERFACES];          /* [n] */
} __attribute__((packed));
//#define AC_HEADER_COUNT 2
#define UAC_DT_AC_HEADER_SIZE(n)        (8 + (n))

/* 4.3.2.1 Input Terminal Descriptor */
struct uac_input_terminal_descriptor {
    uint8_t  bLength;                  /* in bytes: 12 */
    uint8_t  bDescriptorType;          /* CS_INTERFACE descriptor type */
    uint8_t  bDescriptorSubtype;       /* INPUT_TERMINAL descriptor subtype */
    uint8_t  bTerminalID;              /* Constant uniquely terminal ID */
    uint16_t wTerminalType;           /* USB Audio Terminal Types */
    uint8_t  bAssocTerminal;           /* ID of the Output Terminal associated */
    uint8_t  bNrChannels;              /* Number of logical output channels */
    uint16_t wChannelConfig;
    uint8_t  iChannelNames;
    uint8_t  iTerminal;
} __attribute__((packed));

#define UAC_DT_INPUT_TERMINAL_SIZE                      12

/* Terminals - 2.2 Input Terminal Types */
#define UAC_INPUT_TERMINAL_UNDEFINED                    0x200
#define UAC_INPUT_TERMINAL_MICROPHONE                   0x201
#define UAC_INPUT_TERMINAL_DESKTOP_MICROPHONE           0x202
#define UAC_INPUT_TERMINAL_PERSONAL_MICROPHONE          0x203
#define UAC_INPUT_TERMINAL_OMNI_DIR_MICROPHONE          0x204
#define UAC_INPUT_TERMINAL_MICROPHONE_ARRAY             0x205
#define UAC_INPUT_TERMINAL_PROC_MICROPHONE_ARRAY        0x206

/* Terminals - control selectors */

#define UAC_TERMINAL_CS_COPY_PROTECT_CONTROL            0x01

/* 4.3.2.2 Output Terminal Descriptor */
struct uac1_output_terminal_descriptor {
    uint8_t  bLength;                  /* in bytes: 9 */
    uint8_t  bDescriptorType;          /* CS_INTERFACE descriptor type */
    uint8_t  bDescriptorSubtype;       /* OUTPUT_TERMINAL descriptor subtype */
    uint8_t  bTerminalID;              /* Constant uniquely terminal ID */
    uint16_t wTerminalType;           /* USB Audio Terminal Types */
    uint8_t  bAssocTerminal;           /* ID of the Input Terminal associated */
    uint8_t  bSourceID;                /* ID of the connected Unit or Terminal*/
    uint8_t  iTerminal;
} __attribute__((packed));

#define UAC_DT_OUTPUT_TERMINAL_SIZE                     9

/* Terminals - 2.3 Output Terminal Types */
#define UAC_OUTPUT_TERMINAL_UNDEFINED                   0x300
#define UAC_OUTPUT_TERMINAL_SPEAKER                     0x301
#define UAC_OUTPUT_TERMINAL_HEADPHONES                  0x302
#define UAC_OUTPUT_TERMINAL_HEAD_MOUNTED_DISPLAY_AUDIO  0x303
#define UAC_OUTPUT_TERMINAL_DESKTOP_SPEAKER             0x304
#define UAC_OUTPUT_TERMINAL_ROOM_SPEAKER                0x305
#define UAC_OUTPUT_TERMINAL_COMMUNICATION_SPEAKER       0x306
#define UAC_OUTPUT_TERMINAL_LOW_FREQ_EFFECTS_SPEAKER    0x307

/* Set bControlSize = 2 as default setting */
#define UAC_DT_FEATURE_UNIT_SIZE(ch)            (7 + ((ch) + 1) * 2)

/* As above, but more useful for defining your own descriptors: */
//#define DECLARE_UAC_FEATURE_UNIT_DESCRIPTOR(ch)

#define DECLARE_UAC_FEATURE_UNIT_DESCRIPTOR 2
#define DECLARE_FEATURE_UNIY_CONTROL_SIZE 1
#define CHANNEL_CNT 2
struct uac_feature_unit_descriptor_2 {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bDescriptorSubtype;
    uint8_t  bUnitID;
    uint8_t  bSourceID;
    uint8_t  bControlSize;//DECLARE_FEATURE_UNIY_CONTROL_SIZE
    //uint16_t bmaControls[DECLARE_UAC_FEATURE_UNIT_DESCRIPTOR + 1];
    uint8_t bmaControls[((DECLARE_FEATURE_UNIY_CONTROL_SIZE) * (2)) + 1];
    uint8_t  iFeature;
} __attribute__((packed));

struct uac_feature_unit_descriptor_1 {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bDescriptorSubtype;
    uint8_t  bUnitID;
    uint8_t  bSourceID;
    uint8_t  bControlSize;//DECLARE_FEATURE_UNIY_CONTROL_SIZE
    //uint16_t bmaControls[DECLARE_UAC_FEATURE_UNIT_DESCRIPTOR + 1];
    uint8_t bmaControls[((DECLARE_FEATURE_UNIY_CONTROL_SIZE) * (1)) + 1];
    uint8_t  iFeature;
} __attribute__((packed));

struct uac_feature_unit_descriptor_0 {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bDescriptorSubtype;
    uint8_t  bUnitID;
    uint8_t  bSourceID;
    uint8_t  bControlSize;//DECLARE_FEATURE_UNIY_CONTROL_SIZE
    //uint16_t bmaControls[DECLARE_UAC_FEATURE_UNIT_DESCRIPTOR + 1];
    uint8_t bmaControls[((DECLARE_FEATURE_UNIY_CONTROL_SIZE) * (0)) + 1];
    uint8_t  iFeature;
} __attribute__((packed));

/* 4.3.2.3 Mixer Unit Descriptor */
// fongpin modify
#define MIXER_UNIT_CONNECTED 2 // BBRRO DEFINE
struct uac_mixer_unit_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bUnitID;
    uint8_t bNrInPins;
    uint8_t baSourceID[MIXER_UNIT_CONNECTED];// modify
    uint8_t bNrChannels;// add
    uint16_t bmChannelConfig;//add
    uint8_t iChannelNames;//add
    uint8_t bmControls;// add
    uint8_t iMixer;//add

} __attribute__((packed));

/* 4.3.2.4 Selector Unit Descriptor */
// fongpin modify
struct uac_selector_unit_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bUintID;
    uint8_t bNrInPins;
    uint8_t baSourceID;
    uint8_t iSelector;
} __attribute__((packed));

/* 4.5.2 Class-Specific AS Interface Descriptor */
struct uac1_as_header_descriptor {
    uint8_t  bLength;                  /* in bytes: 7 */
    uint8_t  bDescriptorType;          /* USB_DT_CS_INTERFACE */
    uint8_t  bDescriptorSubtype;       /* AS_GENERAL */
    uint8_t  bTerminalLink;            /* Terminal ID of connected Terminal */
    uint8_t  bDelay;                   /* Delay introduced by the data path */
    uint16_t wFormatTag;              /* The Audio Data Format */
} __attribute__((packed));

#define UAC_DT_AS_HEADER_SIZE           7


/* Formats - A.1.1 Audio Data Format Type I Codes */
#define UAC_FORMAT_TYPE_I_UNDEFINED     0x0
#define UAC_FORMAT_TYPE_I_PCM           0x1
#define UAC_FORMAT_TYPE_I_PCM8          0x2
#define UAC_FORMAT_TYPE_I_IEEE_FLOAT    0x3
#define UAC_FORMAT_TYPE_I_ALAW          0x4
#define UAC_FORMAT_TYPE_I_MULAW         0x5

struct uac_format_type_i_discrete_descriptor {
    uint8_t  bLength;                  /* in bytes: 8 + (ns * 3) */
    uint8_t  bDescriptorType;          /* USB_DT_CS_INTERFACE */
    uint8_t  bDescriptorSubtype;       /* FORMAT_TYPE */
    uint8_t  bFormatType;              /* FORMAT_TYPE_1 */
    uint8_t  bNrChannels;              /* physical channels in the stream */
    uint8_t  bSubframeSize;            /* */
    uint8_t  bBitResolution;
    uint8_t  bSamFreqType;
    uint8_t  tSamFreq[3][3];
} __attribute__((packed));

/* Formats - A.2 Format Type Codes */
#define UAC_FORMAT_TYPE_UNDEFINED       0x0
#define UAC_FORMAT_TYPE_I               0x1
#define UAC_FORMAT_TYPE_II              0x2
#define UAC_FORMAT_TYPE_III             0x3
#define UAC_EXT_FORMAT_TYPE_I           0x81
#define UAC_EXT_FORMAT_TYPE_II          0x82
#define UAC_EXT_FORMAT_TYPE_III         0x83

struct uac_iso_endpoint_descriptor {
    uint8_t  bLength;                  /* in bytes: 7 */
    uint8_t  bDescriptorType;          /* USB_DT_CS_ENDPOINT */
    uint8_t  bDescriptorSubtype;       /* EP_GENERAL */
    uint8_t  bmAttributes;
    uint8_t  bLockDelayUnits;
    uint16_t wLockDelay;
} __attribute__((packed));
#define UAC_ISO_ENDPOINT_DESC_SIZE      7

#define UAC_EP_CS_ATTR_SAMPLE_RATE      0x01
#define UAC_EP_CS_ATTR_PITCH_CONTROL    0x02
#define UAC_EP_CS_ATTR_FILL_MAX         0x80

/* status word format (3.7.1.1) */

#define UAC1_STATUS_TYPE_ORIG_MASK              0x0f
#define UAC1_STATUS_TYPE_ORIG_AUDIO_CONTROL_IF  0x0
#define UAC1_STATUS_TYPE_ORIG_AUDIO_STREAM_IF   0x1
#define UAC1_STATUS_TYPE_ORIG_AUDIO_STREAM_EP   0x2

#define UAC1_STATUS_TYPE_IRQ_PENDING            (1 << 7)
#define UAC1_STATUS_TYPE_MEM_CHANGED            (1 << 6)

struct uac1_status_word {
    uint8_t bStatusType;
    uint8_t bOriginator;
} __attribute__((packed));
#pragma pack(pop)

#endif // CONFIG_USBD_AUDIO

#endif // __USB_AUDIO_H

