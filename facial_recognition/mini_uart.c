#include <sys/mman.h> //mmap
#include <err.h>	  //error handling
#include <fcntl.h>	  //file ops
#include <unistd.h>	  //usleep
#include <stdio.h>
#include <string.h>

// base
#define PBASE (0x3F000000)
volatile unsigned int *ptr_PBASE;

// gpio
#define GP_BASE (0x3F200000)
volatile unsigned int *ptr_GP_BASE;

#define GPFSEL1 (0x00000004 / 4)
#define GPSET0 (0x0000001C / 4)
#define GPCLR0 (0x00000028 / 4)
#define GPPUD (0x00000094 / 4)
#define GPPUDCLK0 (0x00000098 / 4)


// miniuart
#define AUX_BASE (0x3F215000)
volatile unsigned int *ptr_AUX_BASE;

#define AUX_ENABLES (0x00000004 / 4)
#define AUX_MU_IO_REG (0x00000040 / 4)
#define AUX_MU_IER_REG (0x00000044 / 4)
#define AUX_MU_IIR_REG (0x00000048 / 4)
#define AUX_MU_LCR_REG (0x0000004C / 4)
#define AUX_MU_MCR_REG (0x00000050 / 4)
#define AUX_MU_LSR_REG (0x00000054 / 4)
#define AUX_MU_MSR_REG (0x00000058 / 4)
#define AUX_MU_SCRATCH (0x0000005C / 4)
#define AUX_MU_CNTL_REG (0x00000060 / 4)
#define AUX_MU_STAT_REG (0x00000064 / 4)
#define AUX_MU_BAUD_REG (0x00000068 / 4)

char input_string[30];

// void delay(unsigned long tiem)
// {
// 	usleep(tiem);
// }

void put32_GP(unsigned long reg, unsigned int value)
{
	volatile unsigned int *temp;
	temp = ptr_GP_BASE + reg;
	*temp = value;
}

unsigned int get32_GP(unsigned long reg)
{
	volatile unsigned int *temp;
	temp = ptr_GP_BASE + reg;
	return *temp;
}

void put32_AUX(unsigned long reg, unsigned int value)
{
	volatile unsigned int *temp;
	temp = ptr_AUX_BASE + reg;
	*temp = value;
}

unsigned int get32_AUX(unsigned long reg)
{
	volatile unsigned int *temp;
	temp = ptr_AUX_BASE + reg;
	return *temp;
}

void uart_send(char c)
{
	while (1)
	{
		if (get32_AUX(AUX_MU_LSR_REG) & 0x20)
			break;
	}
	put32_AUX(AUX_MU_IO_REG, c);
}

char uart_recv(void)
{
	while (1)
	{
		if (get32_AUX(AUX_MU_LSR_REG) & 0x01)
			break;
	}
	return (get32_AUX(AUX_MU_IO_REG) & 0xFF);
}

int serial_avaiable()
{
	return get32_AUX(AUX_MU_LSR_REG) & 0x01;
}

void uart_send_string(char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		uart_send((char)str[i]);
	}
}

char *uart_read_string()
{
	memset(input_string, '\0', 30);
	input_string[0] = uart_recv();
	for (int i = 1; (i < 29); i++)
	{
		input_string[i] = uart_recv();
		if(input_string[i] == '\n')break; 
	}
	return input_string;
}

void uart_init(void)
{
	int fd = -1;
	//Loading /dev/mem into a file
	if ((fd = open("/dev/mem", O_RDWR, 0)) == -1)
		err(1, "Error opening /dev/mem");

	ptr_PBASE = (unsigned int *)mmap(0, getpagesize(), PROT_WRITE, MAP_SHARED, fd, PBASE);
	ptr_GP_BASE = (unsigned int *)mmap(0, getpagesize(), PROT_WRITE, MAP_SHARED, fd, GP_BASE);
	ptr_AUX_BASE = (unsigned int *)mmap(0, getpagesize(), PROT_WRITE, MAP_SHARED, fd, AUX_BASE);

	if (ptr_PBASE == MAP_FAILED | ptr_GP_BASE == MAP_FAILED | ptr_AUX_BASE == MAP_FAILED)
		errx(1, "Error during mapping devices");

	printf("ptr_PBASE = 0x%08x\n", ptr_PBASE);

	unsigned int selector;
	selector = get32_GP(GPFSEL1);

	selector &= ~(7 << 12); // clean gpio14
	selector |= 2 << 12;	// set alt5 for gpio14
	selector &= ~(7 << 15); // clean gpio15
	selector |= 2 << 15;	// set alt5 for gpio15
	put32_GP(GPFSEL1, selector);

	put32_GP(GPPUD, 0);
	usleep(150);
	put32_GP(GPPUDCLK0, (1 << 14) | (1 << 15));
	usleep(150);
	put32_GP(GPPUDCLK0, 0);

	put32_AUX(AUX_ENABLES, 1);		 //Enable mini uart (this also enables access to its registers)
	put32_AUX(AUX_MU_CNTL_REG, 0);	 //Disable auto flow control and disable receiver and transmitter (for now)
	put32_AUX(AUX_MU_IER_REG, 0);	 //Disable receive and transmit interrupts
	put32_AUX(AUX_MU_LCR_REG, 3);	 //Enable 8 bit mode
	put32_AUX(AUX_MU_MCR_REG, 0);	 //Set RTS line to be always high
	put32_AUX(AUX_MU_BAUD_REG, 270); //Set baud rate to 115200

	put32_AUX(AUX_MU_CNTL_REG, 3); //Finally, enable transmitter and receiver
}

// void main()
// {

// 	uart_init();

// 	while (1)
// 	{
// 		uart_send_string("L0\n");
// 		usleep(1000000);
// 		uart_send_string("L1\n");
// 		usleep(1000000);
// 		if (serial_avaiable())
// 		{
// 			// char * input_s = uart_read_string();
// 			// printf("string: %s\n",input_s);
// 			 char input_C = uart_recv();
// 			printf("char: %c\n", input_C);
// 		}
// 	}

// 	uart_send_string("Hello, world!\n");

// 	while (1)
// 	{
// 		for (int i = 0; i < 10; i++)
// 		{
// 			uart_send_string("Hello, world!\n");
// 		}

// 		for (int j = 0; j < 10; j++)
// 		{
// 			printf("prechar\n");
// 			char *input_s = uart_read_string();
// 			// char input_C = uart_recv();
// 			printf("char #%d: %s\n", j, input_s);
// 		}
// 	}
// }
