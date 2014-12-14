#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

/* ����� ����� �������� ��� �������� 115200 ���/� */
#define USART_BD_M_VAL 		17 << 4
/* �������� ����� �������� ���� */
#define USART_C_NOT_REC 	-1

/* ASCII ���� ����������� ��� ���������� �������� */
#define CNTLQ      			0x11
#define DEL        			0x7F
#define BACKSPACE  			'\b'
#define CR         			'\r'
#define LF         			'\n'

/*****************************************************************************/

enum {

    UART_INIT_OK,
    UART_INIT_FAIL = 2

};

/*****************************************************************************/

extern uint32_t uart_init(void);
extern int32_t uart_put_char(uint32_t c);
extern int32_t uart_get_char(void);
extern uint32_t uart_get_cmd(void);

#endif // UART_H_INCLUDED
