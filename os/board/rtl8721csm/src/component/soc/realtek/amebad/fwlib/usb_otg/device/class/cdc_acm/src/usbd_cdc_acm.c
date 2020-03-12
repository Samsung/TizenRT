#include <platform_opts.h>

#ifdef CONFIG_USBD_CDC_ACM

#include "cdc_acm/inc/usbd_cdc_acm.h"
#include "usb_multi.h"

// CDC ACM Communication Interface
struct usb_interface_descriptor usbd_cdc_acm_comm_if_desc = {
    .bLength            = USB_DT_INTERFACE_SIZE,
    .bDescriptorType    = USB_DT_INTERFACE,
    .bInterfaceNumber   = 0,                        // This will be assign automatically
    .bAlternateSetting  = 0,
    .bNumEndpoints      = 0,
    .bInterfaceClass    = USB_CLASS_COMM,
    .bInterfaceSubClass = USBD_CDC_SUBCLASS_ACM,    // Abstract Control Model
    .bInterfaceProtocol = USBD_CDC_ACM_PROTO_NONE,  // None
    .iInterface         = 0,
};

// CDC ACM Function Header Descriptor
struct usb_cdc_header_desc usbd_cdc_acm_header_desc = {
    .bLength            = USBD_CDC_DT_HEADER_SIZE,
    .bDescriptorType    = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = USBD_CDC_HEADER_TYPE,

    .bcdCDC             = 0x0110,
};

// CDC ACM Call Magagement Functional Descriptor
struct usb_cdc_call_mgmt_descriptor usbd_cdc_acm_call_mgmt_desc = {
    .bLength            = USBD_CDC_DT_CALL_MGMT_SIZE,
    .bDescriptorType    = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = USBD_CDC_CALL_MANAGEMENT_TYPE,

    .bmCapabilities     = 0x00,
    .bDataInterface     = 0x01,
};

// CDC ACM ACM Functional Descriptor
struct usb_cdc_acm_descriptor usbd_cdc_acm_acm_desc = {
    .bLength            = USBD_CDC_DT_ACM_SIZE,
    .bDescriptorType    = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = USBD_CDC_ACM_TYPE,

    .bmCapabilities     = 0x02,
};

// CDC ACM Union Functional Descriptor
struct usb_cdc_union_desc usbd_cdc_acm_union_desc = {
    .bLength            = USBD_CDC_DT_UNION_SIZE,
    .bDescriptorType    = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = USBD_CDC_UNION_TYPE,

    .bControlInterface  = 0,     // index of control interface
    .bSubordinateInterface0 = 1, // index of data interface
};

// CDC ACM Data Interface
struct usb_interface_descriptor usbd_cdc_acm_data_if_desc = {
    .bLength            = USB_DT_INTERFACE_SIZE,
    .bDescriptorType    = USB_DT_INTERFACE,

    .bInterfaceNumber   = 1,
    .bAlternateSetting  = 0,
    .bNumEndpoints      = 2,
    .bInterfaceClass    = USB_CLASS_CDC_DATA,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface         = 0,
};

// CDC ACM Data Bulk IN Endpoint for Low-speed/Full-speed
struct usb_endpoint_descriptor usbd_cdc_acm_data_bulk_in_ep_desc_FS = {
    .bLength            = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType    = USB_DT_ENDPOINT,
    .bEndpointAddress   = USBD_CDC_ACM_BULK_IN_EP_ADDRESS,
    .bmAttributes       = USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize     = 64,
    .bInterval          = 0,
};

// CDC ACM Data Bulk OUT Endpoint for Low-speed/Full-speed
struct usb_endpoint_descriptor usbd_cdc_acm_data_bulk_out_ep_desc_FS = {
    .bLength            = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType    = USB_DT_ENDPOINT,
    .bEndpointAddress   = USBD_CDC_ACM_BULK_OUT_EP_ADDRESS,
    .bmAttributes       = USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize     = 64,
    .bInterval          = 0,
};

