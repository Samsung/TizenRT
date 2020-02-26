#include <platform_opts.h>

#ifdef CONFIG_USBD_AUDIO

#include "audio/inc/usb_audio.h"
#include "audio/inc/usb_uac1.h"
#include "audio/inc/usb_audio_8763.h"
#include "audio/inc/usb_audio_if.h"
#include "usb_composite.h"
#include "osdep_service.h"
#include "ameba_soc.h"
#if defined (CONFIG_PLATFORM_8721D)
#include "ameba_otg.h"
#endif

extern  int usb_assign_descriptors(struct usb_function *f,
    struct usb_descriptor_header **fs,
    struct usb_descriptor_header **hs,
    struct usb_descriptor_header **ss);

extern void usb_free_all_descriptors(struct usb_function *function);

int generic_set_cmd(struct usb_audio_control *con, uint8_t cmd, int value);
int generic_get_cmd(struct usb_audio_control *con, uint8_t cmd);

#define AUDIO_FUNCTION_DESC_CNT 23// desc count

struct usb_descriptor_header **audio_fs_function;
uint16_t g_req_out_buf_size;
uint16_t g_req_in_buf_size;

/*====================================================*/
struct f_audio *audio;

#define PCM_TABLE_CNT 8
uint32_t *pcm_rate_table;

static struct uac1_ac_header_descriptor ac_header_desc = {
    .bLength =            sizeof ac_header_desc,
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubtype = UAC_HEADER,
    .bcdADC =             0x0100,
    .wTotalLength =       0x64,
    .bInCollection =      F_AUDIO_NUM_INTERFACES,
    .baInterfaceNr[0] =   0x01,
    .baInterfaceNr[1] =   0x02,
};


static struct uac_input_terminal_descriptor input_terminal_desc0 = {
    .bLength =            UAC_DT_INPUT_TERMINAL_SIZE,
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubtype = UAC_INPUT_TERMINAL,
    .bTerminalID =        INPUT_TERMINAL_2CH_ID,
    .wTerminalType =      UAC_TERMINAL_STREAMING,
    .bAssocTerminal =     0,
    .bNrChannels =        0x02,
    .wChannelConfig =     0x3,
    .iChannelNames =      0x00,
    .iTerminal =          0x00,
};

static struct uac_input_terminal_descriptor input_terminal_desc1 = {
    .bLength =            UAC_DT_INPUT_TERMINAL_SIZE,
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubtype = UAC_INPUT_TERMINAL,
    .bTerminalID =        INPUT_TERMINAL_MIC_ID,
    .wTerminalType =      UAC_INPUT_TERMINAL_MICROPHONE,
    .bAssocTerminal =     0,
    .bNrChannels =        0x01,
    .wChannelConfig =     0x1,
    .iChannelNames =      0x00,
    .iTerminal =          0x00,
};


#define OUTPUT_TERMINAL_ID  0x10
static struct uac1_output_terminal_descriptor output_terminal_desc0 = {
    .bLength =            UAC_DT_OUTPUT_TERMINAL_SIZE,
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubtype = UAC_OUTPUT_TERMINAL,
    .bTerminalID =        OUTPUT_TERMINAL_SPEAKER_ID,
    .wTerminalType =      UAC_OUTPUT_TERMINAL_SPEAKER,
    .bAssocTerminal =     0x00,
    .bSourceID =          FEATURE_UNIT_0_ID,
    .iTerminal =          0x00,
};

static struct uac1_output_terminal_descriptor output_terminal_desc1 = {
    .bLength =            UAC_DT_OUTPUT_TERMINAL_SIZE,
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubtype = UAC_OUTPUT_TERMINAL,
    .bTerminalID =        OUTPUT_TERMINAL_USB_ID,
    .wTerminalType =      UAC_TERMINAL_STREAMING,
    .bAssocTerminal =     0x00,
    .bSourceID =          SELECTOR_UNIT_ID,
    .iTerminal =          0x00,
};

static struct uac_selector_unit_descriptor selector_unit_desc = {
    .bLength =            7,
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubtype = UAC_SELECTOR_UNIT,
    .bUintID =            SELECTOR_UNIT_ID,
    .bNrInPins =          0x01,
    .baSourceID =         FEATURE_UNIT_1_ID,
    .iSelector =          0,
};

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

#define FEATURE_UNIT_ID                 0x06
#define UAC_CONTROL_BIT(CS)             (1 << ((CS) - 1))

static struct uac_feature_unit_descriptor_2 feature_unit_desc0 = {
    .bLength =            sizeof(feature_unit_desc0),
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubtype = UAC_FEATURE_UNIT,
    .bUnitID =            FEATURE_UNIT_0_ID,
    .bSourceID =          MIXER_UNIT_2_ID,
    .bControlSize =       0x01,
    .bmaControls[0] =     UAC_CONTROL_BIT(UAC_FU_MUTE),
    .bmaControls[1] =     UAC_CONTROL_BIT(UAC_FU_VOLUME),
    .bmaControls[2] =     UAC_CONTROL_BIT(UAC_FU_VOLUME),
    .iFeature =           0,
};

static struct uac_feature_unit_descriptor_1 feature_unit_desc1 = {
    .bLength =            sizeof(feature_unit_desc1),
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubtype = UAC_FEATURE_UNIT,
    .bUnitID =            FEATURE_UNIT_1_ID,
    .bSourceID =          INPUT_TERMINAL_MIC_ID,
    .bControlSize =       0x01,
    .bmaControls[0] =     UAC_CONTROL_BIT(UAC_FU_AUTOMATIC_GAIN) | UAC_CONTROL_BIT(UAC_FU_MUTE) | UAC_CONTROL_BIT(UAC_FU_VOLUME),
    .bmaControls[1] =     0,
    .iFeature =           0,
};
static struct uac_feature_unit_descriptor_1 feature_unit_desc2 = {
    .bLength =            sizeof(feature_unit_desc2),
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubtype = UAC_FEATURE_UNIT,
    .bUnitID =            FEATURE_UNIT_2_ID,
    .bSourceID =          INPUT_TERMINAL_MIC_ID,
    .bControlSize =       0x01,
    .bmaControls[0] =     UAC_CONTROL_BIT(UAC_FU_MUTE) | UAC_CONTROL_BIT(UAC_FU_VOLUME),
    .bmaControls[1] =     0,
    .iFeature =           0,
};

