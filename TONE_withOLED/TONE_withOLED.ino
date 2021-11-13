#include <Wire.h>
#include "SSD1306Wire.h"
#include "pictures.h"
#include "pitches.h"

#define c4 554
#define e4 659
#define a3 440
#define b3 494
#define g4 831
#define a4 880

int melody[] = {
 a3,a3,a3,b3,a3,b3,c4,e4,c4,b3,a3,a3,
 a3,a3,a3,b3,a3,b3,a3,a4,g4,
 e4,c4,b3,b3,c4,b3,a3,b3,a3
};
SSD1306Wire  display(0x3c, 21, 22);

void Drawbackground(){
  display.drawXbm(0, 0, bg_width, bg_height, pic[0]);
  display.display();
}//畫出背景
void Drawvirus(int b_x,int b_y){
    display.drawXbm(b_x, b_y, virus_width, virus_height, pic[1]);
    display.display();
}//畫出角色

int freq = 2000;
int channel = 0;
int resolution = 8;
int tonePin = 12;
int buttomPin=17;
int noteDurations[] = {
2,5,5,2,4,4,1,2,2,1,4,1,
2,5,5,2,4,4,2,2,1,
4,3,2,5,4,2,5,4
};

unsigned long currentTime = 0, tonePreviousTime = 0, picPreviousTime = 0;
int pauseBetweenNotes = 0;
int noteDuration = 0;
int thisNote = 0;
boolean nextTone = true;
int dropY = -virus_height;
boolean nextPic = true;

void setup() {
   Serial.begin(115200);
   ledcSetup(channel, freq, resolution);
   ledcAttachPin(tonePin, channel);

   //OLED setting
   display.init();
   display.flipScreenVertically();
   display.clear(); //清除螢幕和緩衝區(clear)
   pinMode(buttomPin,INPUT);
}

void loop() {
    //buzzer
    if(nextTone){
      noteDuration = 1300 / noteDurations[thisNote];
      ledcWriteTone(channel, melody[thisNote]);
      pauseBetweenNotes = noteDuration * 1.3;
      nextTone = false;
    }
  
    //畫出角色與其位置
    if(nextPic){
        display.clear();
        Drawbackground(); //畫出背景
        Drawvirus(64 - virus_width / 2, dropY); //畫出角色
        
        if(digitalRead(buttomPin)==HIGH){
        dropY = dropY;
      }
        else{
        dropY = dropY + 3;
        }
        if(dropY > 64) dropY = 0 - virus_height;
        nextPic = false;
    }

    currentTime = millis(); //記錄現在時間
    if((currentTime - tonePreviousTime) > noteDuration){
      ledcWriteTone(channel, 0);
    } //buzzer停止播放
    if((currentTime - tonePreviousTime) > (pauseBetweenNotes)){
      nextTone = true;
      thisNote++;
      if(thisNote == 10) thisNote = 0;
      tonePreviousTime = currentTime; 
    } //buzzer播放下一個音
    if((currentTime - picPreviousTime) > 150 ){
       nextPic = true;
       picPreviousTime = currentTime;
    } //角色每150毫秒移動一次
}
