#
# Copyright (c) 2015-2020, ARM Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

ifneq (${MBEDTLS_COMMON_MK},1)
MBEDTLS_COMMON_MK	:=	1

# MBEDTLS_DIR must be set to the mbed TLS main directory (it must contain
# the 'include' and 'library' subdirectories).
ifeq (${MBEDTLS_DIR},)
  $(error Error: MBEDTLS_DIR not set)
endif

MBEDTLS_INC		=	-I${MBEDTLS_DIR}/include

# Specify mbed TLS configuration file
MBEDTLS_CONFIG_FILE	:=	"<drivers/auth/mbedtls/mbedtls_config.h>"
$(eval $(call add_define,MBEDTLS_CONFIG_FILE))

MBEDTLS_SOURCES	+=		drivers/auth/mbedtls/mbedtls_common.c


LIBMBEDTLS_SRCS		:= $(addprefix ${MBEDTLS_DIR}/library/,	\
					aes.c 					\
					asn1parse.c 				\
					asn1write.c 				\
					cipher.c 				\
					cipher_wrap.c 				\
					memory_buffer_alloc.c			\
					oid.c 					\
					platform.c 				\
					platform_util.c				\
					bignum.c				\
					gcm.c 					\
					md.c					\
					md_wrap.c				\
					pk.c 					\
					pk_wrap.c 				\
					pkparse.c 				\
					pkwrite.c 				\
					sha256.c            			\
					sha512.c            			\
					ecdsa.c					\
					ecp_curves.c				\
					ecp.c					\
					rsa.c					\
					rsa_internal.c				\
					x509.c 					\
					x509_crt.c 				\
					)

# The platform may define the variable 'TF_MBEDTLS_KEY_ALG' to select the key
# algorithm to use. If the variable is not defined, select it based on
# algorithm used for key generation `KEY_ALG`. If `KEY_ALG` is not defined,
# then it is set to `rsa`.
ifeq (${TF_MBEDTLS_KEY_ALG},)
    ifeq (${KEY_ALG}, ecdsa)
        TF_MBEDTLS_KEY_ALG		:=	ecdsa
    else
        TF_MBEDTLS_KEY_ALG		:=	rsa
    endif
endif

ifeq (${TF_MBEDTLS_KEY_SIZE},)
    ifneq ($(findstring rsa,${TF_MBEDTLS_KEY_ALG}),)
	ifeq (${KEY_SIZE},)
            TF_MBEDTLS_KEY_SIZE		:=	2048
	else
            TF_MBEDTLS_KEY_SIZE		:=	${KEY_SIZE}
	endif
    endif
endif

ifeq (${HASH_ALG}, sha384)
    TF_MBEDTLS_HASH_ALG_ID	:=	TF_MBEDTLS_SHA384
    MBEDTLS_MD_ID		:=	MBEDTLS_MD_SHA384
    TPM_ALG_ID			:=	TPM_ALG_SHA384
    TCG_DIGEST_SIZE		:=	48
else ifeq (${HASH_ALG}, sha512)
    TF_MBEDTLS_HASH_ALG_ID	:=	TF_MBEDTLS_SHA512
    MBEDTLS_MD_ID		:=	MBEDTLS_MD_SHA512
    TPM_ALG_ID			:=	TPM_ALG_SHA512
    TCG_DIGEST_SIZE		:=	64
else
    TF_MBEDTLS_HASH_ALG_ID	:=	TF_MBEDTLS_SHA256
    MBEDTLS_MD_ID		:=	MBEDTLS_MD_SHA256
    TPM_ALG_ID			:=	TPM_ALG_SHA256
    TCG_DIGEST_SIZE		:=	32
endif

ifeq (${TF_MBEDTLS_KEY_ALG},ecdsa)
    TF_MBEDTLS_KEY_ALG_ID	:=	TF_MBEDTLS_ECDSA
else ifeq (${TF_MBEDTLS_KEY_ALG},rsa)
    TF_MBEDTLS_KEY_ALG_ID	:=	TF_MBEDTLS_RSA
else ifeq (${TF_MBEDTLS_KEY_ALG},rsa+ecdsa)
    TF_MBEDTLS_KEY_ALG_ID	:=	TF_MBEDTLS_RSA_AND_ECDSA
else
    $(error "TF_MBEDTLS_KEY_ALG=${TF_MBEDTLS_KEY_ALG} not supported on mbed TLS")
endif

ifeq (${DECRYPTION_SUPPORT}, aes_gcm)
    TF_MBEDTLS_USE_AES_GCM	:=	1
else
    TF_MBEDTLS_USE_AES_GCM	:=	0
endif

# Needs to be set to drive mbed TLS configuration correctly
$(eval $(call add_define,TF_MBEDTLS_KEY_ALG_ID))
$(eval $(call add_define,TF_MBEDTLS_KEY_SIZE))
$(eval $(call add_define,TF_MBEDTLS_HASH_ALG_ID))
$(eval $(call add_define,TF_MBEDTLS_USE_AES_GCM))

# Set definitions for measured boot driver
$(eval $(call add_define,MBEDTLS_MD_ID))
$(eval $(call add_define,TPM_ALG_ID))
$(eval $(call add_define,TCG_DIGEST_SIZE))

$(eval $(call MAKE_LIB,mbedtls))

endif
