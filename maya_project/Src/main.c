/*
  ******************************************************************************
	ALEXANDER LUKIN 12044804                 
	Mihnea Fleseriu 12202391	
	In diesem Projekt gilt:
	*=============================================================================
  *        SYSCLK(Hz)                             | 64000000
  *-----------------------------------------------------------------------------
  *        AHB Prescaler                          | 1
  *-----------------------------------------------------------------------------
  *        APB2 Prescaler                         | 1
  *-----------------------------------------------------------------------------
  *        APB1 Prescaler                         | 2
  *=============================================================================
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F3xx_LL_Examples
  * @{
  */

/** @addtogroup Templates_LL
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

// Priority grouping
int encoded_priority = 0;
int priority_grouping = 5;

// Constants for direction
int left_to_right = 0; // if 1 --> left to right
int right_to_left = 0; // if 1 --> right to left 
double rps = 0;

// Initial values for all diodes intensity
int diode_0 = 0;
int diode_1 = 0;
int diode_2 = 0;
int diode_3 = 0;

// Values for rotations in pre-def. area
int count_rotations = 0;

// Engle
double engle = 0.0;

// All functions
void StepMotor_PINsB_Full(int step);
void StepMotor_PINsC_Full(int step);
void StepMotor_PINsB_Half(int step);
void StepMotor_PINsC_Half(int step); 
void Rotation_Vertical(int direction);
void Rotation_Vertical_Half_Step(int direction);
void Rotation_Horizontal(int direction);
void Rotation_Horizontal_Half_Step(int direction);
void Move_Full();
void Move_Half();


int main(void)
{
  /* Configure the system clock to 64 MHz */
  SystemClock_Config();
  
  /* Add your application code here */
  
	LL_GPIO_InitTypeDef GPIO_Init_Struct;
	LL_TIM_InitTypeDef TIM_Init_Struct;
	LL_EXTI_InitTypeDef EXTI_Init_Struct;
	LL_USART_InitTypeDef USART_Init_Struct;
  
	//=======================GPIOs CONFIG=======================//
	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); // Clock for all GPIOA PINs
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB); // Clock for all GPIOB PINs
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC); // Clock for all GPIOC PINs
	
	// USART PINs CONFIG PC
	LL_GPIO_StructInit(&GPIO_Init_Struct);
	GPIO_Init_Struct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_Init_Struct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_Init_Struct.Pin = LL_GPIO_PIN_2 | LL_GPIO_PIN_3;
	GPIO_Init_Struct.Pull = LL_GPIO_PULL_NO;
	GPIO_Init_Struct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_Init_Struct.Alternate = LL_GPIO_AF_7; // for USART2_TX (PIN2) and USART2_RX (PIN3)
	
	LL_GPIO_Init(GPIOA, &GPIO_Init_Struct);
	
	
	// USART PINs CONFIG MAYA
	LL_GPIO_StructInit(&GPIO_Init_Struct);
	GPIO_Init_Struct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_Init_Struct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_Init_Struct.Pin = LL_GPIO_PIN_9 | LL_GPIO_PIN_10;
	GPIO_Init_Struct.Pull = LL_GPIO_PULL_NO;
	GPIO_Init_Struct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_Init_Struct.Alternate = LL_GPIO_AF_7; // for USART1_TX (PIN 9) and USART1_RX (PIN 10)
	
	LL_GPIO_Init(GPIOA, &GPIO_Init_Struct);
	

		// Vertikale Ablenkung Schrittmotor 1,2,3,4
	LL_GPIO_StructInit(&GPIO_Init_Struct);
	GPIO_Init_Struct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_Init_Struct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_Init_Struct.Pin = LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_8 | LL_GPIO_PIN_9;
	GPIO_Init_Struct.Pull = LL_GPIO_PULL_NO;
	GPIO_Init_Struct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	
	LL_GPIO_Init(GPIOB, &GPIO_Init_Struct);

	// Rotierende Scheibe Lichtschranke 1 and Lichtschranke 2
	LL_GPIO_StructInit(&GPIO_Init_Struct);
	GPIO_Init_Struct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_Init_Struct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_Init_Struct.Pin = LL_GPIO_PIN_3 | LL_GPIO_PIN_4;
	GPIO_Init_Struct.Pull = LL_GPIO_PULL_NO;
	GPIO_Init_Struct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	
	LL_GPIO_Init(GPIOB, &GPIO_Init_Struct);
