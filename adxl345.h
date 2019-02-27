/**@file
 * @brief      ADXL345 Interface API
 */
 
#include "nrf_drv_spi.h"
#include "SEGGER_RTT.h"
#include "app_util_platform.h"

#define SPI_CS_PIN   4  /**< SPI CS Pin.*/

#define SPI_INSTANCE  0 /**< SPI instance index. */

#define POWER_CTL  0x2D	//Power Control Register
#define DATA_FORMAT  0x31
#define DATAX0  0x32	//X-Axis Data 0
#define DATAX1  0x33	//X-Axis Data 1
#define DATAY0  0x34	//Y-Axis Data 0
#define DATAY1  0x35	//Y-Axis Data 1
#define DATAZ0  0x36	//Z-Axis Data 0
#define DATAZ1  0x37	//Z-Axis Data 1


void adxl345_init(nrf_drv_spi_t spi);
void adxl345_read_register(nrf_drv_spi_t spi, uint8_t registerAddress, uint8_t numBytes, uint8_t* result);
void adxl345_read_values(nrf_drv_spi_t spi, int32_t* xyz);