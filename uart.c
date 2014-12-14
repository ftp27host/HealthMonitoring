
#include "common.h"
#include "uart.h"
#include "main.h"

static uint8_t *uart_line_p;
static uint32_t uart_cnt;
static uint8_t uart_putchar_enable;

/*****************************************************************************/

uint32_t uart_init(void){

    /* USART2 Tx PA2 output push-pull*/
    //GPIOA->CRH  |=  GPIO_CRH_CNF2_1 | GPIO_CRH_MODE9;
	GPIOA->MODER  |= GPIO_MODER_MODER2_1; 	// Alternative function mode to PA2
	GPIOA->AFR[0] |= 0x7700; 				// UART 1..3 to PA2
	GPIOA->OTYPER &= ~GPIO_OTYPER_ODR_2; 	// Push-pull mode
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2;

    /* USART2 Rx PA3 input floating */
	//GPIOA->MODER &= ~GPIO_MODER_MODER3; // Input function mode to PA3
	GPIOA->MODER  |= GPIO_MODER_MODER3_1; 	// Alternative function mode to PA3
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3;

    //GPIOA->CRH  |=  GPIO_CRH_CNF3_0;

	/* разрешение тактирования  USART1 */
    RCC->APB1ENR |=  RCC_APB1ENR_USART2EN;
    /* разрешение RX, TX */
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE;
    /* 115200 бод/с 8 бит данных, 1 стоп бит, без аппаратного контроля*/
    // (32000000/115200)/16 == 17.361
    USART2->BRR |= USART_BD_M_VAL & USART_BRR_DIV_MANTISSA;
    /* разрешение работы UART2 */
    USART2->CR1 |= USART_CR1_UE;

    /* инициализация указателя буфера команд */
    uart_line_p = rim_cmd_line;
    /* обнуление счетчика символов */
    uart_cnt = 0;
    /* инициализация флага разрешения вывода */
		uart_putchar_enable = 1;

    return (UART_INIT_OK);

}

/*****************************************************************************/

int32_t uart_put_char(uint32_t c){
	/* передача символа */
    while(!(USART2->SR & USART_SR_TXE));
    USART2->DR = (c & USART_DR_DR);

    return (c);
}

/*****************************************************************************/

int32_t uart_get_char(void){
	/* прием символа */
    if(USART2->SR & USART_SR_RXNE) {
    	return (USART2->DR & USART_DR_DR);
    }

    else
        return USART_C_NOT_REC;

}

/*****************************************************************************/

uint32_t uart_get_cmd(void){

	/* результат процесса получения команды */
    uint32_t uart_res = 0;
    int32_t uart_c;
    /* получение символа */
    if((uart_c = uart_get_char()) == '\r') {

    	uart_c = '\n';
    }


		if(uart_c == CNTLQ){
		
			uart_putchar_enable = 0;
			
		}else if((uart_c == BACKSPACE) || (uart_c == DEL)){
			/* обработка удаления символов */
			if(uart_cnt != 0){

				uart_cnt--;
				uart_line_p--;
				if(uart_putchar_enable){
					uart_put_char(BACKSPACE);
					uart_put_char(' ');
					uart_put_char(BACKSPACE);
				}

			}

    }else if(uart_c != USART_C_NOT_REC){
				/* РЅР°РїРѕР»РЅРµРЅРёРµ СЃС‚СЂРѕРєРё РєРѕРјР°РЅРґС‹ */
        *uart_line_p = uart_c;
			if(uart_putchar_enable)
				uart_put_char(uart_c);
        uart_line_p++;
        uart_cnt++;

    }

    if((uart_c == '\n') || (uart_cnt >= RIM_CMD_LINE_SIZE - 1)){
				/* РєРѕРјР°РЅРґР° РїРѕР»СѓС‡РµРЅР° */
        *uart_line_p = '\0';
        uart_cnt = 0;
        uart_line_p = rim_cmd_line;
        uart_res = 1;        

    }

    return (uart_res);

}

/*****************************************************************************/