/*
		// Lichtschranke vertikal
	LL_GPIO_StructInit(&GPIO_Init_Struct);
	GPIO_Init_Struct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_Init_Struct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_Init_Struct.Pin = LL_GPIO_PIN_5;
	GPIO_Init_Struct.Pull = LL_GPIO_PULL_NO;
	GPIO_Init_Struct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	
	LL_GPIO_Init(GPIOB, &GPIO_Init_Struct);
	
			// Lichtschranke horizontal
	LL_GPIO_StructInit(&GPIO_Init_Struct);
	GPIO_Init_Struct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_Init_Struct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_Init_Struct.Pin = LL_GPIO_PIN_10;
	GPIO_Init_Struct.Pull = LL_GPIO_PULL_NO;
	GPIO_Init_Struct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	
	LL_GPIO_Init(GPIOB, &GPIO_Init_Struct);
*/
	// Horizontale Ablenkung Schrittmotor 1,2,3,4
	LL_GPIO_StructInit(&GPIO_Init_Struct);
	GPIO_Init_Struct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_Init_Struct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_Init_Struct.Pin = LL_GPIO_PIN_9 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12;
	GPIO_Init_Struct.Pull = LL_GPIO_PULL_NO;
	GPIO_Init_Struct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	
	LL_GPIO_Init(GPIOC, &GPIO_Init_Struct);

	// TIMER CONFIG
		// Timer for calculate rotation
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	LL_TIM_StructInit(&TIM_Init_Struct);
	TIM_Init_Struct.Prescaler = 639; // Timer frequency 100 kHz
	TIM_Init_Struct.Autoreload = 64999 ; // 0,65 s
	LL_TIM_Init(TIM3, &TIM_Init_Struct);
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	LL_TIM_StructInit(&TIM_Init_Struct);
	TIM_Init_Struct.Prescaler = 63999; // Timer frequency 1 kHz pre 63999 ARR 1 --> 2ms
	TIM_Init_Struct.Autoreload = 1; // 1 ms
	LL_TIM_Init(TIM2, &TIM_Init_Struct);
	LL_TIM_EnableCounter(TIM2);
	LL_TIM_EnableIT_UPDATE(TIM2);
	
	
	

	// USART1 CONFIG
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
	LL_USART_StructInit(&USART_Init_Struct);
	USART_Init_Struct.BaudRate = 115200;
	USART_Init_Struct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_Init_Struct.StopBits = LL_USART_STOPBITS_1;
	USART_Init_Struct.Parity = LL_USART_PARITY_NONE;
	USART_Init_Struct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_Init_Struct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	LL_USART_Init(USART1,&USART_Init_Struct);
	LL_USART_EnableIT_RXNE(USART1);
	LL_USART_Enable(USART1);

	// USART2 CONFIG 
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	LL_USART_StructInit(&USART_Init_Struct);
	USART_Init_Struct.BaudRate = 115200;
	USART_Init_Struct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_Init_Struct.StopBits = LL_USART_STOPBITS_1;
	USART_Init_Struct.Parity = LL_USART_PARITY_NONE;
	USART_Init_Struct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_Init_Struct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	LL_USART_Init(USART2,&USART_Init_Struct);
	LL_USART_Enable(USART2);
	//LL_USART_EnableIT_RXNE(USART2);
	
	// NVICs CONFIG and EXTI CONFIG
		// EXTI 
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE3); // for PB3
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE4); // for PB4
	
	EXTI_Init_Struct.Line_0_31 = LL_EXTI_LINE_3; // for PB3
	EXTI_Init_Struct.Mode = LL_EXTI_MODE_IT;
	EXTI_Init_Struct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
	EXTI_Init_Struct.LineCommand = ENABLE;
	LL_EXTI_Init(&EXTI_Init_Struct);
	
	EXTI_Init_Struct.Line_0_31 = LL_EXTI_LINE_4; // for PB4
	EXTI_Init_Struct.Mode = LL_EXTI_MODE_IT;
	EXTI_Init_Struct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
	EXTI_Init_Struct.LineCommand = ENABLE;
	LL_EXTI_Init(&EXTI_Init_Struct);
	
	encoded_priority = NVIC_EncodePriority(priority_grouping,1,0);
	NVIC_SetPriority(EXTI3_IRQn, encoded_priority);
	NVIC_EnableIRQ(EXTI3_IRQn);
	
	encoded_priority = NVIC_EncodePriority(priority_grouping,1,1);
	NVIC_SetPriority(EXTI4_IRQn, encoded_priority);
	NVIC_EnableIRQ(EXTI4_IRQn);

		// USART1
	encoded_priority = NVIC_EncodePriority(priority_grouping,0,1);
	NVIC_SetPriority(USART1_IRQn, encoded_priority);
	NVIC_EnableIRQ(USART1_IRQn);
		// TIM3
	encoded_priority = NVIC_EncodePriority(priority_grouping,2,1);
	NVIC_SetPriority(TIM3_IRQn, encoded_priority);
	NVIC_EnableIRQ(TIM3_IRQn);
	//LL_TIM_EnableIT_UPDATE(TIM3);
		// TIM2
	encoded_priority = NVIC_EncodePriority(priority_grouping,0,0);
	NVIC_SetPriority(TIM2_IRQn, encoded_priority);
	NVIC_EnableIRQ(TIM2_IRQn);
	LL_TIM_EnableIT_UPDATE(TIM2);
	
	
	// Get data von Pin 3 send to PC
	
	/*	int a = LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_3);
		while(!LL_USART_IsActiveFlag_TXE(USART2)){};
		LL_USART_TransmitData8(USART2, a); */
		
	/*
	// Get data von LEDs and send to PC
	
		int leddata;
		leddata = LL_USART_ReceiveData8(USART1);
		while(!LL_USART_IsActiveFlag_TXE(USART2)){};
		LL_USART_TransmitData8(USART2, leddata);
	*/	
	int i = 0;
  /* Infinite loop */
	while (1)
  {
			//Move_Laser_UP();
	}
}

