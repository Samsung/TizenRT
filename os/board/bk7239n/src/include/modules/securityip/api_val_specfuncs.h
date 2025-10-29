/**
 * @file api_val_specfuncs.h
 *
 * @brief VAL API - Special Functions related services
 */

/*****************************************************************************
* Copyright (c) 2017-2018 INSIDE Secure B.V. All Rights Reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef INCLUDE_GUARD_API_VAL_SPECFUNCS_H
#define INCLUDE_GUARD_API_VAL_SPECFUNCS_H

/*-----------------------------------------------------------------------------
 * This module uses (requires) the following interface(s):
 */
#include "api_val_asset.h"          // ValAssetId_t, val_AssetFree


/*-----------------------------------------------------------------------------
 * val_SFMilenageSqnAdminCreate
 */
/**
 * This function allocates and initializes (creates) a SQNms Administration
 * Asset that must be used for the val_SFMilenageAutnVerificationSqn function
 * to enable sequence number checking according 3GPP TS 33.102.
 *
 * @param [in] StaticAssetNumber
 *     The asset number to search for in the (static) Asset Store that refers
 *     to Milenage private data that contains the 128-bit subscriber key (K)
 *     and the 128-bit value derived from OP and K (OPc).
 *     Note that the supported values for StaticAssetNumber are defined by the
 *     device manufacturer and vendor and can differ from product to product.
 *
 * @param [in] fAMF_SBtest
 *     When set, the AMF (Authenication Management Field) Separation Bit test
 *     is enable when performing the sequence number check.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId for SQNms Administration
 *     will be written in case of successful creation. The AssetId is needed
 *     for the AUTN Verification operation when sequence number checking is
 *     required.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SFMilenageSqnAdminCreate(
        const ValAssetNumber_t StaticAssetNumber,
        const bool fAMF_SBtest,
        ValAssetId_t * const AssetId_p);


/*-----------------------------------------------------------------------------
 * val_SFMilenageSqnAdminReset
 */
/**
 * This function resets the SQNms Administration to its initial state when it
 * was created.
 *
 * @param [in] AssetId
 *     Reference to the SQNms Administration Asset.
 *     This reference is provided by val_SFMilenageSqnAdminCreate().
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SFMilenageSqnAdminReset(
        const ValAssetId_t AssetId);


/*-----------------------------------------------------------------------------
 * val_SFMilenageSqnAdminExport
 */
/**
 * This function exports the SQNms Administration Asset as a data blob that
 * can be load via val_AssetLoadImport() to restore the the SQNms Administration
 * after a hibernation or a device power cycle.
 * Note that val_AssetAlloc() must be used to create a SQNms Administration
 * Asset (Size: 200 Bytes; Policy: Private Data) before val_AssetLoadImport()
 * can be used.
 *
 * @param [in] AssetId
 *     Reference to the SQNms Administration Asset.
 *     This reference is provided by val_SFMilenageSqnAdminCreate().
 *
 * @param [in] KekAssetId
 *     Reference to the KEK Asset to be used when generating the data blob.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the data
 *     blob authentication. The exact same Associated Data must be provided
 *     when data blob is imported
 *
 * @param [out] DataBlob_p
 *     Pointer to the data blob buffer.
 *
 * @param [in,out] SizeDataBlob_p
 *     Size of the buffer intended for the data blob.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SFMilenageSqnAdminExport(
        const ValAssetId_t AssetId,
        const ValAssetId_t KekAssetId,
        ValOctetsIn_t * const  AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValOctetsOut_t * const DataBlob_p,
        ValSize_t * const SizeDataBlob_p);


/*-----------------------------------------------------------------------------
 * val_SFMilenageSqnAdminRelease
 */
/**
 * This function releases (frees) the SQNms Administration asset.
 *
 * @param [in] AssetId
 *     Reference to the SQNms Administration Asset.
 *     This reference is provided by val_SFMilenageSqnAdminCreate().
 *
 * @return One of the ValStatus_t values.
 */
#define val_SFMilenageSqnAdminRelease(AssetId) val_AssetFree(AssetId)


/*-----------------------------------------------------------------------------
 * val_SFMilenageAutnVerification
 */
