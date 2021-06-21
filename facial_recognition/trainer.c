#include "facial_recognition.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> //to use system()
#include <string.h> //to use strcpy()


int main(int argc, char *argv[])
{
    char command[100];
    int e1 = 20; //cara
    int e0 = 13; //conocido
    int max_mismatch = 88;
    char *folderpath = "../../Database";
    int rows = 112;
    int cols = 92;

    if (argc == 3)
    {
                if (argv[1][1]==0x63) /*comparar con "c"*/
                {
                    printf("file to compare: %s \n", argv[2]);
                    compare(argv[2], rows, cols, e1, e0, max_mismatch);
                }
        if (argv[1][1]==0x61) /*comparar con "a"*/
        {
            printf("database filepath: %s \n", argv[2]);
            analyze_database(argv[2], rows, cols);
        }
    }
    return 0;
}
