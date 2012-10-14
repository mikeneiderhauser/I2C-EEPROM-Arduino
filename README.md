I2C-EEPROM-Arduino
==================

Interface Library from Arduino V1.X to Microchip 24LC256 or 24LC512 EEPROM

/*************************************************** 
  This is a library for the 24LC256 & 24LC512 i2c EEPROM under the
  Arduino Platform.
  
  It provides an easy way to use the above chips while supplying 
  the functionality to read and write pages (64 or 128 bytes) and 
  individual bytes to a page number or address on the EEPROM.
  
  Each page is written in a 16 byte burst mode to account for the
  buffer limit of the Wire library in the Arduino Platform.

  This library is to be used "as-is" without warranty. Please report
  bugs to the GitHub Project.
  
  Written by Michael Neiderhauser. October 10, 2012
  LGPL license, all text above must be included in any redistribution
 ****************************************************/

Install Notes:
View the README file in the EEPROM24LC256_512 Folder for more details.
Copy the EEPROM24LC256_512 Folder into the Arduino libraries folder.

This Library was only tested on Arduino IDE v1.0.1

Check the datasheet for more info on 24LC256: http://ww1.microchip.com/downloads/en/DeviceDoc/21203R.pdf
Check the datasheet for more info on 24LC512: http://ww1.microchip.com/downloads/en/DeviceDoc/21754M.pdf