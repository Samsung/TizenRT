#include <platform_opts.h>

#ifdef CONFIG_USBD_MSC

#include "msc/inc/usbd_msc.h"
#include "msc/inc/usbd_scsi.h"
#include "usb_composite.h"
#include "usb_errno.h"
#include "osdep_service.h"
#include "sd.h"

/* MSC thread priority*/
#define USBD_IRQ_THREAD_PRIORITY             (tskIDLE_PRIORITY + 3)
#define USBD_MSC_BULK_CMD_THREAD_PRIORITY    (tskIDLE_PRIORITY + 1) // Should be lower than USBD_IRQ_THREAD_PRIORITY

/* MSC device buffer configuration */
#define USBD_MSC_NBR_BUFHD                   2       /* Number of buffer header for bulk in/out data */
#define USBD_MSC_BUFLEN                      (32*512) /* Default size of buffer length */

/* MSC tasks stack size */
#define USBD_MSC_BULK_CMD_TASK_STACK_SIZE    512
#define USBD_MSC_BULK_DATA_TASK_STACK_SIZE   512

#define USBD_MSC_HALT_ERROR_TOLERANCE        20

// define global variables
struct msc_dev gMSC_DEVICE;
struct msc_common gMSC_COMMON;

// physical disk access methods
struct msc_opts disk_operation;

/* MSC Interface, Alternate Setting 0*/
struct usb_interface_descriptor usbd_msc_intf_desc = {
    .bLength            = USB_DT_INTERFACE_SIZE,
    .bDescriptorType    = USB_DT_INTERFACE,
    .bInterfaceNumber   = 0, // this will be assign automatically
    .bAlternateSetting  = 0,
    .bNumEndpoints      = 2,
    .bInterfaceClass    = USB_CLASS_MASS_STORAGE,
    .bInterfaceSubClass = USBD_MSC_SC_SCSI,
    .bInterfaceProtocol = USBD_MSC_PR_BULK,
    .iInterface         = 0,
};

/* MSC Endpoints for Low-speed/Full-speed */
/* Endpoint, EP Bulk IN */
struct usb_endpoint_descriptor usbd_msc_source_desc_FS = {
    .bLength            = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType    = USB_DT_ENDPOINT,
    .bEndpointAddress   = USBD_MSC_BULK_IN_EP_ADDRESS,
    .bmAttributes       = USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize     = 64,
    .bInterval          = 0,

};
/* Endpoint, EP Bulk OUT */
struct usb_endpoint_descriptor usbd_msc_sink_desc_FS = {
    .bLength            = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType    = USB_DT_ENDPOINT,
    .bEndpointAddress   = USBD_MSC_BULK_OUT_EP_ADDRESS,
    .bmAttributes       = USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize     = 64,
    .bInterval          = 0,
};

/* MSC Endpoints for High-speed */
/* Endpoint, EP Bulk IN */
struct usb_endpoint_descriptor usbd_msc_source_desc_HS = {
    .bLength            = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType    = USB_DT_ENDPOINT,
    .bEndpointAddress   = USBD_MSC_BULK_IN_EP_ADDRESS,
    .bmAttributes       = USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize     = 512,
    .bInterval          = 0,
};

/* Endpoint, EP Bulk OUT */
struct usb_endpoint_descriptor usbd_msc_sink_desc_HS = {
    .bLength            = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType    = USB_DT_ENDPOINT,
    .bEndpointAddress   = USBD_MSC_BULK_OUT_EP_ADDRESS,
    .bmAttributes       = USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize     = 512,
    .bInterval          = 0,
};

struct usb_descriptor_header *usbd_msc_descriptors_FS [] = {
    /* data interface has no altsetting */
    (struct usb_descriptor_header *) &usbd_msc_intf_desc,
    (struct usb_descriptor_header *) &usbd_msc_source_desc_FS,
    (struct usb_descriptor_header *) &usbd_msc_sink_desc_FS,
    NULL,
};

struct usb_descriptor_header *usbd_msc_descriptors_HS [] = {
    /* data interface has no altsetting */
    (struct usb_descriptor_header *) &usbd_msc_intf_desc,
    (struct usb_descriptor_header *) &usbd_msc_source_desc_HS,
    (struct usb_descriptor_header *) &usbd_msc_sink_desc_HS,
    NULL,
};

usb_cfg_t usbd_msc_cfg = {
    .bSpeed           = USB_SPEED_HIGH,
        
    .bDeviceClass     = USB_CLASS_PER_INTERFACE,
    .bDeviceSubClass  = 0,
    .bDeviceProtocol  = 0,
    .idVendor         = REALTEK_USB_VID,
    .idProduct        = REALTEK_USB_PID,
    .bcdDevice        = 0x0100,
    
    .bmAttributes     = USB_CONFIG_ATT_ONE,
    .bMaxPower        = 50,

    .bIrqTaskPriority = USBD_IRQ_THREAD_PRIORITY,
    
    .sManufacturer    = "Realtek",
    .sProduct         = "USB MSC Device",
    .sSerialNumber    = "0123456789AB",
};

#if USBD_MSC_RAM_DISK
#define USBD_MSC_RAM_DISK_SIZE    (USBD_MSC_BUFLEN * 6)
#define USBD_MSC_RAM_DISK_SECTORS (USBD_MSC_RAM_DISK_SIZE >> USBD_MSC_BLK_BITS)