void StepMotor_PINsB_Full(int step) 
{
	switch (step) 
	{
		case 0:
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9);
			break;
		case 1:
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8);
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_9);
			break;
		case 2:
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_8);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9);
			break;
		case 3:
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9);
			break;
		default: break;
	}
}

void StepMotor_PINsC_Full(int step) 
{
	switch (step) 
	{
		case 0:
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_10);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_11);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_12);
			break;
		case 1:
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_10);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_11);
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_12);
			break;
		case 2:
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_10);
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_11);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_12);
			break;
		case 3:
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9);
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_10);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_11);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_12);
			break;
		default: break;
	}
}

void StepMotor_PINsB_Half(int step)
{
	switch (step) 
	{
		case 0:
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9);
			break;
		case 1:
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8);
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_9);
			break;
		case 2:
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8);
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_9);
			break;
		case 3:
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_8);
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_9);
			break;
		case 4:
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_8);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9);
			break;
		case 5:
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7);
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_8);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9);
			break;
		case 6:
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9);
			break;
		case 7:
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6);
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8);
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9);
			break;
		default: break;
	}
}

void StepMotor_PINsC_Half(step)
{
	switch (step) 
	{
		case 0:
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_10);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_11);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_12);
			break;
		case 1:
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_10);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_11);
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_12);
			break;
		case 2:
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_10);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_11);
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_12);
			break;
		case 3:
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_10);
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_11);
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_12);
			break;
		case 4:
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_10);
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_11);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_12);
			break;
		case 5:
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9);
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_10);
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_11);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_12);
			break;
		case 6:
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9);
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_10);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_11);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_12);
			break;
		case 7:
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9);
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_10);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_11);
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_12);
			break;
		default: break;
	}
}

