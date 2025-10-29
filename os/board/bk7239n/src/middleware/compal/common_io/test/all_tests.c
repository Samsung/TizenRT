#include "unity_fixture.h"
#include <common/bk_include.h>


static void run_all_tests(void)
{
    RUN_TEST_GROUP(TEST_IOT_UART);
    //RUN_TEST_GROUP(TEST_IOT_ADC);
    //RUN_TEST_GROUP(TEST_IOT_SPI);
    
    // RUN_TEST_GROUP(TEST_IOT_I2C);
    // RUN_TEST_GROUP(TEST_IOT_POWER);
   
    //RUN_TEST_GROUP(TEST_IOT_RESET);
#if(defined(CONFIG_PWM))
    //RUN_TEST_GROUP(TEST_IOT_PWM);
#endif    
#if(defined(CONFIG_EFUSE))
    //RUN_TEST_GROUP(TEST_IOT_EFUSE);
#endif
#if(defined(CONFIG_FLASH))
    //RUN_TEST_GROUP(TEST_IOT_FLASH);
#endif
    //RUN_TEST_GROUP(TEST_IOT_GPIO);

#if(defined(CONFIG_TEMP_DETECT))
    //RUN_TEST_GROUP(TEST_IOT_TSENSOR);
#endif

#if(defined(CONFIG_INT_WDT))
    //RUN_TEST_GROUP(TEST_IOT_WATCHDOG);
#endif


#if(defined(CONFIG_AON_RTC))
    // RUN_TEST_GROUP(TEST_IOT_RTC);
#endif

}

int common_io_test_main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, run_all_tests);
}
