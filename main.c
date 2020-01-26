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

void put_BIN_U32(uint32_t w)
{
  char bin[32];
  memset(bin, 0x00, sizeof(bin));

  int idx = 0;
  int rest = 0;
  while ( w > 0 ) {
    rest = w % 2;
    w = ( w - rest ) / 2;

    bin[idx] = rest;
    ++idx;
  }

  put(' ');
  idx = 0;
  // drukuję od najbardziej znaczącego bitu
  while (idx < 32) {
    put('0' + bin[idx]);
    idx++;
  }
  put(' ');

  receivedCharacter = '\0';
}

void get_DEC_U32(unsigned char *x)
{
  while (receivedCharacter != ' '){
    if(receivedFlag && (receivedCharacter <= '9' && receivedCharacter >= '0')) {
      *x = *x * 10;
      *x += receivedCharacter - '0';
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
    w = z1 - 3;
    put_BIN_U32(w);
    z1 = 0; // po wypisaniu musimy wyzerowac zmienna
  }
}
