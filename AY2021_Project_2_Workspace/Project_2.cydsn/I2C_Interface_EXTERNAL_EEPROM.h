/* ========================================
 *
 * Laboratorio di Tecnologie Elettroniche e Biosensori
 * Politecnico di Milano
 * AA 2020-2021 - I semester
 *
 * Final Projects:
 * Project 2 
 * Authors: Daniela Garofalo, Benedetta Pedica, Davide Sala
 * Date: 10/01/2021
 *
 * ========================================
*/

/*
 * Header file for I2C interface for the External EEPROM
 *
 * Here are implemented all the functions that works as 
 * an interface to the I2C peripheral and the external EEPROM. 
 * Functions are structurally similar to the ones related with accelerometer, 
 * but the most important change lies in the dimensions of input data.
 *
*/

#ifndef I2C_Interface_EXTERNAL_EEPROM_H
    #define I2C_Interface_EXTERNAL_EEPROM_H
    
    #include "cytypes.h"
    #include "ErrorCodes.h"
    
    /* 
    This function starts the I2C peripheral so that it is ready to work
    */
    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_Start(void);
    
    
    /*
    This function stops the I2C peripheral from working
    */
    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_Stop(void);
    

    /*  
    This function performs a complete reading operation over I2C from a single
    register
    -device_address: I2C address of the device to talk to
    -register_address: address of the register to be read(uint16 because address HIGH byte + address LOW byte)
    -data: pointer to a variable where the byte will be saved
    */
    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_ReadRegister(uint8_t device_address, 
                                            uint16_t register_address,
                                            uint8_t* data);
    
    
    /*  
    This function performs a complete reading operation over I2C from multiple
    registers
    -device_address I2C address of the device to talk to
    -register_address: address of the first register to be read
    -register_count: number of registers we want to read
    -data: pointer to an array where data will be saved
    */ 
    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(uint8_t device_address,
                                                uint16_t register_address,
                                                uint32_t register_count,
                                                uint8_t* data);
    
    
    /*   
    This function performs a complete writing operation over I2C to a single 
    register.
    -device_address: I2C address of the device to talk to
    -register_address: address of the register to be written
    -data: data to be written
    */   
    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(uint8_t device_address,
                                            uint16_t register_address,
                                            uint8_t data);
    
    
    /*
    This function performs a complete writing operation over I2C to multiple
    registers
    -device_address: I2C address of the device to talk to
    -register_address: address of the first register to be written
    -register_count: number of registers that need to be written
    -data: array of data to be written
    */
    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(uint8_t device_address,
                                            uint16_t register_address,
                                            uint8_t register_count,
                                            uint8_t* data);
    
    
    /*
    This function checks if a device is connected over the I2C lines.
    -device_address: I2C address of the device to be checked
    -retval: Returns true (>0) if device is connected
    */
    uint8_t I2C_Peripheral_EXTERNAL_EEPROM_IsDeviceConnected(uint8_t device_address);
    
#endif // I2C_Interface_External_EEPROM_H
/* [] END OF FILE */

