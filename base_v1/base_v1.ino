/******************************************************************************
 *  File:        base_v1.ino
 *  Project:     Micro-StudioPLC - CLV-DIY-V1
 *  Author:      Jean-François Simon, Chrysalide Engineering
 *  Created:     6/12/2025
 *  Description: Micro-StudioPLC CLV-DIY-V1 Source Code - Open-Source
 *  Contact:     jfsimon@startmail.com
 *
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

#define VER "1.0"

void wait();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // Serial init
  Serial.begin(115200);
  delay(1000);
  Serial.print("\r\nCLV-DIY-V1:");
  Serial.print(VER);
  Serial.print("\r\n");
}

int mode = 0;

void loop() {
  if (mode == 0)
    wait();
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