static u8 *usbd_msc_ram_disk_buf;
#endif

/* maxpacket and other transfer characteristics vary by speed. */
#define ep_desc(g,hs,fs) (((g)->speed == USB_SPEED_HIGH)?(hs):(fs))

int usbd_msc_halt_bulk_in_endpoint(struct msc_dev *mscdev)
{
    int rc;
    int error_cnt = 0;
    
    rc = usb_ep_set_halt(mscdev->in_ep);
    if (rc == -EAGAIN) {
        USBD_MSC_ERROR("delayed bulk-in endpoint halt");
    }

    while (rc != 0) {
        if (rc != -EAGAIN) {
            USBD_MSC_ERROR("usb_ep_set_halt -> %d", rc);
            rc = 0;
            break;
        }

        ++error_cnt;
        if (error_cnt > USBD_MSC_HALT_ERROR_TOLERANCE)
        {
            USBD_MSC_ERROR("usb_ep_set_halt fail");
            rc = 0;
            break;
        }

        /* Wait for a short time and then try again */
        rtw_mdelay_os(10);
        rc = usb_ep_set_halt(mscdev->in_ep);;
    }

    return rc;
}

int usbd_msc_bulk_in_transfer(struct msc_dev *mscdev, struct usb_request *req)
{
    req->dma = (dma_addr_t)req->buf;
    return usb_ep_queue(mscdev->in_ep, req, 1);
}

int usbd_msc_bulk_out_transfer(struct msc_dev *mscdev, struct usb_request *req)
{
    req->dma = (dma_addr_t)req->buf;
    return usb_ep_queue(mscdev->out_ep, req, 1);
}

static void usbd_msc_bulk_in_complete(struct usb_ep *ep, struct usb_request *req)
{
    struct msc_bufhd *bufhd = (struct msc_bufhd *)req->context;
    struct msc_dev *mscdev = (struct msc_dev *)ep->driver_data;
    struct msc_common *common = mscdev->common;
    int status = req->status;
    
    /* check request status */
    switch (status) {
        case ESUCCESS:          /* tx seccussfuly*/
            break;

        case -ECONNRESET:       /* unlink */
            USBD_MSC_ERROR("ECONNRESET");
            break;

        case -ESHUTDOWN:        /* disconnect etc */
            USBD_MSC_ERROR("ESHUTDOWN");
            break;

        default:
            USBD_MSC_ERROR("ERROR(%d)", status);
            break;
    }

    if (bufhd->type == BUFHD_DATA) {
        usbd_msc_put_bufhd(common, bufhd);
    }
}

static thread_return usbd_msc_bulk_cmd_handler(thread_context context)
{
    struct msc_dev *mscdev = (struct msc_dev *)context;
    struct msc_common *common = mscdev->common;
    struct msc_bufhd *bufhd = NULL;
    struct usb_request *req = NULL;
    int result;

    while (1) {
        result = rtw_down_sema(&(mscdev->msc_outCmdTask.wakeup_sema));
        if (FAIL == result) {
            USBD_MSC_ERROR("rtw down sema fail");
            break;
        }

        if (mscdev->msc_outCmdTask.blocked) {
            break;
        }

        /* stop the current task */
        if (mscdev->common->state != MSC_STATE_RUNNING) {
            continue;
        }

        // handle bulk out request
        rtw_mutex_get(&common->boc_mutex);
        result = rtw_is_list_empty(&common->boc_list);
        rtw_mutex_put(&common->boc_mutex);
        if (result == TRUE) {
            continue;
        }

next:
        if (mscdev->common->state != MSC_STATE_RUNNING) {
            continue;
        }

        rtw_mutex_get(&common->boc_mutex);
        bufhd = list_first_entry(&common->boc_list, struct msc_bufhd, list);
        rtw_mutex_put(&common->boc_mutex);
        if (bufhd == NULL)
        {
            continue;
        }
        
        req = bufhd->reqout;
        
#if defined(CONFIG_PLATFORM_8721D)
        DCache_Invalidate(((u32)(req->buf) & CACHE_LINE_ADDR_MSK), (req->actual + CACHE_LINE_SIZE));
#endif

        // receive CBW
        if (bufhd->type == BUFHD_CBW) {
            result = usbd_msc_receive_cbw(mscdev, req);
        }

        if (result == 0) { //response successfully
            rtw_list_delete(&bufhd->list);
        }

        rtw_mutex_get(&common->boc_mutex);
        result = rtw_is_list_empty(&common->boc_list);
        rtw_mutex_put(&common->boc_mutex);

        if (result == FALSE) {
            goto next;
        }
        /* submit bulk out request agains*/
        else {
            if ((mscdev->common->state == MSC_STATE_RUNNING) && (bufhd->type == BUFHD_CBW)) {
                req->length = US_BULK_CB_WRAP_LEN;
                result = usb_ep_queue(mscdev->out_ep, req, 1);
                if (result) {
                    USBD_MSC_ERROR("%s queue req --> %d", mscdev->out_ep->name, result);
                }
            }
        }
    }

    rtw_up_sema(&(mscdev->msc_outCmdTask.terminate_sema));
    USBD_MSC_ERROR("TASK DELETE");
    rtw_thread_exit();
}