// CDC ACM Data Bulk IN Endpoint for High-speed
struct usb_endpoint_descriptor usbd_cdc_acm_data_bulk_in_ep_desc_HS = {
    .bLength            = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType    = USB_DT_ENDPOINT,
    .bEndpointAddress   = USBD_CDC_ACM_BULK_IN_EP_ADDRESS,
    .bmAttributes       = USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize     = 512,
    .bInterval          = 0,
};

// CDC ACM Data Bulk OUT Endpoint for High-speed
struct usb_endpoint_descriptor usbd_cdc_acm_data_bulk_out_ep_desc_HS = {
    .bLength            = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType    = USB_DT_ENDPOINT,
    .bEndpointAddress   = USBD_CDC_ACM_BULK_OUT_EP_ADDRESS,
    .bmAttributes       = USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize     = 512,
    .bInterval          = 0,
};

struct usb_descriptor_header *usbd_cdc_acm_descriptors_FS[] = {
    (struct usb_descriptor_header *) &usbd_cdc_acm_comm_if_desc,
    (struct usb_descriptor_header *) &usbd_cdc_acm_header_desc,
    (struct usb_descriptor_header *) &usbd_cdc_acm_call_mgmt_desc,
    (struct usb_descriptor_header *) &usbd_cdc_acm_acm_desc,
    (struct usb_descriptor_header *) &usbd_cdc_acm_union_desc,
    (struct usb_descriptor_header *) &usbd_cdc_acm_data_if_desc,
    (struct usb_descriptor_header *) &usbd_cdc_acm_data_bulk_in_ep_desc_FS,
    (struct usb_descriptor_header *) &usbd_cdc_acm_data_bulk_out_ep_desc_FS,
    NULL,
};

struct usb_descriptor_header *usbd_cdc_acm_descriptors_HS[] = {
    (struct usb_descriptor_header *) &usbd_cdc_acm_comm_if_desc,
    (struct usb_descriptor_header *) &usbd_cdc_acm_header_desc,
    (struct usb_descriptor_header *) &usbd_cdc_acm_call_mgmt_desc,
    (struct usb_descriptor_header *) &usbd_cdc_acm_acm_desc,
    (struct usb_descriptor_header *) &usbd_cdc_acm_union_desc,
    (struct usb_descriptor_header *) &usbd_cdc_acm_data_if_desc,
    (struct usb_descriptor_header *) &usbd_cdc_acm_data_bulk_in_ep_desc_HS,
    (struct usb_descriptor_header *) &usbd_cdc_acm_data_bulk_out_ep_desc_HS,
    NULL,
};

usbd_cdc_acm_line_coding_t acm_line_coding = {
    .dwDTERate   = 115200,                          // baud rate
    .bCharFormat = USBD_CDC_ACM_CHAR_FORMAT_STOP_1, // stop bit - 1
    .bParityType = USBD_CDC_ACM_PARITY_TYPE_NONE,   // parity - none
    .bDataBits   = USBD_CDC_ACM_DATA_BIT_8,         // data bits - 8
};

usbd_cdc_acm_ctrl_line_state_t acm_ctrl_line_state = {
    .d16 = 0,
};

usb_cfg_t usbd_cdc_acm_cfg = {
    .bSpeed           = USB_SPEED_HIGH,
        
    .bDeviceClass     = USB_CLASS_COMM,
    .bDeviceSubClass  = USBD_CDC_SUBCLASS_ACM,
    .bDeviceProtocol  = 0,
    .idVendor         = REALTEK_USB_VID,
    .idProduct        = REALTEK_USB_PID,
    .bcdDevice        = 0x0100,
    
    .bmAttributes     = USB_CONFIG_ATT_ONE,
    .bMaxPower        = 50,

    .bIrqTaskPriority = tskIDLE_PRIORITY + 3,
    
    .sManufacturer    = "Realtek",
    .sProduct         = "USB CDC ACM Device",
    .sSerialNumber    = "0123456789AB",
};

