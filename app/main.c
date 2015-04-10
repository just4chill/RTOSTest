
#include "LPC17xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include "system_LPC17xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lpc17xx_uart.h"
#include "uart_interrupt.h"
#include "lpc17xx_pinsel.h"

void 		prvSetupHardware( void );
static void appBoot(void * pvParameters);

int main(void)
{
	/* Setup the Hardware */
	prvSetupHardware();

	xTaskCreate(appBoot,
			(signed portCHAR *)"appBoot",
			256,
			NULL,
			3,
			NULL);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Never reach here */
	return 0;
}

static void appBoot(void * pvParameters)
{
	UART_CFG_Type * UARTConfigStruct;
	UART_FIFO_CFG_Type * UARTFIFOConfigStruct;
	PINSEL_CFG_Type * PinCfg;

	uint32_t idx, len;
	__IO FlagStatus exitflag;
	uint8_t buffer[10];

	PinCfg->Funcnum = 1;
	PinCfg->OpenDrain = 0;
	PinCfg->Pinmode = 0;
	PinCfg->Pinnum = 2;
	PinCfg->Portnum = 0;
	PINSEL_ConfigPin(PinCfg);
	PinCfg->Pinnum = 3;
	PINSEL_ConfigPin(PinCfg);

	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_ConfigStructInit(UARTConfigStruct);	
	UART_Init((LPC_UART_TypeDef *)LPC_UART0, UARTConfigStruct);

	/* Initialize FIFOConfigStruct to default state:
	 * 				- FIFO_DMAMode = DISABLE
	 * 				- FIFO_Level = UART_FIFO_TRGLEV0
	 * 				- FIFO_ResetRxBuf = ENABLE
	 * 				- FIFO_ResetTxBuf = ENABLE
	 * 				- FIFO_State = ENABLE
	 */
	UART_FIFOConfigStructInit(UARTFIFOConfigStruct);
	UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART0, UARTFIFOConfigStruct);
	// Enable UART Transmit
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART0, ENABLE);
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_RBR, ENABLE);
	/* Enable UART line status interrupt */
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_RLS, ENABLE);
	/*
	 * Do not enable transmit interrupt here, since it is handled by
	 * UART_Send() function, just to reset Tx Interrupt state for the
	 * first time
	 */
	extern __IO FlagStatus TxIntStat;
	TxIntStat = RESET;

	extern UART_RING_BUFFER_T * rb;

	// Reset ring buf head and tail idx
	__BUF_RESET(rb->rx_head);
	__BUF_RESET(rb->rx_tail);
	__BUF_RESET(rb->tx_head);
	__BUF_RESET(rb->tx_tail);

    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(UART0_IRQn, ((0x01<<3)|0x01));
	/* Enable Interrupt for UART0 channel */
    NVIC_EnableIRQ(UART0_IRQn);

    UART_Send(LPC_UART0, "System started\r", 
    	sizeof("System started\r"),BLOCKING);
}

void prvSetupHardware( void )
{
	/* Initialte Clock */
	SystemInit();
	SystemCoreClockUpdate();
}


