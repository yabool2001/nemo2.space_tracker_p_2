/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
  /*
   * Breakpoint ustawiony w linii 1050 w pliku astronode_application.c żeby debugować rcv_cmd
   */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart5;

/* USER CODE BEGIN PV */

// SYSTEM
char*		hello = "\nHello nemo2.space tracker p 2\r\n\0" ;
const char*	fv = "0.0.3-alpha" ;
uint8_t 	rx_byte = 0 ;
bool 		sw1 , sw2 ;
uint8_t		sys_mode = 0 ; // 0 - Production, 1 - Simulation, 2 - Test , 3 - Reserved (maybe Development)
uint8_t		sys_mission = 0 ; // 0: Active, 1: Sustainable
uint16_t	sys_watchdog_time_ths = TIME_THS_15_MIN ;

// RTC
char		rtc_dt_s[20] ;
uint32_t 	my_rtc_alarmA_time = TIME_THS_1_H ;

// ASTRO
char		my_astro_payload[ASTRONODE_PAYLOAD_MAX_LEN] = {0} ;
cmd_astro	my_astro_cmd ;
uint16_t	uplink_id = 0 ;
uint32_t	last_uplink_send_ts = 0 ;

// GNSS
fix_astro	fix3d ;

// TIM

// ACC
stmdev_ctx_t	my_acc_ctx ;
int16_t 		my_acc_temp = 0 ;

// Flags
bool my_rtc_alarm_flag = false ;
bool my_gnss_3dfix_flag = false ;
bool my_acc_problem_flag = false ;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM6_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART5_UART_Init(void);
/* USER CODE BEGIN PFP */
void my_ldg_sw_on ( void ) ;
void my_ldb_sw_on ( void ) ;
void my_ldg_sw_off ( void ) ;
void my_ldb_sw_off ( void ) ;
void my_ldg_blink ( uint8_t , uint16_t ) ; // number of blinks, period of blink
void my_ldb_blink ( uint8_t , uint16_t ) ; // number of blinks, period of blink
void my_sys_init ( void ) ;
bool is_system_initialized ( void ) ;
void my_sys_restart ( void ) ;
void my_sys_standby ( void ) ;
void my_sys_sleep ( char* ) ;
void my_sys_deepsleep ( char* ) ;
void my_sys_change_watchdog_time_ths ( uint32_t ) ;
void my_sys_change_AlarmA_time ( uint32_t ) ;
void my_sys_change_fix_acq_ths ( uint32_t ) ;
void my_sys_change_min_tns_time_ths ( uint32_t ) ;
void my_sys_change_pdop_ths ( uint32_t ) ;
bool my_tracker_handle_cmd ( void ) ;
void my_astro_turn_payload_id_counter ( void ) ;

void my_gnss_sw_on ( void ) ;
void my_gnss_sw_off ( void ) ;
void my_gnss_verbose ( uint16_t ) ;

// ASTRO
void my_astro_send_uplink ( char* , char* ) ;

// ACC
bool 	my_acc_init ( void ) ;
bool 	my_acc_start ( void ) ;
bool 	my_acc_stop ( void ) ;
int32_t my_st_acc_platform_write ( void* , uint8_t , const uint8_t* , uint16_t ) ;
int32_t my_st_acc_platform_read ( void* , uint8_t , uint8_t* , uint16_t ) ;

void my_ant_sw_pos ( uint8_t ) ;

void my_tim_init ( void ) ;