static void usbd_msc_bulk_out_complete(struct usb_ep *ep, struct usb_request *req)
{
    struct msc_dev *mscdev = ep->driver_data;
    struct msc_common *common = mscdev->common;
    struct msc_bufhd *bufhd = (struct msc_bufhd *)req->context;
    int status = req->status;

    /*check request status*/
    switch (status) {
        case ESUCCESS:  /* normal complete */
            if (req->actual > 0) {
                if (bufhd->type == BUFHD_CBW && req->actual == US_BULK_CB_WRAP_LEN) {
                    rtw_mutex_get(&common->boc_mutex);
                    rtw_list_insert_tail(&bufhd->list, &common->boc_list);
                    rtw_mutex_put(&common->boc_mutex);
                    /*release sema to wake up bulk out handler*/
                    rtw_up_sema(&(mscdev->msc_outCmdTask.wakeup_sema));
                } else if (bufhd->type == BUFHD_DATA && req->actual >= common->curlun->blksize) {
                    rtw_mutex_get(&common->bod_mutex);
                    rtw_list_insert_tail(&bufhd->list, &common->bod_list);
                    rtw_mutex_put(&common->bod_mutex);
                } else {
                    USBD_MSC_ERROR("invalid package received (%d)", req->actual);

                    if (req->actual == US_BULK_CB_WRAP_LEN) {
                        struct msc_bufhd *bhd = common->cbw_bh;
                        memcpy(bhd->reqout->buf, req->buf, req->actual);
                        bhd->reqout->actual = req->actual;
                        rtw_mutex_get(&common->boc_mutex);
                        rtw_list_insert_tail(&bhd->list, &common->boc_list);
                        rtw_mutex_put(&common->boc_mutex);
                        /*release sema to wake up bulk out handler*/
                        rtw_up_sema(&(mscdev->msc_outCmdTask.wakeup_sema));

                        if (bufhd->type == BUFHD_DATA) {
                            usbd_msc_put_bufhd(common, bufhd);
                        }
                    }
                }
            }

            break;

        case -ECONNRESET:       /* unlink, software-driven interface shutdown */
            USBD_MSC_ERROR("ECONNRESET");
            break;

        case -ESHUTDOWN:        /* disconnect etc */
            USBD_MSC_ERROR("ESHUTDOWN");
            break;

        case -ECONNABORTED:     /* endpoint reset */
            USBD_MSC_ERROR("ECONNABORTED");
            break;

        case -EOVERFLOW:
            USBD_MSC_ERROR("EOVERFLOW");
            break;

        default:
            USBD_MSC_ERROR("tx status %d", status);
            break;
    }
}

int usbd_msc_lun_write(struct msc_lun *curlun, u32 sector, const u8 *buffer, u32 count)
{
    int ret = 0;

    if (curlun == NULL) {
        return -1;
    }

    rtw_mutex_get(&curlun->lun_mutex);

    if (curlun->lun_opts->disk_write) {
        ret = curlun->lun_opts->disk_write(sector, buffer, count);
    }

    rtw_mutex_put(&curlun->lun_mutex);
    return ret;
}

int usbd_msc_lun_read(struct msc_lun *curlun, u32 sector, u8 *buffer, u32 count)
{
    int ret = 0;

    if (curlun == NULL) {
        return -1;
    }

    rtw_mutex_get(&curlun->lun_mutex);

    if (curlun->lun_opts->disk_read) {
        ret = curlun->lun_opts->disk_read(sector, buffer, count);
    }

    rtw_mutex_put(&curlun->lun_mutex);
    return ret;
}


static void usbd_msc_lun_close(struct msc_lun *curlun)
{
    if (curlun->is_open) {
        if (curlun->lun_opts->disk_deinit) {
            curlun->lun_opts->disk_deinit();
        }

        curlun->is_open = 0;
    }
}

static int usbd_msc_lun_open(struct msc_lun *curlun)
{
    int     ro = 0;
    u32     num_sectors = 0;
    u32     blkbits;
    u32     blksize;

    /* init hard disk */
    if (curlun->lun_opts->disk_init) {
        if (curlun->lun_opts->disk_init()) {
            return -ENOMEDIUM;
        }

        curlun->is_open = 1;
    } else {
        return -ENOMEDIUM;
    }

    /* get disk capacity */
    if (curlun->lun_opts->disk_getcapacity) {
        curlun->lun_opts->disk_getcapacity(&num_sectors);
    }

    blksize = USBD_MSC_BLK_SIZE;
    blkbits = USBD_MSC_BLK_BITS;
    curlun->blksize = blksize;
    curlun->blkbits = blkbits;
    curlun->ro = ro;
    curlun->num_sectors = num_sectors;
    return 0;
}

static int usbd_msc_create_lun(struct msc_common *common, unsigned int id)
{
    struct msc_lun *lun;
    int status;
    
    lun = (struct msc_lun *)rtw_zmalloc(sizeof(struct msc_lun));
    if (!lun) {
        USBD_MSC_ERROR("fail to malloc lun");
        return -ENOMEM;
    }

    lun->cdrom = 0;
    lun->ro = 0;
    lun->initially_ro = lun->ro;
    lun->removable = 1;
    common->luns[id] = lun;
    lun->lun_opts = &disk_operation;
    rtw_mutex_init(&lun->lun_mutex);
    status = usbd_msc_lun_open(lun);
    return status;
}

