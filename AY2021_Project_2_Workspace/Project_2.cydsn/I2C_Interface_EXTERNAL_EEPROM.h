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

#ifndef _I2C_Interface_EXTERNAL_EEPROM_H_
    
    #define _I2C_Interface_EXTERNAL_EEPROM_H_
    
    // Include related to all the cytypes functions 
    #include "cytypes.h"
    
    // Include related to the functions implemented into the actual project
    #include "ErrorCodes.h"
    #include "I2C_Master.h"
    
    
    /* 
    This function starts the I2C peripheral so that it is ready to work
    
    It is needed to initiate the communication between the components
    according to the implementation of the transmission protocol -->
    this command is the first one send
    */
    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_Start(void);
    
    
    /*
    This function stops the I2C peripheral from working
    
    It is needed to stop the communication between the components
    according to the implementation of the transmission protocol -->
    this command is the last one send
    */
    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_Stop(void);
    

    /*  
    This function performs a complete reading operation over I2C from a single
    register
    - device_address: I2C address of the device to talk to
    - register_address: address of the register to be read (uint16 because address HIGH byte + 
      address LOW byte)
    - data: pointer to a variable where the byte will be saved
    
    It is needed when a single reading operation has to be done
    depending on the dimension of the data --> the implementation
    of the I2C communication protocol for the reading operations
    expects:
    1) Send the START condition
    2) Writing the address of the register to be read
    3) Send the RESTART condition
    4) Read the data
    5) Send the NACK (no acknowledgement) 
    6) Send the STOP condition
    
    The transmission protocol is slightly different from the one used to 
    communicate with the accelerometer component because the external EEPROM 
    is characterised by a register address that is 2 bytes long
    */
    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_ReadRegister(uint8_t device_address, 
                                            uint16_t register_address,
                                            uint8_t* data);
    
    
    /*  
    This function performs a complete reading operation over I2C from multiple
    registers
    - device_address I2C address of the device to talk to
    - register_address: address of the first register to be read
    - register_count: number of registers we want to read
    - data: pointer to an array where data will be saved
    
    It is needed when a multi-reading operation has to be done
    depending on the dimension of the data --> the implementation
    of the I2C communication protocol for the multi-reading operations
    expects:
    1) Send the START condition
    2) Writing the address of the register to be read
    3) Send the RESTART condition
    4) Read the data
    5) Send the ACK (acknowledgement)
    6) Read the last information
    7) Send the NACK (no acknowledgement)
    8) Send the STOP condition
    
    The transmission protocol is slightly different from the one used to 
    communicate with the accelerometer component because the external EEPROM 
    is characterised by a register address that is 2 bytes long
    */  
    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(uint8_t device_address,
                                                uint16_t register_address,
                                                uint32_t register_count,
                                                uint8_t* data);
    
    
    /*   
    This function performs a complete writing operation over I2C to a single 
    register.
    - device_address: I2C address of the device to talk to
    - register_address: address of the register to be written
    - data: data to be written
    
    It is needed when a single writing operation has to be done
    depending on the dimension of the data --> the implementation
    of the I2C communication protocol for the writing operations
    expects:
    1) Send the START condition
    2) Writing the address of the register to be write
    3) Write the data
    4) Send the NACK (no acknowledgement) 
    5) Send the STOP condition
    
    The transmission protocol is slightly different from the one used to 
    communicate with the accelerometer component because the external EEPROM 
    is characterised by a register address that is 2 bytes long
    */
    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(uint8_t device_address,
                                            uint16_t register_address,
                                            uint8_t data);
    
    
    /*
    This function performs a complete writing operation over I2C to multiple
    registers
    - device_address: I2C address of the device to talk to
    - register_address: address of the first register to be written
    - register_count: number of registers that need to be written
    - data: array of data to be written
    
    It is needed when a multi-writing operation has to be done
    depending on the dimension of the data --> the implementation
    of the I2C communication protocol for the multi-writing operations
    expects:
    1) Send the START condition
    2) Writing the address of the register to be read
    3) Write all the data
    4) Send the STOP condition
    
    The transmission protocol is slightly different from the one used to 
    communicate with the accelerometer component because the external EEPROM 
    is characterised by a register address that is 2 bytes long
    */
    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(uint8_t device_address,
                                            uint16_t register_address,
                                            uint8_t register_count,
                                            uint8_t* data);
    
    
    /*
    This function checks if a device is connected over the I2C lines.
    - device_address: I2C address of the device to be checked
    - retval: Returns true (>0) if device is connected
    
    It is necessary to control if there is a proper connection between
    the components so it is possible to start the communication
    */
    uint8_t I2C_Peripheral_EXTERNAL_EEPROM_IsDeviceConnected(uint8_t device_address);
    
#endif // _I2C_Interface_External_EEPROM_H_

/* [] END OF FILE */

