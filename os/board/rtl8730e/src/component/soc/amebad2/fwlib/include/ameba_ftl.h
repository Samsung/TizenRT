#define IS_Ftl_len(NUM) ((NUM) <= 0x80)
#define CA7_CACHE_SIZE 64
/*============================================================================*
  *                                Functions
  *============================================================================*/
#if defined (ARM_CORE_CM4)

/**
    * @brief    Save specified value to specified ftl offset
    * @param    pdata  specify data buffer
    * @param    offset specify FTL offset to store
    * @param    size   size to store
    * @return   status
    * @retval   0  status successful
    * @retval   otherwise fail
    * @note     FTL offset is pre-defined and no confict with ROM
    */
_LONG_CALL_ uint32_t ftl_save_to_storage(void *pdata, uint16_t offset, uint16_t size);

/**
    * @brief    Load specified ftl offset parameter to specified buffer
    * @param    pdata  specify data buffer
    * @param    offset specify FTL offset to load
    * @param    size   size to load
    * @return   status
    * @retval   0  status successful
    * @retval   otherwise fail
    * @note     FTL offset is pre-defined and no confict with ROM
    */
_LONG_CALL_ uint32_t ftl_load_from_storage(void *pdata, uint16_t offset, uint16_t size);

_LONG_CALL_ void FTL_ap_request_get(VOID *Data, u32 IrqStatus, u32 ChanNum);

#endif

#if defined (ARM_CORE_CA32)
_LONG_CALL_ void Ftl_ipc_int(VOID *Data, u32 IrqStatus, u32 ChanNum);
extern uint32_t ftl_init(uint32_t u32PageStartAddr, uint8_t pagenum);
extern uint32_t ftl_save_to_storage(void *pdata, uint16_t offset, uint16_t size);
extern uint32_t ftl_load_from_storage(void *pdata, uint16_t offset, uint16_t size);

#endif
