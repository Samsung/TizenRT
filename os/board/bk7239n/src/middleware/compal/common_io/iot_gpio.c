// Copyright 2020-2021 Beken 
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include <os/mem.h>



/*Common IO includes*/
#include "iot_gpio.h"

/* Beken Board includs. */
#include "bk_gpio.h"
#include "driver/gpio.h"
#include "driver/hal/hal_gpio_types.h"
#include "gpio_hal.h"
#include "gpio_driver_base.h"
#include "gpio_driver.h"

/************************************************************************************
*       Private Helpers
**************************************************************************************/
/* The State of Gpio */
typedef enum
{
    IOT_GPIO_CLOSED=0u,
    IOT_GPIO_OPENED=1u,
    

}IotGpioState_t;

#define IOT_TST_GPIO_NUM    2//set pin number const pin0 and pin1
//#define IOT_TAG "IOT"
#ifdef IOT_TAG
#define IOT_LOGI(...) BK_LOGI(IOT_TAG, ##__VA_ARGS__)
#define IOT_LOGW(...) BK_LOGW(IOT_TAG, ##__VA_ARGS__)
#define IOT_LOGE(...) BK_LOGE(IOT_TAG, ##__VA_ARGS__)
#define IOT_LOGD(...) BK_LOGD(IOT_TAG, ##__VA_ARGS__)
#else
#define IOT_LOGI(...) 
#define IOT_LOGW(...) 
#define IOT_LOGE(...) 
#define IOT_LOGD(...) 

#endif
/* The Property of Gpio */
typedef struct IotGpioConfig
{
    IotGpioDirection_t  xDirection;
    IotGpioOutputMode_t xOutMode;
    int32_t             lDriveStrength;//equal to the value of capacity range:0~3
    IotGpioPull_t       xPull;
    IotGpioInterrupt_t  xInterruptMode;

}IotGpioConfig_t;

typedef struct IotGpioDescriptor
{
    int32_t         lGpioNumber;
    IotGpioConfig_t xConfig;
    IotGpioCallback_t   xUserCallBack;
    void *pvUserContext;
    uint8_t ucState;

}IotGpioDescriptor_t;

static IotGpioDescriptor_t pxGpioDesc[IOT_TST_GPIO_NUM]={0};

static const IotGpioDescriptor_t xDefaultGpioDesc =
{
    .lGpioNumber=-1,
    .xConfig    =
    {
        .xDirection = eGpioDirectionInput,
        .xOutMode    = eGpioPushPull,
        .lDriveStrength = 0,
        .xPull      = eGpioPullNone,
        .xInterruptMode =eGpioInterruptNone,
    },
    .xUserCallBack    = NULL,
    .pvUserContext    = NULL,
    .ucState          =IOT_GPIO_CLOSED,
};
static bool prvIsValidPinIndex(int32_t lGpioNumber)
{
    if((lGpioNumber<0)||(lGpioNumber>=IOT_TST_GPIO_NUM))
    {
        return false;
    }

    return true;

}
static bool prvIsValidHandle(IotGpioHandle_t const pxGpio)
{
    return (pxGpio!=NULL)&&(pxGpio->ucState==IOT_GPIO_OPENED);
}

static bool prvGetPullForHal(IotGpioPull_t xPull,
                             gpio_pull_mode_t *xPull_Bk)
{
    bool bRtnCd=true;
    switch(xPull)
    {
        case eGpioPullNone:
            *xPull_Bk=GPIO_PULL_DISABLE;
        break;
        case eGpioPullUp:
            *xPull_Bk=GPIO_PULL_UP_EN;
        break;
        case eGpioPullDown:
            *xPull_Bk=GPIO_PULL_DOWN_EN;
        break;
        default:
           *xPull_Bk=GPIO_PULL_INVALID;
            bRtnCd=false;
        break;
    }
    return bRtnCd;
}

