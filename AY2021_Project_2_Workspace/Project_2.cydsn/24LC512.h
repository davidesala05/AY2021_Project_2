/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "I2C_Interface.h"
#include "Global.h"

#define EEPROM_EXTERNAL_ADDRESS 0x50

#ifndef EEPROM_Interface_H
    #define EEPROM_Interface _H
    
    #include "cytypes.h"
    #include "ErrorCodes.h"
    
ErrorCode I2C_Peripheral_ReadEEPROMRegisterMulti(uint8_t device_address,
                                            uint8_t register_address_high,
                                            uint8_t register_address_low,
                                            uint8_t register_count,
                                            uint8_t* data);
#endif
/* [] END OF FILE */
