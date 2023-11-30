#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <termios.h>

int Value = 0;

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
    printf("[u] : Count Up\n");
    printf("[d] : Count Down\n");
    printf("[p] : Count Setting\n");
    printf("[q] : Exit program\n");
    printf("--------------------- \n\n");
}

void displaySegment(int dev, int segIdx){
    int offset = 1;
    int targetSeg = 8;
    for(int i = 0; i < segIdx; i++){
        offset *= 10;
        targetSeg /= 2;
    }
    int segValue = (int)((double)Value / offset) % 10;
    int tmp = ((targetSeg) << 4) | segValue;
    //printf("%d %d\n", offset, tmp);
    write(dev, &tmp, tmp);
}

int main(int argc, char **argv)
{
    int devSeg = open("/dev/SevenSegment", O_RDWR); 
    int devBtn = open("/dev/SegmentButtons", O_RDWR); 
    
    char buff;
    char temp[2] = {0,0};
    char prev[2] = {0,0};

    if(devSeg == -1 || devBtn == -1){
        printf("Opening was not possible!\n");
        return -1;
    }

    init_keyboard();
    print_menu();
    
    int segIdx = 0;
    char ch;
    Value = 10;

    while (1){
        ch = get_key();

        switch(ch){
            case 'u':
                Value = Value + 1 < 10000 ? Value + 1 : 0;
            break;
            case 'd':
                Value = Value - 1 > 0 ? Value - 1 : 9999;
            break;
            case 'p':
            close_keyboard();
            printf("Set Number: ");
            scanf("%d", &Value);
            if(Value > 9999)
                Value = 9999;
            else if (Value < 0)
                Value = 0;
            init_keyboard();
            break;
            case 'q':
            printf("shut down program. \n");
            close(devSeg);
            close(devBtn);
            close_keyboard();
            return 0;
            break;
            default:
            break;
        }

        segIdx = (segIdx + 1) % 4;
        displaySegment(devSeg, segIdx);

        read(devBtn, &buff, 1);
        
        temp[0] = buff & 0x01;
        temp[1] = buff & 0x02;
        
        if(!temp[0] && temp[0] != prev[0])
            Value = Value + 1 < 10000 ? Value + 1 : 0;

        if(!temp[1] && temp[1] != prev[1])
            Value = Value - 1 > 0 ? Value - 1 : 9999;

        prev[0] = temp[0];
        prev[1] = temp[1];

        // prev = tmp;
        // tmp = gpio[Offset2MemIdx(GPLEV0)]&(1<<17);
        
        // if(tmp!=prev){
        //     if(tmp & (1<<17)){
        //         LedOn(gpio, 4);
        //         printf("set led on \n"); 
        //     }
        //     else{
        //         LedOff(gpio, 4);
        //         printf("set led off \n");    
        //     }
        // }
    }


    close(devSeg);
    close(devBtn);
    close_keyboard();
    return 0;
}
