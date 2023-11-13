#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <termios.h>


#define BCM2711_PERI_BASE 0xFE000000
#define BLOCK_SIZE (4*1024)
#define PAGE_SIZE (4*1024)
#define GPIO_BASE (BCM2711_PERI_BASE + 0x200000)

#define GPFSEL0 0x00
#define GPFSEL1 0x04
#define GPSET0 0x1c
#define GPCLR0 0x28
#define GPLEV0 0x34

#define Offset2MemIdx(Offset) Offset / 4
#define LedOn(gpio, num) gpio[Offset2MemIdx(GPSET0)] |= (1 << num)
#define LedOff(gpio, num) gpio[Offset2MemIdx(GPCLR0)] |= (1 << num)

// termios: terminal + os
// structure for terminal output control
static struct termios init_setting, new_setting;

// struct termios
// {
//     tcflag_t c_iflag;       /* input modes */
//     tcflag_t c_oflag;      /* output modes */
//     tcflag_t c_cflag;      /* control modes */

//     tcflag_t c_lflag;       /* local modes */

//     cc_t c_cc[NCCS];    /* control chars */

// }

void init_keyboard()
{
    // STDIN_FILENO: defined in unistd.h | standard input
    // get current 
    tcgetattr(STDIN_FILENO, &init_setting);
    new_setting = init_setting;
    // ICANON: standard mode or not
    // off standard mode
    new_setting.c_lflag &= ~ICANON;
    // ECHO: print input(echo)
    // off echo
    new_setting.c_lflag &= ~ECHO;
    // special usage like ctrl+c, ctrl+z
    // VMIN: min letter for return
    new_setting.c_cc[VMIN] = 0;
    // VTIME: check timer length
    new_setting.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_setting);
}

void close_keyboard()
{
    tcsetattr(0, TCSANOW, &init_setting);
}

char get_key()
{
    char ch =-1;
    if(read(STDIN_FILENO, &ch, 1)!=1)
        ch =-1;
    return ch;
}

void print_menu()
{
    printf("\n\n ------------------- \n");
    printf("Switch On : LED ON\n");
    printf("Switch Off : LED OFF\n");
    printf("[q] : Exit program\n");
    printf("--------------------- \n\n");
}

int main(int argc, char **argv)
{
    int data;
    int memfd;
    char *addr_gpio;
    volatile unsigned int *gpio;
    char ch;
    int tmp;
    int prev;

    // O_RDWR: Read and Write
    // O_SYNC: do other thing after writting complete
    memfd = open("/dev/mem", O_RDWR | O_SYNC);
    if (memfd <0 ){
        printf("mem open fail \n");
        return -1;
    }

    // get start point of gpio
    addr_gpio = (char*)mmap(NULL, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, GPIO_BASE);
    if(addr_gpio == MAP_FAILED){
        printf("mmap fail \n");
        return -1;
    }


    gpio = (volatile unsigned int*)addr_gpio;
    // led pin: gpio4
    // gpio4 funcion select: [12:14]bits
    // set function select to 001 -> output
    gpio[Offset2MemIdx(GPFSEL0)] |= (1 << 12);

    init_keyboard();
    print_menu();

    if(argc > 1){
        if(argv[1][0] == '1'){
            LedOn(gpio, 4);
            printf("Default LED: On\n");
        }
        else{
            LedOff(gpio, 4);
            printf("Default LED: Off\n");
        }
    }
	else{
        LedOff(gpio, 4);
		printf("Default LED: Off\n");
	}
    
    prev = tmp = gpio[Offset2MemIdx(GPLEV0)]&(1<<17);
    
    while (1){
        ch = get_key();
        if (ch == 'q'){
            LedOff(gpio, 4);
            printf("shut down program. \n");
            break;
        }
        
        prev = tmp;
        tmp = gpio[Offset2MemIdx(GPLEV0)]&(1<<17);
        
        if(tmp!=prev){
            if(tmp & (1<<17)){
                LedOn(gpio, 4);
                printf("set led on \n"); 
            }
            else{
                LedOff(gpio, 4);
                printf("set led off \n");    
            }
        }
    }

    munmap(addr_gpio, BLOCK_SIZE);
    close_keyboard();
    close(memfd);

    return 0;

}