#define USBD_CDC_GET_EP_DESC(g,hs,fs) (((g)->speed == USB_SPEED_HIGH)?(hs):(fs))

struct usb_cdc_dev_t usb_cdc_dev;

static int usbd_cdc_acm_receive_data(struct usb_cdc_dev_t *dev);

void usbd_cdc_acm_set_bulk_transfer_buffer_size(u16 bulk_out_size, u16 bulk_in_size)
{
    struct usb_cdc_dev_t *dev = &usb_cdc_dev;
    if (bulk_out_size > 0) {
        dev->bulk_out_buf_len = bulk_out_size;
    }
    if (bulk_in_size > 0) {
        dev->bulk_in_buf_len = bulk_in_size;
    }
}

static void cdc_ctrl_out_complete(struct usb_ep *ep, struct usb_request *req)
{
    UNUSED(ep);
    USBD_CDC_ENTER;

    if ((req->status == ESUCCESS) && 
        (req->actual == USBD_CDC_ACM_LINE_CODING_SIZE) &&
        (req->direct == EP0_DATA_OUT)) {
        memcpy((void *)&acm_line_coding, req->buf, req->actual);
    }
    
    USBD_CDC_EXIT;
}

static void cdc_bulk_in_complete(struct usb_ep *ep, struct usb_request *req)
{
    UNUSED(ep);
    UNUSED(req);
}

static void cdc_bulk_out_complete(struct usb_ep *ep, struct usb_request *req)
{
    struct usb_cdc_dev_t *dev = ep->driver_data;
    int result = 0;
    int status = req->status;
    
    USBD_CDC_ENTER;

    switch (status) {
        case ESUCCESS:
            if ((req->actual > 0) && (dev->cb->receive != NULL)) {
                result = dev->cb->receive(req->buf, req->actual);
                if (result != ESUCCESS) {
                    USBD_CDC_WARN("user data processing fail\n");
                }
                
                usbd_cdc_acm_receive_data(dev);
            }

            break;

        case -ESHUTDOWN:
            USBD_CDC_WARN("%s shutdown\n", ep->name);
            break;

        default:
            USBD_CDC_WARN("status %d\n", status);
            break;
    }

    USBD_CDC_EXIT;
}

static void cdc_remove_bulk_in_requests(struct usb_cdc_dev_t *dev)
{
    if (dev->bulk_in_req != NULL) {
        usb_ep_dequeue(dev->bulk_out_ep, dev->bulk_in_req);
    }
}

static void cdc_remove_bulk_out_requests(struct usb_cdc_dev_t *dev)
{
    if (dev->bulk_out_req != NULL) {
        usb_ep_dequeue(dev->bulk_out_ep, dev->bulk_out_req);
    }
}

static struct usb_request *cdc_alloc_request(struct usb_ep *ep, u32 buf_len)
{
    struct usb_request *req = NULL;
    
    req = usb_ep_alloc_request(ep, 1);
    if (!req) {
        USBD_CDC_ERROR("alloc request fail\n");
        return NULL;
    }

    req->buf = DWC_ALLOC(buf_len);
    if (NULL == req->buf) {
        USBD_CDC_ERROR("alloc request buffer fail\n");
        usb_ep_free_request(ep, req);
        return NULL;
    }

    req->dma = (dma_addr_t)req->buf;
    req->length = buf_len;
    
    return req;
}

static void cdc_free_request(struct usb_ep *ep, struct usb_request *req)
{
    if (ep != NULL && req != NULL) {
        DWC_FREE(req->buf);
        usb_ep_free_request(ep, req);
    }
}