static struct uac_mixer_unit_descriptor mixer_unit_desc = {
    .bLength =            sizeof(mixer_unit_desc),
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubtype = UAC_MIXER_UNIT,
    .bUnitID =            MIXER_UNIT_2_ID,
    .bNrInPins =          0x02,
    .baSourceID[0] =      INPUT_TERMINAL_2CH_ID, // 0x01; //INPUT_TERMINAL_2CH_ID ?
    .baSourceID[1] =      FEATURE_UNIT_2_ID,     // 0x0D; //FEATURE_UNIT_2_ID  ?
    // Number of logical output channels in the Mixer?s output audio channel cluster.
    .bNrChannels =        2,
    .bmChannelConfig =    0x03,    //left + right
    .iChannelNames =      0,       // unused , 0
    // mixer controlable // Bit map indicating which mixing Controls are programmable.
    .bmControls =         0,
    .iMixer =             0,       // string index
};


/* B.4.1  Standard AS Interface Descriptor */
static struct usb_interface_descriptor as_interface_alt_0_desc = {
    .bLength =            USB_DT_INTERFACE_SIZE,
    .bDescriptorType =    USB_DT_INTERFACE,
    .bInterfaceNumber =   0x01,
    .bAlternateSetting =  0,
    .bNumEndpoints =      0,
    .bInterfaceClass =    USB_CLASS_AUDIO,
    .bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
    .bInterfaceProtocol = 0,
    .iInterface =         0x0,
};

static struct usb_interface_descriptor as_interface_alt_1_desc = {
    .bLength =            USB_DT_INTERFACE_SIZE,
    .bDescriptorType =    USB_DT_INTERFACE,
    .bInterfaceNumber =   0x01,
    .bAlternateSetting =  1,
    .bNumEndpoints =      1,
    .bInterfaceClass =    USB_CLASS_AUDIO,
    .bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
    .bInterfaceProtocol = 0,
    .iInterface =         0x00,
};

#define UAC_DT_AS_HEADER_SIZE       7

/* B.4.2  Class-Specific AS Interface Descriptor */
static struct uac1_as_header_descriptor as_header_desc1 = {
    .bLength =            UAC_DT_AS_HEADER_SIZE,
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubtype = UAC_AS_GENERAL,
    .bTerminalLink =      INPUT_TERMINAL_2CH_ID,
    .bDelay =             1,
    .wFormatTag =         UAC_FORMAT_TYPE_I_PCM,
};

#define UAC_FORMAT_TYPE_I_DISCRETE_DESC_SIZE(n) (8 + (n * 3))

static struct uac_format_type_i_discrete_descriptor as_type_i_desc1 = {
    .bLength =            UAC_FORMAT_TYPE_I_DISCRETE_DESC_SIZE(2),
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubtype = UAC_FORMAT_TYPE,
    .bFormatType =        UAC_FORMAT_TYPE_I,
    .bNrChannels =        0x2,
    .bSubframeSize =      2,
    .bBitResolution =     16,
    .bSamFreqType =       1,
    .tSamFreq[0][0] =     0x80,
    .tSamFreq[0][1] =     0xBB,
    .tSamFreq[0][2] =     0x00,
};

/* Standard ISO OUT Endpoint Descriptor */
static struct usb_endpoint_descriptor as_out_ep_desc1  = {
    .bLength =            USB_DT_ENDPOINT_AUDIO_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,
    .bEndpointAddress =   USB_ISO_OUT_ENDPOINT_ADDRESS,
    .bmAttributes =       USB_ENDPOINT_SYNC_ADAPTIVE | USB_ENDPOINT_XFER_ISOC,
    .wMaxPacketSize =     576, //UAC1_ISO_OUT_EP_MAX_PACKET_SIZE,
    .bInterval =          0x01,
};

/* A.8 Audio Class-Specific Endpoint Descriptor Subtypes */
#define UAC_EP_GENERAL    0x01

/* Class-specific AS ISO OUT Endpoint Descriptor */
static struct uac_iso_endpoint_descriptor as_iso_out_desc1  = {
    .bLength =            UAC_ISO_ENDPOINT_DESC_SIZE,
    .bDescriptorType =    USB_DT_CS_ENDPOINT,
    .bDescriptorSubtype = UAC_EP_GENERAL,
    .bmAttributes =       1,
    .bLockDelayUnits =    1,
    .wLockDelay =         1,
};

#define AUDIO_STREAM_IN_INTF_NUM 2
static struct usb_interface_descriptor as_interface_desc2_alt0 = {
    .bLength =            USB_DT_INTERFACE_SIZE,
    .bDescriptorType =    USB_DT_INTERFACE,
    .bInterfaceNumber =   AUDIO_STREAM_IN_INTF_NUM,
    .bAlternateSetting =  0,
    .bNumEndpoints =      0,
    .bInterfaceClass =    USB_CLASS_AUDIO,
    .bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
    .bInterfaceProtocol = 0,
    .iInterface =         0x00,
};

static struct usb_interface_descriptor as_interface_desc2_alt1 = {
    .bLength =            USB_DT_INTERFACE_SIZE,
    .bDescriptorType =    USB_DT_INTERFACE,
    .bInterfaceNumber =   AUDIO_STREAM_IN_INTF_NUM,
    .bAlternateSetting =  1,
    .bNumEndpoints =      1,
    .bInterfaceClass =    USB_CLASS_AUDIO,
    .bInterfaceSubClass = USB_SUBCLASS_AUDIOSTREAMING,
    .bInterfaceProtocol = 0,
    .iInterface =         0x00,
};

static struct uac1_as_header_descriptor as_header_desc2 = {
    .bLength =            sizeof(as_header_desc2),
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubtype = UAC_AS_GENERAL,
    .bTerminalLink =      OUTPUT_TERMINAL_USB_ID,
    .bDelay =             0x01,
    .wFormatTag =         UAC_FORMAT_TYPE_I_PCM,
};

static struct  uac_format_type_i_discrete_descriptor as_type_i_desc2 = {
    .bLength =            sizeof(as_type_i_desc2),
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubtype = UAC_FORMAT_TYPE,
    .bFormatType =        UAC_FORMAT_TYPE_I,
    .bNrChannels =        2,
    .bSubframeSize =      2,
    .bBitResolution =     0x10,
    .bSamFreqType =       1,
#if defined(CONFIG_EXAMPLE_USBD_AUDIO) && CONFIG_EXAMPLE_USBD_AUDIO
    .tSamFreq[0][0] =     0x80,
    .tSamFreq[0][1] =     0xBB,
    .tSamFreq[0][2] =     0x00,
#else
    .tSamFreq[0][0] =     0x80,
    .tSamFreq[0][1] =     0x3E,
    .tSamFreq[0][2] =     0x00,
#endif

};

static struct usb_audio_endpoint_descriptor as_in_ep_desc2 = {
    .bLength =            USB_DT_ENDPOINT_AUDIO_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,
    .bEndpointAddress =   USB_ISO_IN_ENDPOINT_ADDRESS,
    .bmAttributes =       USB_ENDPOINT_XFER_ISOC | USB_ENDPOINT_SYNC_SYNC,
    .wMaxPacketSize =     0x200,
    .bInterval =          1,
    .bRefresh =           0,
    .bSynchAddress =      0,
};