static void usbd_msc_remove_luns(struct msc_common *common, int n)
{
    int i;

    if (common->luns) {
        for (i = 0; i < n; ++i) {
            if (common->luns[i]) {
                usbd_msc_lun_close(common->luns[i]);
                rtw_mfree((u8 *)common->luns[i], sizeof(struct msc_lun));
                common->luns[i] = NULL;
            }
        }
        rtw_mfree((u8 *)common->luns, common->nluns * sizeof(struct msc_lun *));
    }
}

static int usbd_msc_create_luns(struct msc_common *common)
{
    struct msc_lun **luns;
    int i;
    int status;
    
    luns = (struct msc_lun **)rtw_zmalloc(common->nluns * sizeof(struct msc_lun *));
    if (!luns) {
        USBD_MSC_ERROR("fail to malloc luns");
        return -ENOMEM;
    }

    common->luns = luns;

    for (i = 0; i < common->nluns; ++i) {
        status = usbd_msc_create_lun(common, i);

        if (status) {
            goto err;
        }
    }

    return 0;
err:
    usbd_msc_remove_luns(common, i);
    return status;
}

void usbd_msc_put_bufhd(struct msc_common *common, struct msc_bufhd *bufhd)
{
    rtw_mutex_get(&common->bufhd_mutex);
    if (common->nbufhd_a < common->nbufhd)
    {
        rtw_list_insert_tail(&bufhd->list, &common->bufhd_pool);
        common->nbufhd_a++;
    }
    rtw_mutex_put(&common->bufhd_mutex);
}

struct msc_bufhd *usbd_msc_get_bufhd(struct msc_common *common)
{
    struct msc_bufhd *bufhd = NULL;
    rtw_mutex_get(&common->bufhd_mutex);

    while (rtw_is_list_empty(&common->bufhd_pool) == TRUE) {
        rtw_mutex_put(&common->bufhd_mutex);
        return NULL;
    }

    bufhd = list_first_entry(&common->bufhd_pool, struct msc_bufhd, list);
    rtw_list_delete(&bufhd->list);
    common->nbufhd_a--;
    rtw_mutex_put(&common->bufhd_mutex);
    
    return bufhd;
}

static struct msc_bufhd *usbd_msc_alloc_bufhd(struct msc_common *common, int bufsize, bufhd_type type)
{
    struct msc_bufhd *bufhd = NULL;
    UNUSED(common);
    bufhd = (struct msc_bufhd *)rtw_zmalloc(sizeof(struct msc_bufhd));

    if (bufhd) {
        if (type != BUFHD_DATA) {
            bufhd->buf = rtw_zmalloc(bufsize);
            if (bufhd->buf == NULL) {
                rtw_mfree((void *)bufhd, sizeof(struct msc_bufhd));
                USBD_MSC_ERROR("fail to malloc bufhd->buf, size %d", bufsize);
                goto fail;
            }
        } else {
            // for disk_write/read, buf addr should be 32byte align to increase write/read speed
            bufhd->prebuf = rtw_zmalloc(bufsize + 0x1F);
            if (bufhd->prebuf == NULL) {
                rtw_mfree((void *)bufhd, sizeof(struct msc_bufhd));
                USBD_MSC_ERROR("fail to malloc bufhd->prebuf, size %d", bufsize);
                goto fail;
            }

            bufhd->buf = (u8 *)_RND((u32)bufhd->prebuf, 32); //buf 32 byte align
        }

        bufhd->buf_size = bufsize;
    }

    bufhd->type = type;
    return bufhd;
fail:
    return NULL;
}

static void usbd_msc_free_bufhds(struct msc_common *common)
{
    struct msc_bufhd *bufhd = NULL;

    /* free bufhd for cwb*/
    if (common->cbw_bh) {
        bufhd = common->cbw_bh;

        if (bufhd->reqout) {
            usb_ep_free_request(common->mscdev->out_ep, bufhd->reqout);
            bufhd->reqout = NULL;
        }

        if (bufhd->buf) {
            rtw_mfree(bufhd->buf, bufhd->buf_size);
            bufhd->buf = NULL;
        }
        rtw_mfree((void *)bufhd, sizeof(struct msc_bufhd));
        common->cbw_bh = NULL;
    }

    /* free bufhd for csb*/
    if (common->csw_bh) {
        bufhd = common->csw_bh;
        
        if (bufhd->reqin) {
            usb_ep_free_request(common->mscdev->in_ep, bufhd->reqin);
            bufhd->reqin = NULL;
        }
        
        if (bufhd->buf) {
            rtw_mfree(bufhd->buf, bufhd->buf_size);
            bufhd->buf = NULL;
        }
        
        rtw_mfree((void *)bufhd, sizeof(struct msc_bufhd));
        common->csw_bh = NULL;
    }

    /* free bufhd for data*/
    while (!rtw_is_list_empty(&common->bufhd_pool)) {
        rtw_mutex_get(&common->bufhd_mutex);
        bufhd = list_first_entry(&common->bufhd_pool, struct msc_bufhd, list);
        rtw_list_delete(&bufhd->list);
        rtw_mutex_put(&common->bufhd_mutex);

        if (bufhd->reqin) {
            usb_ep_free_request(common->mscdev->in_ep, bufhd->reqin);
            bufhd->reqin = NULL;
        }

        if (bufhd->reqout) {
            usb_ep_free_request(common->mscdev->out_ep, bufhd->reqout);
            bufhd->reqout = NULL;
        }

        if (bufhd->prebuf) {
            rtw_mfree(bufhd->prebuf, bufhd->buf_size + 0x1F);
            bufhd->prebuf = NULL;
        }
        rtw_mfree((void *)bufhd, sizeof(struct msc_bufhd));
        if (common->nbufhd_a > 0) {
            common->nbufhd_a--;
        }
    }
}

