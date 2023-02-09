#ifndef _RL6842_RW_PROT_H_
#define _RL6842_RW_PROT_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup RW_PROT
* @{
*/

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup RW_PROT_Register_Definitions RW_PROT Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup REG_RW_PROT_SYSON_WR_MASK0
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_31_0      ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_31_0(x)        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_31_0(x)    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK1
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_63_32     ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_63_32(x)       ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_63_32(x)   ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK2
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_95_64     ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_95_64(x)       ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_95_64(x)   ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK3
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_127_96    ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_127_96(x)      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_127_96(x)  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK4
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_159_128   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_159_128(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_159_128(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK5
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_191_160   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_191_160(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_191_160(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK6
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_223_192   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_223_192(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_223_192(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK7
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_255_224   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_255_224(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_255_224(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK8
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_287_256   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_287_256(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_287_256(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK9
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_319_288   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_319_288(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_319_288(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK10
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_351_320   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_351_320(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_351_320(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK11
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_383_352   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_383_352(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_383_352(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK12
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_415_384   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_415_384(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_415_384(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK13
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_447_416   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_447_416(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_447_416(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK14
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_479_448   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_479_448(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_479_448(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK15
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_511_480   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_511_480(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_511_480(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK16
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_543_512   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_543_512(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_543_512(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK17
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_575_544   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_575_544(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_575_544(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK18
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_607_576   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_607_576(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_607_576(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK19
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_639_608   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_639_608(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_639_608(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK20
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_671_640   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_671_640(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_671_640(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK21
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_703_672   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_703_672(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_703_672(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK22
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_735_704   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_735_704(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_735_704(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_RW_PROT_SYSON_WR_MASK23
 * @brief
 * @{
 **/
#define RW_MASK_PROT_SYSON_WR_MASK_767_736   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  write mask for kr4/hifi/gdma access syson low 4KB address space each bit is for write mask control 4byte reg the reg can't be access by kr4/hifi/gdma 1:write mask 0:write enable */
#define RW_PROT_SYSON_WR_MASK_767_736(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define RW_GET_PROT_SYSON_WR_MASK_767_736(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */


/*==========RW_PROT Register Address Definition==========*/
#define REG_RW_PROT_SYSON_WR_MASK0                   0x0000
#define REG_RW_PROT_SYSON_WR_MASK1                   0x0004
#define REG_RW_PROT_SYSON_WR_MASK2                   0x0008
#define REG_RW_PROT_SYSON_WR_MASK3                   0x000C
#define REG_RW_PROT_SYSON_WR_MASK4                   0x0010
#define REG_RW_PROT_SYSON_WR_MASK5                   0x0014
#define REG_RW_PROT_SYSON_WR_MASK6                   0x0018
#define REG_RW_PROT_SYSON_WR_MASK7                   0x001C
#define REG_RW_PROT_SYSON_WR_MASK8                   0x0020
#define REG_RW_PROT_SYSON_WR_MASK9                   0x0024
#define REG_RW_PROT_SYSON_WR_MASK10                  0x0028
#define REG_RW_PROT_SYSON_WR_MASK11                  0x002C
#define REG_RW_PROT_SYSON_WR_MASK12                  0x0030
#define REG_RW_PROT_SYSON_WR_MASK13                  0x0034
#define REG_RW_PROT_SYSON_WR_MASK14                  0x0038
#define REG_RW_PROT_SYSON_WR_MASK15                  0x003C
#define REG_RW_PROT_SYSON_WR_MASK16                  0x0040
#define REG_RW_PROT_SYSON_WR_MASK17                  0x0044
#define REG_RW_PROT_SYSON_WR_MASK18                  0x0048
#define REG_RW_PROT_SYSON_WR_MASK19                  0x004C
#define REG_RW_PROT_SYSON_WR_MASK20                  0x0050
#define REG_RW_PROT_SYSON_WR_MASK21                  0x0054
#define REG_RW_PROT_SYSON_WR_MASK22                  0x0058
#define REG_RW_PROT_SYSON_WR_MASK23                  0x005C
#define REG_RW_PROT_DUMMY                            0x00FC

/** @} */

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here
#define SYSON_RW_PROT_BASE						(0x4100DE00)
/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