static struct uac_iso_endpoint_descriptor as_iso_in_desc2 = {
    .bLength =            USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_CS_ENDPOINT,
    .bDescriptorSubtype = UAC_EP_GENERAL,
    .bmAttributes =       1,
    .bLockDelayUnits =    0,
    .wLockDelay =         0x00,
};

static struct usb_interface_descriptor ac_interface_desc0 = {
    .bLength =            sizeof ac_interface_desc0,
    .bDescriptorType =    USB_DT_INTERFACE,

    .bInterfaceNumber =   0x00, // this will be assign automatically
    .bAlternateSetting =  0x00,
    .bNumEndpoints =      0x00,
    .bInterfaceClass =    USB_CLASS_AUDIO,
    .bInterfaceSubClass = USB_SUBCLASS_AUDIOCONTROL,
    .bInterfaceProtocol = 0,
    .iInterface =         0,
};

static struct usb_descriptor_header *f_audio_desc[]  = {
    (struct usb_descriptor_header *) &ac_interface_desc0,
    (struct usb_descriptor_header *) &ac_header_desc,
    (struct usb_descriptor_header *) &input_terminal_desc0,
    (struct usb_descriptor_header *) &input_terminal_desc1,
    (struct usb_descriptor_header *) &output_terminal_desc0,
    (struct usb_descriptor_header *) &output_terminal_desc1,
    (struct usb_descriptor_header *) &selector_unit_desc,
    (struct usb_descriptor_header *) &feature_unit_desc0,
    (struct usb_descriptor_header *) &feature_unit_desc1,
    (struct usb_descriptor_header *) &feature_unit_desc2,
    (struct usb_descriptor_header *) &mixer_unit_desc,


    (struct usb_descriptor_header *) &as_interface_alt_0_desc,
    (struct usb_descriptor_header *) &as_interface_alt_1_desc,
    (struct usb_descriptor_header *) &as_header_desc1,
    (struct usb_descriptor_header *) &as_type_i_desc1,
    (struct usb_descriptor_header *) &as_out_ep_desc1,
    (struct usb_descriptor_header *) &as_iso_out_desc1,

    (struct usb_descriptor_header *) &as_interface_desc2_alt0,
    (struct usb_descriptor_header *) &as_interface_desc2_alt1,
    (struct usb_descriptor_header *) &as_header_desc2,
    (struct usb_descriptor_header *) &as_type_i_desc2,
    (struct usb_descriptor_header *) &as_in_ep_desc2,
    (struct usb_descriptor_header *) &as_iso_in_desc2,
    NULL,
};

static struct usb_audio_control_selector feature_unit_id9 = {
    .id   = FEATURE_UNIT_0_ID,
    .name = "Mute & Volume Control",
    .type = UAC_FEATURE_UNIT,
    .desc = (struct usb_descriptor_header *)&feature_unit_desc0,
};

static struct usb_audio_control mute_control_id9 = {
    .name = "Mute Control",
    .type = UAC_FU_MUTE,
    .set  = generic_set_cmd, /* Todo: add real Mute control code */
    .get  = generic_get_cmd, /* Todo: add real Mute control code */
};
    
static struct usb_audio_control volume_control_id9 = {
    .name = "Volume Control",
    .type = UAC_FU_VOLUME,
    .set  = generic_set_cmd, /* Todo: add real volume control code */
    .get  = generic_get_cmd, /* Todo: add real volume control code */
    .data = {0x0000, 0xF600, 0xDB00, 0x0000, 0x0100},
};

static struct usb_audio_control_selector feature_unit_idA = {
    .id   = FEATURE_UNIT_1_ID,
    .name = "Mute & Volume Control",
    .type = UAC_FEATURE_UNIT,
    .desc = (struct usb_descriptor_header *)&feature_unit_desc1,
};
    
static struct usb_audio_control mute_control_idA = {
    .name = "Mute Control",
    .type = UAC_FU_MUTE,
    .set = generic_set_cmd, /* Todo: add real Mute control code */
    .get = generic_get_cmd, /* Todo: add real Mute control code */
};

static struct usb_audio_control volume_control_idA = {
    .name = "Volume Control",
    .type = UAC_FU_VOLUME,
    .set  = generic_set_cmd, /* Todo: add real Mute control code */
    .get  = generic_get_cmd, /* Todo: add real Mute control code */
    .data = {0x0000, 0x0800, 0xF400, 0x0700, 0x0100},
};

static struct usb_audio_control_selector feature_unit_idD = {
    .id = FEATURE_UNIT_2_ID,
    .name = "Mute & Volume Control",
    .type = UAC_FEATURE_UNIT,
    .desc = (struct usb_descriptor_header *)&feature_unit_desc2,
};

static struct usb_audio_control mute_control_idD = {
    .name = "Mute Control",
    .type = UAC_FU_MUTE,
    .set = generic_set_cmd, /* Todo: add real Mute control code */
    .get = generic_get_cmd, /* Todo: add real Mute control code */
};

static struct usb_audio_control volume_control_idD = {
    .name = "Volume Control",
    .type = UAC_FU_VOLUME,
    .set = generic_set_cmd, /* Todo: add real Mute control code */
    .get = generic_get_cmd, /* Todo: add real Mute control code */
    .data = {0x0000, 0xF900, 0xE900, 0x0800, 0x0100},
};

static struct usb_audio_control_selector feature_unit_idF = {
    .id = MIXER_UNIT_2_ID,
    .name = "Mute & Volume Control",
    .type = UAC_MIXER_UNIT,
    .desc = (struct usb_descriptor_header *)&mixer_unit_desc,
};

static struct usb_audio_control mute_control_idF = {
    .name = "Mute Control",
    .type = UAC_FU_MUTE,
    .set = generic_set_cmd, /* Todo: add real Mute control code */
    .get = generic_get_cmd, /* Todo: add real Mute control code */
};

static struct usb_audio_control volume_control_idF = {
    .name = "Volume Control",
    .type = UAC_FU_VOLUME,
    .set = generic_set_cmd, /* Todo: add real Mute control code */
    .get = generic_get_cmd, /* Todo: add real Mute control code */
    .data = {0x0000, 0xF900, 0x0000, 0x0000, 0x0000},   // {0x0000, 0xF900, 0xE900, 0x0800, 0x0100}
};


uint16_t frame_size[2][2][8] = {
    // 16 bit
    {
        {16, 32, 64, 90, 96, 176, 192, 384},      // frame_size_2[0][0][0] = 8k, 1mono, 16 bits
        {32, 64, 128, 180, 192, 352, 384, 768}    // frame_size_2[0][1][0] = 8k, stereo, 16 bits
    },
    // 24 bit -> represet as 32 bit
    {
        {24, 48, 96, 135, 144, 264, 288, 576},    // frame_size_2[1][0][0] = 8k, 1mono, 24 bits
        {48, 96, 192, 270, 288, 528, 576, 1152}   // frame_size_2[1][1][0] = 8k, stereo, 24 bits
    }
};

