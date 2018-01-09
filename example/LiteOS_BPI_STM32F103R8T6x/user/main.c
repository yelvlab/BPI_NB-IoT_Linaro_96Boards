#include "los_sys.h"
#include "los_tick.h"
#include "los_task.ph"
#include "los_config.h"

#include "los_bsp_led.h"
#include "los_bsp_key.h"
#include "los_bsp_uart.h"
#include "los_inspect_entry.h"
#include "los_demo_entry.h"
#include "cmsis_os.h"

#include <string.h>

#include "stm32f1xx_hal.h"

#include "los_dev_st_uart.h"



extern void LOS_EvbSetup(void);

static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

}
void SystemClock_Config(void);

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
/* USER CODE BEGIN 4 */

/* USER CODE END 4 */



#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif


static UINT32 g_uwboadTaskID;
LITE_OS_SEC_TEXT VOID LOS_BoadExampleTskfunc(VOID)
{
    while (1)
    {
        //LOS_EvbLedControl(LOS_LED2, LED_ON);
        LOS_EvbUartWriteStr("Board Test\n");
        LOS_TaskDelay(500);
        //LOS_EvbLedControl(LOS_LED2, LED_OFF);
        LOS_TaskDelay(500);
    }
}
void LOS_BoadExampleEntry(void)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTaskInitParam;

    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LOS_BoadExampleTskfunc;
    stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
    stTaskInitParam.pcName = "BoardDemo";
    stTaskInitParam.usTaskPrio = 10;
    uwRet = LOS_TaskCreate(&g_uwboadTaskID, &stTaskInitParam);

    if (uwRet != LOS_OK)
    {
        return;
    }
    return;
}

static char tb1[500];
static char rtb1[500];
void init_testuart(void * pvParameters)
{
    int len;
    int ret = 0;
    ret = los_dev_uart_init(LOS_STM32F103_UART4, 9600, tb1, 500);
    if (ret != 0)
    {
        while(1);
    }
	while(1)
	{
        //len = uart_data_read(tb1, 500, 0);
        len = los_dev_uart_read(3, rtb1, 100, 5000);
        if (len > 0)
            los_dev_uart_write(3, rtb1, len, 100);
		osDelay(10);
	}
}
static char tb12[500];
static char rtb12[500];
void init_testuart2(void * pvParameters)
{
    int len;
    int ret = 0;
    ret = los_dev_uart_init(LOS_STM32F103_UART2, 9600, tb12, 500);
    if (ret != 0)
    {
        while(1);
    }
	while(1)
	{
        //len = uart_data_read(tb1, 500, 0);
        len = los_dev_uart_read(1, rtb12, 100, 5000);
        if (len > 0)
            los_dev_uart_write(1, rtb12, len, 100);
		osDelay(10);
	}
}

/*****************************************************************************
Function    : main
Description : Main function entry
Input       : None
Output      : None
Return      : None
 *****************************************************************************/
LITE_OS_SEC_TEXT_INIT
int main(void)
{
	UINT32 uwRet;
	osThreadDef_t thread_def;
    /*
        add you hardware init code here
        for example flash, i2c , system clock ....
     */
    /*Init LiteOS kernel */
    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK) {
        return LOS_NOK;
    }
    //HAL_init();....
    HAL_Init();

    /* Configure the system clock to 216 MHz */
		
    SystemClock_Config();
    MX_GPIO_Init();
    /* Enable LiteOS system tick interrupt */
    LOS_EnableTick();

    /*
        Notice: add your code here
        here you can create task for your function 
        do some hw init that need after systemtick init
     */
    //LOS_EvbSetup();//init the device on the dev baord
    
    //MX_RTC_Init();//enable RTC
    
    //LOS_Demo_Entry();

    //LOS_Inspect_Entry();

    //LOS_BoadExampleEntry();

    thread_def.name = "Test";
	thread_def.stacksize = 1900;
	thread_def.tpriority = osPriorityLow;
	thread_def.pthread = (os_pthread)init_testuart;
	//osThreadCreate(&thread_def, NULL);

    thread_def.name = "Test2";
	thread_def.stacksize = 1900;
	thread_def.tpriority = osPriorityLow;
	thread_def.pthread = (os_pthread)init_testuart2;
	//osThreadCreate(&thread_def, NULL);

	//neul_bc95_create_data_task();
    
    LOS_OceanCon_NB_Sample();
    
    //LOS_PostBox_NB_Sample();
    /* Kernel start to run */
    LOS_Start();
    for (;;);
    /* Replace the dots (...) with your own code. */
}