static int32_t prvGetIntModeForHAL(IotGpioInterrupt_t xInterrupt,
                                   gpio_int_type_t *xIntType)
{
    int32_t lRtdCd=IOT_GPIO_SUCCESS;
    switch(xInterrupt)
    {
        case eGpioInterruptNone:
            *xIntType=GPIO_INT_TYPE_MAX;
        break;
        case eGpioInterruptRising:
            *xIntType=GPIO_INT_TYPE_RISING_EDGE;
        break;
        case eGpioInterruptFalling:
            *xIntType=GPIO_INT_TYPE_FALLING_EDGE;
        break;
        case eGpioInterruptEdge:
            lRtdCd=IOT_GPIO_FUNCTION_NOT_SUPPORTED;
        break;
        case eGpioInterruptLow:
            *xIntType=GPIO_INT_TYPE_LOW_LEVEL;
        break;
        case eGpioInterruptHigh:
            *xIntType=GPIO_INT_TYPE_HIGH_LEVEL;
        break;
        default:
            lRtdCd=IOT_GPIO_INVALID_VALUE;
        break;
    }
    return lRtdCd;

}

static void prvPinIntEventHandler(gpio_id_t xPin)
{
    IotGpioHandle_t pxGpio=NULL;
    pxGpio=&pxGpioDesc[xPin];
    
    if((pxGpio != NULL)&&(pxGpio->xUserCallBack != NULL)&&(pxGpio->xConfig.xInterruptMode != eGpioInterruptNone))
    {
        pxGpio->xUserCallBack(bk_gpio_get_input(xPin),pxGpio->pvUserContext);
        IOT_LOGI("RcvGpio[%d]Int\n",xPin);
    }
}
                                            

static int32_t prvConfigureInputPin(IotGpioHandle_t const pxGpio,
                                    IotGpioConfig_t* pxNewConfig
                                   )
{
    int32_t lRtnCode=IOT_GPIO_SUCCESS;
    gpio_pull_mode_t xPull_Bk;
    bk_err_t ret=BK_ERR_BUSY;

    if(!prvGetPullForHal(pxNewConfig->xPull,&xPull_Bk))
    {
        lRtnCode=IOT_GPIO_INVALID_VALUE;
        IOT_LOGI("GetPullFail:%d %d",pxNewConfig->xPull,xPull_Bk);
    }
    
    gpio_int_type_t xGpioIntCfg;

    if(lRtnCode==IOT_GPIO_SUCCESS)
    {
        lRtnCode=prvGetIntModeForHAL(pxNewConfig->xInterruptMode,&xGpioIntCfg);
    }
    gpio_config_t xGpioBkCfg;
    xGpioBkCfg.io_mode=GPIO_INPUT_ENABLE;
    xGpioBkCfg.pull_mode=xPull_Bk;
    xGpioBkCfg.func_mode=GPIO_SECOND_FUNC_DISABLE;
    if(lRtnCode==IOT_GPIO_SUCCESS)
    {
        gpio_dev_unmap(pxGpio->lGpioNumber);
        BK_LOG_ON_ERR(bk_gpio_disable_output(pxGpio->lGpioNumber));//fisrt time do with dev_unmap
        ret=bk_gpio_set_config(pxGpio->lGpioNumber, &xGpioBkCfg);
        if(ret==BK_OK)
        {
            
            if(pxNewConfig->xInterruptMode!=eGpioInterruptNone)
            {
                //check if gpio initialized first
                IOT_LOGI("GPIO Int Mode:%d \n",xGpioIntCfg);
                //bk_set_printf_sync(1);//add for debug remember to delete
                //init interrupt
                ret=bk_gpio_register_isr(pxGpio->lGpioNumber, prvPinIntEventHandler);
                if(ret==BK_OK)
                {
                    ret=bk_gpio_set_interrupt_type(pxGpio->lGpioNumber, xGpioIntCfg);
                    if(ret==BK_OK)
                    {
                        BK_LOG_ON_ERR(bk_gpio_enable_interrupt(pxGpio->lGpioNumber));
                        lRtnCode=IOT_GPIO_SUCCESS;
                        IOT_LOGI("Test Gpio Int Set Ok\n");
                    }
                    else
                    {
                        IOT_LOGE("ERR GPIO[%d] int type fail [%d]\n",pxGpio->lGpioNumber,ret);
                        lRtnCode=IOT_GPIO_INVALID_VALUE;
                    }
                }
                else
                {
                    IOT_LOGE("ERR GPIO[%d] int register fail [%d]\n",pxGpio->lGpioNumber,ret);
                    lRtnCode=IOT_GPIO_INVALID_VALUE;

                }
            }
            
        }
        else
        {
            IOT_LOGE("ERR GPIO[%d] config fail [%d]\n",pxGpio->lGpioNumber,ret);
            lRtnCode=IOT_GPIO_INVALID_VALUE;
        }
    }
    else
    {
        IOT_LOGE("GpioIntModeFail:%d\n",pxNewConfig->xInterruptMode);
    }

  return lRtnCode;
}