uint8_t bit_res_table[2] = {2, 3}; // index = 16bit = 2 bytes

usb_cfg_t usbd_audio_cfg = {
    .bSpeed           = USB_SPEED_FULL,
        
    .bDeviceClass     = USB_CLASS_PER_INTERFACE,
    .bDeviceSubClass  = 0,
    .bDeviceProtocol  = 0,
    .idVendor         = REALTEK_USB_VID,
    .idProduct        = REALTEK_USB_PID,
    .bcdDevice        = 0x0100,
    
    .bmAttributes     = USB_CONFIG_ATT_ONE,
    .bMaxPower        = 50,

    .bIrqTaskPriority = tskIDLE_PRIORITY + 3,
    
    .sManufacturer    = "Realtek",
    .sProduct         = "USB Audio Device",
    .sSerialNumber    = "0123456789AB",
};

struct usb_iso_request *isooutreq = NULL;
struct usb_iso_request *isoinreq = NULL;
uint8_t audio_change_ep_cur = 0xFF;
USB_DSP_STATE usb_dsp_state;
USB_ISOEP_STATE usb_isoep_state;
usbd_audio_usr_cb_t *usb_audio_usr_cb;// usb to app interface
extern uint32_t g_otg_iso_bug_fix;
extern uint8_t g_otg_iso_in_process_buf;

inline struct f_audio *func_to_audio(struct usb_function *f)
{
    return CONTAINER_OF(f, struct f_audio, card.func);
}

static void process_iso_out_buffer(struct usb_ep *ep, struct usb_iso_request *isoreq)
{
    uint32_t real_ep_len;
    uint32_t real_len = 0;
    uint32_t *addr = NULL;
    UNUSED(ep);
    real_ep_len = (UAC1_ISO_OUT_EP_MAX_PACKET_SIZE - g_otg_iso_bug_fix);

    if (isoreq->iso_packet_desc0->actual_length != 0) {
        addr = (u32 *)isoreq->buf0;
        real_len = real_ep_len;//isoreq->iso_packet_desc0->actual_length;
        usb_audio_usr_cb->put_data((uint8_t *)addr, real_len);
        isoreq->iso_packet_desc0->actual_length = 0;
    }

    if (isoreq->iso_packet_desc1->actual_length != 0) {
        addr = (uint32_t *)isoreq->buf1;
        real_len = real_ep_len; //isoreq->iso_packet_desc1->actual_length;
        usb_audio_usr_cb->put_data((uint8_t *)addr, real_len);
        isoreq->iso_packet_desc1->actual_length = 0;
    }
}

static void process_iso_in_buffer(struct usb_ep *ep, struct usb_iso_request *isoreq)
{
    uint8_t tmp;
    uint32_t tempLen = isoinreq->data_per_frame;
    
    UNUSED(ep);

    if (g_otg_iso_in_process_buf == 0) {
        tmp = usb_audio_usr_cb->pull_data((uint8_t *)isoreq->buf1, tempLen);
        if (tmp == 0) {
            memset((uint8_t *)isoreq->buf1, 0, tempLen);
        }
    } else {
        tmp = usb_audio_usr_cb->pull_data((uint8_t *)isoreq->buf0, tempLen);
        if (tmp == 0) {
            memset((uint8_t *)isoreq->buf0, 0, tempLen);
        }
    }
}

static int f_audio_out_ep_complete(struct usb_ep *ep, struct usb_request *req)
{
    UNUSED(ep);
    UNUSED(req);
    return 0;
}

static void f_audio_complete(struct usb_ep *ep, struct usb_request *req)
{
    struct f_audio *audio = req->context;
    int status = req->status;
    uint32_t data = 0;
    struct usb_ep *out_ep = audio->out_ep;

    switch (status) {
        case 0:                         /* normal completion? */
            if (ep == out_ep) {
                f_audio_out_ep_complete(ep, req);
            } else if (audio->set_con) {
                memcpy(&data, req->buf, req->length);
                audio->set_con->set(audio->set_con, audio->set_cmd,
                    //le16_to_cpu(data));
                    (data));
                audio->set_con = NULL;
            } else if (audio_change_ep_cur == USB_ISO_OUT_ENDPOINT_ADDRESS) {
                uint32_t new_samp_freq = (*((uint32_t *)req->buf)) & 0xFFFFFF;
                int new_sample_freq_idx = audio->card.playback.sample_freq_idx;

                for (uint8_t i = 0; i < 8; i++) {
                    if (new_samp_freq == pcm_rate_table[i]) {
                        new_sample_freq_idx = i;
                    }
                }

                if (new_sample_freq_idx != audio->card.playback.sample_freq_idx) {
                    audio->card.playback.sample_freq_idx = new_sample_freq_idx;
                    audio_change_ep_cur = 0xFF;

                    if ((isooutreq != NULL) && (usb_dsp_state.usb_da_state ==  DSP_START)) {
                        // stop
                        usb_audio_usr_cb->unplug(IDX_DA);
                        usb_dsp_state.usb_da_state =  DSP_STOP;
                        audio->out_ep->ops->iso_ep_stop(audio->out_ep, isooutreq);
                        // start
                        uint8_t ch_cnt = audio->card.playback.channels - 1;
                        isooutreq->data_per_frame =
                            frame_size[audio->card.playback.format][ch_cnt][audio->card.playback.sample_freq_idx];
                        audio->out_ep->ops->iso_ep_start(audio->out_ep, isooutreq, 1);
                        usb_audio_usr_cb->plug(IDX_DA, audio->card.playback.format,
                            audio->card.playback.sample_freq_idx, audio->card.playback.channels);
                        usb_dsp_state.usb_da_state =  DSP_START;
                    }
                }
            } else if ((audio_change_ep_cur == USB_ISO_IN_ENDPOINT_ADDRESS)) {
                // handle
                uint32_t new_samp_freq = (*((uint32_t *)req->buf)) & 0xFFFFFF;
                int new_sample_freq_idx = audio->card.capture.sample_freq_idx;

                for (uint8_t i = 0; i < 8; i++) {
                    if (new_samp_freq == pcm_rate_table[i]) {
                        new_sample_freq_idx = i;
                    }
                }

                if (new_sample_freq_idx != audio->card.capture.sample_freq_idx) {
                    audio->card.capture.sample_freq_idx = new_sample_freq_idx;
                    audio_change_ep_cur = 0xFF;

                    if ((isoinreq != NULL) && (usb_dsp_state.usb_ad_state ==  DSP_START)) {
                        // stop
                        usb_audio_usr_cb->unplug(IDX_AD);
                        usb_dsp_state.usb_ad_state =  DSP_STOP;
                        audio->in_ep->ops->iso_ep_stop(audio->in_ep, isoinreq);
                        // start
                        uint8_t ch_cnt = audio->card.capture.channels - 1;
                        isoinreq->data_per_frame //= DATA_PER_FRAME_TMP; //= 192;   //= 96;
                            = frame_size[audio->card.capture.format][ch_cnt][audio->card.capture.sample_freq_idx];
                        audio->in_ep->ops->iso_ep_start(audio->in_ep, isoinreq, 1);
                        usb_audio_usr_cb->plug(IDX_AD, audio->card.capture.format,
                            audio->card.capture.sample_freq_idx, audio->card.capture.channels);
                        usb_dsp_state.usb_ad_state =  DSP_START;
                    }
                }
            }

            break;

        default:
            break;
    }
}

