#include "facial_recognition.h"
#include "mini_uart.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> //to use system()
#include <string.h> //to use strcpy()
void open_lock()
{
    uart_send_string("L1\n");
}

void close_lock()
{
    uart_send_string("L0\n");
}

int check_button()
{
    if (serial_avaiable())
    {
        char input_C = uart_recv();
        if (input_C == 'B')
        {
            input_C = uart_recv();
            if (input_C == '1')
            {
                return 1;
            }
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    uart_init();

    char command[100];

    int e1 = 8; //cara
    int e0 = 13; //conocido
    int max_mismatch = 113;

    char *folderpath = "../../Database";
    int rows = 112;
    int cols = 92;

    if (argc == 3)
    {
        //         if (argv[1][1]==0x63) /*comparar con "c"*/
        //         {
        //             printf("file to compare: %s \n", argv[2]);
        //             compare(argv[2], rows, cols, e1, e0, max_mismatch);
        //         }
        if (argv[1][1]==0x61) /*comparar con "a"*/
        {
            printf("database filepath: %s \n", argv[2]);
            analyze_database(argv[2], rows, cols);
        }
    }
    else
    {

        while (1)
        {
            if (check_button())
            {
                printf("boton presionado\n");
                strcpy(command, "raspistill -w 92 -h 112 -e png -t 100 -o /home/root/lock_server/input.png");
                printf("tomando foto\n");
                system(command);
                printf("foto tomada\n");
                int compare_result = compare("/home/root/lock_server/input.png", rows, cols, e1, e0, max_mismatch);

                switch (compare_result)
                {
                case 0:
                    printf("cara reconocida\n");
                    open_lock();
                    printf("cerrojo abierto\n");
                    usleep(1500000);
                    close_lock();
                    printf("cerrojo cerrado\n");
                    break;
                case -1:
                    printf("no se reconoce ninguna cara\n");
                    break;

                case -2:
                    printf("cara desconocida\n");
                    break;
                default:
                    break;
                }
                // if (compare_result == 0)
                // {
                //     printf("cara reconocida\n");
                //     open_lock();
                //     printf("cerrojo abierto\n");
                //     usleep(1500000);
                //     close_lock();
                //     printf("cerrojo cerrado\n");
                // }
                // else
                // {
                //     printf("cara desconocida\n");
                // }
            }
        }
    }
    return 0;
}
