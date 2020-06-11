/**
 * Jake Armendariz
 * dog.c
 * 165032
 */

//Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//read, write and close
#include <unistd.h>
//warm
#include <err.h>

//I made this size very very big
#define BUFFER_SIZE 4096
char buffer[BUFFER_SIZE];

int print_buffer(int file_size){
    if (file_size == -1)
    {
        perror("Could not read file");
        return -1;
    }
    else
    {
        write(1, buffer, file_size);
        return 0;
    }
}

//Main function of dog
int main(int argc, char **argv)
{
    int file;
    if (argc >= 2)
    {
        for (int i = argc - 1; i >= 1; i--)
        {
            //Continues to run until ctrl D/END OF FILE stops program
            if (strcmp(argv[i], "-") == 0)
            {
                int file_size = 1;
                while(file_size != 0)
                {
                    file_size = read(0, buffer, BUFFER_SIZE);
                    print_buffer(file_size);
                }
                continue;
            }
            else{
                file = open(argv[i], O_RDONLY);
                if (file == -1)
                {
                    warn("%s", argv[i]);
                    continue;
                }
                int file_size = 1; //initial size
                while(file_size > 0){
                    file_size = read(file, buffer, BUFFER_SIZE);
                    print_buffer(file_size);
                }
                close(file);
            }
        }
    }else{
        int file_size = 1;
        while(file_size != 0)
        {
            file_size = read(0, buffer, BUFFER_SIZE);
            print_buffer(file_size);
        }
    }
    return 0;
}