static int audio_set_intf_req(struct usb_function *f, const struct usb_control_request *ctrl)
{
    struct f_audio *audio = func_to_audio(f);
    struct usb_composite_dev *cdev = f->config->cdev;
    struct usb_request *req = cdev->req;
    uint8_t id = (((ctrl->wIndex) >> 8) & 0xFF);
    uint16_t len = (ctrl->wLength);
    uint16_t w_value = (ctrl->wValue);
    uint8_t con_sel = (w_value >> 8) & 0xFF;
    uint8_t cmd = (ctrl->bRequest & 0x0F);
    struct usb_audio_control_selector *cs;//dwc_list_link
    struct usb_audio_control *con;
    
    dwc_list_link_t *cs_list = NULL;
    dwc_list_link_t *con_list = NULL;
    
    DWC_LIST_FOREACH(cs_list, &audio->cs) {
        cs = DWC_LIST_ENTRY(cs_list, struct usb_audio_control_selector, list);
        if (cs->id == id) {
            DWC_LIST_FOREACH(con_list, &cs->control) {
                con = DWC_LIST_ENTRY(con_list, struct usb_audio_control, list);
                if (con->type == con_sel) {
                    audio->set_con = con;
                    break;
                }
            }
        }
    }
    
    audio->set_cmd = cmd;
    req->context = audio;
    req->complete = f_audio_complete;
    
    return len;
}

static int audio_get_intf_req(struct usb_function *f, const struct usb_control_request *ctrl)
{
    struct usb_composite_dev *cdev = f->config->cdev;
    struct usb_request *req = cdev->req;
    int value = -EOPNOTSUPP;
    uint8_t id = (((ctrl->wIndex) >> 8) & 0xFF);
    uint16_t len = (ctrl->wLength);
    uint16_t w_value = (ctrl->wValue);
    uint8_t con_sel = (w_value >> 8) & 0xFF;
    uint8_t cmd = (ctrl->bRequest & 0x0F);
    struct usb_audio_control_selector *cs;
    struct usb_audio_control *con;
    
    dwc_list_link_t *cs_list = NULL;
    dwc_list_link_t *con_list = NULL;
    
    DWC_LIST_FOREACH(cs_list, &audio->cs) {
        cs = DWC_LIST_ENTRY(cs_list, struct usb_audio_control_selector, list);
        if (cs->id == id) {
            DWC_LIST_FOREACH(con_list, &cs->control) {
                con = DWC_LIST_ENTRY(con_list, struct usb_audio_control, list);
                if (con->type == con_sel && con->get) {
                    value = con->get(con, cmd);
                    break;
                }
            }
        }
    }
    
    req->context = audio;
    req->complete = f_audio_complete;
    
    len = MIN(sizeof(value), len);
    memcpy(req->buf, &value, len);
    
    return len;
}

static int audio_set_endpoint_req(struct usb_function *f, const struct usb_control_request *ctrl)
{
    struct usb_composite_dev *cdev = f->config->cdev;
    int                     value = -EOPNOTSUPP;
    uint16_t                    ep = (ctrl->wIndex);
    uint16_t                    len = (ctrl->wLength);
    struct usb_request    *req = cdev->req;

    switch (ctrl->bRequest) {
        case UAC_SET_CUR:
            value = len;

            if (ep == USB_ISO_IN_ENDPOINT_ADDRESS) {
                audio_change_ep_cur = USB_ISO_IN_ENDPOINT_ADDRESS;
            } else if (ep == USB_ISO_OUT_ENDPOINT_ADDRESS) {
                audio_change_ep_cur = USB_ISO_OUT_ENDPOINT_ADDRESS;
            }

            break;

        case UAC_SET_MIN:
            break;

        case UAC_SET_MAX:
            break;

        case UAC_SET_RES:
            break;

        case UAC_SET_MEM:
            break;

        default:
            break;
    }

    // bbpro add
    req->context = audio;
    req->complete = f_audio_complete;
    return value;
}

static int audio_get_endpoint_req(struct usb_function *f, const struct usb_control_request *ctrl)
{
    int value = -EOPNOTSUPP;
    uint16_t len = (ctrl->wLength);
    UNUSED(f);

    switch (ctrl->bRequest) {
        case UAC_GET_CUR:
        case UAC_GET_MIN:
        case UAC_GET_MAX:
        case UAC_GET_RES:
            value = len;
            break;

        case UAC_GET_MEM:
            break;

        default:
            break;
    }

    return value;
}

static int f_audio_setup(struct usb_function *f, const struct usb_control_request *ctrl)
{
    struct usb_composite_dev *cdev = f->config->cdev;
    struct usb_request      *req = cdev->req;
    int                     value = -EOPNOTSUPP;

    /* composite driver infrastructure handles everything; interface
     * activation uses set_alt().
     */
    switch (ctrl->bmRequestType) {
        case USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE:
            value = audio_set_intf_req(f, ctrl);
            break;

        case USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE:
            value = audio_get_intf_req(f, ctrl);
            break;

        case USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_ENDPOINT:
            USB_PRINT_TRACE0("AUDIO USB_RECIP_ENDPOINT");
            value = audio_set_endpoint_req(f, ctrl);
            break;

        case USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_ENDPOINT:
            //USB_PRINT_TRACE0("AUDIO USB_RECIP_ENDPOINT");
            value = audio_get_endpoint_req(f, ctrl);
            break;

        default:
            USB_PRINT_ERROR5("invalid control req%02x.%02x v%04x i%04x l%d",
                ctrl->bmRequestType, ctrl->bRequest,
                ctrl->wValue, ctrl->wIndex, ctrl->wLength);
    }

    /* respond with data transfer or status phase? */
    if (value >= 0) {
        req->zero = 0;
        req->length = value;
        value = usb_ep_queue(cdev->gadget->ep0, req, 1);
        if (value < 0) {
            USB_PRINT_ERROR1("audio response on err %d\n", value);
        }
    }

    /* device either stalls (value < 0) or reports success */
    return value;
}