static int usbd_msc_alloc_bufhds(struct msc_common *common)
{
    struct msc_bufhd *bufhd = NULL;
    int i, rc;
    /* malloc bufhd for cwb*/
    bufhd = usbd_msc_alloc_bufhd(common, US_BULK_CB_WRAP_LEN, BUFHD_CBW);

    if (bufhd) {
        common->cbw_bh = bufhd;
    } else {
        USBD_MSC_ERROR("fail to malloc bufhd cbw");
        rc = -ENOMEM;
        goto fail;
    }

    /* malloc bufhd for csb*/
    bufhd = usbd_msc_alloc_bufhd(common, US_BULK_CS_WRAP_LEN, BUFHD_CSW);
    if (bufhd) {
        common->csw_bh = bufhd;
    } else {
        USBD_MSC_ERROR("fail to malloc bufhd csw");
        rc = -ENOMEM;
        goto fail;
    }

    /* malloc bufhd for bulk data stage*/
    for (i = 0; i < common->nbufhd; i++) {
        bufhd = usbd_msc_alloc_bufhd(common, USBD_MSC_BUFLEN, BUFHD_DATA);

        if (bufhd) {
            /* add bufhd to list*/
            rtw_init_listhead(&bufhd->list);
            rtw_list_insert_tail(&bufhd->list, &common->bufhd_pool);
        } else {
            USBD_MSC_ERROR("fail to malloc bufhd(No.%d)", i + 1);
            rc = -ENOMEM;
            goto fail;
        }
    }

    common->nbufhd_a = common->nbufhd;
    return 0;
fail:
    usbd_msc_free_bufhds(common);
    return rc;
}

static int fun_set_alt(struct usb_function *f, unsigned interface, unsigned alt)
{
    struct msc_dev  *mscdev = CONTAINER_OF(f, struct msc_dev, func);
    struct msc_common *common = mscdev->common;
    struct msc_bufhd *bufhd;
    struct usb_request  *req;
    int status = 0;
    
    UNUSED(interface);
    UNUSED(alt);

    USBD_MSC_ENTER;

    if (common->state != MSC_STATE_INIT) {
        /* deallocate the request */
        list_for_each_entry(bufhd, &common->bufhd_pool, list, struct msc_bufhd) {
            if (bufhd->reqin) {
                usb_ep_free_request(mscdev->in_ep, bufhd->reqin);
                bufhd->reqin = NULL;
            }

            if (bufhd->reqout) {
                usb_ep_free_request(mscdev->out_ep, bufhd->reqout);
                bufhd->reqout = NULL;
            }
        }

        /* Disable the endpoints */
        if (mscdev->bulk_in_enabled) {
            usb_ep_disable(mscdev->in_ep);
            mscdev->in_ep->driver_data = NULL;
            mscdev->bulk_in_enabled = 0;
        }

        if (mscdev->bulk_out_enabled) {
            usb_ep_disable(mscdev->out_ep);
            mscdev->out_ep->driver_data = NULL;
            mscdev->bulk_out_enabled = 0;
        }

        common->mscdev = NULL;
        common->state = MSC_STATE_INIT;
    }

    common->mscdev = mscdev;

    // enable bulk in endpoint
    if (mscdev->in_ep != NULL) {
        /* restart endpoint */
        if (mscdev->in_ep->driver_data != NULL) {
            usb_ep_disable(mscdev->in_ep);
        }

        /*
         * assign an corresponding endpoint descriptor to an endpoint
         * according to the gadget speed
         */
        mscdev->in_ep->desc = ep_desc(common->gadget, &usbd_msc_source_desc_HS, &usbd_msc_source_desc_FS);
        status = usb_ep_enable(mscdev->in_ep, mscdev->in_ep->desc);
        if (status < 0) {
            USBD_MSC_ERROR("fail to enable IN endpoint");
            goto fail;
        }

        mscdev->in_ep->driver_data = mscdev;
        mscdev->bulk_in_enabled = 1;
        /*malloc request for CSW*/
        req = usb_ep_alloc_request(mscdev->in_ep, 1);
        if (!req) {
            USBD_MSC_ERROR("fail to malloc bufhd->reqin");
            goto fail;
        }

        bufhd = common->csw_bh;
        bufhd->reqin = req;
        bufhd->type = BUFHD_CSW;
        req->buf = bufhd->buf;
        req->dma = (dma_addr_t)bufhd->buf;
        req->context = bufhd;
        req->complete = (usb_req_complete_t)usbd_msc_bulk_in_complete;
    }

    // enable bulk out endpoint and alloc requset to recieve data from host
    if (mscdev->out_ep != NULL) {
        /* restart endpoint */
        if (mscdev->out_ep->driver_data != NULL) {
            usb_ep_disable(mscdev->out_ep);
        }

        /*
        * assign an corresponding endpoint descriptor to an endpoint
        * according to the gadget speed
        */
        mscdev->out_ep->desc = ep_desc(common->gadget, &usbd_msc_sink_desc_HS, &usbd_msc_sink_desc_FS);
        status = usb_ep_enable(mscdev->out_ep, mscdev->out_ep->desc);
        if (status < 0) {
            USBD_MSC_ERROR("fail to enable out endpoint");
            goto fail;
        }

        mscdev->out_ep->driver_data = mscdev;
        mscdev->bulk_out_enabled = 1;
        /*malloc request for CBW*/
        req = usb_ep_alloc_request(mscdev->out_ep, 1);
        if (!req) {
            USBD_MSC_ERROR("fail to malloc bufhd->reqin");
            goto fail;
        }

        bufhd = common->cbw_bh;
        bufhd->reqout = req;
        bufhd->type = BUFHD_CBW;
        req->buf = bufhd->buf;
        req->dma = (dma_addr_t)bufhd->buf;
        req->context = bufhd;
        req->complete = (usb_req_complete_t)usbd_msc_bulk_out_complete;
    }

    /* malloc request for data*/
    list_for_each_entry(bufhd, &common->bufhd_pool, list, struct msc_bufhd) {
        bufhd->reqin = usb_ep_alloc_request(mscdev->in_ep, 1);
        if (!bufhd->reqin) {
            USBD_MSC_ERROR("fail to malloc bufhd->reqin");
            goto fail;
        }

        bufhd->reqout = usb_ep_alloc_request(mscdev->out_ep, 1);
        if (!bufhd->reqout) {
            USBD_MSC_ERROR("fail to malloc bufhd->reqout");
            goto fail;
        }

        bufhd->reqin->buf = bufhd->reqout->buf = bufhd->buf;
        bufhd->reqin->dma = bufhd->reqout->dma = (dma_addr_t)bufhd->buf;
        bufhd->reqin->context = bufhd->reqout->context = bufhd;
        bufhd->reqin->complete = (usb_req_complete_t)usbd_msc_bulk_in_complete;
        bufhd->reqout->complete = (usb_req_complete_t)usbd_msc_bulk_out_complete;
        bufhd->type = BUFHD_DATA;
    }
    /* enqueue CBW request to receive SCSI command from host*/
    req = common->cbw_bh->reqout;
    req->length = US_BULK_CB_WRAP_LEN;
    status = usb_ep_queue(mscdev->out_ep, req, 1);

    if (status) {
        USBD_MSC_ERROR("fail to enqueue out endpoint");
        goto fail;
    }

    common->state = MSC_STATE_RUNNING;
    
    USBD_MSC_EXIT;
    
    return 0;
    
fail:
    USBD_MSC_EXIT_ERR;
    return status;
}

