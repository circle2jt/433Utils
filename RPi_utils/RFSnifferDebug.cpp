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
  mySwitch.enableReceive(PIN);

  unsigned long pulseHigh = 0;
  unsigned long pulseLow = 0;
  unsigned long pulseLengthSum = 0;
  unsigned long pulseCount = 0;
  unsigned long value = 0;

  while (1)
  {
    if (mySwitch.available())
    {
      value = mySwitch.getReceivedValue();
      if (value != 0)
      {
        printf("Code: %lu\n", value);
        pulseHigh = pulseIn(PIN, HIGH);
        pulseLow = pulseIn(PIN, LOW);
        pulseLength = (pulseHigh + pulseLow) / 2;
        pulseLengthSum += pulseLength;
        pulseCount++;
        unsigned long averagePulseLength = pulseLengthSum / pulseCount;
        int bitLength = mySwitch.getReceivedBitlength();
        int protc = mySwitch.getReceivedProtocol();
        unsigned int delay = mySwitch.getReceivedDelay();
        
        printf("Pulse High Duration (microseconds): %lu\n", pulseHigh);
        printf("Pulse Low Duration (microseconds): %lu\n", pulseLow);
        printf("Pulse Length (microseconds): %lu\n", pulseLength);
        printf("Average Pulse Length (microseconds): %lu\n", averagePulseLength);
        printf("Average Pulse Length (microseconds): %lu\n", averagePulseLength);
        printf("Bit Length: %d\n", bitLength);
        printf("Protocol: %d\n", protc);
        
      }
      // fflush(stdout);
      mySwitch.resetAvailable();
    }
    usleep(sleepTime); // Sleep 100ms
  }
  exit(0);
}