static int cdc_init_resource(struct usb_cdc_dev_t *dev)
{
    struct usb_request *req;
    int status = -EINVAL;

    USBD_CDC_ENTER;

    // Enable data in endpoint
    if (dev->bulk_in_ep != NULL) {
        if (dev->bulk_in_ep->driver_data != NULL) {
            usb_ep_disable(dev->bulk_in_ep);
        }

        dev->bulk_in_ep->desc = USBD_CDC_GET_EP_DESC(dev->gadget, &usbd_cdc_acm_data_bulk_in_ep_desc_HS,
                &usbd_cdc_acm_data_bulk_in_ep_desc_FS);
        
        status = usb_ep_enable(dev->bulk_in_ep, dev->bulk_in_ep->desc);
        if (status < 0) {
            USBD_CDC_ERROR("enable Bulk IN endpoint fail\n");
            USBD_CDC_EXIT_ERR;
            return -EINVAL;
        }

        dev->bulk_in_ep->driver_data = dev;
    }

    // Enable data out endpoint and alloc requset to recieve data from host
    if (dev->bulk_out_ep != NULL) {
        if (dev->bulk_out_ep->driver_data != NULL) {
            usb_ep_disable(dev->bulk_out_ep);
        }

        dev->bulk_out_ep->desc = USBD_CDC_GET_EP_DESC(dev->gadget, &usbd_cdc_acm_data_bulk_out_ep_desc_HS,
                &usbd_cdc_acm_data_bulk_out_ep_desc_FS);
        
        status = usb_ep_enable(dev->bulk_out_ep, dev->bulk_out_ep->desc);
        if (status < 0) {
            USBD_CDC_ERROR("enable Bulk OUT endpoint fail\n");
            USBD_CDC_EXIT_ERR;
            return -EINVAL;
        }

        dev->bulk_out_ep->driver_data = dev;
    }
    
    req = cdc_alloc_request(dev->bulk_out_ep, dev->bulk_out_buf_len);
    if (req == NULL) {
        USBD_CDC_ERROR("allocate Bulk OUT request fail\n");
        USBD_CDC_EXIT_ERR;
        return -ENOMEM;
    } else {
        req->complete = cdc_bulk_out_complete;
        req->context = dev;
        dev->bulk_out_req = req;
    }

    req = cdc_alloc_request(dev->bulk_in_ep, dev->bulk_in_buf_len);
    if (req == NULL) {
        USBD_CDC_ERROR("allocate Bulk IN request fail\n");
        USBD_CDC_EXIT_ERR;
        return -ENOMEM;
    } else {
        req->complete = cdc_bulk_in_complete;
        req->context = dev;
        dev->bulk_in_req = req;
    }

    USBD_CDC_EXIT;

    return ESUCCESS;
}

static int cdc_deinit_resource(struct usb_cdc_dev_t *dev)
{
    cdc_remove_bulk_out_requests(dev);
    cdc_free_request(dev->bulk_out_ep, dev->bulk_out_req);
    cdc_remove_bulk_in_requests(dev);
    cdc_free_request(dev->bulk_in_ep, dev->bulk_in_req);

    return ESUCCESS;
}