static bool prvGetDriveStrengthForHal(int32_t lDrvStrength,
                                                                            int32_t *lDrvStrength_Bk)
{
    bool bRtnCode=true;
    if(lDrvStrength>3)
    {
        *lDrvStrength_Bk=3;
        IOT_LOGE("GPIO Strength [%d] greater 3\r\n",lDrvStrength);
    }
    else
    {
        *lDrvStrength_Bk=lDrvStrength;
    }
    return bRtnCode;
}
                                                    
static int32_t prvConfigOutputPin(IotGpioHandle_t const pxGpio,
                                                                    IotGpioConfig_t *pxNewConfig)
{
    int32_t lRetCd=IOT_GPIO_INVALID_VALUE;
    gpio_pull_mode_t    xPullBk;
    bool bValidInputs ;
    int32_t xDrive_Bk;
    gpio_config_t xGpioCfgOut;
        
    if(pxNewConfig->xOutMode==eGpioOpenDrain)
    {
        pxNewConfig->xPull=eGpioPullNone;
    }
    bValidInputs=prvGetPullForHal(pxNewConfig->xPull,&xPullBk);
    bValidInputs&=prvGetDriveStrengthForHal(pxNewConfig->lDriveStrength,&xDrive_Bk);
    IOT_LOGI("%s valid:%d\n",__func__,bValidInputs);    
    
    if(bValidInputs)
    {
        BK_LOG_ON_ERR(gpio_dev_unmap(pxGpio->lGpioNumber));
        BK_LOG_ON_ERR(bk_gpio_disable_input(pxGpio->lGpioNumber));
        BK_LOG_ON_ERR(bk_gpio_set_capacity(pxGpio->lGpioNumber, xDrive_Bk));//no matter
        xGpioCfgOut.io_mode=GPIO_OUTPUT_ENABLE;
        xGpioCfgOut.pull_mode=xPullBk;
        xGpioCfgOut.func_mode=GPIO_SECOND_FUNC_DISABLE;
        BK_LOG_ON_ERR(bk_gpio_set_config(pxGpio->lGpioNumber,&xGpioCfgOut));
//      BK_LOG_ON_ERR(bk_gpio_enable_output(pxGpio->lGpioNumber));// the same as the io_mode of GPIO_OUTPUT_ENABLE
        lRetCd=IOT_GPIO_SUCCESS;
    }
    return lRetCd;

}                                      
static int32_t prvConfigurePin(IotGpioHandle_t  const pxGpio,
                               IotGpioConfig_t *pxNewConfig)
{
    int32_t lRtnCode=IOT_GPIO_INVALID_VALUE;


    if(pxNewConfig->xDirection==eGpioDirectionInput)
    {
        lRtnCode=prvConfigureInputPin(pxGpio,pxNewConfig);
    }
    else
    {
        lRtnCode=prvConfigOutputPin(pxGpio,pxNewConfig);
    }
    if(lRtnCode==IOT_GPIO_SUCCESS)
    {
        pxGpio->xConfig=*pxNewConfig;
//      IOT_LOGI("%s\n",__func__);
    }

    return lRtnCode;
}
/*---------------------------------------------------------------------------------*
*                               API Implementation                                *
*---------------------------------------------------------------------------------*/

IotGpioHandle_t iot_gpio_open( int32_t lGpioNumber )
{
    IotGpioHandle_t xReturnHandle=NULL;
    if(prvIsValidPinIndex(lGpioNumber))
    {
        IotGpioHandle_t pxGpio=&pxGpioDesc[lGpioNumber];
        
        if(pxGpio->ucState==IOT_GPIO_CLOSED)
        {
            *pxGpio=xDefaultGpioDesc;
            pxGpio->lGpioNumber=lGpioNumber;
            
            //add for debug
            if(pxGpio->xConfig.xInterruptMode!=eGpioInterruptNone)
            {
                BK_LOG_ON_ERR(bk_gpio_disable_interrupt(pxGpio->lGpioNumber));
            }     
            if(IOT_GPIO_SUCCESS==prvConfigurePin(pxGpio,&pxGpio->xConfig))
            {
                pxGpio->ucState=IOT_GPIO_OPENED;
                xReturnHandle=pxGpio;
                IOT_LOGI("%s pin[%d] ok \r\n",__func__,pxGpio->lGpioNumber);
            }
            else
            {
                IOT_LOGE("%s Config Fail GPIO[%]\r\n",__func__,lGpioNumber);
            }
            
        }
        else
        {
            IOT_LOGE("%s Cannot open.GPIO[%d] is already opened\r\n",__func__,lGpioNumber);
        }
    }
    else
    {
        IOT_LOGE("%s Incorrect Pin[%d] Index,Please vefy with IOT_TST_GPIO_NUM\r\n",__func__,lGpioNumber);
    }

    return xReturnHandle;
}

