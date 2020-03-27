#ifndef _USB_COMPOSITE_H_
#define _USB_COMPOSITE_H_

#include "usb_gadget.h"
#include "usb.h"

//#define USB_MALLOC_DESC_BUF

#define MAX_CONFIG_INTERFACES           16  /* arbitrary; max 255 */

// predefine structure
struct usb_composite_dev;
struct usb_composite_driver;

/**
 * struct usb_composite_driver - groups configurations into a gadget
 * @name: For diagnostics, identifies the driver.
 * @dev: Template descriptor for the device, including default device
 *  identifiers.
 * @strings: tables of strings, keyed by identifiers assigned during @bind
 *  and language IDs provided in control requests. Note: The first entries
 *  are predefined. The first entry that may be used is
 *  USB_GADGET_FIRST_AVAIL_IDX
 * @max_speed: Highest speed the driver supports.
 * @needs_serial: set to 1 if the gadget needs userspace to provide
 *  a serial number.  If one is not provided, warning will be printed.
 * @bind: (REQUIRED) Used to allocate resources that are shared across the
 *  whole device, such as string IDs, and add its configurations using
 *  @usb_add_config(). This may fail by returning a negative errno
 *  value; it should return zero on successful initialization.
 * @unbind: Reverses @bind; called as a side effect of unregistering
 *  this driver.
 * @disconnect: optional driver disconnect method
 * @suspend: Notifies when the host stops sending USB traffic,
 *  after function notifications
 * @resume: Notifies configuration when the host restarts USB traffic,
 *  before function notifications
 * @gadget_driver: Gadget driver controlling this driver
 *
 * Devices default to reporting self powered operation.  Devices which rely
 * on bus powered operation should report this in their @bind method.
 *
 * Before returning from @bind, various fields in the template descriptor
 * may be overridden.  These include the idVendor/idProduct/bcdDevice values
 * normally to bind the appropriate host side driver, and the three strings
 * (iManufacturer, iProduct, iSerialNumber) normally used to provide user
 * meaningful device identifiers.  (The strings will not be defined unless
 * they are defined in @dev and @strings.)  The correct ep0 maxpacket size
 * is also reported, as defined by the underlying controller driver.
 */

struct usb_composite_driver {
    const char                     *name;
    struct usb_device_descriptor   *dev_desc;
    struct usb_gadget_strings     **strings;
    enum usb_device_speed           max_speed;

    int (*bind)(struct usb_composite_dev *cdev);
    int (*unbind)(struct usb_composite_dev *);

    void (*disconnect)(struct usb_composite_dev *);

    /* global suspend hooks */
    void (*suspend)(struct usb_composite_dev *);
    void (*resume)(struct usb_composite_dev *);
    struct usb_gadget_driver        gadget_driver;
};
/**
 * struct usb_composite_device - represents one composite usb gadget
 * @gadget: read-only, abstracts the gadget's usb peripheral controller
 * @req: used for control responses; buffer is pre-allocated
 * @os_desc_req: used for OS descriptors responses; buffer is pre-allocated
 * @config: the currently active configuration
 * @qw_sign: qwSignature part of the OS string
 * @b_vendor_code: bMS_VendorCode part of the OS string
 * @use_os_string: false by default, interested gadgets set it
 * @os_desc_config: the configuration to be used with OS descriptors
 *
 * One of these devices is allocated and initialized before the
 * associated device driver's bind() is called.
 *
 * OPEN ISSUE:  it appears that some WUSB devices will need to be
 * built by combining a normal (wired) gadget with a wireless one.
 * This revision of the gadget framework should probably try to make
 * sure doing that won't hurt too much.
 *
 * One notion for how to handle Wireless USB devices involves:
 * (a) a second gadget here, discovery mechanism TBD, but likely
 *     needing separate "register/unregister WUSB gadget" calls;
 * (b) updates to usb_gadget to include flags "is it wireless",
 *     "is it wired", plus (presumably in a wrapper structure)
 *     bandgroup and PHY info;
 * (c) presumably a wireless_ep wrapping a usb_ep, and reporting
 *     wireless-specific parameters like maxburst and maxsequence;
 * (d) configurations that are specific to wireless links;
 * (e) function drivers that understand wireless configs and will
 *     support wireless for (additional) function instances;
 * (f) a function to support association setup (like CBAF), not
 *     necessarily requiring a wireless adapter;
 * (g) composite device setup that can create one or more wireless
 *     configs, including appropriate association setup support;
 * (h) more, TBD.
 */

typedef int (*user_control_callback)(struct usb_gadget *gadget, const struct usb_control_request *ctrl);

struct usb_composite_dev {
    struct usb_gadget           *gadget;
    struct usb_request          *req;
    struct usb_configuration    *config;
    struct usb_device_descriptor dev_desc;

    dwc_list_link_t              config_list; // by jimmy
    dwc_list_link_t              gstring_list;// by jimmy

    struct usb_composite_driver *driver;
    unsigned int                 suspended: 1;
};


