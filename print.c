#include <unistd.h>
#include <string.h>

void print(const char *fmt){
    write(STDOUT_FILENO, fmt, strlen(fmt)); 
}

void prints(const char *fmt, char *arg){
    char buf[128];
    strncpy(buf, fmt, strlen(fmt)+1);
    char * second = strchr(buf, '%');
    second[0] = '\0';
    second+= 2;
    write(STDOUT_FILENO, fmt, strlen(buf));
    write(STDOUT_FILENO, arg, strlen(arg));
    write(STDOUT_FILENO, second, strlen(second));
}

void printd(const char *fmt, unsigned long arg){
    char buf[128];
    strncpy(buf, fmt, strlen(fmt)+1);
    char * second = strchr(buf, '%');
    second[0] = '\0';
    second+= 2;
    write(STDOUT_FILENO, fmt, strlen(buf));
    // Print out the characters in the number
    if(arg == 0){
        char c = '0';
        write(STDOUT_FILENO, &c, sizeof(char));
    }else{
        long divisor = 1;
        long cpy_of_arg = arg;
        while (cpy_of_arg){
            cpy_of_arg /= 10;
            divisor *= 10;
        }
        while (divisor > 1)
        {
            divisor /= 10;
            char c = arg/divisor + '0';
            arg = arg % divisor;
            write(STDOUT_FILENO, &c, sizeof(char));
        }
    }
    write(STDOUT_FILENO, second, strlen(second));
}