static int down_isoin(struct f_audio *audio)
{
    int err = 0;
    struct usb_ep *in_ep = audio->in_ep;

    if (usb_dsp_state.usb_ad_state == DSP_START) {
        usb_audio_usr_cb->unplug(IDX_AD);
        usb_dsp_state.usb_ad_state = DSP_STOP;
        err = in_ep->ops->iso_ep_stop(audio->in_ep, isoinreq);
    }

    return err;
}

static int up_isoin(struct f_audio *audio)
{
    int err = 0;
    struct usb_ep *in_ep = audio->in_ep;

    if ((isoinreq != NULL) && (usb_dsp_state.usb_ad_state == DSP_STOP)) {
        if (usb_isoep_state.usb_INEP_state == ISOEP_DISABLE) {
            isoinreq->process_buffer = process_iso_in_buffer;
            isoinreq->context = audio;
            isoinreq->buf_proc_intrvl = 1;
            isoinreq->sync_frame = 0;   // useless?
            isoinreq->flags = 1;
            isoinreq->start_frame = 0;
            isoinreq->dma0 = (dma_addr_t)isoinreq->buf0;
            isoinreq->dma1 = (dma_addr_t)isoinreq->buf1;
            in_ep->desc = (struct usb_endpoint_descriptor *)&as_in_ep_desc2;
            err = usb_ep_enable(in_ep, in_ep->desc);
            usb_isoep_state.usb_INEP_state = ISOEP_ENABLE;

            if (err != 0) {
                USB_PRINT_ERROR0("enable ISO IN EP fail");
            }
        }

        //2 enable dsp
        uint8_t ch_cnt = audio->card.capture.channels - 1;
        isoinreq->data_per_frame = frame_size[audio->card.capture.format][ch_cnt][audio->card.capture.sample_freq_idx];
        usb_audio_usr_cb->plug(IDX_AD, audio->card.capture.format,
            audio->card.capture.sample_freq_idx, audio->card.capture.channels);
        in_ep->ops->iso_ep_start(audio->in_ep, isoinreq, 1);
        usb_dsp_state.usb_ad_state = DSP_START;
    }

    return err;
}

static int down_isoout(struct f_audio *audio)
{
    int err = 0;
    struct usb_ep *out_ep = audio->out_ep;

    if (usb_dsp_state.usb_da_state == DSP_START) {
        usb_audio_usr_cb->unplug(IDX_DA);
        usb_dsp_state.usb_da_state = DSP_STOP;
        err = out_ep->ops->iso_ep_stop(audio->out_ep, isooutreq);
    }

    return err;
}

static int up_isoout(struct f_audio *audio)
{
    int err = 0;
    struct usb_ep *out_ep = audio->out_ep;

    if ((isooutreq != NULL) && (usb_dsp_state.usb_da_state == DSP_STOP)) {
        if (usb_isoep_state.usb_OUTEP_state == ISOEP_DISABLE) {
            isooutreq->process_buffer = process_iso_out_buffer;//complete_cb
            isooutreq->context = audio;
            isooutreq->buf_proc_intrvl = 1;//1;
            isooutreq->sync_frame = 0;// useless?
            isooutreq->flags = 1;
            isooutreq->start_frame = 0;
            isooutreq->dma0 = (dma_addr_t)isooutreq->buf0;
            isooutreq->dma1 = (dma_addr_t)isooutreq->buf1;
            out_ep->desc = (struct usb_endpoint_descriptor *)&as_out_ep_desc1;
            err = usb_ep_enable(out_ep, out_ep->desc);
            usb_isoep_state.usb_OUTEP_state = ISOEP_ENABLE;

            if (err != 0) {
                USB_PRINT_ERROR0("enable ISO OUT EP fail");
            }
        }

        //2 enable dsp
        uint8_t ch_cnt = audio->card.playback.channels - 1;
        isooutreq->data_per_frame = frame_size[audio->card.playback.format][ch_cnt][audio->card.playback.sample_freq_idx];
        usb_audio_usr_cb->plug(IDX_DA, audio->card.playback.format,
            audio->card.playback.sample_freq_idx, audio->card.playback.channels);
        usb_dsp_state.usb_da_state = DSP_START;
        out_ep->ops->iso_ep_start(audio->out_ep, isooutreq, 1);
    }

    return err;
}

/**
* enable endpoints and allocate usb request
* this cb will be called when valid set_configuration, and init all iinterface's alt = 0
*/
static int f_audio_set_alt(struct usb_function *function, unsigned intf, unsigned alt)
{
    struct f_audio  *audio = func_to_audio(function);
    int err = 0;

    //1 TODO: should be init ac_alt, as_alt, ac_intf, ac_intf = 0
    if (AUDIO_CONTROL_INTF_NUM == intf) {
        audio->ac_intf = AUDIO_CONTROL_INTF_NUM;
        audio->ac_alt = alt; // audio control alt must 0 for bbpro
    } else if (AUDIO_STREAM_OUT_INTF_NUM == intf) {
        audio->as_out_intf = AUDIO_STREAM_OUT_INTF_NUM;
        audio->as_out_alt = alt;
    } else if (AUDIO_STREAM_IN_INTF_NUM == intf) {
        audio->as_in_intf = AUDIO_STREAM_IN_INTF_NUM;
        audio->as_in_alt = alt;
    }

    if (intf == AUDIO_STREAM_OUT_INTF_NUM) {
        if (alt == 0) {
            if (isooutreq != NULL) {
                err = down_isoout(audio);
            }
        } else if (alt == 1) {
            err = up_isoout(audio);
        } else {
            err = -1;
        }
    }// if (intf == AUDIO_STREAM_OUT_INTF_NUM)
    else if (intf == AUDIO_STREAM_IN_INTF_NUM) {
        if (alt == 0) {
            if (isoinreq != NULL) {
                down_isoin(audio);
            }
        } else if (alt == 1) {
            err = up_isoin(audio);
        } else {
            err = -ENOMEM;
        }
    }

    return err;
}

static int f_audio_get_alt(struct usb_function *function, unsigned intf)
{
    struct f_audio          *audio = func_to_audio(function);
    uint8_t alt;

    // bbpro add
    if (intf == audio->ac_intf) {
        alt = audio->ac_alt;
    } else if (intf == audio->as_out_intf) {
        alt = audio->as_out_alt;
    } else  if (intf == audio->as_in_intf) {
        alt = audio->as_in_alt;
    } else {
        alt = 0;
    }

    return alt;
}