/**
 * struct usb_configuration - represents one gadget configuration
 * @label: For diagnostics, describes the configuration.
 * @strings: Tables of strings, keyed by identifiers assigned during @bind()
 *  and by language IDs provided in control requests.
 * @descriptors: Table of descriptors preceding all function descriptors.
 *  Examples include OTG and vendor-specific descriptors.
 * @unbind: Reverses @bind; called as a side effect of unregistering the
 *  driver which added this configuration.
 * @setup: Used to delegate control requests that aren't handled by standard
 *  device infrastructure or directed at a specific interface.
 * @bConfigurationValue: Copied into configuration descriptor.
 * @iConfiguration: Copied into configuration descriptor.
 * @bmAttributes: Copied into configuration descriptor.
 * @MaxPower: Power consumtion in mA. Used to compute bMaxPower in the
 *  configuration descriptor after considering the bus speed.
 * @cdev: assigned by @usb_add_config() before calling @bind(); this is
 *  the device associated with this configuration.
 *
 * Configurations are building blocks for gadget drivers structured around
 * function drivers.  Simple USB gadgets require only one function and one
 * configuration, and handle dual-speed hardware by always providing the same
 * functionality.  Slightly more complex gadgets may have more than one
 * single-function configuration at a given speed; or have configurations
 * that only work at one speed.
 *
 * Composite devices are, by definition, ones with configurations which
 * include more than one function.
 *
 * The lifecycle of a usb_configuration includes allocation, initialization
 * of the fields described above, and calling @usb_add_config() to set up
 * internal data and bind it to a specific device.  The configuration's
 * @bind() method is then used to initialize all the functions and then
 * call @usb_add_function() for them.
 *
 * Those functions would normally be independent of each other, but that's
 * not mandatory.  CDC WMC devices are an example where functions often
 * depend on other functions, with some functions subsidiary to others.
 * Such interdependency may be managed in any way, so long as all of the
 * descriptors complete by the time the composite driver returns from
 * its bind() routine.
 */
struct usb_configuration {
    const char          *label;
    struct usb_gadget_strings   **strings;
    const struct usb_descriptor_header **descriptors;

    /* REVISIT:  bind() functions can be marked __init, which
     * makes trouble for section mismatch analysis.  See if
     * we can't restructure things to avoid mismatching...
     */

    /* configuration management: unbind/setup */
    void (*unbind)(struct usb_configuration *);
    int (*setup)(struct usb_configuration *, const struct usb_control_request *);

    /* fields in the config descriptor */
    uint8_t         bConfigurationValue;
    uint8_t         iConfiguration;
    uint8_t         bmAttributes;
    uint16_t        MaxPower;

    struct usb_composite_dev    *cdev;
    dwc_list_link_t list;
    dwc_list_link_t function_lists; // by jimmy

    uint8_t         next_interface_id;
    unsigned        superspeed: 1;
    unsigned        highspeed: 1;
    unsigned        fullspeed: 1;
    struct usb_function *interface[MAX_CONFIG_INTERFACES];
};

/**
* usb_interface_id() - allocate an unused interface ID
* @config: configuration associated with the interface
* @function: function handling the interface
* Context: single threaded during gadget setup
*
* usb_interface_id() is called from usb_function.bind() callbacks to
* allocate new interface IDs.  The function driver will then store that
* ID in interface, association, CDC union, and other descriptors.  It
* will also handle any control requests targeted at that interface,
* particularly changing its altsetting via set_alt().  There may
* also be class-specific or vendor-specific requests to handle.
*
* All interface identifier should be allocated using this routine, to
* ensure that for example different functions don't wrongly assign
* different meanings to the same identifier.  Note that since interface
* identifiers are configuration-specific, functions used in more than
* one configuration (or more than once in a given configuration) need
* multiple versions of the relevant descriptors.
*
* Returns the interface ID which was allocated; or -ENODEV if no
* more interface IDs can be allocated.
*/
int usb_interface_id(struct usb_configuration *config, struct usb_function *function);

int usb_add_config(struct usb_composite_dev *, struct usb_configuration *,
    int (*)(struct usb_configuration *));

void usb_remove_config(struct usb_composite_dev *, struct usb_configuration *);

int usb_add_function(struct usb_configuration *config, struct usb_function *function);

void usb_put_function(struct usb_function *function);

void usb_remove_function(struct usb_configuration *config, struct usb_function *function);