/**
 * This function verifies the provided AUTN and returns RES, CK, IK, SQN and
 * AMF on success. In case of a MAC failure 'Verify Error' is returned and RES,
 * CK, IK, SQN and AMF are invalid.
 *
 * @param [in] StaticAssetNumber
 *     The asset number to search for in the (static) Asset Store that refers
 *     to Milenage private data that contains the 128-bit subscriber key (K)
 *     and the 128-bit value derived from OP and K (OPc).
 *     Note that the supported values for StaticAssetNumber are defined by the
 *     device manufacturer and vendor and can differ from product to product.
 *
 * @param [in] RAND_p
 *     Pointer to the 128-bit (16 Bytes) RAND.
 *
 * @param [in] AUTN_p
 *     Pointer to the 128-bit (16 Bytes) AUTN.
 *
 * @param [out] RES_p
 *     Pointer to the 64-bit (8 Bytes) buffer in which the RES must be written.
 *
 * @param [out] CK_p
 *     Pointer to the 128-bit (16 Bytes) buffer in which the CK must be written.
 *
 * @param [out] IK_p
 *     Pointer to the 128-bit (16 Bytes) buffer in which the IK must be written.
 *
 * @param [out] SQN_p
 *     Pointer to the 48-bit (6 Bytes) buffer in which the SQN must be written.
 *
 * @param [out] AMF_p
 *     Pointer to the 16-bit (2 Bytes) buffer in which the AMF must be written.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SFMilenageAutnVerification(
        const ValAssetNumber_t StaticAssetNumber,
        ValOctetsIn_t * const RAND_p,
        ValOctetsIn_t * const AUTN_p,
        ValOctetsOut_t * const RES_p,
        ValOctetsOut_t * const CK_p,
        ValOctetsOut_t * const IK_p,
        ValOctetsOut_t * const SQN_p,
        ValOctetsOut_t * const AMF_p);


/*-----------------------------------------------------------------------------
 * val_SFMilenageAutnVerificationSqn
 */
/**
 * This function verifies the provided AUTN and returns RES, CK, IK on success.
 * The function returns 'Verify Error' in case of:
 * - MAC failure
 *   -> EMM Cause 20
 * - Synch failure
 *   -> EMM Cause 21 and an AUTS is returned based on SQNms and AMF(= {0,0})
 * - AMF Separation Bit failure - Non-EPS authentication unacceptable
 *   -> EMM Cause 26
 *
 * @param [in] AssetId
 *     Reference to the SQNms Administration Asset.
 *     This reference is provided by val_SFMilenageSqnAdminCreate().
 *
 * @param [in] RAND_p
 *     Pointer to the 128-bit (16 Bytes) RAND.
 *
 * @param [in] AUTN_p
 *     Pointer to the 128-bit (16 Bytes) AUTN.
 *
 * @param [out] EMMCause_p
 *     Pointer to a location where the EMM Cause must be written in case a
 *     'Verify Error' occurs.
 *
 * @param [out] RES_p
 *     Pointer to the 64-bit (8 Bytes) buffer in which the RES must be written.
 *
 * @param [out] CK_p
 *     Pointer to the 128-bit (16 Bytes) buffer in which the CK must be written.
 *
 * @param [out] IK_p
 *     Pointer to the 128-bit (16 Bytes) buffer in which the IK must be written.
 *
 * @param [out] AUTS_p
 *     Pointer to the 96-bit (12 Bytes) buffer in which the AUTS must be written
 *     in case of Sync failure (EMM Cause 21).
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SFMilenageAutnVerificationSqn(
        const ValAssetId_t AssetId,
        ValOctetsIn_t * const RAND_p,
        ValOctetsIn_t * const AUTN_p,
        uint32_t * const EMMCause_p,
        ValOctetsOut_t * const RES_p,
        ValOctetsOut_t * const CK_p,
        ValOctetsOut_t * const IK_p,
        ValOctetsOut_t * const AUTS_p);


/*-----------------------------------------------------------------------------
 * val_SFMilenageAutsGeneration
 */
/**
 * This function generates an AUTS based on the provided SQNms and AMF.
 *
 * @param [in] StaticAssetNumber
 *     The asset number to search for in the (static) Asset Store that refers
 *     to Milenage private data that contains the 128-bit subscriber key (K)
 *     and the 128-bit value derived from OP and K (OPc).
 *     Note that the supported values for StaticAssetNumber are defined by the
 *     device manufacturer and vendor and can differ from product to product.
 *
 * @param [in] RAND_p
 *     Pointer to the 128-bit (16 Bytes) RAND.
 *
 * @param [in] SQNms_p
 *     Pointer to the 48-bit (6 Bytes) buffer in which the SQNms must be written.
 *
 * @param [in] AMF_p
 *     Pointer to the 16-bit (2 Bytes) buffer in which the AMF must be written.
 *
 * @param [out] AUTS_p
 *     Pointer to the 96-bit (12 Bytes) buffer in which the AUTS must be written.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SFMilenageAutsGeneration(
        const ValAssetNumber_t StaticAssetNumber,
        ValOctetsIn_t * const RAND_p,
        ValOctetsIn_t * const SQNms_p,
        ValOctetsIn_t * const AMF_p,
        ValOctetsOut_t * const AUTS_p);


/*-----------------------------------------------------------------------------
 * val_SFMilenageConformance
 */
