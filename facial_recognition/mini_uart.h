#if !defined(MINI_UART)
#define MINI_UART


void uart_init(void);
char uart_recv(void);
void uart_send(char c);
void uart_send_string(char *str);
int serial_avaiable();


#endif // MINI_UART
