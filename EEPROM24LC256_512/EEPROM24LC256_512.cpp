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

#ifndef EEPROM24LC256_512_C
#define EEPROM24LC256_512_C

#include "Arduino.h"
#include <Wire.h>
#include "EEPROM24LC256_512.h"

EEPROM256_512::EEPROM256_512()
{
	uint8_t i=0;
	for(i=0;i<MINIBUFFER;i++){minibuffer[i]=0b00000000;}
	page_size = 256;
	fulladdr=I2CBASEADDR;
}

EEPROM256_512::~EEPROM256_512(){/*Nothing to deconstruct*/}

void EEPROM256_512::begin(uint8_t addr, uint8_t mode)
{
	if(addr > 7) addr=7;
    fulladdr = I2CBASEADDR|addr;
	if(mode==1)
	{
		page_size=PAGE512;
	}
	else
	{
		//assume default as 256
		page_size=PAGE256;
	}
}//end begin

int EEPROM256_512::getPageSize()
{
	return page_size;
}//end get page size

uint8_t EEPROM256_512::readByte(unsigned int addr)
{
	uint8_t data = 0xFF;//set return byte to 0xFF or (255 DEC) or (11111111 BIN)
    Wire.beginTransmission(fulladdr);//begin transmission on the wire
    //32768 bytes... addressed by 2 bytes (16bits)
    Wire.write((int)(addr >> 8));   // MSB write upper 8 bits to wire (by bit shifting to right)
    Wire.write((int)(addr & 0xFF)); // LSB write lower 8 bits to wire (by bit & by 0xFF)
    Wire.endTransmission();//end the transmission on the wire
    Wire.requestFrom((uint8_t)fulladdr,(uint8_t)1);//request 1 byte from EEPROM (memory location sent above)
    if (Wire.available()) data = Wire.read();
    return data;
}//end readByte

void EEPROM256_512::writeByte(unsigned int addr, uint8_t data)
{
	Wire.beginTransmission(fulladdr);//begin transmission on wire
    //32768 bytes... addressed by 2 bytes (16bits)
    Wire.write((int)(addr >> 8));   // MSB write upper 8 bits to wire (by bit shifting to right)
    Wire.write((int)(addr & 0xFF)); // LSB write lower 8 bits to wire (by bit & by 0xFF)
    Wire.write(data);//write data byte
    Wire.endTransmission();//end transmission on wire
    delay(5);//wait max write time of chip
}//end writeByte

void EEPROM256_512::readPage(unsigned int page_number, uint8_t* data)
{
	//assuming that the char* data is the size of a page
    uint8_t index = 0;
    unsigned int lower_addr=0;
    unsigned int addr_base = 0;
    uint8_t itr=0;
    uint8_t offset = 0;
    
    if(page_number < 0){page_number=0;}   
    else if(page_number > 511)
    {page_number = 511;}
	
    addr_base = page_number * page_size;
    for(itr=0;itr<((page_size)/(MINIBUFFER)); itr++)
    {
		offset = (MINIBUFFER*itr);
		lower_addr = addr_base + offset;
		read16Byte(lower_addr);
		for(index=offset;index<((MINIBUFFER)+offset);index++)
		{
			data[index] = minibuffer[(index-offset)];
		}//end copy loop
    }//end itr loop
}//end readPage

void EEPROM256_512::writePage(unsigned int page_number, uint8_t* data)
{
	//assuming the data* is size of a page
    uint8_t index = 0;
    unsigned int lower_addr=0;
    unsigned int addr_base=0;
    uint8_t itr = 0;
    uint8_t offset = 0;

    //check page out of bounds of chip memory
    if(page_number < 0)
    {page_number=0;}   
    else if(page_number > 511)
    {page_number = 511;}
    
    addr_base = page_number * page_size;
    
    for(itr=0;itr<((page_size)/(MINIBUFFER)); itr++)
    {
		offset = (MINIBUFFER*itr);
		lower_addr = addr_base + offset;
		for(index=offset;index<((MINIBUFFER)+offset);index++)
		{
			minibuffer[(index-offset)] = data[index];
		}//end copy loop 
		write16Byte(lower_addr);
	}//end itr loop
}//end writePage

void EEPROM256_512::read16Byte(unsigned int addr)
{
    uint8_t ct = 0;
    //assuming char* data is 16 byte buffer
    Wire.beginTransmission(fulladdr);//begin transmission on the wire
    //32768 bytes... addressed by 2 bytes (16bits)
    Wire.write((int)(addr >> 8));   // MSB write upper 8 bits to wire (by bit shifting to right)
    Wire.write((int)(addr & 0xFF)); // LSB write lower 8 bits to wire (by bit & by 0xFF)
    Wire.endTransmission();//end the transmission on the wire
    	
    Wire.requestFrom((uint8_t)fulladdr,(uint8_t)16);//request 1 byte from EEPROM (memory location sent above)
    while (Wire.available())
    {
        minibuffer[ct] = Wire.read();
        ct++;
    }
}//end read16Byte

void EEPROM256_512::write16Byte(unsigned int addr)
{
	uint8_t ct=0;
    Wire.beginTransmission(fulladdr);//begin transmission on wire
    //32768 bytes... addressed by 2 bytes (16bits)
    Wire.write((int)(addr >> 8));   // MSB write upper 8 bits to wire (by bit shifting to right)
    Wire.write((int)(addr & 0xFF)); // LSB write lower 8 bits to wire (by bit & by 0xFF)
    while(ct<16)
    {
		Wire.write((uint8_t)minibuffer[ct]);
		ct++;
    }  
    Wire.endTransmission();
	delay(5);
}//end write16Byte
#endif