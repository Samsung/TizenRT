#include <common/bk_include.h>
#include "bk_arm_arch.h"

#include "bk_drv_model.h"
#include "drv_model.h"
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>


static __attribute__((section(".dtcm_sec_data "))) DRV_DEV_S drv_dev_tbl[DD_MAX_DEV] = {
	{0},
};

UINT32 drv_model_init(void)
{
	os_memset(drv_dev_tbl, 0, sizeof(drv_dev_tbl));

	return DRV_SUCCESS;
}

UINT32 drv_model_uninit(void)
{
	os_memset(drv_dev_tbl, 0, sizeof(drv_dev_tbl));

	return DRV_SUCCESS;
}

UINT32 ddev_check_handle(DD_HANDLE handle)
{
	UINT32 magic;
	UINT32 idx;

	magic = handle & DD_HANDLE_MAGIC_MASK;
	idx   = handle & DD_HANDLE_ID_MASK;
	if ((DD_HANDLE_MAGIC_WORD == magic)
		&& (idx < DD_MAX_DEV))
		return DRV_SUCCESS;
	else
		return DRV_FAILURE;
}

DD_HANDLE ddev_make_handle(UINT32 idx)
{
	UINT32 handle = DD_HANDLE_UNVALID;

	if (idx >= DD_MAX_DEV)
		goto make_exit;

	handle = idx + DD_HANDLE_MAGIC_WORD;

make_exit:
	return handle;
}

UINT32 ddev_get_id_from_handle(DD_HANDLE handle)
{
	UINT32 magic;
	UINT32 idx;

	magic = handle & DD_HANDLE_MAGIC_MASK;
	idx   = handle & DD_HANDLE_ID_MASK;

	if ((magic != DD_HANDLE_MAGIC_WORD) || (idx >= DD_DEV_TYPE_END))
		return DD_ID_UNVALID;

	return idx;
}

DD_HANDLE ddev_open(dd_device_type dev, UINT32 *status, UINT32 op_flag)
{
	UINT32 idx;
	UINT32 handle;
	DRV_DEV_PTR dev_ptr;
	DD_OPERATIONS *operation;

	GLOBAL_INT_DECLARATION();

	handle = DD_HANDLE_UNVALID;
	idx = ddev_get_id_from_handle(dev);
	if (DD_ID_UNVALID == idx)
		return DRV_FAILURE;
	*status = DRV_FAILURE;

	dev_ptr = &drv_dev_tbl[idx];
	if (DD_STATE_OPENED == dev_ptr->state)
		handle = ddev_make_handle(idx);
	else if (DD_STATE_CLOSED == dev_ptr->state) {
		handle = ddev_make_handle(idx);

		operation = dev_ptr->op;
		if (operation && (operation->open))
			*status = (operation->open)(op_flag);

		GLOBAL_INT_DISABLE();
		dev_ptr->state = DD_STATE_OPENED;
		dev_ptr->use_cnt = 0;
		GLOBAL_INT_RESTORE();
	} else {
	}

	GLOBAL_INT_DISABLE();
	dev_ptr->use_cnt ++;
	GLOBAL_INT_RESTORE();

	return handle;
}

UINT32 ddev_close(DD_HANDLE handle)
{
	UINT32 idx;
	DRV_DEV_PTR dev_ptr;
	DD_OPERATIONS *operation;
	GLOBAL_INT_DECLARATION();

	idx = ddev_get_id_from_handle(handle);
	if (DD_ID_UNVALID == idx)
		return DRV_FAILURE;

	dev_ptr = &drv_dev_tbl[idx];

	GLOBAL_INT_DISABLE();
	dev_ptr->use_cnt --;
	GLOBAL_INT_RESTORE();

	if (0 == dev_ptr->use_cnt) {
		operation = dev_ptr->op;
		if (operation && (operation->close))
			(operation->close)();

		GLOBAL_INT_DISABLE();
		dev_ptr->state = DD_STATE_CLOSED;
		GLOBAL_INT_RESTORE();
	}

	return DRV_SUCCESS;
}

UINT32 ddev_read(DD_HANDLE handle, char *user_buf, UINT32 count, UINT32 op_flag)
{
	UINT32 idx;
	UINT32 status;
	DRV_DEV_PTR dev_ptr;
	DD_OPERATIONS *operation;

	idx = ddev_get_id_from_handle(handle);
	if (DD_ID_UNVALID == idx)
		return DRV_FAILURE;

	status = DRV_FAILURE;
	dev_ptr = &drv_dev_tbl[idx];
	operation = dev_ptr->op;
	if (operation && (operation->read))
		status = (operation->read)(user_buf, count, op_flag);

	return status;
}

UINT32 ddev_write(DD_HANDLE handle, char *user_buf, UINT32 count, UINT32 op_flag)
{
	UINT32 idx;
	UINT32 status;
	DRV_DEV_PTR dev_ptr;
	DD_OPERATIONS *operation;

	idx = ddev_get_id_from_handle(handle);
	if (DD_ID_UNVALID == idx)
		return DRV_FAILURE;

	status    = DRV_FAILURE;
	dev_ptr   = &drv_dev_tbl[idx];
	operation = dev_ptr->op;
	if (operation && (operation->write))
		status = (operation->write)(user_buf, count, op_flag);

	return status;
}

UINT32 ddev_control(DD_HANDLE handle, UINT32 cmd, VOID *param)
{
	UINT32 idx;
	UINT32 status;
	DRV_DEV_PTR dev_ptr;
	DD_OPERATIONS *operation;

	idx = ddev_get_id_from_handle(handle);
	if (DD_ID_UNVALID == idx)
		return DRV_FAILURE;

	if(idx >= DD_MAX_DEV)
		return DRV_FAILURE;
	
	status    = DRV_FAILURE;
	dev_ptr   = &drv_dev_tbl[idx];
	operation = dev_ptr->op;
	if (operation && (operation->control))
		status = (operation->control)(cmd, param);

	return status;
}

UINT32 ddev_register_dev(DD_HANDLE handle, DD_OPERATIONS *optr)
{
	UINT32 idx;
	DRV_DEV_PTR dev_ptr;

	if (!optr)
		return DRV_FAILURE;

	idx = ddev_get_id_from_handle(handle);
	if (DD_ID_UNVALID != idx) {
		dev_ptr = &drv_dev_tbl[idx];
		if (DD_STATE_NODEVICE == dev_ptr->state) {
			//            dev_ptr->name  = dev_name;
			dev_ptr->op    = optr;
			dev_ptr->state = DD_STATE_CLOSED;
		} else
			return DRV_FAILURE;
	} else
		return DRV_FAILURE;


	return DRV_SUCCESS;
}

UINT32 ddev_unregister_dev(DD_HANDLE handle)
{
	UINT32 idx;
	DRV_DEV_PTR dev_ptr;

	idx = ddev_get_id_from_handle(handle);
	if (DD_ID_UNVALID != idx) {
		dev_ptr = &drv_dev_tbl[idx];
		//        dev_ptr->name  = 0;
		dev_ptr->op    = 0;
		dev_ptr->state = DD_STATE_NODEVICE;
	}

	return DRV_SUCCESS;
}


// EOF