void iot_gpio_set_callback( IotGpioHandle_t const pxGpio,
                            IotGpioCallback_t xGpioCallback,
                            void * pvUserContext )
{
    if(prvIsValidHandle(pxGpio)&&(xGpioCallback!=NULL))
    {
        pxGpio->xUserCallBack=xGpioCallback;
        pxGpio->pvUserContext=pvUserContext;
    }
}

int32_t iot_gpio_read_sync( IotGpioHandle_t const pxGpio,
                            uint8_t * pucPinState )
{
    int32_t lRtnCd=IOT_GPIO_SUCCESS;
    if(prvIsValidHandle(pxGpio)&&(pxGpio->xConfig.xDirection==eGpioDirectionInput))
    {
        IOT_LOGI("%s pin[%d] ok\r\n",__func__,pxGpio->lGpioNumber);
        *pucPinState=bk_gpio_get_input(pxGpio->lGpioNumber);
    }
    else
    {
        IOT_LOGI("%s pin[%d] Dir[%d] fail\r\n",__func__,pxGpio->lGpioNumber,pxGpio->xConfig.xDirection);
        lRtnCd=IOT_GPIO_INVALID_VALUE;
    }
    return lRtnCd;
}

int32_t iot_gpio_write_sync( IotGpioHandle_t const pxGpio,
                             uint8_t ucPinState )
{
    int32_t lRetCd=IOT_GPIO_SUCCESS;
    if(prvIsValidHandle(pxGpio)&&(pxGpio->xConfig.xDirection==eGpioDirectionOutput))
    {
        IOT_LOGI("%s pin[%d] val:%d\r\n",__func__,pxGpio->lGpioNumber,ucPinState);
        if(ucPinState)
        {
            BK_LOG_ON_ERR(bk_gpio_set_output_high(pxGpio->lGpioNumber));
        }
        else
        {
            BK_LOG_ON_ERR(bk_gpio_set_output_low(pxGpio->lGpioNumber));
        }
    }
    else
    {
        IOT_LOGI("%s pin[%d] state[%d] fail\r\n",__func__,pxGpio->lGpioNumber,ucPinState);
        lRetCd=IOT_GPIO_INVALID_VALUE;
    }
    return lRetCd;

}

int32_t iot_gpio_close( IotGpioHandle_t const pxGpio )
{
    int32_t lRetCd=IOT_GPIO_SUCCESS;
    IotGpioConfig_t xTmpGpioCfg;
    
    if(prvIsValidHandle(pxGpio))
    {
        IOT_LOGI("%s pin[%d] ok\r\n",__func__,pxGpio->lGpioNumber);
        //add for debug
        if(pxGpio->xConfig.xInterruptMode!=eGpioInterruptNone)
        {
            BK_LOG_ON_ERR(bk_gpio_disable_interrupt(pxGpio->lGpioNumber));
        }   
        
        os_memcpy(&xTmpGpioCfg,&xDefaultGpioDesc.xConfig,sizeof(IotGpioConfig_t));
        pxGpio->ucState=IOT_GPIO_CLOSED;
        lRetCd=prvConfigurePin(pxGpio, &xTmpGpioCfg);
        pxGpio->lGpioNumber=xDefaultGpioDesc.lGpioNumber;
        pxGpio->xUserCallBack=xDefaultGpioDesc.xUserCallBack;
        pxGpio->pvUserContext=xDefaultGpioDesc.pvUserContext;
    
    }
    else
    {
        if(pxGpio!=NULL)
        {
            IOT_LOGE("%s pin[%d] state [%d]\r\n",__func__,pxGpio->lGpioNumber,pxGpio->ucState);
        }
        else
        {
            IOT_LOGI("%s NULL\r\n",__func__);
        }
         lRetCd=IOT_GPIO_INVALID_VALUE;
    }
    
    return lRetCd;
}