static int f_audio_bind(struct usb_configuration *c, struct usb_function *function)
{
    struct usb_ep *ep;
    int status = -ENODEV;
    int id;
    
    audio->card.gadget = c->cdev->gadget;
    
    /* allocate instance-specific interface IDs, and patch descriptors */
    id = usb_interface_id(c, function); // this will return the allocated interface id
    if (id < 0) {
        status = id;
        goto fail;
    }

    ac_interface_desc0.bInterfaceNumber = id;
    id = usb_interface_id(c, function); // this will return the allocated interface id
    if (id < 0) {
        status = id;
        goto fail;
    }

    as_interface_alt_0_desc.bInterfaceNumber = id;
    as_interface_alt_1_desc.bInterfaceNumber = id;
    
    /* search endpoint according to endpoint descriptor and modify endpoint address*/
    ep = usb_ep_autoconfig(c->cdev->gadget, (struct usb_endpoint_descriptor *)&as_out_ep_desc1);
    if (!ep) {
        goto fail;
    }

    ep->driver_data = audio; /* claim */
    audio->out_ep = ep;
    
    id = usb_interface_id(c, function); // this will return the allocated interface id
    if (id < 0) {
        status = id;
        goto fail;
    }

    as_interface_desc2_alt0.bInterfaceNumber = id;
    as_interface_desc2_alt1.bInterfaceNumber = id;
    
    /* search endpoint according to endpoint descriptor and modify endpoint address*/
    ep = usb_ep_autoconfig(c->cdev->gadget, (struct usb_endpoint_descriptor *)&as_in_ep_desc2);
    if (!ep) {
        goto fail;
    }

    ep->driver_data = audio; /* claim */
    audio->in_ep = ep;
    
    status = usb_assign_descriptors(function, (struct usb_descriptor_header **)&f_audio_desc, NULL, NULL);
    if (status) {
        goto fail;
    }

    return 0;
fail:
    usb_free_all_descriptors(function);
    return status;
}

static void f_audio_unbind(struct usb_configuration *c, struct usb_function *function)
{
    UNUSED(c);
    UNUSED(function);
}

static void f_audio_disable(struct usb_function *function)
{
    UNUSED(function);
    usb_ep_disable(audio->out_ep);
    audio->out_ep->driver_data = NULL;
    // disable iso in EP
    usb_ep_disable(audio->in_ep);
    audio->in_ep->driver_data = NULL;
}

static void f_audio_free(struct usb_function *function)
{
    if (isoinreq != NULL) {
        if (isoinreq->buf0 != NULL) {
            DWC_FREE(isoinreq->buf0);
        }
        if (isoinreq->buf1 != NULL) {
            DWC_FREE(isoinreq->buf1);
        }
        audio->in_ep->ops->free_iso_request(audio->in_ep, isoinreq);
    }
    
    if (isooutreq != NULL) {
        audio->out_ep->ops->free_iso_request(audio->out_ep, isooutreq);
    }
    
    if (pcm_rate_table != NULL) {
        DWC_FREE(pcm_rate_table);
    }
    
    usb_free_all_descriptors(function);
}

static int generic_set_cmd(struct usb_audio_control *con, uint8_t cmd, int value)
{
    con->data[cmd] = value;
    return 0;
}

static int generic_get_cmd(struct usb_audio_control *con, uint8_t cmd)
{
    return con->data[cmd];
}

/* Todo: add more control selecotor dynamically */
static int control_selector_init(struct f_audio *audio)
{
    DWC_LIST_INIT(&audio->cs);
    
    //3 ID 9
    DWC_LIST_INIT(&feature_unit_id9.list);
    DWC_LIST_INIT(&mute_control_id9.list);
    DWC_LIST_INIT(&volume_control_id9.list);
    DWC_LIST_INSERT_TAIL(&audio->cs, &feature_unit_id9.list);
    DWC_LIST_INIT(&feature_unit_id9.control);
    DWC_LIST_INSERT_TAIL(&feature_unit_id9.control, &mute_control_id9.list);
    DWC_LIST_INSERT_TAIL(&feature_unit_id9.control, &volume_control_id9.list);
    
    //3 ID A
    DWC_LIST_INIT(&feature_unit_idA.list);
    DWC_LIST_INIT(&volume_control_idA.list);
    DWC_LIST_INIT(&mute_control_idA.list);
    DWC_LIST_INSERT_TAIL(&audio->cs, &feature_unit_idA.list);
    DWC_LIST_INIT(&feature_unit_idA.control);
    DWC_LIST_INSERT_TAIL(&feature_unit_idA.control, &mute_control_idA.list);
    DWC_LIST_INSERT_TAIL(&feature_unit_idA.control, &volume_control_idA.list);
    
    //3 ID D
    DWC_LIST_INIT(&feature_unit_idD.list);
    DWC_LIST_INIT(&mute_control_idD.list);
    DWC_LIST_INIT(&volume_control_idD.list);
    DWC_LIST_INSERT_TAIL(&audio->cs, &feature_unit_idD.list);
    DWC_LIST_INIT(&feature_unit_idD.control);
    DWC_LIST_INSERT_TAIL(&feature_unit_idD.control, &mute_control_idD.list);
    DWC_LIST_INSERT_TAIL(&feature_unit_idD.control, &volume_control_idD.list);

    //3 ID F
    DWC_LIST_INIT(&feature_unit_idF.list);
    DWC_LIST_INIT(&mute_control_idF.list);
    DWC_LIST_INIT(&volume_control_idF.list);
    DWC_LIST_INSERT_TAIL(&audio->cs, &feature_unit_idF.list);
    DWC_LIST_INIT(&feature_unit_idF.control);
    DWC_LIST_INSERT_TAIL(&feature_unit_idF.control, &mute_control_idF.list);
    DWC_LIST_INSERT_TAIL(&feature_unit_idF.control, &volume_control_idF.list);
    
    return 0;
}

static void control_selector_deinit(struct f_audio *audio)
{
    DWC_LIST_INIT(&audio->cs);
    
    //3 ID 9
    DWC_LIST_INIT(&feature_unit_id9.list);
    DWC_LIST_INIT(&mute_control_id9.list);
    DWC_LIST_INIT(&volume_control_id9.list);
    DWC_LIST_INIT(&feature_unit_id9.control);
    
    //3 ID A
    DWC_LIST_INIT(&feature_unit_idA.list);
    DWC_LIST_INIT(&volume_control_idA.list);
    DWC_LIST_INIT(&mute_control_idA.list);
    DWC_LIST_INIT(&feature_unit_idA.control);
    
    //3 ID D
    DWC_LIST_INIT(&feature_unit_idD.list);
    DWC_LIST_INIT(&mute_control_idD.list);
    DWC_LIST_INIT(&volume_control_idD.list);
    DWC_LIST_INIT(&feature_unit_idD.control);

    //3 ID F
    DWC_LIST_INIT(&feature_unit_idF.list);
    DWC_LIST_INIT(&mute_control_idF.list);
    DWC_LIST_INIT(&volume_control_idF.list);
    DWC_LIST_INIT(&feature_unit_idF.control);
}

#define MAX_REQ_SIZE 1500
struct f_audio g_audio;
SRAM_NOCACHE_DATA_SECTION static u8 usb_rx_buf0[1500];
SRAM_NOCACHE_DATA_SECTION static u8 usb_rx_buf1[1500];

static int usbd_audio_init(struct usb_configuration *c)
{
    int status;
    
    audio = &g_audio;

    g_req_out_buf_size = MAX_REQ_SIZE; //otp.efuse_usb_audio_policy.play_max_packet_size;;
    g_req_in_buf_size = MAX_REQ_SIZE; //otp.efuse_usb_audio_policy.capture_max_packet_size;
    
    //2 usb function function map
    c->cdev->gadget->device = audio; //1 FIX IT
    audio->gadget = c->cdev->gadget;
    audio->card.func.name = "audio_function_sets";
    audio->card.func.bind = f_audio_bind;
    audio->card.func.unbind = f_audio_unbind;
    audio->card.func.set_alt = f_audio_set_alt;
    audio->card.func.get_alt = f_audio_get_alt;
    audio->card.func.setup = f_audio_setup; // process audio class command only, ex GET_CUR
    audio->card.func.disable = f_audio_disable;
    audio->card.func.free_func = f_audio_free;
    control_selector_init(audio);
    audio->card.playback.sample_freq_idx = ICODEC_SR_48K;
    audio->card.playback.format = 0; //(otp.efuse_usb_audio_policy.play_bit_resolution == 24) ? 1 : 0;
    audio->card.playback.channels = DEFAULT_PLAY_CH;
    audio->card.capture.sample_freq_idx = ICODEC_SR_48K;
    audio->card.capture.format = 0; //(otp.efuse_usb_audio_policy.capture_bit_resolution == 24) ? 1 : 0;
    audio->card.capture.channels =  DEFAULT_RECORD_CH;
    status = usb_add_function(c, &audio->card.func);// add interface 0 to this configuration
    if (status != ESUCCESS) {
        USB_PRINT_ERROR0("add function fail");
        return status;
    }
    
    // pre allocate request
    pcm_rate_table = DWC_ALLOC(PCM_TABLE_CNT * 4);
    if (pcm_rate_table == NULL) {
        USB_PRINT_ERROR0("PCM rate table allocate fail");
        usb_put_function(&audio->card.func);
        return -ENOMEM;
    } else {
        *(pcm_rate_table + 0) = 8000;
        *(pcm_rate_table + 1) = 16000;
        *(pcm_rate_table + 2) = 32000;
        *(pcm_rate_table + 3) = 44100;
        *(pcm_rate_table + 4) = 48000;
        *(pcm_rate_table + 5) = 88000;
        *(pcm_rate_table + 6) = 96000;
        *(pcm_rate_table + 7) = 192000;
    }
    
    isoinreq = audio->in_ep->ops->alloc_iso_request(audio->in_ep, 1, 1);
    isoinreq->buf0 =  DWC_ALLOC(g_req_in_buf_size);
    isoinreq->buf1 = DWC_ALLOC(g_req_in_buf_size);
    if ((isoinreq == NULL) || (isoinreq->buf0 == NULL) || (isoinreq->buf1 == NULL)) {
        USB_PRINT_ERROR0("isoinreq allocate fail");
        usb_put_function(&audio->card.func);
        DWC_FREE(pcm_rate_table);
        return -ENOMEM;
    }

    isooutreq = audio->out_ep->ops->alloc_iso_request(audio->out_ep, 1, 1);
    isooutreq->buf0 = usb_rx_buf0;
    isooutreq->buf1 = usb_rx_buf1;
    if ((isooutreq == NULL) || (isooutreq->buf0 == NULL) || (isooutreq->buf1 == NULL)) {
        USB_PRINT_ERROR0("isooutreq allocate fail");
        usb_put_function(&audio->card.func);
        DWC_FREE(pcm_rate_table);
        DWC_FREE(isoinreq->buf0);
        DWC_FREE(isoinreq->buf1);
        DWC_FREE(isoinreq);
        return -ENOMEM;
    }

    usb_isoep_state.usb_INEP_state = ISOEP_DISABLE;
    usb_isoep_state.usb_OUTEP_state = ISOEP_DISABLE;
    usb_dsp_state.usb_ad_state = DSP_STOP;
    usb_dsp_state.usb_da_state = DSP_STOP;

    if (usb_audio_usr_cb->init != NULL) {
        status = usb_audio_usr_cb->init();
        if (status != ESUCCESS) {
            USB_PRINT_ERROR0("user init fail");
            usb_put_function(&audio->card.func);
            DWC_FREE(pcm_rate_table);
            DWC_FREE(isoinreq->buf0);
            DWC_FREE(isoinreq->buf1);
            DWC_FREE(isoinreq);
            DWC_FREE(isooutreq);
            return status;
        }
    }

    return ESUCCESS;
}

static void usbd_audio_deinit(struct usb_configuration *c)
{
    if (usb_audio_usr_cb->deinit != NULL) {
        usb_audio_usr_cb->deinit();
    }
    
    control_selector_deinit(audio);
    usb_remove_function(c, &audio->card.func);
    usb_put_function(&audio->card.func);
    
    usb_isoep_state.usb_INEP_state = ISOEP_DISABLE;
    usb_isoep_state.usb_OUTEP_state = ISOEP_DISABLE;
    usb_dsp_state.usb_ad_state = DSP_STOP;
    usb_dsp_state.usb_da_state = DSP_STOP;
}

static int usbd_audio_stop(struct usb_configuration *c)
{
    if (usb_audio_usr_cb->unplug) {
        usb_audio_usr_cb->unplug(IDX_DA);
        usb_audio_usr_cb->unplug(IDX_AD);
        usb_dsp_state.usb_da_state =  DSP_STOP;
        usb_isoep_state.usb_OUTEP_state = ISOEP_DISABLE;
        usb_isoep_state.usb_INEP_state = ISOEP_DISABLE;
    }

    return ESUCCESS;
}

usb_class_cb_t usbd_audio_cb = {
    .init = usbd_audio_init,
    .deinit = usbd_audio_deinit,
    .stop = usbd_audio_stop
};

int usbd_audio_register(usbd_audio_usr_cb_t *cb)
{
    int status = -EINVAL;
    if (cb != NULL) {
        usb_audio_usr_cb = cb;
        usb_register_class(&usbd_audio_cfg, &usbd_audio_cb);
        status = ESUCCESS;
    }
    
    return status;
}

#endif // CONFIG_USBD_AUDIO

