/*******************************************************************************

 Bare Conductive Touch MP3 player
 ------------------------------

 Touch_MP3.ino - touch triggered MP3 playback

 You need twelve MP3 files named TRACK000.mp3 to TRACK011.mp3 in the root of the
 microSD card.

 When you touch electrode E0, TRACK000.mp3 will play. When you touch electrode
 E1, TRACK001.mp3 will play, and so on.

  SD card
  │
    Inhabitant nb 0
    TRACK000.mp3 - file 0
    TRACK001.mp3 - file 1
    TRACK002.mp3 - file 2
    Inhabitant nb 1
    TRACK010.mp3 - file 0
    TRACK011.mp3 - file 1
    TRACK002.mp3 - file 2
    Inhabitant nb 2
    TRACK020.mp3 - file 0
    TRACK021.mp3 -file 1
    Inhabitant nb 3
    TRACK020.mp3 - file 0
    Inhbitant nb 4
    TRACK040.mp3 - file 0
    TRACK041.mp3 - file 1
    TRACK042.mp3 - file 2 

 Based on code by Jim Lindblom and plenty of inspiration from the Freescale
 Semiconductor datasheets and application notes.

 Bare Conductive code written by Stefan Dzisiewski-Smith, Peter Krige, Pascal
 Loose

 This work is licensed under a MIT license https://opensource.org/licenses/MIT

 Copyright (c) 2016, Bare Conductive

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

*******************************************************************************/

// compiler error handling
#include "Compiler_Errors.h"

// touch includes
#include <MPR121.h>
#include <MPR121_Datastream.h>
#include <Wire.h>

// MP3 includes
#include <SPI.h>
#include <SdFat.h>
#include <FreeStack.h>
#include <SFEMP3Shield.h>

// Il a 2 machines correspondants à 2 contenus de mp3
// Une machine principale et machine secondaire
// Commenter cette ligne pour la machine secondaire.
#define MAIN_MACHINE

// !!!! Volume is inversed
// VOLUME 10 means maximum volume
// VOLUME 240 means silence
#define VOLUME 35

// touch constants
const uint32_t BAUD_RATE = 115200;
const uint8_t MPR121_ADDR = 0x5C;
const uint8_t MPR121_INT = 4;

// serial monitor behaviour constants
const bool WAIT_FOR_SERIAL = false;

// MPR121 datastream behaviour constants
const bool MPR121_DATASTREAM_ENABLE = false;

// MP3 variables
uint8_t result;
uint8_t lastPlayed = 0;


// MP3 constants
SFEMP3Shield MP3player;

// MP3 behaviour constants
const bool REPLAY_MODE = true;  // by default, touching an electrode repeatedly will
                                // play the track again from the start each time
                                //
                                // if you set this to false, repeatedly touching an
                                // electrode will stop the track if it is already
                                // playing, or play it from the start if it is not

// SD card instantiation
SdFat sd;

// Nb of files 
#ifdef MAIN_MACHINE
// Machine principale ( MAIN MACHINE )
const int nbFilePerInhabitant[5] = { 3 , 1, 2, 3, 4 };
#else
// Machine secondaire
const int nbFilePerInhabitant[5] = { 2 , 1, 2, 2, 3 };
#endif


// Actual Index
int indexPerInhabitant[5] = { 0, 0, 0, 0, 0};

// Threshold : ability to trigger a track
int treshold_touch = 80;