void Rotation_Vertical_Half_Step(int direction) // direction == 0 --> down
{
	static int step = 0;
	StepMotor_PINsB_Half(step);
	
	step = (direction == 0) ? (step+1) : (step-1);
		if (step < 0)
			step = 7;
		if (step > 7)
			step = 0;
}

void Rotation_Horizontal_Half_Step(int direction) // direction == 0 --> right_to_left
{
	static int step = 0;
	StepMotor_PINsC_Half(step);
	
	step = (direction == 0) ? (step+1) : (step-1);
		if (step < 0)
			step = 7;
		if (step > 7)
			step = 0;
}

void Rotation_Vertical(int direction) // direction == 0 --> down
{
	static int step = 0;
	StepMotor_PINsB_Full(step);

	step = (direction == 0) ? (step+1) : (step-1);
		if (step < 0)
			step = 3;
		if (step > 3)
			step = 0;
}

void Rotation_Horizontal(int direction) // direction == 0 --> left_to_right
{
	static int step = 0;
	StepMotor_PINsC_Full(step);

	step = (direction == 0) ? (step+1) : (step-1);
		if (step < 0)
			step = 3;
		if (step > 3)
			step = 0;
}


void Move_Full()
{
	if((diode_0 + diode_1) < (diode_2 + diode_3))
	{
		Rotation_Vertical(1);
	}
	if ((diode_0 + diode_1) > (diode_2 + diode_3))
	{
		Rotation_Vertical(0);
	}
	if ((diode_0 + diode_2) > ((diode_1 + diode_3)))
	{
		Rotation_Horizontal(1);
	}
	if ((diode_0 + diode_2) < (diode_1 + diode_3))
	{
		Rotation_Horizontal(0);
	}
}

