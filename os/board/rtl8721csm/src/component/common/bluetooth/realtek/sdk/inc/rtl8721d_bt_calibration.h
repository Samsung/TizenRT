#ifndef _RTL8721D_BT_CALIBRATION_H_
#define _RTL8721D_BT_CALIBRATION_H_


typedef struct {
	u32 IQK_xx;
	u32 IQK_yy;
	u32 IDAC;
	u32 QDAC;
}BT_Cali_TypeDef;

/**
  * @brief Do BT IQK and LOK, do not call this function in formal SDK
  * @param store, whether need store the calibration data in flash
  * @retval SUCESS or FAIL
  * @note calibration data will write to 0x0800_1FF0
  */
u32 bt_iqk_8721d(BT_Cali_TypeDef *cal_data,BOOLEAN store);

/**
  * @brief Do BT dck, this API will remove if all function works well and has no impact on wifi
  * @param none
  * @retval SUCESS or FAIL
  * @note calibration data will write to 0x0800_1FF0
  */
u32 bt_adda_dck_8721d(void);

/**
  * @brief Write LOK data to RFC
  * @param  calibration data
  * @retval NA
  */
u32 bt_lok_write(u32 lok_xx, u32 lok_yy);

/**
  * @brief Write DCK data to RFC
  * @param  calibration data
  * @retval NA
  */
u32 bt_dck_write(u8 q_dck, u8 i_dck);

/**
  * @brief BT FLATK
  * @param  calibration data
  * @retval NA
  */
u32 bt_flatk_8721d(u16 txgain_flatk);

/**
  * @brief Set PTA, This function is temporary, Will remove when bt coex mailbox ok.
  * @param  NA
  * @retval NA
  */
#endif