/**
 * struct usb_function - describes one function of a configuration
 * @name: For diagnostics, identifies the function.
 * @strings: tables of strings, keyed by identifiers assigned during bind()
 *  and by language IDs provided in control requests
 * @fs_descriptors: Table of full (or low) speed descriptors, using interface and
 *  string identifiers assigned during @bind().  If this pointer is null,
 *  the function will not be available at full speed (or at low speed).
 * @hs_descriptors: Table of high speed descriptors, using interface and
 *  string identifiers assigned during @bind().  If this pointer is null,
 *  the function will not be available at high speed.
 * @ss_descriptors: Table of super speed descriptors, using interface and
 *  string identifiers assigned during @bind(). If this
 *  pointer is null after initiation, the function will not
 *  be available at super speed.
 * @config: assigned when @usb_add_function() is called; this is the
 *  configuration with which this function is associated.
 * @os_desc_table: Table of (interface id, os descriptors) pairs. The function
 *  can expose more than one interface. If an interface is a member of
 *  an IAD, only the first interface of IAD has its entry in the table.
 * @os_desc_n: Number of entries in os_desc_table
 * @bind: Before the gadget can register, all of its functions bind() to the
 *  available resources including string and interface identifiers used
 *  in interface or class descriptors; endpoints; I/O buffers; and so on.
 * @unbind: Reverses @bind; called as a side effect of unregistering the
 *  driver which added this function.
 * @free_func: free the struct usb_function.
 * @mod: (internal) points to the module that created this structure.
 * @set_alt: (REQUIRED) Reconfigures altsettings; function drivers may
 *  initialize usb_ep.driver data at this time (when it is used).
 *  Note that setting an interface to its current altsetting resets
 *  interface state, and that all interfaces have a disabled state.
 * @get_alt: Returns the active altsetting.  If this is not provided,
 *  then only altsetting zero is supported.
 * @disable: (REQUIRED) Indicates the function should be disabled.  Reasons
 *  include host resetting or reconfiguring the gadget, and disconnection.
 * @setup: Used for interface-specific control requests.
 * @suspend: Notifies functions when the host stops sending USB traffic.
 * @resume: Notifies functions when the host restarts USB traffic.
 * @get_status: Returns function status as a reply to
 *  GetStatus() request when the recipient is Interface.
 * @func_suspend: callback to be called when
 *  SetFeature(FUNCTION_SUSPEND) is reseived
 *
 * A single USB function uses one or more interfaces, and should in most
 * cases support operation at both full and high speeds.  Each function is
 * associated by @usb_add_function() with a one configuration; that function
 * causes @bind() to be called so resources can be allocated as part of
 * setting up a gadget driver.  Those resources include endpoints, which
 * should be allocated using @usb_ep_autoconfig().
 *
 * To support dual speed operation, a function driver provides descriptors
 * for both high and full speed operation.  Except in rare cases that don't
 * involve bulk endpoints, each speed needs different endpoint descriptors.
 *
 * Function drivers choose their own strategies for managing instance data.
 * The simplest strategy just declares it "static', which means the function
 * can only be activated once.  If the function needs to be exposed in more
 * than one configuration at a given speed, it needs to support multiple
 * usb_function structures (one for each configuration).
 *
 * A more complex strategy might encapsulate a @usb_function structure inside
 * a driver-specific instance structure to allows multiple activations.  An
 * example of multiple activations might be a CDC ACM function that supports
 * two or more distinct instances within the same configuration, providing
 * several independent logical data links to a USB host.
 */

struct usb_function {
    const char                    *name;
    struct usb_gadget_strings    **strings;
    struct usb_descriptor_header **fs_descriptors;
    struct usb_descriptor_header **hs_descriptors;
    struct usb_configuration      *config;

    /* REVISIT:  bind() functions can be marked __init, which
     * makes trouble for section mismatch analysis.  See if
     * we can't restructure things to avoid mismatching.
     * Related:  unbind() may kfree() but bind() won't...
     */

    /* configuration management:  bind/unbind */
    int (*bind)(struct usb_configuration *, struct usb_function *);
    void (*unbind)(struct usb_configuration *, struct usb_function *);
    void (*free_func)(struct usb_function *f);

    /* runtime state management */
    int (*set_alt)(struct usb_function *, unsigned interface, unsigned alt);
    int (*get_alt)(struct usb_function *, unsigned interface);
    void (*disable)(struct usb_function *);
    int (*setup)(struct usb_function *, const struct usb_control_request *);
    void (*suspend)(struct usb_function *);
    void (*resume)(struct usb_function *);

    /* USB 3.0 additions */
    int (*get_status)(struct usb_function *);
    dwc_list_link_t list; // by jimmy
    uint32_t function_endp_bitmap;//bitmap// Rom2 add: store which endpoint belongs to this function
};

typedef struct {
    int (*init)(struct usb_configuration *c);
    int (*stop)(struct usb_configuration *c);
    void (*deinit)(struct usb_configuration *c);
} usb_class_cb_t;

_LONG_CALL_ int usb_assign_descriptors(struct usb_function *f,
    struct usb_descriptor_header **fs,
    struct usb_descriptor_header **hs,
    struct usb_descriptor_header **ss);

_LONG_CALL_ void usb_free_all_descriptors(struct usb_function *f);

int usb_composite_probe(struct usb_composite_driver *driver);

void usb_register_class(usb_cfg_t *cfg, usb_class_cb_t *ops);

int usb_composite_init(void);

void usb_composite_deinit(void);

void usb_composite_stop(void);

#endif