static void fun_disable(struct usb_function *f)
{
    struct msc_dev  *mscdev = CONTAINER_OF(f, struct msc_dev, func);
    
    USBD_MSC_ENTER;
    
    if (mscdev->bulk_in_enabled) {
        usb_ep_disable(mscdev->in_ep);
        mscdev->in_ep->driver_data = NULL;
        mscdev->bulk_in_enabled = 0;
    }

    if (mscdev->bulk_out_enabled) {
        usb_ep_disable(mscdev->out_ep);
        mscdev->out_ep->driver_data = NULL;
        mscdev->bulk_out_enabled = 0;
    }
        
    USBD_MSC_EXIT;
}

static int fun_bind(struct usb_configuration *c, struct usb_function *f)
{
    struct msc_dev  *mscdev = CONTAINER_OF(f, struct msc_dev, func);
    struct usb_ep *ep;
    int status = -ENODEV;
    int id;

    USBD_MSC_ENTER;

    if (rtw_create_task(&mscdev->msc_outCmdTask, (const char *)"MSC_BULK_CMD",
            USBD_MSC_BULK_CMD_TASK_STACK_SIZE, USBD_MSC_BULK_CMD_THREAD_PRIORITY, usbd_msc_bulk_cmd_handler, (void *)mscdev) != 1) {
        USBD_MSC_ERROR("fail to create thread MSC_BULK_CMD");
        goto fail;
    }

    /* allocate instance-specific interface IDs, and patch descriptors */
    id = usb_interface_id(c, f); // this will return the allocated interface id

    if (id < 0) {
        status = id;
        goto fail;
    }

    usbd_msc_intf_desc.bInterfaceNumber = id;
    mscdev->interface_number = id; // record interface number
    /* search endpoint according to endpoint descriptor and modify endpoint address*/
    ep = usb_ep_autoconfig(c->cdev->gadget, &usbd_msc_source_desc_FS);

    if (!ep) {
        goto fail;
    }

    ep->driver_data = mscdev;/* claim */
    mscdev->in_ep = ep;
    ep = usb_ep_autoconfig(c->cdev->gadget, &usbd_msc_sink_desc_FS);

    if (!ep) {
        goto fail;
    }

    ep->driver_data = mscdev;/* claim */
    mscdev->out_ep = ep;
    usbd_msc_source_desc_HS.bEndpointAddress = usbd_msc_source_desc_FS.bEndpointAddress;
    usbd_msc_sink_desc_HS.bEndpointAddress = usbd_msc_sink_desc_FS.bEndpointAddress;
    status = usb_assign_descriptors(f, usbd_msc_descriptors_FS, usbd_msc_descriptors_HS, NULL);

    if (status) {
        goto fail;
    }

    USBD_MSC_EXIT;

    return 0;
fail:
    usb_free_all_descriptors(f);

    if (mscdev->msc_outCmdTask.task) {
        rtw_delete_task(&mscdev->msc_outCmdTask);
    }

    USBD_MSC_EXIT_ERR;

    return status;
}

