/* ========================================
 *
 * Laboratorio di Tecnologie Elettroniche e Biosensori
 * Politecnico di Milano
 * AY 2020/2021 - I semester
 *
 * Final Projects:
 * - Project 2 - 
 * Authors: Daniela Garofalo, Benedetta Pedica, Davide Sala
 * Date: 10/01/2021
 *
 * ========================================
*/


/*
* This file includes all the required source code for the External EEPROM
* to interface the I2C peripheral.
*/


#ifndef DEVICE_CONNECTED
    // Definition of the value returned if the device is present on the I2C bus
    #define DEVICE_CONNECTED        1
#endif

#ifndef DEVICE_UNCONNECTED
    // Definition of the value returned if the device is NOT present on the I2C bus
    #define DEVICE_UNCONNECTED      0 
#endif

// Include related to the header file related to the functions defined in this file
#include "I2C_Interface_ACC.h" 


/*
This function starts the I2C peripheral so that it is ready to work.

It is needed to initiate the communication between the components
according to the implementation of the transmission protocol -->
this command is the first one send
*/
ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_Start(void) 
{
    // Start I2C peripheral
    I2C_Master_Start();  
    
    // Return no error since start function does not return any error
    return NO_ERROR;
} // I2C_Peripheral_EXTERNAL_EEPROM_Start()


/*
This function stops the I2C peripheral from working

It is needed to stop the communication between the components
according to the implementation of the transmission protocol -->
this command is the last one send
*/
ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_Stop(void)
{
    // Stop I2C peripheral
    I2C_Master_Stop();
    
    // Return no error since stop function does not return any error
    return NO_ERROR;
} // I2C_Peripheral_EXTERNAL_EEPROM_Stop()


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
                                        uint8_t* data)
{
    // Send START condition
    uint8_t error = I2C_Master_MasterSendStart(device_address,I2C_Master_WRITE_XFER_MODE);
    if (error == I2C_Master_MSTR_NO_ERROR)
    {
        uint8 reg = register_address >> 8;
        // Write register address HIGH (because address is divided in HIGH byte and LOW byte)
        error = I2C_Master_MasterWriteByte(reg);
        if (error == I2C_Master_MSTR_NO_ERROR)
        {
            reg = register_address;
            // Write register address LOW
            error = I2C_Master_MasterWriteByte(reg);
            if (error == I2C_Master_MSTR_NO_ERROR)
            {
                // Send RESTART condition
                error = I2C_Master_MasterSendRestart(device_address, I2C_Master_READ_XFER_MODE);
                if (error == I2C_Master_MSTR_NO_ERROR)
                {
                    // Read data without acknowledgement
                    *data = I2C_Master_MasterReadByte(I2C_Master_NAK_DATA);
                }
            }
        }
    }
    
    // Send STOP condition
    I2C_Master_MasterSendStop();
    
    // Return error code
    return error ? ERROR : NO_ERROR;
} // I2C_Peripheral_EXTERNAL_EEPROM_ReadRegister()


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
                                            uint8_t* data)
{
    // Send START condition
    uint8_t error = I2C_Master_MasterSendStart(device_address,I2C_Master_WRITE_XFER_MODE);
    if (error == I2C_Master_MSTR_NO_ERROR)
    {
        uint8 reg = register_address >> 8;
        
        // Write register address HIGH
        error = I2C_Master_MasterWriteByte(reg);
        if (error == I2C_Master_MSTR_NO_ERROR)
        {
            reg = register_address;
            
            // Write register address LOW
            error = I2C_Master_MasterWriteByte(reg);
            if (error == I2C_Master_MSTR_NO_ERROR)
            {
                // Send RESTART condition
                error = I2C_Master_MasterSendRestart(device_address, I2C_Master_READ_XFER_MODE);
                if (error == I2C_Master_MSTR_NO_ERROR)
                {
                    // Continue reading until we have register to read
                    uint32_t counter = register_count;
                    while(counter>1)
                    {
                        data[register_count-counter] =
                            I2C_Master_MasterReadByte(I2C_Master_ACK_DATA);
                        counter--;
                    }
                    // Read last data without acknowledgement
                    data[register_count-1]
                        = I2C_Master_MasterReadByte(I2C_Master_NAK_DATA);
                }
            }
        }
    }
    
    // Send STOP condition
    I2C_Master_MasterSendStop();
    
    // Return error code
    return error ? ERROR : NO_ERROR;
} // I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti()


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
                                        uint8_t data)
{
    // Send START condition
    uint8_t error = I2C_Master_MasterSendStart(device_address, I2C_Master_WRITE_XFER_MODE);
    if (error == I2C_Master_MSTR_NO_ERROR)
    {
        uint8 reg = register_address >> 8;
        
        // Write register address HIGH
        error = I2C_Master_MasterWriteByte(reg);
        if (error == I2C_Master_MSTR_NO_ERROR)
        {
            reg = register_address;
            
            // Write register address LOW
            error = I2C_Master_MasterWriteByte(reg);
            if (error == I2C_Master_MSTR_NO_ERROR)
            {
                // Write byte of interest
                error = I2C_Master_MasterWriteByte(data);
            }
        }
    }
    
    // Send STOP condition
    I2C_Master_MasterSendStop();
    
    // Return error code
    return error ? ERROR : NO_ERROR;
} // I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister()


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
                                        uint8_t* data)
{
    // Send START condition
    uint8_t error = I2C_Master_MasterSendStart(device_address, I2C_Master_WRITE_XFER_MODE);
    if (error == I2C_Master_MSTR_NO_ERROR)
    {
        uint8 reg = register_address >> 8;
        
        // Write register address HIGH
        error = I2C_Master_MasterWriteByte(reg);
        if (error == I2C_Master_MSTR_NO_ERROR)
        {
            reg = register_address;
            
            // Write register address LOW
            error = I2C_Master_MasterWriteByte(reg);
            
            if (error == I2C_Master_MSTR_NO_ERROR)
            {
                // Continue writing as long as we have data to write
                uint8_t counter = register_count;
                while(counter > 0)
                {
                     error =
                        I2C_Master_MasterWriteByte(data[register_count-counter]);
                    if (error != I2C_Master_MSTR_NO_ERROR)
                    {
                        // Send STOP condition
                        I2C_Master_MasterSendStop();
                        
                        // Return error code
                        return ERROR;
                    }
                    counter--;
                }
            }
        }
    }
    
    // Send STOP condition in case something didn't work out correctly
    I2C_Master_MasterSendStop();
    
    // Return error code
    return error ? ERROR : NO_ERROR;
} // I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti()


/*
This function checks if a device is connected over the I2C lines.
- device_address: I2C address of the device to be checked
- retval: Returns true (>0) if device is connected

It is necessary to control if there is a proper connection between
the components so it is possible to start the communication
*/
uint8_t I2C_Peripheral_EXTERNAL_EEPROM_IsDeviceConnected(uint8_t device_address)
{
    // Send a START condition followed by a STOP condition
    uint8_t error = I2C_Master_MasterSendStart(device_address, I2C_Master_WRITE_XFER_MODE);
    I2C_Master_MasterSendStop();
    // If no error generated during stop, device is connected
    if (error == I2C_Master_MSTR_NO_ERROR)
    {
        return DEVICE_CONNECTED;
    }
    return DEVICE_UNCONNECTED;
}

/* [] END OF FILE */