/**
 * This function performance a conformance check with a test set as specified
 * in 3GPP TS 35.208.
 *
 * @param [in] RAND_p
 *     Pointer to the 128-bit (16 Bytes) RAND.
 *
 * @param [in] SQN_p
 *     Pointer to the 48-bit (6 Bytes) SQN.
 *
 * @param [in] AMF_p
 *     Pointer to the 16-bit (2 Bytes) AMF.
 *
 * @param [in] K_p
 *     Pointer to the 128-bit (16 Bytes) K.
 *
 * @param [in] OP_p
 *     Pointer to the 128-bit (16 Bytes) OP.
 *
 * @param [out] RES_p
 *     Pointer to the 64-bit (8 Bytes) buffer in which the RES must be written.
 *
 * @param [out] CK_p
 *     Pointer to the 128-bit (16 Bytes) buffer in which the CK must be written.
 *
 * @param [out] IK_p
 *     Pointer to the 128-bit (16 Bytes) buffer in which the IK must be written.
 *
 * @param [out] MACA_p
 *     Pointer to the 64-bit (8 Bytes) buffer in which the MAC-A must be written.
 *
 * @param [out] MACS_p
 *     Pointer to the 64-bit (8 Bytes) buffer in which the MAC-S must be written.
 *
 * @param [out] AK_p
 *     Pointer to the 48-bit (6 Bytes) buffer in which the AK must be written.
 *
 * @param [out] AKstar_p
 *     Pointer to the 48-bit (6 Bytes) buffer in which the AK* must be written.
 *
 * @param [out] OPc_p
 *     Pointer to the 128-bit (16 Bytes) buffer in which the OPc must be written.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SFMilenageConformance(
        ValOctetsIn_t * const RAND_p,
        ValOctetsIn_t * const SQN_p,
        ValOctetsIn_t * const AMF_p,
        ValOctetsIn_t * const K_p,
        ValOctetsIn_t * const OP_p,
        ValOctetsOut_t * const RES_p,
        ValOctetsOut_t * const CK_p,
        ValOctetsOut_t * const IK_p,
        ValOctetsOut_t * const MACA_p,
        ValOctetsOut_t * const MACS_p,
        ValOctetsOut_t * const AK_p,
        ValOctetsOut_t * const AKstar_p,
        ValOctetsOut_t * const OPc_p);


/*-----------------------------------------------------------------------------
 * val_SFBluetoothLE_f5
 */
/**
 * This function performs the Bluetooth LE Secure Connections Key Generation
 * Function f5. This Key Generation Function is defined in the BLUETOOTH
 * SPECIFICATION Version 5.0, Vol 3, Part H, section 2.2.7.
 *
 * @param [in] SharedSecretAssetId
 *     Asset identifier of the Shared Secret that was performed as part of the
 *     LE Secure Connections pairing process.
 *
 * @param [in] LongTermKeyAssetId
 *     Asset identifier of the Long Term Key (LTK).
 *
 * @param [in] N1_p
 *     Pointer to the 128-bit (16 Bytes) buffer with N1 data.
 *
 * @param [in] N2_p
 *     Pointer to the 128-bit (16 Bytes) buffer with N2 data.
 *
 * @param [in] N1_p
 *     Pointer to the 56-bit (7 Bytes) buffer with A1 data.
 *
 * @param [in] N2_p
 *     Pointer to the 56-bit (7 Bytes) buffer with A2 data.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SFBluetoothLE_f5(
        const ValAssetId_t SharedSecretAssetId,
        const ValAssetId_t LongTermKeyAssetId,
        ValOctetsIn_t * const N1_p,
        ValOctetsIn_t * const N2_p,
        ValOctetsIn_t * const A1_p,
        ValOctetsIn_t * const A2_p,
        ValOctetsOut_t * const MacKey_p);


/*-----------------------------------------------------------------------------
 * val_CoverageTkn
 */
/**
 * This function performs the Code Coverage data extraction from the RISC-V
 * dataram. RISC-V protected application need to be built with enabled GCOV
 * support. This function specifies the destination address where the gcda data
 * have to be saved and name of the object file whose coverage data have been
 * requested.
 *
 * @param [in] DstData_p
 *     Destination address of allocated buffer for storing the raw content of
 *     .gcda file. If NULL pointer is provided for DstData_p, RISC-V token handler
 *     should return size of the without coping actual data.
 *
 * @param [in] Filename
 *     Name of the object file which .gcda data are requested
 *
 * @param [out] OutDataSize
 *     Pointer to the location where to store size of coverage data returned through
 *     DMA.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_CoverageTkn(
        ValOctetsOut_t * const DstData_p,
        const char * Filename,
        ValSize_t * const OutDataSize);


/*-----------------------------------------------------------------------------
 * val_SF_ProtectedApp
 */
/**
 * This function can be used for exchanging the data between Host application and
 * user defined protected application from PRoT.
 *
 * @param [in] ProtectedAppId
 *     Id value of the user defined protected application from PRoT.
 *     This is actually a SubCode field from the Token's Word 0.
 *
 * @param [in] DataInLen
 *     Specifies the size of the input token data.
 *
 * @param [in] DataIn_p
 *     Pointer to the memory with input token data.
 *
 * @param [out] DataOutLen_p
 *     Pointer to the location where to store info about the size
 *     of received data from PRoT.
 *
 * @param [out] DataOut_p
 *     Pointer to the memory with received data from PRoT.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_SF_ProtectedApp(
        const ValAssetId_t ProtectedAppId,
        const ValSize_t DataInLen,
        ValOctetsIn_t * const DataIn_p,
        ValSize_t * const DataOutLen_p,
        ValOctetsOut_t * const DataOut_p);

#endif /* Include Guard */


/* end of file api_val_specfuncs.h */