static int fun_setup(struct usb_function *f, const struct usb_control_request *ctrl)
{
    struct usb_cdc_dev_t *dev = CONTAINER_OF(f, struct usb_cdc_dev_t, funs);
    struct usb_composite_dev *cdev = f->config->cdev;
    struct usb_request *req0 = cdev->req;
    struct usb_ep *ep0 = dev->gadget->ep0;
    u8 bmRequestType = ctrl->bmRequestType;
    u8 bRequest = ctrl->bRequest;
    u16 wLength = ctrl->wLength;
    u16 wIndex = ctrl->wIndex;
    u16 wValue = ctrl->wValue;
    int value = -EOPNOTSUPP;
    
    USBD_CDC_ENTER;

    if (USB_GET_TYPE(bmRequestType) != USB_TYPE_CLASS) {
        USBD_CDC_EXIT_ERR;
        return -EOPNOTSUPP;
    }

    switch (bRequest) {
        case USBD_CDC_SET_LINE_CODING:
            USBD_CDC_INFO("USBD_CDC_SET_LINE_CODING");
            if ((bmRequestType == USB_REQ_TYPE_WRITE_CLASS_INTERFACE) &&
                (wIndex == dev->interface_number) &&
                (wValue == 0) &&
                (wLength == USBD_CDC_ACM_LINE_CODING_SIZE)) {
                value = wLength;
                req0->direct = EP0_DATA_OUT;
                req0->complete = cdc_ctrl_out_complete;
            }
            break;

        case USBD_CDC_GET_LINE_CODING:
            USBD_CDC_INFO("USBD_CDC_GET_LINE_CODING");
            if ((bmRequestType == USB_REQ_TYPE_READ_CLASS_INTERFACE) &&
                (wIndex == dev->interface_number) &&
                (wValue == 0) &&
                (wLength == USBD_CDC_ACM_LINE_CODING_SIZE)) {
                u8 *buf = (u8 *)req0->buf;
                buf[0] = (u8)(acm_line_coding.dwDTERate & 0xFF);
                buf[1] = (u8)((acm_line_coding.dwDTERate >> 8) & 0xFF);
                buf[2] = (u8)((acm_line_coding.dwDTERate >> 16) & 0xFF);
                buf[3] = (u8)((acm_line_coding.dwDTERate >> 24) & 0xFF);
                buf[4] = acm_line_coding.bCharFormat;
                buf[5] = acm_line_coding.bParityType;
                buf[6] = acm_line_coding.bDataBits;
                value = wLength;
                req0->direct = EP0_DATA_IN;
            }
            break;
        case USBD_CDC_SET_CONTROL_LINE_STATE:
            USBD_CDC_INFO("USBD_CDC_SET_CONTROL_LINE_STATE");
            if ((bmRequestType == USB_REQ_TYPE_WRITE_CLASS_INTERFACE) &&
                (wIndex == dev->interface_number) &&
                (wLength == 0)) {
                acm_ctrl_line_state.d16 = wValue;
                value = USB_GADGET_DELAYED_STATUS;
                req0->direct = EP0_DATA_OUT;
            }
            break;

        default:
            USBD_CDC_WARN("Unsupported request = %d\n", bRequest);
            break;
    }

    if ((value >= 0) && (value != USB_GADGET_DELAYED_STATUS)) {
        req0->length = value;
        req0->zero = 0;
        value = usb_ep_queue(ep0, req0, 1);
        if (value != 0 && value != -ESHUTDOWN) {
            USBD_CDC_WARN("EP0 request queue error: %d\n", value);
        }
    }

    USBD_CDC_EXIT;
    return value;
}

static int fun_set_alt(struct usb_function *f, unsigned interface, unsigned alt)
{
    int status = -EINVAL;
    struct usb_cdc_dev_t *dev = CONTAINER_OF(f, struct usb_cdc_dev_t, funs);
    
    UNUSED(interface);
    UNUSED(alt);
    
    USBD_CDC_ENTER;
    
    status = cdc_init_resource(dev);
    if (status != ESUCCESS) {
        cdc_deinit_resource(dev);
        USBD_CDC_EXIT_ERR;
        return status;
    }

    usbd_cdc_acm_receive_data(dev);
    
    USBD_CDC_EXIT;
    
    return ESUCCESS;
}

static void fun_disable(struct usb_function *f)
{
    struct usb_cdc_dev_t *dev = CONTAINER_OF(f, struct usb_cdc_dev_t, funs);
    
    USBD_CDC_ENTER;

    if (dev->bulk_in_ep != NULL) {
        cdc_remove_bulk_in_requests(dev);
        if (dev->bulk_in_ep->driver_data != NULL) {
            usb_ep_disable(dev->bulk_in_ep);
            dev->bulk_in_ep->driver_data = NULL;
        }
    }
    
    if (dev->bulk_out_ep != NULL) {
        cdc_remove_bulk_out_requests(dev);
        if (dev->bulk_out_ep->driver_data != NULL) {
            usb_ep_disable(dev->bulk_out_ep);
            dev->bulk_out_ep->driver_data = NULL;
        }
    }
    
    USBD_CDC_EXIT;
}