int32_t iot_gpio_ioctl( IotGpioHandle_t const pxGpio,
                        IotGpioIoctlRequest_t xRequest,
                        void * const pvBuffer )
{
    int32_t lRetCd=IOT_GPIO_INVALID_VALUE;
    
    IOT_LOGI("%s pin[%d] \r\n",__func__,pxGpio->lGpioNumber);
    if(prvIsValidHandle(pxGpio))
    {
        IotGpioConfig_t xNewConfig=pxGpio->xConfig;
        lRetCd=IOT_GPIO_SUCCESS;
        switch(xRequest)
        {
            case eSetGpioDirection:
                os_memcpy(&xNewConfig.xDirection,pvBuffer,sizeof(xNewConfig.xDirection));
                lRetCd=prvConfigurePin(pxGpio,&xNewConfig);
                IOT_LOGI("eSetGpioDirection \r\n");
            break;
            case eGetGpioDirection:
                os_memcpy(pvBuffer,&pxGpio->xConfig.xDirection,sizeof(pxGpio->xConfig.xDirection));
                IOT_LOGI("eGetGpioDirection\r\n");
            break;
            case eSetGpioPull:
                os_memcpy(&xNewConfig.xPull,pvBuffer,sizeof(xNewConfig.xPull));
                lRetCd=prvConfigurePin(pxGpio,&xNewConfig);
                IOT_LOGI("eSetGpioPull\r\n");
            break;
            case eGetGpioPull:
                os_memcpy(pvBuffer,&pxGpio->xConfig.xPull,sizeof(pxGpio->xConfig.xPull));
                IOT_LOGI("eGetGpioPull\r\n");
            break;
            case eSetGpioOutputMode:
                os_memcpy(&xNewConfig.xOutMode,pvBuffer,sizeof(xNewConfig.xOutMode));
                lRetCd=prvConfigurePin(pxGpio,&xNewConfig);
                IOT_LOGI("eSetGpioOutputMode\r\n");
            break;
            case eGetGpioOutputType:
                os_memcpy(pvBuffer,&pxGpio->xConfig.xOutMode,sizeof(pxGpio->xConfig.xOutMode));
                IOT_LOGI("eGetGpioOutputType\r\n");
            break;
            case eSetGpioInterrupt:
                os_memcpy(&xNewConfig.xInterruptMode,pvBuffer,sizeof(xNewConfig.xInterruptMode));
                lRetCd=prvConfigurePin(pxGpio,&xNewConfig);
                IOT_LOGI("eSetGpioInterrupt:%d Rlt:%d Dir:%d\n",xNewConfig.xInterruptMode,pxGpio->xConfig.xInterruptMode,xNewConfig.xDirection);
            break;
            case eGetGpioInterrupt:
                os_memcpy(pvBuffer,&pxGpio->xConfig.xInterruptMode,sizeof(pxGpio->xConfig.xInterruptMode));
                IOT_LOGI("eGetGpioInterrupt:%d\n",pxGpio->xConfig.xInterruptMode);
            break;
            case eSetGpioDriveStrength:
                os_memcpy(&xNewConfig.lDriveStrength,pvBuffer,sizeof(xNewConfig.lDriveStrength));
                lRetCd=prvConfigurePin(pxGpio,&xNewConfig);
                IOT_LOGI("eSetGpioDriveStrength\r\n");
            break;
            case eGetGpioDriveStrength:
                os_memcpy(pvBuffer,&pxGpio->xConfig.lDriveStrength,sizeof(pxGpio->xConfig.lDriveStrength));
                IOT_LOGI("eGetGpioDriveStrength\r\n");
            break;
            /*Unsupported functions*/
            case eSetGpioFunction:
            case eGetGpioFunction:
            case eSetGpioSpeed:
            case eGetGpioSpeed:
            default:
                lRetCd=IOT_GPIO_FUNCTION_NOT_SUPPORTED;
                IOT_LOGW("Warning:ioctlMode:[%d] is unsuppored\r\n",xRequest);
            break;

        }
    }

    return lRetCd;
}

