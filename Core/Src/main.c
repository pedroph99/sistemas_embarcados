/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>

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

/* frequência atual */
volatile uint16_t current_frequency = 60;

/* ponteiro que aponta para current_frequency */
volatile uint16_t *pointer_current_frequency = &current_frequency;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */

void software_pwm(uint16_t frequency, uint8_t duty_cycle);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/

/* USER CODE BEGIN 0 */

void software_pwm(uint16_t frequency, uint8_t duty_cycle)
{
    uint32_t period_ms;
    uint32_t on_time;
    uint32_t off_time;

    /* calcula período em milissegundos */
    period_ms = 1000 / frequency;

    /* calcula tempos ligado e desligado */
    on_time = (period_ms * duty_cycle) / 100;
    off_time = period_ms - on_time;

    /* liga LED */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

    HAL_Delay(on_time);

    /* desliga LED */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

    HAL_Delay(off_time);
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

    HAL_Init();

    SystemClock_Config();

    /* Initialize all configured peripherals */

    MX_GPIO_Init();

    /* USER CODE BEGIN 2 */

    /*
     * Inicializa o botão azul no modo de interrupção.
     */
    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

    /* USER CODE END 2 */

    /* Infinite loop */

    /* USER CODE BEGIN WHILE */

    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

        /*
         * O ponteiro contém o endereço de current_frequency.
         *
         * *pointer_current_frequency acessa o valor armazenado
         * nesse endereço.
         */
        software_pwm(*pointer_current_frequency, 50);

        /* USER CODE END 3 */
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /*
     * Configure the main internal regulator output voltage
     */

    if (HAL_PWREx_ControlVoltageScaling(
            PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        Error_Handler();
    }

    /*
     * Initializes the RCC Oscillators
     */

    RCC_OscInitStruct.OscillatorType =
        RCC_OSCILLATORTYPE_HSI;

    RCC_OscInitStruct.HSIState =
        RCC_HSI_ON;

    RCC_OscInitStruct.HSICalibrationValue =
        RCC_HSICALIBRATION_DEFAULT;

    RCC_OscInitStruct.PLL.PLLState =
        RCC_PLL_ON;

    RCC_OscInitStruct.PLL.PLLSource =
        RCC_PLLSOURCE_HSI;

    RCC_OscInitStruct.PLL.PLLM = 1;

    RCC_OscInitStruct.PLL.PLLN = 10;

    RCC_OscInitStruct.PLL.PLLP =
        RCC_PLLP_DIV7;

    RCC_OscInitStruct.PLL.PLLQ =
        RCC_PLLQ_DIV2;

    RCC_OscInitStruct.PLL.PLLR =
        RCC_PLLR_DIV2;

    if (HAL_RCC_OscConfig(
            &RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /*
     * Initializes the CPU, AHB and APB buses clocks
     */

    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource =
        RCC_SYSCLKSOURCE_PLLCLK;

    RCC_ClkInitStruct.AHBCLKDivider =
        RCC_SYSCLK_DIV1;

    RCC_ClkInitStruct.APB1CLKDivider =
        RCC_HCLK_DIV1;

    RCC_ClkInitStruct.APB2CLKDivider =
        RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(
            &RCC_ClkInitStruct,
            FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Inicialmente deixa o LED apagado */

    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_5,
        GPIO_PIN_RESET
    );

    /*
     * Configura PA5 como saída
     */

    GPIO_InitStruct.Pin =
        GPIO_PIN_5;

    GPIO_InitStruct.Mode =
        GPIO_MODE_OUTPUT_PP;

    GPIO_InitStruct.Pull =
        GPIO_NOPULL;

    GPIO_InitStruct.Speed =
        GPIO_SPEED_FREQ_VERY_HIGH;

    HAL_GPIO_Init(
        GPIOA,
        &GPIO_InitStruct
    );
}

/* USER CODE BEGIN 4 */

/*
 * Callback chamada pela HAL quando ocorre
 * uma interrupção externa de GPIO.
 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    /*
     * Botão azul normalmente está ligado ao PC13.
     */
    if (GPIO_Pin == GPIO_PIN_13)
    {
        /*
         * O ponteiro aponta para current_frequency.
         *
         * Portanto:
         *
         * *pointer_current_frequency
         *
         * equivale a acessar current_frequency.
         */

        if (*pointer_current_frequency == 60)
        {
            /*
             * altera current_frequency para 1
             * usando o ponteiro
             */
            *pointer_current_frequency = 1;
        }
        else
        {
            /*
             * altera current_frequency para 60
             * usando o ponteiro
             */
            *pointer_current_frequency = 60;
        }
    }
    else
    {
        __NOP();
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */

void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief Reports the name of the source file and
  * source line number where assert_param error occurred.
  */

void assert_failed(uint8_t *file, uint32_t line)
{
}

#endif
