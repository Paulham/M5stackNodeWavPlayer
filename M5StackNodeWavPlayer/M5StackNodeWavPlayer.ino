#include <WM8978.h> /* https://github.com/CelliesProjects/wm8978-esp32 */
#include <Audio.h>  /* https://github.com/schreibfaul1/ESP32-audioI2S */
#include <SD.h>
#include <FS.h>
#include <M5Stack.h>
#include "WiFi.h"

/* M5Stack Node WM8978 I2C pins */
#define I2C_SDA     21
#define I2C_SCL     22

/* M5Stack Node I2S pins */
#define I2S_BCK      5
#define I2S_WS      13
#define I2S_DOUT     2
#define I2S_DIN     34

/* M5Stack WM8978 MCLK gpio number */
#define I2S_MCLKPIN  0

WM8978 dac;
Audio audio(I2S_BCK,I2S_WS,I2S_DOUT,I2S_DIN);

int volume;

//Audio audio;

void listDir(fs::FS &fs, const char * dirname){
    M5.Lcd.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        M5.Lcd.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        M5.Lcd.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            M5.Lcd.print("  DIR : ");
            M5.Lcd.println(file.name());
        } else {
            M5.Lcd.print("  FILE: ");
            M5.Lcd.print(file.name());
            M5.Lcd.print("  SIZE: ");
            M5.Lcd.println(file.size());
        }
        file = root.openNextFile();
    }
}


void setup() {
  // Initialize the M5Stack object

  M5.begin();
  M5.Power.begin();
  Wire.begin();

  M5.Lcd.setBrightness(100);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  
  M5.Lcd.println("Paul's Music Player");

  /* Setup wm8978 I2C interface */
  if (!dac.begin(I2C_SDA, I2C_SCL)) {
    Serial.println("DAC not started");
    while (1) delay(1000);
  }

  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(BLUE);
  listDir(SD,"/");
  audio.i2s_mclk_pin_select(I2S_MCLKPIN);
  
  audio.connecttoFS(SD, "Alba.wav"); /* change for your file */

  volume = 40;
  
  dac.setSPKvol(volume); /* max 63 */
  dac.setHPvol(0, 0);
}


void loop() {
  
  audio.loop();

  M5.update();
  
  if(M5.BtnA.wasPressed()) {
     M5.Lcd.printf("Next track");
     }
  if(M5.BtnB.wasPressed()) {
     volume = volume + 5;
     if (volume >63) volume = 63;
     dac.setSPKvol(volume); 
     }
  if(M5.BtnC.wasPressed()) {
     volume = volume -5;
     if (volume <0) volume = 0;
     dac.setSPKvol(volume); 
     }
  
}

// optional
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
}
void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);
}
void audio_showstreaminfo(const char *info){
    Serial.print("streaminfo  ");Serial.println(info);
}
void audio_showstreamtitle(const char *info){
    Serial.print("streamtitle ");Serial.println(info);
}
void audio_bitrate(const char *info){
    Serial.print("bitrate     ");Serial.println(info);
}
void audio_commercial(const char *info){  //duration in sec
    Serial.print("commercial  ");Serial.println(info);
}
void audio_icyurl(const char *info){  //homepage
    Serial.print("icyurl      ");Serial.println(info);
}
void audio_lasthost(const char *info){  //stream URL played
    Serial.print("lasthost    ");Serial.println(info);
}
void audio_eof_speech(const char *info){
    Serial.print("eof_speech  ");Serial.println(info);
}