static void fun_unbind_msc(struct usb_configuration *c, struct usb_function *f)
{
    struct msc_dev *mscdev = CONTAINER_OF(f, struct msc_dev, func);
    
    UNUSED(c);
    
    USBD_MSC_ENTER;

    if (mscdev->msc_outCmdTask.task) {
        rtw_delete_task(&mscdev->msc_outCmdTask);
    }

    usb_free_all_descriptors(f);

    USBD_MSC_EXIT;
}

static int fun_setup_msc(struct usb_function *fun, const struct usb_control_request *ctrl)
{
    struct msc_dev *mscdev = CONTAINER_OF(fun, struct msc_dev, func);
    struct usb_composite_dev *cdev = fun->config->cdev;
    struct usb_request *req0 = cdev->req;
    struct usb_ep *ep0 = mscdev->common->gadget->ep0;
    u16 wLength = ctrl->wLength;
    u16 wIndex = ctrl->wIndex;
    u16 wValue = ctrl->wValue;
    int value = -EOPNOTSUPP;
    req0->context = NULL;
    req0->length = 0;

    USBD_MSC_ENTER;

    if (mscdev->common->state == MSC_STATE_RUNNING) {
        switch (ctrl->bRequest) {
            case USBD_MSC_REQUEST_RESET:
                USBD_MSC_INFO("USBD_MSC_REQUEST_RESET\n");
                if ((ctrl->bmRequestType == USB_REQ_TYPE_WRITE_CLASS_INTERFACE) &&
                    (wIndex == mscdev->interface_number) &&
                    (wValue == 0) &&
                    (wLength == 0)) {
                    value = USB_GADGET_DELAYED_STATUS;
                }
                break;
            case USBD_MSC_REQUEST_GET_MAX_LUN:
                USBD_MSC_INFO("USBD_MSC_REQUEST_GET_MAX_LUN\n");
                if ((ctrl->bmRequestType == USB_REQ_TYPE_READ_CLASS_INTERFACE) &&
                    (wIndex == mscdev->interface_number) &&
                    (wValue == 0) &&
                    (wLength == 1)) {
                    *(u8 *)req0->buf = mscdev->common->nluns - 1;
                    req0->length = 1;
                    value = usb_ep_queue(ep0, req0, 1);
                    if (value != 0) {
                        /* We can't do much more than wait for a reset */
                        USBD_MSC_WARN("EP0 request queue fail: %d", value);
                    }
                }
                break;
            default:
                break;
        }
    }

    USBD_MSC_EXIT;

    return value;
}

static void fun_free(struct usb_function *f)
{
    struct msc_dev *mscdev = CONTAINER_OF(f, struct msc_dev, func);
    struct msc_common *common = mscdev->common;
    
    USBD_MSC_ENTER;
    
    if (mscdev->msc_outCmdTask.task) {
        rtw_delete_task(&mscdev->msc_outCmdTask);
    }
    
#if defined (CONFIG_PLATFORM_8721D)
    rtw_free_sema(&mscdev->msc_outCmdTask.wakeup_sema);
    rtw_free_sema(&mscdev->msc_outCmdTask.terminate_sema);
#endif

    usb_free_all_descriptors(f);

    usbd_msc_remove_luns(common, common->nluns);
    usbd_msc_free_bufhds(common);
    rtw_mutex_free(&common->bod_mutex);
    rtw_mutex_free(&common->boc_mutex);
    rtw_mutex_free(&common->bufhd_mutex);

    USBD_MSC_EXIT;
}

static void fun_suspend(struct usb_function *f)
{
    USBD_MSC_ENTER;
    UNUSED(f);
    USBD_MSC_EXIT;
}

#if USBD_MSC_RAM_DISK

static SD_RESULT RAM_init(void)
{
    SD_RESULT result = SD_OK;
    usbd_msc_ram_disk_buf = (u8 *)rtw_zmalloc(USBD_MSC_RAM_DISK_SIZE);
    if (usbd_msc_ram_disk_buf == NULL) {
        USBD_MSC_ERROR("fail to allocate RAM disk buffer");
        result = SD_NODISK;
    }
    return result;
}

static SD_RESULT RAM_deinit(void)
{
    if (usbd_msc_ram_disk_buf != NULL) {
        rtw_mfree(usbd_msc_ram_disk_buf, USBD_MSC_RAM_DISK_SIZE);
    }
    return SD_OK;
}

static SD_RESULT RAM_GetCapacity(u32* sector_count)
{
    *sector_count = USBD_MSC_RAM_DISK_SECTORS;
    return SD_OK;
}

