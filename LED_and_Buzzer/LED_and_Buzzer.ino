#include "pitches.h"
#define c4 554
#define e4 659
#define a3 440
#define b3 494
#define g4 831
#define a4 880

int freq = 2000;
int channel = 0;
int resolution = 8;
int tonePin =26;
int ledPin=16;
int channel1=1;

int melody[] = {
  a3,a3,a3,b3,a3,b3,c4,e4,c4,b3,a3,a3,
   a3,a3,a3,b3,a3,b3,a3,a4,g4,
 e4,c4,b3,b3,c4,b3,a3,b3,a3
};
int noteDurations[] = {
2,5,5,2,4,4,1,2,2,1,4,1,
2,5,5,2,4,4,2,2,1,
4,3,2,5,4,2,5,4
};
int dutycycle[] = {
  85,85,85,127,85,127,15,42,15,127,85,85,
   85,85,85,127,85,127,85,255,170,
 42,15,127,127,15,127,85,127,85
};
void setup() {
  Serial.begin(115200);
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(tonePin, channel);
  ledcSetup(channel,5000, resolution);
  ledcAttachPin(ledPin, channel1);
}

void loop() {
    for (int thisNote = 0; thisNote < 42; thisNote++) {
      ledcWrite(channel1,dutycycle[thisNote]);
      int noteDuration = 1000 / noteDurations[thisNote];
      ledcWriteTone(channel, melody[thisNote]);
      delay(noteDuration);
      ledcWriteTone(channel,0);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
    }
}
