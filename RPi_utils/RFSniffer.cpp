/*
  RFSniffer

  Usage: ./RFSniffer [<pulseLength>]
  [] = optional

  Hacked from http://code.google.com/p/rc-switch/
  by @justy to provide a handy RF code sniffer
*/

#include "../rc-switch/RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  if (argv[1] == NULL)
  {
    printf("PIN is required");
    return 1;
  }
  int PIN = atoi(argv[1]);

  if (wiringPiSetup() == -1)
  {
    printf("wiringPiSetup failed, exiting...");
    return 0;
  }

  int sleepTime = 100;
  if (argv[2] != NULL)
  {
    sleepTime = atoi(argv[2]);
  }

  int pulseLength = 0;
  if (argv[3] != NULL)
  {
    pulseLength = atoi(argv[3]);
  }

  setbuf(stdout, NULL);

  RCSwitch mySwitch = RCSwitch();
  if (pulseLength != 0)
  {
    mySwitch.setPulseLength(pulseLength);
  }
  mySwitch.enableReceive(PIN); // Receiver on interrupt 0 => that is pin #2

  while (1)
  {
    if (mySwitch.available())
    {
      unsigned long value = mySwitch.getReceivedValue();
      if (value != 0)
      {
        printf("%lu\n", value);
      }
      // fflush(stdout);
      mySwitch.resetAvailable();
    }
    usleep(sleepTime); // Sleep 100ms
  }
  exit(0);
}