void Move_Half()
{
	if((engle < 0.3926991 && engle >= 0) || (engle <= 6.28319 && engle > 5.8904862) || (engle >= -6.28319 && engle <= -5.8904862) || (engle <= 0 && engle > -0.3926991)) // 1. Area 
	{
		if((diode_0 + diode_1) < (diode_2 + diode_3))
		{
			Rotation_Horizontal_Half_Step(1);
		}
		if ((diode_0 + diode_1) > (diode_2 + diode_3))
		{
			Rotation_Horizontal_Half_Step(0);
		}
		if ((diode_0 + diode_2) > ((diode_1 + diode_3)))
		{
			Rotation_Vertical_Half_Step(0);
		}
		if ((diode_0 + diode_2) < (diode_1 + diode_3))
		{
			Rotation_Vertical_Half_Step(1);
		}
	}
	if((engle > 0.3926991 && engle <= 1.178097) || (engle > -5.8904862 && engle <= -5.1050881)) // 2. Area
	{
		if(diode_2 < diode_1)
		{
			Rotation_Vertical_Half_Step(1);
		}
		if(diode_2 > diode_1)
		{
			Rotation_Vertical_Half_Step(0);
		}
		if(diode_3 < diode_0)
		{
			Rotation_Horizontal_Half_Step(0);
		}
		if(diode_3 > diode_0)
		{
			Rotation_Horizontal_Half_Step(1);
		}
	}
	if((engle > 1.178097 && engle <= 1.9634954) || (engle > -5.1050881 && engle <= -4.3196899)) // 3. Area 
	{
		if((diode_0 + diode_1) < (diode_2 + diode_3))
		{
			Rotation_Vertical_Half_Step(0);
		}
		if ((diode_0 + diode_1) > (diode_2 + diode_3))
		{
			Rotation_Vertical_Half_Step(1);
		}
		if ((diode_0 + diode_2) > ((diode_1 + diode_3)))
		{
			Rotation_Horizontal_Half_Step(0);
		}
		if ((diode_0 + diode_2) < (diode_1 + diode_3))
		{
			Rotation_Horizontal_Half_Step(1);
		}
	}
	if((engle > 1.9634954 && engle <= 2.7488936) || (engle > -4.3196899 && engle <= -3.5342917)) // 4. Area
	{
		if(diode_2 < diode_1)
		{
			Rotation_Horizontal_Half_Step(1);
		}
		if(diode_2 > diode_1)
		{
			Rotation_Horizontal_Half_Step(0);
		}
		if(diode_3 < diode_0)
		{
			Rotation_Vertical_Half_Step(1);
		}
		if(diode_3 > diode_0)
		{
			Rotation_Vertical_Half_Step(0);
		}
	}
	if((engle > 2.7488936 && engle <= 3.5342917) || (engle > -3.5342917 && engle <= -2.7488936)) // 5. Area
	{
		if((diode_0 + diode_1) < (diode_2 + diode_3))
		{
			Rotation_Horizontal_Half_Step(0);
		}
		if ((diode_0 + diode_1) > (diode_2 + diode_3))
		{
			Rotation_Horizontal_Half_Step(1);
		}
		if ((diode_0 + diode_2) > ((diode_1 + diode_3)))
		{
			Rotation_Vertical_Half_Step(1);
		}
		if ((diode_0 + diode_2) < (diode_1 + diode_3))
		{
			Rotation_Vertical_Half_Step(0);
		}
	}
	if((engle > 3.5342917 && engle <= 4.3196899) || (engle > -2.7488936 && engle <= -1.9634954)) // 6. Area
	{
		if(diode_2 < diode_1)
		{
			Rotation_Vertical_Half_Step(0);
		}
		if(diode_2 > diode_1)
		{
			Rotation_Vertical_Half_Step(1);
		}
		if(diode_3 < diode_0)
		{
			Rotation_Horizontal_Half_Step(1);
		}
		if(diode_3 > diode_0)
		{
			Rotation_Horizontal_Half_Step(0);
		}
	}
	if((engle > 4.3196899 && engle <= 5.1050881) || (engle > -1.9634954 && engle <= -1.178097)) // 7. Area
	{
		if((diode_0 + diode_1) < (diode_2 + diode_3))
		{
			Rotation_Vertical_Half_Step(1);
		}
		if ((diode_0 + diode_1) > (diode_2 + diode_3))
		{
			Rotation_Vertical_Half_Step(0);
		}
		if ((diode_0 + diode_2) > ((diode_1 + diode_3)))
		{
			Rotation_Horizontal_Half_Step(1);
		}
		if ((diode_0 + diode_2) < (diode_1 + diode_3))
		{
			Rotation_Horizontal_Half_Step(0);
		}
	}
	if((engle > 5.1050881 && engle <= 5.8904862) || (engle > -1.178097 && engle <= -0.3926991)) // 8. Area
	{
		if(diode_2 < diode_1)
		{
			Rotation_Horizontal_Half_Step(0);
		}
		if(diode_2 > diode_1)
		{
			Rotation_Horizontal_Half_Step(1);
		}
		if(diode_3 < diode_0)
		{
			Rotation_Vertical_Half_Step(0);
		}
		if(diode_3 > diode_0)
		{
			Rotation_Vertical_Half_Step(1);
		}
	}
}

