#include <cstdint>

extern uint32_t __GLOBAL_STACK_TOP__;

extern "C" void resetIsr();
extern void nmiIsr();
extern void hardFaultIsr();
extern void unhandledIrqIsr();

__attribute__((section(".vectors_table"), used))
void (* const vectors_table[])() = {
    reinterpret_cast<void(*)()>(&__GLOBAL_STACK_TOP__),
    resetIsr,
    nmiIsr,
    hardFaultIsr,
    unhandledIrqIsr,
    unhandledIrqIsr,
    unhandledIrqIsr,
    unhandledIrqIsr,
    unhandledIrqIsr,
    unhandledIrqIsr,
    unhandledIrqIsr,
    unhandledIrqIsr, // SVC
    unhandledIrqIsr,
    unhandledIrqIsr,
    unhandledIrqIsr, // pendSv
    unhandledIrqIsr, // sysTick
    unhandledIrqIsr, // Window WatchDog              
    unhandledIrqIsr, // PVD through EXTI Line detect 
    unhandledIrqIsr, // RTC through the EXTI line    
    unhandledIrqIsr, // FLASH                        
    unhandledIrqIsr, // RCC & CRS                    
    unhandledIrqIsr, // EXTI Line 0 and 1            
    unhandledIrqIsr, // EXTI Line 2 and 3            
    unhandledIrqIsr, // EXTI Line 4 to 15            
    unhandledIrqIsr, // USB, UCPD1, UCPD2            
    unhandledIrqIsr, // DMA1 Channel 1               
    unhandledIrqIsr, // DMA1 Channel 2 and Channel 3 
	unhandledIrqIsr, // DMA1 Ch4 to Ch7, DMA2 Ch1 to Ch5, DMAMUX1 overrun 
    unhandledIrqIsr, // ADC1, COMP1 and COMP2         
    unhandledIrqIsr, // TIM1 Break, Update, Trigger and Commutation 
    unhandledIrqIsr, // TIM1 Capture Compare         
    unhandledIrqIsr, // TIM2                         
    unhandledIrqIsr, // TIM3, TIM4                   
    unhandledIrqIsr, // TIM6, DAC and LPTIM1         
    unhandledIrqIsr, // TIM7 and LPTIM2              
    unhandledIrqIsr, // TIM14                        
    unhandledIrqIsr, // TIM15                        
    unhandledIrqIsr, // TIM16 & FDCAN1_IT0 & FDCAN2_IT0 
    unhandledIrqIsr, // TIM17 & FDCAN1_IT1 & FDCAN2_IT1 
    unhandledIrqIsr, // I2C1                         
    unhandledIrqIsr, // I2C2, I2C3                   
    unhandledIrqIsr, // SPI1                         
    unhandledIrqIsr, // SPI2, SPI3                   
    unhandledIrqIsr, // USART1                       
    unhandledIrqIsr, // USART2 & LPUART2             
    unhandledIrqIsr, // USART3, USART4, USART5, USART6, LPUART1   
    unhandledIrqIsr, // CEC                          
    unhandledIrqIsr  // AES and RNG                  
};
