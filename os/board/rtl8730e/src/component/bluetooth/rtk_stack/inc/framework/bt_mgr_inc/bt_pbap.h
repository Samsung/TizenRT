/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BT_PBAP_H_
#define _BT_PBAP_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * \defgroup    BT_PBAP BT PBAP Profile
 *
 * \brief   Provide BT PBAP profile interfaces.
 */

/**
 * bt_pbap.h
 *
 * \defgroup BT_PBAP_PROPERTY_MASK BT PBAP Property Mask
 *
 * \brief  Define BT PBAP Property Mask.
 *
 * \ingroup BT_PBAP
 */
#define BT_PBAP_PROPERTY_VERSION     (1 << 0)    /**< vCard Version */
#define BT_PBAP_PROPERTY_FN          (1 << 1)    /**< Formatted Name */
#define BT_PBAP_PROPERTY_N           (1 << 2)    /**< Structured Presentation of Name */
#define BT_PBAP_PROPERTY_PHOTO       (1 << 3)    /**< Associated Image or Photo */
#define BT_PBAP_PROPERTY_BDAY        (1 << 4)    /**< Birthday */
#define BT_PBAP_PROPERTY_ADR         (1 << 5)    /**< Delivery Address */
#define BT_PBAP_PROPERTY_LABEL       (1 << 6)    /**< Delivery */
#define BT_PBAP_PROPERTY_TEL         (1 << 7)    /**< Telephone Number */
#define BT_PBAP_PROPERTY_EMAIL       (1 << 8)    /**< Electronic Mail Address */
#define BT_PBAP_PROPERTY_MAILER      (1 << 9)    /**< Electronic Mail */
#define BT_PBAP_PROPERTY_TZ          (1 << 10)   /**< Time Zone */
#define BT_PBAP_PROPERTY_GEO         (1 << 11)   /**< Geographic Position */
#define BT_PBAP_PROPERTY_TITLE       (1 << 12)   /**< Job */
#define BT_PBAP_PROPERTY_ROLE        (1 << 13)   /**< Role within the Organization */
#define BT_PBAP_PROPERTY_LOGO        (1 << 14)   /**< Organization Logo */
#define BT_PBAP_PROPERTY_AGENT       (1 << 15)   /**< vCard of Person Representing */
#define BT_PBAP_PROPERTY_ORG         (1 << 16)   /**< Name of Organization */
#define BT_PBAP_PROPERTY_NOTE        (1 << 17)   /**< Comments */
#define BT_PBAP_PROPERTY_REV         (1 << 18)   /**< Revision */
#define BT_PBAP_PROPERTY_SOUND       (1 << 19)   /**< Pronunciation of Name */
#define BT_PBAP_PROPERTY_URL         (1 << 20)   /**< Uniform Resource Locator */
#define BT_PBAP_PROPERTY_UID         (1 << 21)   /**< Unique ID */
#define BT_PBAP_PROPERTY_KEY         (1 << 22)   /**< Public Encryption Key */
#define BT_PBAP_PROPERTY_NICKNAME    (1 << 23)   /**< Nickname */
#define BT_PBAP_PROPERTY_CATEGORIES  (1 << 24)   /**< Categories */
#define BT_PBAP_PROPERTY_PROID       (1 << 25)   /**< Product ID */
#define BT_PBAP_PROPERTY_CLASS       (1 << 26)   /**< Class information */
#define BT_PBAP_PROPERTY_SORTSTRING  (1 << 27)   /**< String used for sorting operations */
#define BT_PBAP_PROPERTY_DATETIME    (1 << 28)   /**< Time stamp */

/**
 * bt_pbap.h
 *
 * \brief  Define BT PBAP repository.
 *
 * \ingroup BT_PBAP
 */
typedef enum t_bt_pbap_repository
{
    BT_PBAP_REPOSITORY_LOCAL   = (1 << 0),   /**< Phone's local memory */
    BT_PBAP_REPOSITORY_SIM1    = (1 << 1),   /**< Phone's SIM card */
} T_BT_PBAP_REPOSITORY;

/**
 * bt_pbap.h
 *
 * \brief  Define BT PBAP path.
 *
 * \ingroup BT_PBAP
 */
typedef enum t_bt_pbap_path
{
    BT_PBAP_PATH_ROOT           = 0x00,
    BT_PBAP_PATH_TELECOM        = 0x01,
    BT_PBAP_PATH_PB             = 0x02,
    BT_PBAP_PATH_ICH            = 0x03,
    BT_PBAP_PATH_OCH            = 0x04,
    BT_PBAP_PATH_MCH            = 0x05,
    BT_PBAP_PATH_CCH            = 0x06,
} T_BT_PBAP_PATH;

/**
 * bt_pbap.h
 *
 * \brief  Define BT PBAP phonebook.
 *
 * \ingroup BT_PBAP
 */
typedef enum t_bt_pbap_phone_book
{
    BT_PBAP_PHONE_BOOK_PB        = 0x00,    /**< Main phone book object */
    BT_PBAP_PHONE_BOOK_ICH       = 0x01,    /**< Incoming calls history object */
    BT_PBAP_PHONE_BOOK_OCH       = 0x02,    /**< Outgoing calls history object */
    BT_PBAP_PHONE_BOOK_MCH       = 0x03,    /**< Missed calls history object */
    BT_PBAP_PHONE_BOOK_CCH       = 0x04,    /**< Combined (incoming, outgoing, and missed) calls history object */
    BT_PBAP_PHONE_BOOK_SPD       = 0x05,    /**< Speed-Dial object */
    BT_PBAP_PHONE_BOOK_FAV       = 0x06,    /**< Favorite contacts object */
} T_BT_PBAP_PHONE_BOOK;