static SD_RESULT RAM_ReadBlocks(u32 sector,u8 *data,u32 count)
{
    SD_RESULT result = SD_ERROR;
    if (sector + count <= USBD_MSC_RAM_DISK_SECTORS) {
        memcpy(data, usbd_msc_ram_disk_buf + sector * USBD_MSC_BLK_SIZE, count * USBD_MSC_BLK_SIZE);
        result = SD_OK;
    }
    return result;
}

static SD_RESULT RAM_WriteBlocks(u32 sector,const u8 *data,u32 count)
{
    SD_RESULT result = SD_ERROR;
    if (sector + count <= USBD_MSC_RAM_DISK_SECTORS) {
        memcpy(usbd_msc_ram_disk_buf + sector * USBD_MSC_BLK_SIZE, data, count * USBD_MSC_BLK_SIZE);
        result = SD_OK;
    }
    return result;
}

#endif // USBD_MSC_RAM_DISK

static int usbd_msc_init(struct usb_configuration *c)
{
    struct msc_dev *mscdev;
    struct msc_common *common;
    int status = 0;
    
    USBD_MSC_ENTER;

#if USBD_MSC_RAM_DISK
    disk_operation.disk_init = RAM_init;
    disk_operation.disk_deinit = RAM_deinit;
    disk_operation.disk_getcapacity = RAM_GetCapacity;
    disk_operation.disk_read = RAM_ReadBlocks;
    disk_operation.disk_write = RAM_WriteBlocks;
#else
    disk_operation.disk_init = SD_Init;
    disk_operation.disk_deinit = SD_DeInit;
    disk_operation.disk_getcapacity = SD_GetCapacity;
    disk_operation.disk_read = SD_ReadBlocks;
    disk_operation.disk_write = SD_WriteBlocks;
#endif
    
    common = &gMSC_COMMON;
    common->gadget = c->cdev->gadget;
    common->nluns = USBD_MSC_MAX_LUN;
    common->nbufhd = USBD_MSC_NBR_BUFHD;
    common->can_stall = 0;
    common->state = MSC_STATE_INIT;
    
    mscdev = &gMSC_DEVICE;
    mscdev->func.name = "msc_function_sets";
    mscdev->func.bind = fun_bind;
    mscdev->func.unbind = fun_unbind_msc;
    mscdev->func.setup = fun_setup_msc;
    mscdev->func.set_alt = fun_set_alt;
    mscdev->func.disable = fun_disable;
    mscdev->func.suspend = fun_suspend;
    mscdev->func.free_func = fun_free;
    mscdev->common = common;
    
#if defined (CONFIG_PLATFORM_8721D)
    // rtw_create_task in Ameba1 has semaphore and initial inside,
    // but AmebaPro(AmebaD) didn't have it, so we create semaphore by ourselves
    rtw_init_sema(&mscdev->msc_outCmdTask.wakeup_sema, 0);
    rtw_init_sema(&mscdev->msc_outCmdTask.terminate_sema, 0);
#endif    

    c->cdev->gadget->device = mscdev;
    
    rtw_init_listhead(&common->bufhd_pool);
    rtw_mutex_init(&common->bufhd_mutex);
    rtw_init_listhead(&common->boc_list);
    rtw_mutex_init(&common->boc_mutex);
    rtw_init_listhead(&common->bod_list);
    rtw_mutex_init(&common->bod_mutex);
    
    /* alloc buffer header */
    status = usbd_msc_alloc_bufhds(common);
    if (status) {
        USBD_MSC_ERROR("fail to malloc buffer header");
        goto fail_alloc_buf;
    }

    /* create logic unit */
    status = usbd_msc_create_luns(common);
    if (status < 0) {
        goto fail_alloc_buf;
    }

    status = usb_add_function(c, &mscdev->func);
    if (status) {
        usb_put_function(&mscdev->func);
        goto fail_set_luns;
    }

    USBD_MSC_EXIT;

    return 0;
fail_set_luns:
    usbd_msc_remove_luns(common, common->nluns);
fail_alloc_buf:
    usbd_msc_free_bufhds(mscdev->common);
    rtw_mutex_free(&common->bod_mutex);
    rtw_mutex_free(&common->boc_mutex);
    rtw_mutex_free(&common->bufhd_mutex);

    USBD_MSC_EXIT_ERR;
    
    return status;
}

static void usbd_msc_deinit(struct usb_configuration *c)
{
    struct msc_dev *mscdev = &gMSC_DEVICE;

    USBD_MSC_ENTER;

    usb_remove_function(c, &mscdev->func);
    usb_put_function(&mscdev->func);
        
    USBD_MSC_EXIT;
}

static int usbd_msc_stop(struct usb_configuration *c)
{
    struct msc_dev *mscdev = &gMSC_DEVICE;
    UNUSED(c);

    USBD_MSC_ENTER;

    if (mscdev->msc_outCmdTask.task) {
        rtw_delete_task(&mscdev->msc_outCmdTask);
    }
        
    USBD_MSC_EXIT;

    return ESUCCESS;
}

usb_class_cb_t usbd_msc_cb = {
    .init = usbd_msc_init,
    .deinit = usbd_msc_deinit,
    .stop = usbd_msc_stop,
};

int usbd_msc_register(void)
{
    usb_register_class(&usbd_msc_cfg, &usbd_msc_cb);
    return ESUCCESS;
}

#endif // CONFIG_USBD_MSC
