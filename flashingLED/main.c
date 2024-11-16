#include "main.h"


int main(void)
{
	/* -------------------- declare all nesessery structures---------------------*/
	// init structure for GPIO
	LL_GPIO_InitTypeDef GPIO_Initstructure;
	// init structure for timer
	LL_TIM_InitTypeDef TIM_Init_Struct;
	/* --------------------------------------------------------------------------*/
	
	/* -----------------------configurate Pin A.5 for board's LED----------------*/
	// enable clock on a GPIO port A
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	// initialize GPIO registers with default parameters
	LL_GPIO_StructInit(&GPIO_Initstructure);
	// set values for GPIO structure
	GPIO_Initstructure.Mode = LL_GPIO_MODE_OUTPUT; // set pin as an output
	GPIO_Initstructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL; // set to push-pull (0-1)
  GPIO_Initstructure.Pin = LL_GPIO_PIN_5; // set to pin 5, used GPIOA == port A
	GPIO_Initstructure.Pull = LL_GPIO_PULL_NO;
	GPIO_Initstructure.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	// send the filled structure to port A
	LL_GPIO_Init(GPIOA, &GPIO_Initstructure);
	/* --------------------------------------------------------------------------*/
	
	/* ----------------------timer configuration---------------------------------*/
	// enable clock on a TIM 2
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	// initialize TIM2 registers with default parameters
	LL_TIM_StructInit(&TIM_Init_Struct);
	TIM_Init_Struct.Prescaler = 7999; // bus clock (8 MHz) is divided by the prescaler (register value + 1).
                                     // divide the clock by 8000 and thus obtain 1kHz “counting frequency”. 
	TIM_Init_Struct.Autoreload = 999;  // timer counts up to the autoreload value and triggers an update interrupt.
	// send the filled structure to TIM2
	LL_TIM_Init(TIM2,&TIM_Init_Struct);
	/* --------------------------------------------------------------------------*/
	
	/* -----------------------NVIC configuration---------------------------------*/
	NVIC_SetPriorityGrouping(0); //http://www.ocfreaks.com/interrupt-priority-grouping-arm-cortex-m-nvic/
	// 1st parameter --> priority grouping
	// 2nd parameter --> PreemptPriority
	// 3rd parameter --> SubPriority
	uint32_t encoded_priority = NVIC_EncodePriority(0,0,0); 
	NVIC_SetPriority(TIM2_IRQn, encoded_priority);
	// activate interupt
	NVIC_EnableIRQ(TIM2_IRQn);
	LL_TIM_EnableCounter(TIM2);
	LL_TIM_EnableIT_UPDATE(TIM2);	
	/* --------------------------------------------------------------------------*/
	//CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPONEXIT_Msk));
	while (1)
  {
  }

}
/* -----------------------Interrupt Handler----------------------------------*/
void TIM2_IRQHandler(void)
{
	if(LL_TIM_IsActiveFlag_UPDATE(TIM2) == 1)
	{
		LL_TIM_ClearFlag_UPDATE(TIM2);
		LL_GPIO_TogglePin(GPIOA,LL_GPIO_PIN_5);
	}
}
		
/* --------------------------------------------------------------------------*/


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