void TIM3_Counter_Routine()
{
	if (LL_TIM_IsEnabledCounter(TIM3) != 1)
	{
		LL_TIM_EnableCounter(TIM3);
	}
	else 
	{
		int timer_value = LL_TIM_GetCounter(TIM3);
		LL_TIM_SetCounter(TIM3, 0);
		if (timer_value != 0)
		{
			rps = 100000.0 / (timer_value * 230);
			if (left_to_right == 1) 
			{
				engle += rps*2*3.14159*timer_value/(100000);
			}
			if (right_to_left == 1)
			{
				engle -= rps*2*3.14159*timer_value/(100000);
			}
		} 
		else 
		{
			rps = 0;
		}
		
	}
}


void Check_Direction(int exti_pin3)
{
	if (exti_pin3 == 1)
	{
		if (((LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_3) == 0)&&(LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_4) == 1))
			||((LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_3) == 1)&&(LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_4) == 0)))			
		{
			if(left_to_right != 1)
			{
				left_to_right = 1;
				right_to_left = 0; 
			} 
			else
			{
				TIM3_Counter_Routine();
			}
		} 
		else if (((LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_3) == 0)&(LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_4) == 0))
			|((LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_3) == 1)&(LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_4) == 1)))
		{
			if(right_to_left != 1)
			{
				left_to_right = 0;
				right_to_left = 1; 
			}
			else
			{
				TIM3_Counter_Routine();
			}
		} 
	} 
	else 
	{
		if (((LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_3) == 0)&&(LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_4) == 0))
			||((LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_3) == 1)&&(LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_4) == 1)))			
		{
			if(left_to_right != 1)
			{
				left_to_right = 1;
				right_to_left = 0; 
			} 
			else
			{
				TIM3_Counter_Routine();
			}
		} 
		else if (((LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_3) == 1)&(LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_4) == 0))
			|((LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_3) == 0)&(LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_4) == 1)))
		{
			if(right_to_left != 1)
			{
				left_to_right = 0;
				right_to_left = 1; 
			}
			else
			{
				TIM3_Counter_Routine();
			}
		} 
	}
}

void Null_Position_Detection()
{
	static int function_called = 0;
	if (LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_3) == 1)
	{
		function_called ++;
		if (function_called > 1)
		{
			engle = 0.0;
		}
	}
	else 
	{
		function_called = 0;
	}
}

void EXTI3_IRQHandler()
{
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
	Check_Direction(1);
}

void EXTI4_IRQHandler()
{
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_4);
	Null_Position_Detection();
}


void USART1_IRQHandler()
{
	int recData = 0;
	recData = LL_USART_ReceiveData8(USART1);
	if (recData < 64){
		diode_0 = recData;
	} else if (recData >= 64 && recData < 128){
		diode_1 = recData - 64;
	} else if (recData >= 128 && recData < 192){
		diode_2 = recData - 128;
	} else {
		diode_3 = recData - 192;
	}
}

void TIM3_IRQHandler(void)
{
	if (LL_TIM_IsActiveFlag_UPDATE(TIM3) == 1)
	{
		LL_TIM_ClearFlag_UPDATE(TIM3);
		left_to_right = 0;
		right_to_left = 0; 
	}
}

void TIM2_IRQHandler(void) 	
{
	if (LL_TIM_IsActiveFlag_UPDATE(TIM2) == 1)
	{
		LL_TIM_ClearFlag_UPDATE(TIM2);
		Move_Half();
	}		
}

/* ==============   BOARD SPECIFIC CONFIGURATION CODE BEGIN    ============== */
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            PLLMUL                         = 16
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Set FLASH latency */ 
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSI if not already activated*/
  if (LL_RCC_HSI_IsReady() == 0)
  {
    /* Enable HSI and wait for activation*/
    LL_RCC_HSI_Enable(); 
    while(LL_RCC_HSI_IsReady() != 1)
    {
    };
  }

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_16);
  
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1) 
  {
  };
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };
  
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  
  /* Set systick to 1ms in using frequency set to 64MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ ((HSI_VALUE / 2), LL_RCC_PLL_MUL_16) */
  LL_Init1msTick(64000000);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(64000000);
}
/* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */
