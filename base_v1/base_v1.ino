/******************************************************************************
 *  File:        base_v1.ino
 *  Project:     Micro-StudioPLC - CLV-DIY-V1
 *  Author:      Jean-François Simon, Chrysalide Engineering
 *  Created:     6/12/2025
 *  Description: Micro-StudioPLC CLV-DIY-V1 Source Code - Open-Source
 *  Contact:     jfsimon@startmail.com
 *  License:     2025 Chrysalide Engineering — Licensed under the GNU General Public License v3.0
 ******************************************************************************/
 /*
 * Computer Conncected Digital Multi-Meter ref CLV-DIY-V1
 * Copyright (C) 2025  Jean-François Simon, Chrysalide Engineering
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this
 * program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
  TODO
  Clear Serial Link Buffer upon start before any r/w
  Blink LED Twice upon start
  Send Infos on Serial Link or
  Do not send (difff softw check)
  Robust Sw Hw detection, 3 times, auto detection
*/

#define VER "1.0"

void wait();
void blink (int, int, int); /// N, T_on[ms], T_off[ms]

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  blink(3, 70,50);
  
#if 1
  // Startup sequence & wait
  delay(1000);
  Serial.print("\r\nCLV-DIY-V1:");
  Serial.print(VER);
  Serial.print("\r\n");
  while (!Serial.available()) {
    delay(200);
    Serial.print(".");
  }
#endif
}

int mode = 1;

const char CMD_GEN_GNRD = 152; // g[n]rd Read GPIO [n]. Returns a boolean (8 bit with boolean at LSB)
const char CMD_GEN_GNRA = 160; // g[n]ra Read analog input [n]. Returns N bytes depending on ADC configuration.
const char CMD_EXTENDED = 255; // Extension. This is followed by the Payload command M56...M67 and an arbitrary Payload. User defined meaning.
const char CMD_INF_HWMCU = 224; // ihwmcu
// The MCU Manufacturer and type as defined in KRT/MSPLC specification
const char HW_MCU_MFR = 6;  // Arduino
const char HW_MCU_TYPE = 9; // UNO R4 Minima

void loop() {
  static int serial_in = 0;
  if (mode == 0) {
    wait();
  } else if (mode == 1) {
    char cin[8];
    for (int i = 0; i < 8; i++)
      cin[i] = 0;
    int cin_count = 0;
    while (Serial.available() > 0) {
      serial_in = 1;
      cin[cin_count] = Serial.read();
      if (++cin_count == 8)
        break;
    }

    if ((cin_count) && (cin[0] == CMD_GEN_GNRA)) {
      unsigned int analog_val = 0;
      {
        // Fixme : pins 1-6 : 0xff80 -> 0xff84
        // Masking. TODO: Check why an 8 bit quantity needs a 32bit LSB Mask to succeed.
        if ((cin[1] & 0x000F) == 0 /*0xff80*/) {
          analog_val = analogRead(PIN_A0);
        } else if ((cin[1] & 0x000F) == 1 /*0xff81*/) {
          analog_val = analogRead(PIN_A1);
        } else if ((cin[1] & 0x000F) == 2 /*0xff82*/) {
          analog_val = analogRead(PIN_A2);
        } else if ((cin[1] & 0x000F) == 3 /*0xff83*/) {
          analog_val = analogRead(PIN_A3);
        } else if ((cin[1] & 0x000F) == 4 /*0xff84*/) {
          analog_val = analogRead(PIN_A4);
        } else if ((cin[1] & 0x000F) == 5 /*0xff85*/) {
          analog_val = analogRead(PIN_A5);
        } else {
         // analog_val = 0;
         // analog_val = analogRead(PIN_A0);
         analog_val = cin[1];
        }
      }
      // Serial Writes
      Serial.write(CMD_GEN_GNRD);
      Serial.write(1);
      Serial.write(analog_val & 0xff);
      Serial.write((analog_val >> 8) & 0xff);
      Serial.write(CMD_EXTENDED);
      //
    } else if ((cin_count) && (cin[0] == CMD_GEN_GNRD)) {
      unsigned int digital_val = 0;
      {
        if (cin[1] == 0) {
          digital_val = digitalRead(PIN_A0);
        } else if ((cin[1] & 0x000F) == 1) {
          digital_val = digitalRead(PIN_A1);
        } else if ((cin[1] & 0x000F) == 2) {
          digital_val = digitalRead(PIN_A2);
        } else if ((cin[1] & 0x000F) == 3) {
          digital_val = digitalRead(PIN_A3);
        } else if ((cin[1] & 0x000F) == 4) {
          digital_val = digitalRead(PIN_A4);
        } else if ((cin[1] & 0x000F) == 5) {
          digital_val = digitalRead(PIN_A5);
        } else {
          digital_val = 0;
        }
      }
      // Serial Writes
      Serial.write(CMD_GEN_GNRD);
      Serial.write(1);
      Serial.write(digital_val & 0xff);
      Serial.write(CMD_EXTENDED);
      //
    } else if ((cin_count) && (cin[0] == CMD_INF_HWMCU)) {
      Serial.write(CMD_INF_HWMCU);
      Serial.write(CMD_EXTENDED);
      Serial.write(HW_MCU_MFR);
      Serial.write(HW_MCU_TYPE);
    } else if (cin_count) {
      // NOP
    }
  }
  // Serial.println("Analog Readback Ok"); // Debug
  if (serial_in) {
    serial_in = 0;
    blink (1, 1, 1);
  }
}

void blink (int n, int t1, int t2) {
  for (int i = 0; i < n; i++) {
      digitalWrite(LED_BUILTIN, 1);
      delay(t1);
      digitalWrite(LED_BUILTIN, 0);
      delay(t2);
  }
}

void wait() {
  digitalWrite(LED_BUILTIN, 1);
  delay(60);
  digitalWrite(LED_BUILTIN, 0);
  delay(60);
  digitalWrite(LED_BUILTIN, 1);
  delay(60);
  digitalWrite(LED_BUILTIN, 0);
  delay(1820);
}