static int fun_bind(struct usb_configuration *c, struct usb_function *f)
{
    struct usb_cdc_dev_t *dev = CONTAINER_OF(f, struct usb_cdc_dev_t, funs);
    struct usb_ep *ep;
    int status = -ENODEV;
    int id;
    
    USBD_CDC_ENTER;
    
    // Allocate instance-specific interface ID
    id = usb_interface_id(c, f); // this will return the allocated interface id
    if (id < 0) {
        status = id;
        goto fail;
    }

    usbd_cdc_acm_comm_if_desc.bInterfaceNumber = id;
    usbd_cdc_acm_union_desc.bControlInterface = id;
    dev->interface_number = id; // record comunication interface number

    // Allocate instance-specific interface ID
    id = usb_interface_id(c, f); // this will return the allocated interface id
    if (id < 0) {
        status = id;
        goto fail;
    }

    usbd_cdc_acm_data_if_desc.bInterfaceNumber = id;
    usbd_cdc_acm_union_desc.bSubordinateInterface0 = id;
    usbd_cdc_acm_call_mgmt_desc.bDataInterface = id;
    
    // Search endpoint according to endpoint descriptor and modify endpoint address
    ep = usb_ep_autoconfig(c->cdev->gadget, &usbd_cdc_acm_data_bulk_in_ep_desc_FS);
    if (!ep) {
        goto fail;
    }

    ep->driver_data = dev;
    dev->bulk_in_ep = ep;
    
    // Search endpoint according to endpoint descriptor and modify endpoint address
    ep = usb_ep_autoconfig(c->cdev->gadget, &usbd_cdc_acm_data_bulk_out_ep_desc_FS);
    if (!ep) {
        goto fail;
    }

    ep->driver_data = dev;
    dev->bulk_out_ep = ep;
    
    usbd_cdc_acm_data_bulk_in_ep_desc_HS.bEndpointAddress = usbd_cdc_acm_data_bulk_in_ep_desc_FS.bEndpointAddress;
    usbd_cdc_acm_data_bulk_out_ep_desc_HS.bEndpointAddress = usbd_cdc_acm_data_bulk_out_ep_desc_FS.bEndpointAddress;
    
    status = usb_assign_descriptors(f, usbd_cdc_acm_descriptors_FS, usbd_cdc_acm_descriptors_HS, NULL);
    if (status != ESUCCESS) {
        goto fail;
    }
    
    USBD_CDC_EXIT;
    return ESUCCESS;
    
fail:
    usb_free_all_descriptors(f);
    USBD_CDC_EXIT_ERR;
    return status;
}

static void fun_unbind(struct usb_configuration *c, struct usb_function *f)
{
    UNUSED(c);
    USBD_CDC_ENTER;
    usb_free_all_descriptors(f);
    USBD_CDC_EXIT;
}

static void fun_suspend(struct usb_function *f)
{
    USBD_CDC_ENTER;
    UNUSED(f);
    USBD_CDC_EXIT;
}

static void fun_free(struct usb_function *f)
{
    USBD_CDC_ENTER;
    UNUSED(f);
    
    struct usb_cdc_dev_t *dev = &usb_cdc_dev;
    cdc_free_request(dev->bulk_in_ep, dev->bulk_in_req);
    cdc_free_request(dev->bulk_out_ep, dev->bulk_out_req);
    usb_free_all_descriptors(f);
    
    USBD_CDC_EXIT;
}

