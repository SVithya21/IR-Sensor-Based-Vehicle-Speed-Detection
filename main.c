/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"
#include "liquidcrystal_i2c.h"
#include <stdio.h>
#include <string.h>

#define BUZZER_PIN GPIO_PIN_1
#define BUZZER_PORT GPIOC

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

uint32_t ir1_time = 0, ir2_time = 0, time_diff = 0;
uint8_t ir1_detected = 0;
uint16_t vehicle_count = 0;
uint16_t overspeed_count = 0;
char line1[20], line2[20];


void SystemClock_Config(void);

void Error_Handler(void);

void LCD_ScrollText(const char *msg, uint8_t row, uint16_t delay_ms)
{
    char buffer[17];
    uint8_t len = strlen(msg);

    for (int i = 0; i <= len; i++)
    {
        memset(buffer, ' ', 16);
        for (int j = 0; j < 16 && i + j < len; j++)
            buffer[j] = msg[i + j];

        buffer[16] = '\0';
        HD44780_SetCursor(0, row);
        HD44780_PrintStr(buffer);
        HAL_Delay(delay_ms);
    }
}

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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

		HAL_Init();
	    SystemClock_Config();
	    MX_GPIO_Init();
	    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
	    HAL_Delay(1000);// Ensure buzzer is OFF initially
	    MX_USART2_UART_Init();
	    MX_I2C1_Init();

	    HD44780_Init(2);
	    HD44780_Clear();
	    HD44780_SetCursor(0, 0);
	    LCD_ScrollText("Vehicle Speed Analyzer   ", 0, 200);
	    HAL_Delay(1000);
	    HD44780_Clear();

	       while (1)
	       {
	           // Vehicle triggers IR1 (PA8)

	           if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == GPIO_PIN_RESET && ir1_detected == 0)
	           {

	               ir1_time = HAL_GetTick();

	               // Wait until IR2 (PB5) is triggered
	               while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) != GPIO_PIN_RESET)
	               {
	            	   HAL_Delay(10);
	               }

	               ir2_time = HAL_GetTick();
	               ir1_detected = 1;
	               time_diff = ir2_time - ir1_time;

	               vehicle_count++;



	           if (time_diff < 5000)
	           {
	                   overspeed_count++;
	                   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);    // LED ON
	                   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);    // Buzzer ON
	                   HAL_Delay(500);
	                   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);  // Buzzer OFF
	                   snprintf(line1, sizeof(line1), "Speed: OVERSPEED");
	           }
	           else
	           {
	                   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);  // LED OFF
	                   snprintf(line1, sizeof(line1), "Speed: Normal");
	           }

	           snprintf(line2, sizeof(line2), "Cnt:%d OS:%d", vehicle_count, overspeed_count);

	           HD44780_Clear();
	           HD44780_SetCursor(0, 0);
	           HD44780_PrintStr(line1);
	           HD44780_SetCursor(0, 1);
	           HD44780_PrintStr(line2);

	               // Wait for vehicle to move away from both IR sensors
	           while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == GPIO_PIN_RESET ||
	                  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_RESET);

	           HAL_Delay(300);   // Debounce
	           ir1_detected = 0;
	       }
	       }
}

  /* USER CODE END 3 */

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
#ifdef USE_FULL_ASSERT
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
