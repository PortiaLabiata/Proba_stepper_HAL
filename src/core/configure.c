#include "core/configure.h"

void ClockConfig(void) {
  RCC->CR |= RCC_CR_HSION_Msk; // Starting HSI
    while (!(RCC->CR & RCC_CR_HSIRDY_Msk)) 
        __NOP();

#ifdef USE_IWDG
    RCC->CSR |= RCC_CSR_LSION; // Enable LSI for IWDG
    while (!(RCC->CR & RCC_CSR_LSIRDY_Msk)) // Wait for LSI to start
        __NOP();
#endif

    // Setting PLL source is unnecessary, since reset value is 0 and
    // it corresponds to HSI/2. f=48MHz
    // Settings: PLL as SYSCLK clock source, PLLMUL=12, ADC PSC=4.

    FLASH->ACR |= FLASH_ACR_LATENCY_2; // Enable flash latency
    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV4 | RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_SW_PLL | RCC_CFGR_PLLMULL16;
    RCC->CR |= RCC_CR_PLLON; // Starting PLL
    while (!(RCC->CR & RCC_CR_PLLRDY)) 
        __NOP();
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) 
        __NOP();
    SystemCoreClockUpdate();

    NVIC_SetPriorityGrouping(4);
    SysTick_Config(SystemCoreClock / 1000); // Set SysTick to fire each ms
    NVIC_EnableIRQ(SysTick_IRQn); // Enable SysTick IRQ
}

void HAL_MspInit(void) {
  __HAL_RCC_AFIO_CLK_ENABLE();
}

void GPIO_Config(void) {
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN_Msk; // Enable GPIOC clocking
  GPIOC->CRH &= ~GPIO_CRH_CNF13_Msk; // Set mode to 2MHz
  GPIOC->CRH |= GPIO_CRH_MODE13_1; // Set mode to output PP

  RCC->APB2ENR |= (RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPAEN);

  GPIOA->CRL &= ~(GPIO_CRL_CNF7_Msk);
  GPIOA->CRL |= (GPIO_CRL_MODE7_1);

  GPIOB->CRL &= ~(GPIO_CRL_CNF0_Msk | GPIO_CRL_CNF1_Msk);
  GPIOB->CRL |= (GPIO_CRL_MODE0_1 | GPIO_CRL_MODE1_1);
}

void UART_Config(void) {
  USART1->BRR = HAL_RCC_GetPCLK2Freq() / UART_BAUD_RATE;
  USART1->CR1 |= (USART_CR1_TE | USART_CR1_RE | \
           USART_CR1_UE); // Enable UART, reciever and transmitter
  // No need to set M bit, word length=8 bit.

  NVIC_SetPriority(USART1_IRQn, 0);
  NVIC_EnableIRQ(USART1_IRQn);
}

void TIM2_Config(void) {
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  GPIOA->CRL &= ~GPIO_CRL_CNF0;
  GPIOA->CRL |= (GPIO_CRL_CNF0_1 | GPIO_CRL_MODE0_1);

  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable clocking
  TIM2->DIER |= TIM_DIER_UIE; // Enable update event interrupt
  TIM2->CR1 &= ~(TIM_CR1_CKD_Msk);
  NVIC_SetPriority(TIM2_IRQn, 0);
  NVIC_EnableIRQ(TIM2_IRQn);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
  RCC->APB2ENR |= (RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN);

  // Configure PB6 (TX) as Alternate Function Push-Pull Output
  GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);
  GPIOA->CRH |= (GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_1);  // 50MHz output

  // Configure PB7 (RX) as Input Floating (no pull-up/pull-down)
  GPIOA->CRH &= ~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10);
  GPIOA->CRH |= GPIO_CRH_CNF10_1;  // Input floating
}

void CAN_Config(void) {
  hcan.Instance = CAN1;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_13TQ;  // 13 + 2 + 1 = 16 TQs
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.Prescaler = 16;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  HAL_CAN_Init(&hcan);
}

void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan) {
  if(hcan->Instance == CAN1)
  {
    RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN);
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;

    GPIOA->CRH &= ~(GPIO_CRH_CNF11_Msk | GPIO_CRH_CNF12_Msk);
    GPIOA->CRH |= (GPIO_CRH_CNF12_1 | GPIO_CRH_MODE12_0 | GPIO_CRH_MODE12_1);
    GPIOA->CRH |= (GPIO_CRH_CNF11_0);
    GPIOA->ODR |= GPIO_ODR_ODR11;

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);

    HAL_NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
  }

}

void TIM3_Config(void) {
  __HAL_RCC_TIM3_CLK_ENABLE();
  htim3.Instance = TIM3;
  htim3.Init.AutoReloadPreload = DISABLE;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Prescaler = HAL_RCC_GetPCLK1Freq() / 500 - 1;
  htim3.Init.Period = 1;
  HAL_TIM_Base_Init(&htim3);

  HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

void Error_Handler(void) {
    
}

void HardFault_Handler(void) {
  __disable_irq();
  while (1) {
    __NOP();
  }
}