static int usbd_cdc_acm_receive_data(struct usb_cdc_dev_t *dev)
{
    struct usb_request  *req = NULL;
    int status = -EINVAL;

    USBD_CDC_ENTER;

    req = dev->bulk_out_req;
    if (req != NULL) {
        req->length = dev->bulk_out_buf_len;
        status = usb_ep_queue(dev->bulk_out_ep, req, 1);
        if (status != ESUCCESS) {
            USBD_CDC_WARN("Fail to equeue bulk out request");
        }
    }

    USBD_CDC_EXIT;

    return status;
}

int usbd_cdc_acm_transmit_data(void *buf, u16 length)
{
    struct usb_cdc_dev_t *dev = &usb_cdc_dev;
    struct usb_request  *req = NULL;
    u16 len = MIN(length, dev->bulk_in_buf_len);
    int status = -EINVAL;
    
    USBD_CDC_ENTER;
    
    req = dev->bulk_in_req;
    if (req != NULL) {
        memcpy(req->buf, buf, len);
        req->length = len;
        status = usb_ep_queue(dev->bulk_in_ep, req, 1);
        if (status != ESUCCESS) {
            USBD_CDC_WARN("Fail to equeue bulk in request");
        }
    }

    USBD_CDC_EXIT;
    return status;
}

static int usbd_cdc_acm_init(struct usb_configuration *c)
{
    int status = -EINVAL;
    struct usb_cdc_dev_t *dev = &usb_cdc_dev;
    
    USBD_CDC_ENTER;

    if ((dev->cb != NULL) && (dev->cb->init != NULL)) {
        status = dev->cb->init();
    }

    if (status != 0) {
        USBD_CDC_ERROR("user init fail\n");
        USBD_CDC_EXIT_ERR;
        return -EINVAL;
    }

    // If not assigned by user, use default buffer length
    if (dev->bulk_in_buf_len <= 0) {
        dev->bulk_in_buf_len = CDC_ACM_DEFAULT_BULK_IN_BUF_LEN;
    }
    if (dev->bulk_out_buf_len <= 0) {
        dev->bulk_out_buf_len = CDC_ACM_DEFAULT_BULK_OUT_BUF_LEN;
    }

    dev->funs.name = "cdc_function_sets";
    dev->funs.bind = fun_bind;
    dev->funs.unbind = fun_unbind;
    dev->funs.setup = fun_setup;
    dev->funs.set_alt = fun_set_alt;
    dev->funs.disable = fun_disable;
    dev->funs.suspend = fun_suspend;
    dev->funs.free_func = fun_free;
    dev->funs.bind = fun_bind;
    dev->gadget = c->cdev->gadget;
    
    c->cdev->gadget->device = dev;
    
    status = usb_add_function(c, &dev->funs);
    if (status != ESUCCESS) {
        usb_put_function(&dev->funs);
        USBD_CDC_EXIT_ERR;
        return status;
    }

    USBD_CDC_EXIT;
    return ESUCCESS;
}

static void usbd_cdc_acm_deinit(struct usb_configuration *c)
{
    struct usb_cdc_dev_t *dev = &usb_cdc_dev;
    
    USBD_CDC_ENTER;

    if ((dev->cb != NULL) && (dev->cb->deinit != NULL)) {
        dev->cb->deinit();
    }

    usb_remove_function(c, &dev->funs);
    usb_put_function(&dev->funs);
    
    USBD_CDC_EXIT;
}

usb_class_cb_t usbd_cdc_acm_cb = {
    .init = usbd_cdc_acm_init,
    .deinit = usbd_cdc_acm_deinit,
    .stop = NULL,
};

int usbd_cdc_acm_register(usbd_cdc_acm_usr_cb_t *cb)
{
    int status = -EINVAL;
    struct usb_cdc_dev_t *dev = &usb_cdc_dev;

    USBD_CDC_ENTER;

    if (cb != NULL) {
        dev->cb = cb;
        status = ESUCCESS;
    }

    usb_register_class(&usbd_cdc_acm_cfg, &usbd_cdc_acm_cb);

    USBD_CDC_EXIT;

    return status;
}

#endif // CONFIG_USBD_CDC_ACM

