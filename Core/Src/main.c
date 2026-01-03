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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

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

/* USER CODE BEGIN PV */
bool alarm_flag = 0;
bool Double_pid = 0, Position_pid = 0, Velocity_pid = 0;

uint8_t task[5];
uint8_t task4_cnt = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Peripheral_Init(void)
{
    LED_OFF;
    Buzzer_OFF;
    
    MotorStart;
    
    OLED_Init();
    
    HAL_TIM_Base_Start_IT(&htim6);                  // 10ms -- µç»úPID
    HAL_TIM_Base_Start_IT(&htim7);                  // 10ms
    
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);       // PWMA
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);       // PWMB
    
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1);   // A_A
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_2);   // A_B
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1);   // B_A
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_2);   // B_B
}
void alarm_proc(void)
{
    if (alarm_flag)
    {
        LED_ON;
        Buzzer_ON;
    }
    else
    {
        LED_OFF;
        Buzzer_OFF;
    }
}
void key_proc(void)
{
    if (key[1].short_flag)
    {
        task[1] = 1;
        key[1].short_flag = 0;
    }
    else if (key[2].short_flag)
    {
        task[2] = 1;
        key[2].short_flag = 0;
    }
    else if (key[3].short_flag)
    {
        task[3] = 1;
        key[3].short_flag = 0;
    }
    else if (key[4].short_flag)
    {
        task[4] = 1;
        key[4].short_flag = 0;
    }
}
void function_proc(void)
{
    switch (task[1])
    {
        case 1:
        {
            Velocity_pid = 1;
            Target_VelocityR = Rpm_Encoder_Cnt(80);
            Target_VelocityL = Rpm_Encoder_Cnt(80);
            if (L2||L1||M0||R1||R2)
                task[1] = 2;
        }break;
        case 2:
        {
            Velocity_pid = 0;
            MotorStop;
            alarm_flag = 1;
            task[1] = 0;
        }break;
    }
    switch (task[2])
    {
        case 1:
        {
            Velocity_pid = 1;
            Target_VelocityR = Rpm_Encoder_Cnt(80);
            Target_VelocityL = Rpm_Encoder_Cnt(80);
            if (L2||L1||M0||R1||R2)
                task[2] = 2;
        }break;
        case 2:
        {
            MotorStop;
            alarm_flag = 1;
            Velocity_pid = 0;
            task[2] = 3;
        }break;
        case 3:
        {
            MotorStart;
            Double_pid = 1;
            Target_VelocityR = Rpm_Encoder_Cnt(67.6);
            Target_VelocityL = Rpm_Encoder_Cnt(91.4);
            Target_PositionR = Num_Encoder_Cnt(5.2);
            Target_PositionL = Num_Encoder_Cnt(7.0);
            if (Reality_PositionR >= 291000)
                task[2] = 4;
        }break;
        case 4:
        {
            Double_pid = 0;
            MotorStop;
            task[2] = 5;
        }break;
        case 5:
        {
            alarm_flag = 1;
            task[2] = 6;
        }break;
        case 6:
        {
            MotorStart;
            Velocity_pid = 1;
            Target_VelocityR = Rpm_Encoder_Cnt(80);
            Target_VelocityL = Rpm_Encoder_Cnt(80);
            if (L2||L1||M0||R1||R2)
                task[2] = 7;
        }break;
        case 7:
        {
            Motor_Stop();
            alarm_flag = 1;
            Velocity_pid = 0;
            task[2] = 8;
        }break;
        case 8:
        {
            MotorStart;
            Double_pid = 1;
            Target_VelocityR = Rpm_Encoder_Cnt(67.6);
            Target_VelocityL = Rpm_Encoder_Cnt(91.4);
            Target_PositionR = Num_Encoder_Cnt(5.2);
            Target_PositionL = Num_Encoder_Cnt(7.0);
            if (Reality_PositionR >= 291000)
                task[2] = 9;
        }break;
        case 9:
        {
            Double_pid = 0;
            MotorStop;
            alarm_flag = 1;
            task[2] = 0;
        }break;
    }
    switch (task[3])
     {
        case 1:
        {
            LED_ON;
            Double_pid = 1;
            Target_PositionR = Num_Encoder_Cnt(28/20.41f);
            Target_PositionL = Num_Encoder_Cnt(28/20.41f);
            Target_VelocityR = Rpm_Encoder_Cnt(80);
            Target_VelocityL = Rpm_Encoder_Cnt(80);
            if (((Reality_PositionR + Reality_PositionL) / 2) >= (28/20.41f*56000))
            {
                Double_pid = 0;
                Motor_Stop();
                task[3] = 2;
            }
        }break;
        case 2:
        {
            LED_OFF;
            MotorStart;
            Double_pid = 1;
            Target_PositionR = Num_Encoder_Cnt(51.653f/20.41f);
            Target_PositionL = Num_Encoder_Cnt(73.947f/20.41f);
            Target_VelocityR = Rpm_Encoder_Cnt(65.8f);
            Target_VelocityL = Rpm_Encoder_Cnt(94.2f);
            if (Reality_PositionR >= (51.653f/20.41f*56000))
            {
                Double_pid = 0;
                Motor_Stop();
                task[3] = 3;
            }
        }break;
        case 3:
        {
            LED_ON;
            MotorStart;
            Double_pid = 1;
            Target_PositionR = Num_Encoder_Cnt(73.947f/20.41f);
            Target_PositionL = Num_Encoder_Cnt(51.653f/20.41f);
            Target_VelocityR = Rpm_Encoder_Cnt(94.2f);
            Target_VelocityL = Rpm_Encoder_Cnt(65.8f);
            if (Reality_PositionL >= (51.653f/20.41f*56000))
            {
                Double_pid = 0;
                Motor_Stop();
                task[3] = 4;
            }
        }break;
        case 4:
        {
            alarm_flag = 1;
            LED_OFF;
            MotorStart;
            Double_pid = 1;
            Target_VelocityR = Rpm_Encoder_Cnt(89.2f);
            Target_VelocityL = Rpm_Encoder_Cnt(65.8f);
            Target_PositionR = Num_Encoder_Cnt(140.894f/20.41f);
            Target_PositionL = Num_Encoder_Cnt(103.306f/20.41f);
            if (Reality_PositionR >= 384000 / 10)
                alarm_flag = 0;
            if ((Reality_PositionR >= 384000) && (Reality_PositionL >= 283000))
            {
                alarm_flag = 1;
                Double_pid = 0;
                Motor_Stop();
                task[3] = 5;
            }
        }break;
        case 5:
        {
            LED_ON;
            MotorStart;
            Double_pid = 1;
            Target_PositionR = Num_Encoder_Cnt(15/20.41f);
            Target_PositionL = Num_Encoder_Cnt(15/20.41f);
            Target_VelocityR = Rpm_Encoder_Cnt(80);
            Target_VelocityL = Rpm_Encoder_Cnt(80);
            if (((Reality_PositionR + Reality_PositionL) / 2) >= (15/20.41f*56000))
            {
                Double_pid = 0;
                OLED_ShowSignedNum(4, 1, Reality_PositionL, 10);
                Motor_Stop();
                task[3] = 6;
            }
        }break;
        case 6:
        {
            LED_OFF;
            MotorStart;
            Double_pid = 1;
            Target_PositionR = Num_Encoder_Cnt(69.947f/20.41f);
            Target_PositionL = Num_Encoder_Cnt(51.653f/20.41f);
            Target_VelocityR = Rpm_Encoder_Cnt(94.2f);
            Target_VelocityL = Rpm_Encoder_Cnt(65.8f);
            if (Reality_PositionL >= (51.653f/20.41f*56000))
            {
                Double_pid = 0;
                Motor_Stop();
                task[3] = 7;
            }
        }break;
        case 7:
        {
            LED_ON;
            MotorStart;
            Double_pid = 1;
            Target_PositionR = Num_Encoder_Cnt(53.653f/20.41f);
            Target_PositionL = Num_Encoder_Cnt(73.947f/20.41f);
            Target_VelocityR = Rpm_Encoder_Cnt(65.8f);
            Target_VelocityL = Rpm_Encoder_Cnt(94.2f);
            if (Reality_PositionR >= (53.653f/20.41f*56000))
            {
                alarm_flag = 1;
                Double_pid = 0;
                Motor_Stop();
                task[3] = 8;
            }
        }break;
        case 8:
        {
            MotorStart;
            Double_pid = 1;
            Target_VelocityR = Rpm_Encoder_Cnt(67.6);
            Target_VelocityL = Rpm_Encoder_Cnt(91.4);
            Target_PositionR = Num_Encoder_Cnt(5.2);
            Target_PositionL = Num_Encoder_Cnt(7.0);
            if (Reality_PositionR >= 291000)
                task[3] = 9;
        }break;
        case 9:
        {
            Double_pid = 0;
            MotorStop;
            alarm_flag = 1;
            task[3] = 0;
        }break;
    }
    switch (task[4])
    {
        case 1:
        {
            task4_cnt += 1;
            LED_ON;
            Double_pid = 1;
            Target_PositionR = Num_Encoder_Cnt(28/20.41f);
            Target_PositionL = Num_Encoder_Cnt(28/20.41f);
            Target_VelocityR = Rpm_Encoder_Cnt(80);
            Target_VelocityL = Rpm_Encoder_Cnt(80);
            if (((Reality_PositionR + Reality_PositionL) / 2) >= (28/20.41f*56000))
            {
                Double_pid = 0;
                Motor_Stop();
                task[4] = 2;
            }
        }break;
        case 2:
        {
            LED_OFF;
            MotorStart;
            Double_pid = 1;
            Target_PositionR = Num_Encoder_Cnt(51.653f/20.41f);
            Target_PositionL = Num_Encoder_Cnt(73.947f/20.41f);
            Target_VelocityR = Rpm_Encoder_Cnt(65.8f);
            Target_VelocityL = Rpm_Encoder_Cnt(94.2f);
            if (Reality_PositionR >= (51.653f/20.41f*56000))
            {
                Double_pid = 0;
                Motor_Stop();
                task[4] = 3;
            }
        }break;
        case 3:
        {
            LED_ON;
            MotorStart;
            Double_pid = 1;
            Target_PositionR = Num_Encoder_Cnt(73.947f/20.41f);
            Target_PositionL = Num_Encoder_Cnt(51.653f/20.41f);
            Target_VelocityR = Rpm_Encoder_Cnt(94.2f);
            Target_VelocityL = Rpm_Encoder_Cnt(65.8f);
            if (Reality_PositionL >= (51.653f/20.41f*56000))
            {
                Double_pid = 0;
                Motor_Stop();
                task[4] = 4;
            }
        }break;
        case 4:
        {
            alarm_flag = 1;
            LED_OFF;
            MotorStart;
            Double_pid = 1;
            Target_VelocityR = Rpm_Encoder_Cnt(89.2f);
            Target_VelocityL = Rpm_Encoder_Cnt(65.8f);
            Target_PositionR = Num_Encoder_Cnt(140.894f/20.41f);
            Target_PositionL = Num_Encoder_Cnt(103.306f/20.41f);
            if (Reality_PositionR >= 384000 / 10)
                alarm_flag = 0;
            if ((Reality_PositionR >= 384000) && (Reality_PositionL >= 283000))
            {
                alarm_flag = 1;
                Double_pid = 0;
                Motor_Stop();
                task[4] = 5;
            }
        }break;
        case 5:
        {
            LED_ON;
            MotorStart;
            Double_pid = 1;
            Target_PositionR = Num_Encoder_Cnt(15/20.41f);
            Target_PositionL = Num_Encoder_Cnt(15/20.41f);
            Target_VelocityR = Rpm_Encoder_Cnt(80);
            Target_VelocityL = Rpm_Encoder_Cnt(80);
            if (((Reality_PositionR + Reality_PositionL) / 2) >= (15/20.41f*56000))
            {
                Double_pid = 0;
                OLED_ShowSignedNum(4, 1, Reality_PositionL, 10);
                Motor_Stop();
                task[4] = 6;
            }
        }break;
        case 6:
        {
            LED_OFF;
            MotorStart;
            Double_pid = 1;
            Target_PositionR = Num_Encoder_Cnt(69.947f/20.41f);
            Target_PositionL = Num_Encoder_Cnt(51.653f/20.41f);
            Target_VelocityR = Rpm_Encoder_Cnt(94.2f);
            Target_VelocityL = Rpm_Encoder_Cnt(65.8f);
            if (Reality_PositionL >= (51.653f/20.41f*56000))
            {
                Double_pid = 0;
                Motor_Stop();
                task[4] = 7;
            }
        }break;
        case 7:
        {
            LED_ON;
            MotorStart;
            Double_pid = 1;
            Target_PositionR = Num_Encoder_Cnt(53.653f/20.41f);
            Target_PositionL = Num_Encoder_Cnt(73.947f/20.41f);
            Target_VelocityR = Rpm_Encoder_Cnt(65.8f);
            Target_VelocityL = Rpm_Encoder_Cnt(94.2f);
            if (Reality_PositionR >= (53.653f/20.41f*56000))
            {
                alarm_flag = 1;
                Double_pid = 0;
                Motor_Stop();
                task[4] = 8;
            }
        }break;
        case 8:
        {
            MotorStart;
            Double_pid = 1;
            Target_VelocityR = Rpm_Encoder_Cnt(67.6);
            Target_VelocityL = Rpm_Encoder_Cnt(91.4);
            Target_PositionR = Num_Encoder_Cnt(4.7);
            Target_PositionL = Num_Encoder_Cnt(6.5);
            if (Reality_PositionR >= 256000)
                task[4] = 9;
        }break;
        case 9:
        {
            Double_pid = 0;
            MotorStop;
            alarm_flag = 1;
            if (task4_cnt >= 1)
                task[4] = 0;
            else
                task[4] = 1;
        }break;
    }
}
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
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  Peripheral_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      key_proc();
      alarm_proc();
      function_proc();
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static int Motor_R = 0, Motor_L = 0; 
    static int alarm_cnt = 0;
    if (htim -> Instance == TIM6)
    {
        key_serv();
        
        if (alarm_flag)
        {
            alarm_cnt++;
            if (alarm_cnt >= 20)    // 500ms
            {
                alarm_flag = 0;
                alarm_cnt = 0;
            }
        }
    }
    if (htim -> Instance == TIM7)
    {
        if (Double_pid)
        {
            Reality_VelocityR = Read_Encoder(2);
            Reality_VelocityL = -Read_Encoder(3);
            Reality_PositionR += Reality_VelocityR;
            Reality_PositionL += Reality_VelocityL;
            
            Motor_R = Position_PID_R(Reality_PositionR, Target_PositionR);
            Motor_R = Xianfu(Motor_R, Target_VelocityR);
            Motor_L = Position_PID_L(Reality_PositionL, Target_PositionL);
            Motor_L = Xianfu(Motor_L, Target_VelocityL);
            
            if (Myabs(Reality_PositionR - Target_PositionR) < 3)
                SetPwm_R(0);
            else
            {
                Motor_R = Incremental_PID_R(Reality_VelocityR, Motor_R);
                SetPwm_R(Motor_R);
            }
            if (Myabs(Reality_PositionL - Target_PositionL) < 3)
                SetPwm_L(0);
            else
            {
                Motor_L = Incremental_PID_L(Reality_VelocityL, Motor_L);
                SetPwm_L(Motor_L);
            }
        }
        else if (Position_pid)
        {
            Reality_PositionR += Read_Encoder(2);
            Reality_PositionL += (-Read_Encoder(3));
            if (Target_PositionR == Reality_PositionR)
                SetPwm_R(0);
            else
            {
                Motor_R = Position_PID_R(Reality_PositionR, Target_PositionR);
                SetPwm_R(Motor_R);
            }
            if (Target_PositionL == Reality_PositionL)
                SetPwm_L(0);
            else
            {
                Motor_L = Position_PID_L(Reality_PositionL, Target_PositionL);
                SetPwm_L(Motor_L);
            }
        }
        else if (Velocity_pid)
        {
            Reality_VelocityR = Read_Encoder(2);
            Reality_VelocityL = -Read_Encoder(3);
            if ((Target_VelocityR == 0) && (Myabs(Reality_VelocityR) < 5))
                PWMA = 0;
            else 
            {
                Motor_R = Incremental_PID_R(Reality_VelocityR, Target_VelocityR);
                SetPwm_R(Motor_R);
            }
            if ((Target_VelocityL == 0) && (Myabs(Reality_VelocityL) < 5))
                PWMB = 0;
            else 
            {
                Motor_L = Incremental_PID_L(Reality_VelocityL, Target_VelocityL);
                SetPwm_L(Motor_L);
            }
        }
    }
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