bool my_astro_evt_pin ( void ) ;

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_TIM6_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART5_UART_Init();
  /* USER CODE BEGIN 2 */

  send_debug_logs ( hello ) ;

  my_sys_init () ;
  sprintf ( dbg_payload , "System mode: %u" , (uint16_t) sys_mode ) ;
  send_debug_logs ( dbg_payload ) ;

  my_tim_init () ;
  my_ant_sw_pos ( 2 ) ;
  if ( !my_acc_init () )
  {
	  my_acc_problem_flag = true ;
	  send_debug_logs ( "ACC init problem" ) ;
  }
  if ( !my_acc_start () )
  {
	  my_acc_problem_flag = true ;
	  send_debug_logs ( "ACC start problem" ) ;
  }
  //my_acc_stop () ;

  if ( !my_astro_init () )
	  my_sys_restart () ;
  else
  {
	  while ( my_astro_evt_pin () )
	  {
		  sprintf ( dbg_payload , "%s,%d,my_astro_evt_pin" , __FILE__ , __LINE__ ) ;
		  send_debug_logs ( dbg_payload ) ;
		  my_astro_handle_evt () ;
	  }
  }

  // my_gnss_verbose ( 15 ) ;

  my_gnss_sw_on () ;
  my_gnss_3dfix_flag = my_gnss_acq_coordinates ( &fix3d ) ;
  my_gnss_sw_off () ;
  my_rtc_get_dt_s ( rtc_dt_s ) ;
  sprintf ( dbg_payload , "%s,%d,%s,fix_mode=%c,pdop=%.1f,acq_time=%u,acq_total_time=%lu" , __FILE__ , __LINE__ , rtc_dt_s , fix3d.fix_mode , fix3d.pdop , fix3d.acq_time , (uint32_t) ( fix3d.acq_total_time / 60 ) ) ;
  send_debug_logs ( dbg_payload ) ;
  if ( !my_gnss_3dfix_flag )
  {
	  if ( my_rtc_set_alarm ( my_rtc_alarmA_time ) )
	  {
		  sprintf ( dbg_payload , "%s,%d,my_sys_sleep (), my_rtc_alarmA [s] = %lu" , __FILE__ , __LINE__ , my_rtc_alarmA_time ) ;
		  my_sys_sleep ( dbg_payload ) ;
	  }
  }
  else
  {
	  sprintf ( my_astro_payload , "%u,%.1f,%u,%lu,%s" , uplink_id , fix3d.pdop , fix3d.acq_time , (uint32_t) ( fix3d.acq_total_time / 60 ) , fv ) ;
	  sprintf ( dbg_payload , "%s,%d,payload: %s" , __FILE__ , __LINE__ , my_astro_payload ) ; // Żeby astro_payload_id był taki jak wysłany, bo po wysłaniu będzie zwiększony
	  my_astro_send_uplink ( my_astro_payload , dbg_payload ) ;
	  if ( my_rtc_set_alarm ( my_rtc_alarmA_time ) )
	  {
		  sprintf ( dbg_payload , "%s,%d,my_sys_sleep (), my_rtc_alarmA [s] = %lu" , __FILE__ , __LINE__ , my_rtc_alarmA_time ) ;
		  my_sys_sleep ( dbg_payload ) ;
	  }
  }



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  while ( my_astro_evt_pin () )
	  {
		  sprintf ( dbg_payload , "%s,%d,my_astro_evt_pin" , __FILE__ , __LINE__ ) ;
		  send_debug_logs ( dbg_payload ) ;
		  my_astro_handle_evt () ;
	  }
	  if ( astro_rcv_cmd_flag )
	  {
		  astro_rcv_cmd_flag = false ;
		  my_tracker_handle_cmd () ;
		  my_astro_rcv_cmd[0] = 0 ;
	  }
	  if ( my_rtc_alarm_flag )
	  {
		  my_rtc_alarm_flag = false ;
		  my_gnss_sw_on () ;
		  my_gnss_3dfix_flag = my_gnss_acq_coordinates ( &fix3d ) ;
		  my_gnss_sw_off () ;
		  my_rtc_get_dt_s ( rtc_dt_s ) ;
		  sprintf ( dbg_payload , "%s,%d,%s,fix_mode=%c,pdop=%.1f,acq_time=%u,acq_total_time=%lu" , __FILE__ , __LINE__ , rtc_dt_s , fix3d.fix_mode , fix3d.pdop , fix3d.acq_time , (uint32_t) ( fix3d.acq_total_time / 60 ) ) ;
		  send_debug_logs ( dbg_payload ) ;
		  if ( my_gnss_3dfix_flag )
		  {
			  my_astro_write_coordinates ( fix3d.latitude_astro_geo_wr , fix3d.longitude_astro_geo_wr ) ;
			  sprintf ( my_astro_payload , "%u,%.1f,%u,%lu,%ld,%ld" , uplink_id , fix3d.pdop , fix3d.acq_time , (uint32_t) ( fix3d.acq_total_time / 60 ) , fix3d.latitude_astro_geo_wr , fix3d.longitude_astro_geo_wr ) ;
			  my_astro_add_payload_2_queue ( uplink_id++ , my_astro_payload ) ;
			  my_astro_turn_payload_id_counter () ;
			  sprintf ( dbg_payload , "%s,%d,payload: %s" , __FILE__ , __LINE__ , my_astro_payload ) ;
			  send_debug_logs ( dbg_payload ) ;
		  }
	  }
	  if ( my_rtc_set_alarm ( my_rtc_alarmA_time ) )
	  {
		  sprintf ( dbg_payload , "%s,%d,my_sys_sleep (), my_rtc_alarmA [s] = %lu" , __FILE__ , __LINE__ , my_rtc_alarmA_time ) ;
		  my_sys_sleep ( dbg_payload ) ;
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.SubSeconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_SATURDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 16000-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 1000-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART5_UART_Init(void)
{

  /* USER CODE BEGIN USART5_Init 0 */

  /* USER CODE END USART5_Init 0 */

  /* USER CODE BEGIN USART5_Init 1 */

  /* USER CODE END USART5_Init 1 */
  huart5.Instance = USART5;
  huart5.Init.BaudRate = 9600;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart5.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART5_Init 2 */

  /* USER CODE END USART5_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ACC_SPI1_CS_GPIO_Port, ACC_SPI1_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ASTRO_WKUP_Pin|ASTRO_RST_Pin|RF_SW_CTL2_Pin|GNSS_RST_Pin
                          |GNSS_PWR_SW_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RF_SW_CTL1_GPIO_Port, RF_SW_CTL1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LDG_Pin|LDB_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : ACC_INT1_IT0_Pin */
  GPIO_InitStruct.Pin = ACC_INT1_IT0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ACC_INT1_IT0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ACC_INT2_Pin */
  GPIO_InitStruct.Pin = ACC_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ACC_INT2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ACC_SPI1_CS_Pin */
  GPIO_InitStruct.Pin = ACC_SPI1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ACC_SPI1_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ASTRO_WKUP_Pin ASTRO_RST_Pin RF_SW_CTL2_Pin RF_SW_CTL1_Pin
                           GNSS_PWR_SW_Pin */
  GPIO_InitStruct.Pin = ASTRO_WKUP_Pin|ASTRO_RST_Pin|RF_SW_CTL2_Pin|RF_SW_CTL1_Pin
                          |GNSS_PWR_SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : ASTRO_EVT_Pin GNSS_3DFIX_IT5_Pin */
  GPIO_InitStruct.Pin = ASTRO_EVT_Pin|GNSS_3DFIX_IT5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : GNSS_RST_Pin */
  GPIO_InitStruct.Pin = GNSS_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GNSS_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ASTRO_ANT_USE_Pin GNSS_JAM_Pin */
  GPIO_InitStruct.Pin = ASTRO_ANT_USE_Pin|GNSS_JAM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LDG_Pin LDB_Pin */
  GPIO_InitStruct.Pin = LDG_Pin|LDB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : SW1_Pin SW2_Pin */
  GPIO_InitStruct.Pin = SW1_Pin|SW2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// *** HARDWARE OPERATIONS

// ** SYSTEM OPERATION
void send_debug_logs ( const char* p_tx_buffer )
{
    uint32_t length = strlen ( p_tx_buffer ) ;

    if ( length > UART_TX_MAX_BUFF_SIZE )
    {
        HAL_UART_Transmit ( &HUART_DBG , ( uint8_t* ) "[ERROR] UART buffer reached max length.\n" , 42 , 1000 ) ;
        length = UART_TX_MAX_BUFF_SIZE;
    }

    HAL_UART_Transmit ( &HUART_DBG , ( uint8_t* ) p_tx_buffer , length , 1000 ) ;
    HAL_UART_Transmit ( &HUART_DBG , ( uint8_t* ) "\n" , 1 , 1000 ) ;
}
// SYS functions
void my_sys_init ( void )
{
	fix3d.acq_time = 0 ;
	fix3d.acq_total_time = 0 ;
	sw1 = HAL_GPIO_ReadPin ( SW1_GPIO_Port , SW1_Pin ) ;
	sw2 = HAL_GPIO_ReadPin ( SW2_GPIO_Port , SW2_Pin ) ;
	if ( !sw1 && !sw2 )
	{
		sys_mode = 0 ;
		my_rtc_alarmA_time = TIME_THS_1_H ;
		fix_acq_ths = TIME_THS_2_MIN ;
		min_tns_time_ths = TIME_THS_30_SEC ;
		pdop_ths = PDOP_THS ;
	}
	if ( sw1 && !sw2 )
	{
		sys_mode = 1 ;
		my_rtc_alarmA_time = TIME_THS_1_H ;
		fix_acq_ths = TIME_THS_2_MIN ;
		min_tns_time_ths = TIME_THS_30_SEC ;
		pdop_ths = PDOP_THS ;
	}
	if ( !sw1 && sw2 )
	{
		sys_mode = 2 ;
		my_rtc_alarmA_time = TIME_THS_5_MIN ;
		fix_acq_ths = TIME_THS_10_MIN ;
		min_tns_time_ths = TIME_THS_1_MIN ;
		pdop_ths = 10 ;
	}
	if ( sw1 && sw2 )
	{
		sys_mode = 3 ;
	}
}

bool is_system_initialized ( void )
{
	// Nie próbuj robić nic z Astronode, bo nie wiesz czy nie trzeba go zainicjować restartem. Ogranicz się do samego systemu.
	uint16_t yyyy = my_rtc_get_dt_s ( rtc_dt_s ) ;
	send_debug_logs ( rtc_dt_s ) ;
	return ( yyyy >= FIRMWARE_RELEASE_YEAR ) ? true : false ;
}

void my_sys_restart ( void )
{
	my_rtc_get_dt_s ( rtc_dt_s ) ;
	sprintf ( dbg_payload , "%s,%d,%s,HAL_NVIC_SystemReset" , __FILE__ , __LINE__ , rtc_dt_s ) ;
	send_debug_logs ( dbg_payload ) ;
	HAL_NVIC_SystemReset () ;
}

void my_sys_standby ( void )
{
	sprintf ( dbg_payload , "%s,%d,HAL_PWR_EnterSTANDBYMode" , __FILE__ , __LINE__ ) ;
	send_debug_logs ( dbg_payload ) ;
	my_tim_stop () ;
	my_rtc_alarm_flag = false ;
	HAL_PWR_EnterSTANDBYMode () ;
	my_rtc_get_dt_s ( rtc_dt_s ) ;
	sprintf ( dbg_payload , "%s,%d,%s, Wake-up after Standby" , __FILE__ , __LINE__ , rtc_dt_s ) ;
	send_debug_logs ( dbg_payload ) ;
}

void my_sys_sleep ( char* m )
{
	send_debug_logs ( m ) ;
	my_tim_stop () ;
	HAL_SuspendTick () ;
	my_rtc_alarm_flag = false ;
	HAL_PWR_EnterSTOPMode ( PWR_LOWPOWERREGULATOR_ON , PWR_STOPENTRY_WFE ) ;
	HAL_ResumeTick () ;
	my_rtc_get_dt_s ( rtc_dt_s ) ;
	sprintf ( m , "%s,%d,%s,Wake-up" , __FILE__ , __LINE__ , rtc_dt_s ) ;
	send_debug_logs ( m ) ;
}

void my_sys_deepsleep ( char* m )
{
	send_debug_logs ( m ) ;
	my_tim_stop () ;
	my_acc_stop () ;
	HAL_SuspendTick () ;
	my_rtc_alarm_flag = false ;
	HAL_PWR_EnterSTOPMode ( PWR_LOWPOWERREGULATOR_ON , PWR_STOPENTRY_WFE ) ;
	HAL_ResumeTick () ;
	my_acc_start () ;
	my_rtc_get_dt_s ( rtc_dt_s ) ;
	sprintf ( m , "%s,%d,%s,Wake-up" , __FILE__ , __LINE__ , rtc_dt_s ) ;
	send_debug_logs ( m ) ;
}

void my_sys_change_watchdog_time_ths ( uint32_t t )
{
	if ( t >= TIME_THS_5_MIN && t <= TIME_THS_1_H )
	{
		sys_watchdog_time_ths = t ;
		my_astro_cmd.is_executed = true ;
	}
}
void my_sys_change_AlarmA_time ( uint32_t t )
{
	if ( t >= TIME_THS_5_MIN && t <= TIME_THS_100_D )
	{
		my_rtc_alarmA_time = t ;
		my_astro_cmd.is_executed = true ;
	}
}
void my_sys_change_fix_acq_ths ( uint32_t t )
{
	if ( t >= TIME_THS_45_SEC && t <= TIME_THS_10_MIN )
	{
		fix_acq_ths = t ;
		my_astro_cmd.is_executed = true ;
	}
}
void my_sys_change_min_tns_time_ths ( uint32_t t )
{
	if ( t >= TIME_THS_15_SEC && t <= TIME_THS_10_MIN )
	{
		min_tns_time_ths = t ;
		my_astro_cmd.is_executed = true ;
	}
}
void my_sys_change_pdop_ths ( uint32_t p )
{
	if ( p >= 0.01 && p <= 100 )
	{
		pdop_ths = p ;
		my_astro_cmd.is_executed = true ;
	}
}
bool my_tracker_handle_cmd ( void )
{
	if ( my_tracker_api_is_cmd ( my_astro_rcv_cmd ) )
	{
		if ( my_tracker_api_parse_cmd ( &my_astro_cmd , my_astro_rcv_cmd ) )
		{
			switch ( my_astro_cmd.code )
			{
			  case 1:
				  my_sys_change_watchdog_time_ths ( my_astro_cmd.value ) ;
				  sprintf ( my_astro_payload , "%u,%u,%u,%lu" , uplink_id , (uint16_t) my_astro_cmd.is_executed , my_astro_cmd.code , my_astro_cmd.value ) ;
				  sprintf ( dbg_payload , "%s,%d,payload: %s" , __FILE__ , __LINE__ , my_astro_payload ) ; // Żeby astro_payload_id był taki jak wysłany, bo po wysłaniu będzie zwiększony
				  send_debug_logs ( dbg_payload ) ;
				  my_astro_add_payload_2_queue ( uplink_id++ , my_astro_payload ) ;
				  my_astro_turn_payload_id_counter () ;
				  break ;
			  case 2:
				  my_sys_change_AlarmA_time ( my_astro_cmd.value ) ;
				  sprintf ( my_astro_payload , "%u,%u,%u,%lu" , uplink_id , (uint16_t) my_astro_cmd.is_executed , my_astro_cmd.code , my_astro_cmd.value ) ;
				  sprintf ( dbg_payload , "%s,%d,payload: %s" , __FILE__ , __LINE__ , my_astro_payload ) ; // Żeby astro_payload_id był taki jak wysłany, bo po wysłaniu będzie zwiększony
				  send_debug_logs ( dbg_payload ) ;
				  my_astro_add_payload_2_queue ( uplink_id++ , my_astro_payload ) ;
				  my_astro_turn_payload_id_counter () ;
				  break ;
			  case 3:
				  my_sys_change_fix_acq_ths ( my_astro_cmd.value ) ;
				  sprintf ( my_astro_payload , "%u,%u,%u,%lu" , uplink_id , (uint16_t) my_astro_cmd.is_executed , my_astro_cmd.code , my_astro_cmd.value ) ;
				  sprintf ( dbg_payload , "%s,%d,payload: %s" , __FILE__ , __LINE__ , my_astro_payload ) ; // Żeby astro_payload_id był taki jak wysłany, bo po wysłaniu będzie zwiększony
				  send_debug_logs ( dbg_payload ) ;
				  my_astro_add_payload_2_queue ( uplink_id++ , my_astro_payload ) ;
				  my_astro_turn_payload_id_counter () ;
				  break ;
			  case 4:
				  my_sys_change_min_tns_time_ths ( my_astro_cmd.value ) ;
				  sprintf ( my_astro_payload , "%u,%u,%u,%lu" , uplink_id , (uint16_t) my_astro_cmd.is_executed , my_astro_cmd.code , my_astro_cmd.value ) ;
				  sprintf ( dbg_payload , "%s,%d,payload: %s" , __FILE__ , __LINE__ , my_astro_payload ) ; // Żeby astro_payload_id był taki jak wysłany, bo po wysłaniu będzie zwiększony
				  send_debug_logs ( dbg_payload ) ;
				  my_astro_add_payload_2_queue ( uplink_id++ , my_astro_payload ) ;
				  my_astro_turn_payload_id_counter () ;
				  break ;
			  case 5:
				  my_sys_change_pdop_ths ( my_astro_cmd.value ) ;
				  sprintf ( my_astro_payload , "%u,%u,%u,%lu" , uplink_id , (uint16_t) my_astro_cmd.is_executed , my_astro_cmd.code , my_astro_cmd.value ) ;
				  sprintf ( dbg_payload , "%s,%d,payload: %s" , __FILE__ , __LINE__ , my_astro_payload ) ; // Żeby astro_payload_id był taki jak wysłany, bo po wysłaniu będzie zwiększony
				  send_debug_logs ( dbg_payload ) ;
				  my_astro_add_payload_2_queue ( uplink_id++ , my_astro_payload ) ;
				  my_astro_turn_payload_id_counter () ;
				  break ;
			  case 8:
				  if ( my_astro_cmd.value == (uint32_t) GET_SYS_CFG )
				  {
					  sprintf ( my_astro_payload , "%u,%s,%u,%u,%lu,%u,%u,%.1f" , uplink_id , fv , (uint16_t) sys_mode , sys_watchdog_time_ths , my_rtc_alarmA_time , fix_acq_ths , min_tns_time_ths , pdop_ths ) ;
					  sprintf ( dbg_payload , "%s,%d,payload: %s" , __FILE__ , __LINE__ , my_astro_payload ) ; // Żeby astro_payload_id był taki jak wysłany, bo po wysłaniu będzie zwiększony
					  send_debug_logs ( dbg_payload ) ;
					  my_astro_add_payload_2_queue ( uplink_id++ , my_astro_payload ) ;
					  my_astro_turn_payload_id_counter () ;
				  }
				  if ( my_astro_cmd.value == (uint32_t) GET_SYS_COURSE )
				  {
					  iis2dh_temperature_raw_get ( &my_acc_ctx , &my_acc_temp ) ;
					  sprintf ( my_astro_payload , "%u,%lu,%d" , uplink_id , (uint32_t) ( fix3d.acq_total_time / 60 ) , my_acc_temp ) ;
					  sprintf ( dbg_payload , "%s,%d,payload: %s" , __FILE__ , __LINE__ , my_astro_payload ) ; // Żeby astro_payload_id był taki jak wysłany, bo po wysłaniu będzie zwiększony
					  send_debug_logs ( dbg_payload ) ;
					  my_astro_add_payload_2_queue ( uplink_id++ , my_astro_payload ) ;
					  my_astro_turn_payload_id_counter () ;
				  }
				  break ;
			  case 9:
				  // Tutaj wyjątkowo nie musi być uplink confimration, bo kolejny pakiet będzie miał id = 0, a logi będą wysłane w funkcji
				  if ( my_astro_cmd.value == (uint32_t) RESET_SYS_CMD_VALUE )
					  my_sys_restart () ;
				  break ;
			  default:
				  sprintf ( my_astro_payload , "%u,%u,%u,%lu" , uplink_id , (uint16_t) my_astro_cmd.is_executed , my_astro_cmd.code , my_astro_cmd.value ) ;
				  sprintf ( dbg_payload , "%s,%d,payload: %s" , __FILE__ , __LINE__ , my_astro_payload ) ; // Żeby astro_payload_id był taki jak wysłany, bo po wysłaniu będzie zwiększony
				  send_debug_logs ( dbg_payload ) ;
				  my_astro_add_payload_2_queue ( uplink_id++ , my_astro_payload ) ;
				  my_astro_turn_payload_id_counter () ;
				  return false ;
			}
		}
	}
	return true ;
}
// ** LED OPERATION
void my_ldg_sw_on ( void )
{
	HAL_GPIO_WritePin ( LDG_GPIO_Port , LDG_Pin , GPIO_PIN_SET ) ;
}
void my_ldb_sw_on ( void )
{
	HAL_GPIO_WritePin ( LDB_GPIO_Port , LDB_Pin , GPIO_PIN_SET ) ;
}
void my_ldg_sw_off ( void )
{
	HAL_GPIO_WritePin ( LDG_GPIO_Port , LDG_Pin , GPIO_PIN_RESET ) ;
}
void my_ldb_sw_off ( void )
{
	HAL_GPIO_WritePin ( LDB_GPIO_Port , LDB_Pin , GPIO_PIN_RESET ) ;
}
void my_ldg_blink ( uint8_t i , uint16_t d )
{
	while ( i )
	{
		HAL_GPIO_TogglePin ( LDG_GPIO_Port , LDG_Pin ) ;
		HAL_Delay ( d ) ;
		HAL_GPIO_TogglePin ( LDG_GPIO_Port , LDG_Pin ) ;
		HAL_Delay ( d ) ;
		i-- ;
	}
}
void my_ldb_blink ( uint8_t i , uint16_t d )
{
	while ( i )
	{
		HAL_GPIO_TogglePin ( LDB_GPIO_Port , LDB_Pin ) ;
		HAL_Delay ( d ) ;
		HAL_GPIO_TogglePin ( LDB_GPIO_Port , LDB_Pin ) ;
		HAL_Delay ( d ) ;
		i-- ;
	}
}

// ** ANT SW Operations
void my_ant_sw_pos ( uint8_t pos )
{
	if ( pos == 1 ) // Włączenie GNSS czyli ustawienie RF_SW_CTL1 = LOW i RF_SW_CTL2 = HIGH
	{
		HAL_GPIO_WritePin ( RF_SW_CTL1_GPIO_Port , RF_SW_CTL1_Pin , GPIO_PIN_RESET ) ;
		HAL_GPIO_WritePin ( RF_SW_CTL2_GPIO_Port , RF_SW_CTL2_Pin , GPIO_PIN_SET ) ;
	}
	else if ( pos == 2 )
	{
		HAL_GPIO_WritePin ( RF_SW_CTL1_GPIO_Port , RF_SW_CTL1_Pin , GPIO_PIN_SET ) ;
		HAL_GPIO_WritePin ( RF_SW_CTL2_GPIO_Port , RF_SW_CTL2_Pin , GPIO_PIN_RESET ) ;
	}
}


// ** GNSS Operations
void my_gnss_sw_on ( void )
{
	my_ant_sw_pos ( 1 ) ;
	HAL_GPIO_WritePin ( GNSS_PWR_SW_GPIO_Port , GNSS_PWR_SW_Pin , GPIO_PIN_SET ) ;
	HAL_GPIO_WritePin ( GNSS_PWR_SW_GPIO_Port , GNSS_RST_Pin , GPIO_PIN_SET ) ;
	MX_USART5_UART_Init () ;
}
void my_gnss_sw_off ( void )
{
	my_ant_sw_pos ( 2 ) ;
	if ( sys_mode < 2 )
	{
		HAL_GPIO_WritePin ( GNSS_PWR_SW_GPIO_Port , GNSS_PWR_SW_Pin , GPIO_PIN_RESET ) ;
		HAL_GPIO_WritePin ( GNSS_PWR_SW_GPIO_Port , GNSS_RST_Pin , GPIO_PIN_RESET ) ;
		HAL_UART_DeInit ( &HUART_GNSS ) ;
	}
}
void my_gnss_receive_byte ( uint8_t* rx_byte , bool verbose )
{
	HAL_UART_Receive ( &HUART_GNSS , rx_byte , 1 , UART_TIMEOUT ) ;
	if ( sys_mode > 0 )
		HAL_UART_Transmit ( &HUART_DBG , rx_byte , 1 , UART_TIMEOUT ) ;
}
bool is_fix3d ()
{
	GPIO_PinState r = HAL_GPIO_ReadPin ( GNSS_3DFIX_IT5_GPIO_Port , GNSS_3DFIX_IT5_Pin ) ;
	if ( r == GPIO_PIN_SET )
	{
		send_debug_logs( "3D Fix" ) ;
		return true ;
	}
	return false ;
}
void my_gnss_verbose ( uint16_t time_seconds_ths )
{
	  my_gnss_sw_on () ;
	  my_tim_start () ;
	  my_gnss_log ( time_seconds_ths ) ;
	  my_tim_stop () ;
	  my_gnss_sw_off () ;
}

// ACC
bool my_acc_init ( void )
{
	uint8_t id = 0 ;

	my_acc_ctx.write_reg = my_st_acc_platform_write ;
	my_acc_ctx.read_reg = my_st_acc_platform_read ;
	my_acc_ctx.handle = &hspi1 ;

	iis2dh_device_id_get ( &my_acc_ctx , &id ) ;
	if ( id != IIS2DH_ID )
		return false ;

	return true ;
}

bool my_acc_start ( void )
{
	uint8_t id = 0 ;

	iis2dh_device_id_get ( &my_acc_ctx , &id ) ;
	if ( id != IIS2DH_ID )
		return false ;

	//  Configuration: 2g, LP and 25Hz gives 4 uA of ACC power consumption
	iis2dh_full_scale_set ( &my_acc_ctx , IIS2DH_2g ) ; // FS bits [ 2 g - 16 g ]
	iis2dh_operating_mode_set ( &my_acc_ctx , IIS2DH_LP_8bit ) ; // [ High Resolution , Normal Mode , Low Power]
	iis2dh_data_rate_set ( &my_acc_ctx , IIS2DH_ODR_10Hz ) ; // Below 25Hz it will be hard to calculate free-fall
	iis2dh_fifo_mode_set ( &my_acc_ctx , IIS2DH_FIFO_MODE ) ; // FIFO mode allows consistent power saving for the system, since the host processor does not need to	continuously poll data from the sensor, but it can wake up only when needed and burst the significant data out from the FIFO.

	// Temperature sensor enable.
	// iis2dh_temperature_meas_set( &my_acc_ctx , IIS2DH_TEMP_ENABLE ) ;
	// To retrieve the temperature sensor data the BDU bit in CTRL_REG4 (23h) must be set to 1.
	// iis2dh_block_data_update_set ( &my_acc_ctx , PROPERTY_ENABLE ) ;

	// Interrupt request on INT1_SRC (31h) and INT2_SRC (35h) latched. Register cleared by reading INTx_SRC itself.
	iis2dh_int1_pin_notification_mode_set ( &my_acc_ctx , IIS2DH_INT2_LATCHED ) ;
	iis2dh_int2_pin_notification_mode_set ( &my_acc_ctx , IIS2DH_INT2_LATCHED ) ;

	// The IIS2DH may also be configured to generate an inertial wake-up and free-fall interrupt signal according to a programmed acceleration event along the enabled axes. Both free-fall and wake-up can be available simultaneously on two different pins.


	return true ;
}

bool my_acc_stop ( void )
{
	iis2dh_data_rate_set ( &my_acc_ctx , IIS2DH_POWER_DOWN ) ; // Below 25Hz it will be hard to calculate free-fall

	// Temperature sensor disable.
	iis2dh_temperature_meas_set( &my_acc_ctx , IIS2DH_TEMP_DISABLE ) ;
	// To retrieve the temperature sensor data the BDU bit in CTRL_REG4 (23h) must be set to 1.
	iis2dh_block_data_update_set ( &my_acc_ctx , PROPERTY_DISABLE ) ;

	// The IIS2DH may also be configured to generate an inertial wake-up and free-fall interrupt signal according to a programmed acceleration event along the enabled axes. Both free-fall and wake-up can be available simultaneously on two different pins.

	return true ;
}

/* ************************************* */
// ASTRO Operations
void my_astronode_reset ( void )
{
    HAL_GPIO_WritePin ( ASTRO_RST_GPIO_Port , ASTRO_RST_Pin , GPIO_PIN_SET ) ;
    HAL_Delay ( 1 ) ;
    HAL_GPIO_WritePin ( ASTRO_RST_GPIO_Port , ASTRO_RST_Pin , GPIO_PIN_RESET ) ;
    HAL_Delay ( 250 ) ;
}
void send_astronode_request ( uint8_t* p_tx_buffer , uint32_t length )
{
    send_debug_logs ( "Message sent to the Astronode --> " ) ;
    send_debug_logs ( ( char* ) p_tx_buffer ) ;
    HAL_UART_Transmit ( &HUART_ASTRO , p_tx_buffer , length , 1000 ) ;
}
uint32_t get_systick ( void )
{
    return HAL_GetTick() ;
}
bool is_systick_timeout_over ( uint32_t starting_value , uint16_t duration )
{
    return ( get_systick () - starting_value > duration ) ? true : false ;
}
bool is_astronode_character_received ( uint8_t* p_rx_char )
{
    return ( HAL_UART_Receive ( &HUART_ASTRO , p_rx_char , 1 , 100 ) == HAL_OK ? true : false ) ;
}
bool my_astro_evt_pin ()
{
	return ( HAL_GPIO_ReadPin ( ASTRO_EVT_GPIO_Port , ASTRO_EVT_Pin ) == GPIO_PIN_SET ? true : false);
}
void my_astro_turn_payload_id_counter ( void )
{
	if ( uplink_id > 99 )
		uplink_id = 1 ;
}
void my_astro_send_uplink ( char* p , char* m )
{
	if ( uplink_id == 0 && sys_mode != 0 ) // Present sys_mode in welcome uplink message if mode is other than production
		sprintf ( p , "%s,%u" , p , (uint16_t) sys_mode ) ;
	my_astro_add_payload_2_queue ( uplink_id++ , p ) ;
	my_astro_turn_payload_id_counter () ;
	last_uplink_send_ts = my_rtc_get_ts () ;
	send_debug_logs ( m ) ;
}

/* ************************************* */
// ACC LL Function

int32_t my_st_acc_platform_write ( void* handle , uint8_t reg , const uint8_t* bufp , uint16_t len )
{
	HAL_GPIO_WritePin	( ACC_SPI1_CS_GPIO_Port , ACC_SPI1_CS_Pin , GPIO_PIN_RESET ) ;
	HAL_Delay ( 20 ) ;
	HAL_SPI_Transmit	( handle , &reg , 1 , 1000 ) ;
	HAL_SPI_Transmit	( handle , (uint8_t*) bufp , len , 1000 ) ;
	HAL_GPIO_WritePin	( ACC_SPI1_CS_GPIO_Port , ACC_SPI1_CS_Pin , GPIO_PIN_SET) ;

	return 0 ;
}

int32_t my_st_acc_platform_read ( void* handle , uint8_t reg , uint8_t* bufp , uint16_t len )
{
	reg |= 0x80;
	HAL_GPIO_WritePin ( ACC_SPI1_CS_GPIO_Port , ACC_SPI1_CS_Pin , GPIO_PIN_RESET) ;
	HAL_Delay ( 20 ) ;
	HAL_SPI_Transmit ( handle , &reg , 1 , 1000 ) ;
	HAL_SPI_Receive ( handle , bufp , len , 1000 ) ;
	HAL_GPIO_WritePin ( ACC_SPI1_CS_GPIO_Port , ACC_SPI1_CS_Pin , GPIO_PIN_SET) ;

	return 0 ;
}

/* ************************************* */
// TIM operations
void my_tim_init ()
{
	__HAL_TIM_CLEAR_IT ( &TIM , TIM_IT_UPDATE ) ;
}

void my_tim_start ()
{
	tim_seconds = 0 ;
	HAL_TIM_Base_Start_IT ( &TIM ) ;
}

void my_tim_stop ()
{
	HAL_TIM_Base_Stop_IT ( &TIM ) ;
}


// *** CALBACKS

// TIM Callback
void HAL_TIM_PeriodElapsedCallback ( TIM_HandleTypeDef *htim )
{
	if ( htim->Instance == TIM6 )
	{
		tim_seconds++ ;
		if ( tim_seconds > sys_watchdog_time_ths )
			my_sys_restart () ;
	}
}

// RTC Callbacks
void HAL_RTC_AlarmAEventCallback ( RTC_HandleTypeDef* hrtc )
{
	// is_rtc_alarm_a_flag = true ;
	//__HAL_RTC_ALARM_CLEAR_FLAG ( hrtc , RTC_FLAG_ALRAF ) ;  // Wyczyść flagę alarmu
	my_rtc_alarm_flag = true ;
}

// ASTRO EVT Callbacks
void HAL_GPIO_EXTI_Rising_Callback ( uint16_t GPIO_Pin )
{

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
