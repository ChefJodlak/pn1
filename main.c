#include <8051.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef unsigned int uint32_t;



unsigned char receivedCharacter;
unsigned char receivedFlag;

int getTH1ForFrequency(int frequency) {
    if(frequency == 19200) {
        return 0xfd;
    }
    return -1;
}

void init(int frequency)
{
    SCON = 0x50;
    TMOD &= 0x0f;
    TMOD |= 0x20;

    TCON = 0x40;
    PCON = 0x80;

    TH1 = getTH1ForFrequency(frequency); // th1 obliczamy z tej stronki tak zeby wyszlo 19200 jak w zadaniu
    TR1 = 1;
    TI = 0; // TI mowi o tym czy mozemy pisac czy czytac
    EA = 1;
    ES = 1;
    IE = 0x90;

    receivedFlag=0;
}

void put(unsigned char c)
{
    SBUF = c;
    while(!TI){};
    TI = 0;
}

// void put_DEC_U8(uint16_t w) {
//     int number = w;
//     int alreadyPrinted = 0;
//     int magnitude = 1;

//     put(' ');

//     while(alreadyPrinted < w) {
//         while(number >= 10) {
//             magnitude = magnitude * 10;
//             number = number / 10;
//         }
//         put(48 + number);
//         alreadyPrinted += magnitude * number;
//         number = w - alreadyPrinted;
//         magnitude = 1;
//     }

//     put(' ');

//     receivedCharacter = '\0';
// }

void put_BIN_U32(uint32_t w){
    
}

void get_DEC_U32(unsigned char *x)
{
    while (receivedCharacter != ' '){
        if(receivedFlag && (receivedCharacter <= '9' && receivedCharacter >= '0')) {
            *x = *x << 1;
            *x+= receivedCharacter - '0';
            receivedFlag = 0;
        }
    }
}

void catchInterrupt() interrupt 4
{
    if(RI)
    {
        receivedCharacter=SBUF;
        RI=0;
        receivedFlag=1;
    }
}

void main(void)
{
    uint32_t z1;
    uint32_t w;
    init(19200);
    for(;;) {
        get_DEC_U32(&z1);
        w = z1 -3;
        put_BIN_U32(w);
        z1 = 0; // po wypisaniu musimy wyzerowac zmienna
    }
}
