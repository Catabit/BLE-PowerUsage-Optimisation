/**@file
 * @brief      ADXL345 Interface API
 */
 
 
#include "adxl345.h"
 
void adxl345_init(nrf_drv_spi_t spi)
{
	uint8_t range[4] = {DATA_FORMAT, 0x01, POWER_CTL, 0x08};
	APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, range, 4, NULL, 0));
	
	//uint8_t mode[2] = {POWER_CTL, 0x08};
	//APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, mode, 2, NULL, 0));
}

void adxl345_read_register(nrf_drv_spi_t spi, uint8_t registerAddress, uint8_t numBytes, uint8_t* result)
{
	uint8_t address = 0x80 | registerAddress; //
	if(numBytes > 1)address = address | 0x40;
	
	APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, &address, 1, result, numBytes));
}

void adxl345_read_values(nrf_drv_spi_t spi, int32_t* xyz)
{
	uint8_t values[10];
	adxl345_read_register(spi, DATAX0, 6, values);
	
	
	xyz[0] = ((int32_t)values[1]<<8)|(int32_t)values[0];
	xyz[1] = ((int32_t)values[3]<<8)|(int32_t)values[2];
	xyz[2] = ((int32_t)values[5]<<8)|(int32_t)values[4];
	//SEGGER_RTT_printf(0, "X:%d Y:%d Z:%d\n", (int8_t)values[1], (int8_t)values[3], (int8_t)values[5]);
}

/*
void setup(){ 
  //Initiate an SPI communication instance.
  SPI.begin();
  //Configure the SPI connection for the ADXL345.
  SPI.setDataMode(SPI_MODE3);
  //Create a serial connection to display the data on the terminal.
  Serial.begin(9600);
  
  //Set up the Chip Select pin to be an output from the Arduino.
  pinMode(CS, OUTPUT);
  //Before communication starts, the Chip Select pin needs to be set high.
  digitalWrite(CS, HIGH);
  
  //Put the ADXL345 into +/- 4G range by writing the value 0x01 to the DATA_FORMAT register.
  writeRegister(DATA_FORMAT, 0x01);
  //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
  writeRegister(POWER_CTL, 0x08);  //Measurement mode  
}

void loop(){
  //Reading 6 bytes of data starting at register DATAX0 will retrieve the x,y and z acceleration values from the ADXL345.
  //The results of the read operation will get stored to the values[] buffer.
  readRegister(DATAX0, 6, values);

  //The ADXL345 gives 10-bit acceleration values, but they are stored as bytes (8-bits). To get the full value, two bytes must be combined for each axis.
  //The X value is stored in values[0] and values[1].
  x = ((int)values[1]<<8)|(int)values[0];
  //The Y value is stored in values[2] and values[3].
  y = ((int)values[3]<<8)|(int)values[2];
  //The Z value is stored in values[4] and values[5].
  z = ((int)values[5]<<8)|(int)values[4];
  
  //Print the results to the terminal.
  Serial.print(x, DEC);
  Serial.print(',');
  Serial.print(y, DEC);
  Serial.print(',');
  Serial.println(z, DEC);      
  delay(10); 
}

//This function will write a value to a register on the ADXL345.
//Parameters:
//  char registerAddress - The register to write a value to
//  char value - The value to be written to the specified register.
void writeRegister(char registerAddress, char value){
  //Set Chip Select pin low to signal the beginning of an SPI packet.
  digitalWrite(CS, LOW);
  //Transfer the register address over SPI.
  SPI.transfer(registerAddress);
  //Transfer the desired register value over SPI.
  SPI.transfer(value);
  //Set the Chip Select pin high to signal the end of an SPI packet.
  digitalWrite(CS, HIGH);
}

//This function will read a certain number of registers starting from a specified address and store their values in a buffer.
//Parameters:
//  char registerAddress - The register addresse to start the read sequence from.
//  int numBytes - The number of registers that should be read.
//  char * values - A pointer to a buffer where the results of the operation should be stored.

void readRegister(char registerAddress, int numBytes, unsigned char * values){
  //Since we're performing a read operation, the most significant bit of the register address should be set.
  char address = 0x80 | registerAddress;
  //If we're doing a multi-byte read, bit 6 needs to be set as well.
  if(numBytes > 1)address = address | 0x40;
  
  //Set the Chip select pin low to start an SPI packet.
  digitalWrite(CS, LOW);
  //Transfer the starting register address that needs to be read.
  SPI.transfer(address);
  //Continue to read registers until we've read the number specified, storing the results to the input buffer.
  for(int i=0; i<numBytes; i++){
    values[i] = SPI.transfer(0x00);
  }
  //Set the Chips Select pin high to end the SPI packet.
  digitalWrite(CS, HIGH);
}
*/