void playFilePerInhabitant( int indexInhabitant ){

  if(indexInhabitant >= 0 && indexInhabitant< 5){

    

    int i = indexInhabitant*10 + indexPerInhabitant[indexInhabitant];
    Serial.print("Playing the file number");
    Serial.println(i);
    MP3player.playTrack(i);

    int newIndexFile = indexPerInhabitant[indexInhabitant] + 1;
    if(newIndexFile >= nbFilePerInhabitant[indexInhabitant]){
      newIndexFile= 0;
    }
    indexPerInhabitant[indexInhabitant] = newIndexFile;
  }
  
  
}

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(LED_BUILTIN, OUTPUT);

  if (WAIT_FOR_SERIAL) {
    while (!Serial);
  }

  if (!sd.begin(SD_SEL, SPI_HALF_SPEED)) {
    sd.initErrorHalt();
  }

  if (!MPR121.begin(MPR121_ADDR)) {
    Serial.println("error setting up MPR121");
    switch (MPR121.getError()) {
      case NO_ERROR:
        Serial.println("no error");
        break;
      case ADDRESS_UNKNOWN:
        Serial.println("incorrect address");
        break;
      case READBACK_FAIL:
        Serial.println("readback failure");
        break;
      case OVERCURRENT_FLAG:
        Serial.println("overcurrent on REXT pin");
        break;
      case OUT_OF_RANGE:
        Serial.println("electrode out of range");
        break;
      case NOT_INITED:
        Serial.println("not initialised");
        break;
      default:
        Serial.println("unknown error");
        break;
    }
    while (1);
  }

  MPR121.setInterruptPin(MPR121_INT);

  if (MPR121_DATASTREAM_ENABLE) {
    MPR121.restoreSavedThresholds();
    MPR121_Datastream.begin(&Serial);
  } else {
    MPR121.setTouchThreshold(treshold_touch);
    MPR121.setReleaseThreshold(treshold_touch/2);
  }

  MPR121.setFFI(FFI_10);
  MPR121.setSFI(SFI_10);
  MPR121.setGlobalCDT(CDT_4US);  // reasonable for larger capacitances
  
  digitalWrite(LED_BUILTIN, HIGH);  // switch on user LED while auto calibrating electrodes
  delay(1000);
  MPR121.autoSetElectrodes();  // autoset all electrode settings
  digitalWrite(LED_BUILTIN, LOW);

  result = MP3player.begin();
  MP3player.setVolume(VOLUME, VOLUME);

  if (result != 0) {
    Serial.print("Error code: ");
    Serial.print(result);
    Serial.println(" when trying to start MP3 player");
  }
}

void loop() {
  MPR121.updateAll();

  // only make an action if we have one or fewer pins touched
  // ignore multiple touches
  if (MPR121.getNumTouches() <= 1) {
    for (int i=0; i < 5; i++) {  // check which electrodes were pressed
      if (MPR121.isNewTouch(i)) {
          if (!MPR121_DATASTREAM_ENABLE) {
            Serial.print("pin ");
            Serial.print(i);
            Serial.println(" was just touched");
          }

          digitalWrite(LED_BUILTIN, HIGH);

          if (i <= 11 && i >= 0) {
            if (MP3player.isPlaying()) { 
              if (lastPlayed == i && !REPLAY_MODE) {
                // if we're already playing the requested track, stop it
                // (but only if we're not in REPLAY_MODE)
                MP3player.stopTrack();

                if (!MPR121_DATASTREAM_ENABLE) {
                  Serial.print("stopping track ");
                  Serial.println(i-0);
                  
                }
              } else {
                // if we're already playing a different track (or we're in
                // REPLAY_MODE), stop and play the newly requested one
                MP3player.stopTrack();
                //MP3player.playTrack(i-0);
                playFilePerInhabitant(i);

                if (!MPR121_DATASTREAM_ENABLE) {
                  Serial.print("playing track ");
                  Serial.println(i-0);
                }

                lastPlayed = i;
              }
            } else {
              // if we're playing nothing, play the requested track
              //MP3player.playTrack(i-0);
              playFilePerInhabitant(i);

              if (!MPR121_DATASTREAM_ENABLE) {
                Serial.print("playing track ");
                Serial.println(i-0);
              }

              lastPlayed = i;
            }
          }
      } else {
        if (MPR121.isNewRelease(i)) {
          if (!MPR121_DATASTREAM_ENABLE) {
            Serial.print("pin ");
            Serial.print(i);
            Serial.println(" is no longer being touched");
          }

          digitalWrite(LED_BUILTIN, LOW);
        }
      }
    }
  }

  if (MPR121_DATASTREAM_ENABLE) {
    MPR121_Datastream.update();
  }
}