/**
 * bt_pbap.h
 *
 * \brief   Initialize PBAP profile.
 *
 * \param[in]  link_num      PBAP maximum connected link number.
 *
 * \return          The status of initializing PBAP profile.
 * \retval true     PBAP profile was initialized successfully.
 * \retval false    PBAP profile was failed to initialize.
 *
 * \ingroup BT_PBAP
 */
bool bt_pbap_init(uint8_t link_num);

/**
 * bt_pbap.h
 *
 * \brief  Send a PBAP connection request.
 *
 * \param[in]  bd_addr         Remote BT address.
 * \param[in]  server_chann    The remote server_channel which can be found from the sdp info.
 * \param[in]  feat_flag       The PSE PBAPSupportedFeatures attribute present flag.
 * \arg    true    The PSE PBAPSupportedFeatures attribute not present in sdp info.
 * \arg    false   The PSE PBAPSupportedFeatures attribute is present in sdp info.
 *
 * \return         The status of sending the PBAP connection request.
 * \retval true    PBAP connection request was sent successfully.
 * \retval false   PBAP connection request was failed to send.
 *
 * \ingroup BT_PBAP
 */
bool bt_pbap_connect_req(uint8_t *bd_addr, uint8_t server_chann, bool feat_flag);

/**
 * bt_pbap.h
 *
 * \brief  Send a PBAP disconnection request.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the PBAP disconnection request.
 * \retval true    PBAP disconnection request was sent successfully.
 * \retval false   PBAP disconnection request was failed to send.
 *
 * \ingroup BT_PBAP
 */
bool bt_pbap_disconnect_req(uint8_t *bd_addr);

/**
 * bt_pbap.h
 *
 * \brief  Send a request to pull the entire phone book.
 *
 * \param[in]  bd_addr         Remote BT address.
 * \param[in]  repos           Repository containing the phone book \ref T_BT_PBAP_REPOSITORY.
 * \param[in]  phone_book      The phone book object to be retrieved \ref T_BT_PBAP_PHONE_BOOK.
 * \param[in]  start_offset    The offset of the first entry to be retrieved compared to
 *                             the first entry of the Phonebook object.
 * \param[in]  max_list_count  The maximum number of entries to be retrieved.
 * \param[in]  filter          The filter attribute mask of 64 bits, and 0 for disabling filter \ref BT_PBAP_PROPERTY_MASK.
 *
 * \return         The status of sending pull phone book message.
 * \retval true    Request was sent successfully.
 * \retval false   Request was failed to send.
 *
 * \ingroup BT_PBAP
 */
bool bt_pbap_phone_book_pull(uint8_t *bd_addr, T_BT_PBAP_REPOSITORY repos,
                             T_BT_PBAP_PHONE_BOOK phone_book, uint16_t start_offset,
                             uint16_t max_list_count, uint64_t filter);

/**
 * bt_pbap.h
 *
 * \brief  Send a request to get the number of indexes in the phone book of interest.
 *
 * \param[in]  bd_addr     Remote BT address.
 * \param[in]  repos       Repository containing the phone book \ref T_BT_PBAP_REPOSITORY.
 * \param[in]  phone_book  The phone book object to be retrieved \ref T_BT_PBAP_PHONE_BOOK.
 *
 * \return         The status of sending get phone book size message.
 * \retval true    Request was sent successfully.
 * \retval false   Request was failed to send.
 *
 * \ingroup BT_PBAP
 */
bool bt_pbap_phone_book_size_get(uint8_t *bd_addr, T_BT_PBAP_REPOSITORY repos,
                                 T_BT_PBAP_PHONE_BOOK phone_book);

/**
 * bt_pbap.h
 *
 * \brief  Send a request to pull more data from the remote server.
 *
 * \param[in]  bd_addr     Remote BT address.
 *
 * \return         The status of sending pull continue message.
 * \retval true    Request was sent successfully.
 * \retval false   Request was failed to send.
 *
 * \ingroup BT_PBAP
 */
bool bt_pbap_pull_continue(uint8_t *bd_addr);

/**
 * bt_pbap.h
 *
 * \brief  Send a request to terminate the ongoing pull operation.
 *
 * \param[in]  bd_addr     Remote BT address.
 *
 * \return         The status of sending pull abort message.
 * \retval true    Request was sent successfully.
 * \retval false   Request was failed to send.
 *
 * \ingroup BT_PBAP
 */
bool bt_pbap_pull_abort(uint8_t *bd_addr);

/**
 * bt_pbap.h
 *
 * \brief  Send a request to set active phone book.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  repos      Repository containing the phone book \ref T_BT_PBAP_REPOSITORY.
 * \param[in]  path       The phone book to be set \ref T_BT_PBAP_PATH.
 *
 * \return         The status of sending set phone book message.
 * \retval true    Request was sent successfully.
 * \retval false   Request was failed to send.
 *
 * \ingroup BT_PBAP
 */
bool bt_pbap_phone_book_set(uint8_t *bd_addr, T_BT_PBAP_REPOSITORY repos, T_BT_PBAP_PATH path);

/**
 * bt_pbap.h
 *
 * \brief  Send a request to pull the vCard listing in current phonebook by phone number.
 *
 * \param[in]  bd_addr        Remote BT address.
 * \param[in]  phone_number   The start address of string buffer that stores phone number.
 *
 * \return         The status of sending pull_vCard_listing message.
 * \retval true    Request was sent successfully.
 * \retval false   Request was failed to send.
 *
 * \ingroup BT_PBAP
 */
bool bt_pbap_vcard_listing_by_number_pull(uint8_t *bd_addr, char *phone_number);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BT_PBAP_H_ */
