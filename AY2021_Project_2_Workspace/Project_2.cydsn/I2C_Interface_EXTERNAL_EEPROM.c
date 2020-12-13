/*
* This file includes all the required source code to interface
* the I2C peripheral.
*/

/**
*   \brief Value returned if device present on I2C bus.
*/
#ifndef DEVICE_CONNECTED
    #define DEVICE_CONNECTED 1
#endif

/**
*   \brief Value returned if device not present on I2C bus.
*/
#ifndef DEVICE_UNCONNECTED
    #define DEVICE_UNCONNECTED 0
#endif

#include "I2C_Interface_EXTERNAL_EEPROM.h" 
#include "I2C_Master.h"

    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_Start(void) 
    {
        // Start I2C peripheral
        I2C_Master_Start();  
        
        // Return no error since start function does not return any error
        return NO_ERROR;
    }
    
    
    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_Stop(void)
    {
        // Stop I2C peripheral
        I2C_Master_Stop();
        // Return no error since stop function does not return any error
        return NO_ERROR;
    }

    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_ReadRegister(uint8_t device_address, 
                                            uint16_t register_address,
                                            uint8_t* data)
    {
        // Send start condition
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
                    // Send restart condition
                    error = I2C_Master_MasterSendRestart(device_address, I2C_Master_READ_XFER_MODE);
                    if (error == I2C_Master_MSTR_NO_ERROR)
                    {
                        // Read data without acknowledgement
                        *data = I2C_Master_MasterReadByte(I2C_Master_NAK_DATA);
                    }
                }
            }
        }
        // Send stop condition
        I2C_Master_MasterSendStop();
        // Return error code
        return error ? ERROR : NO_ERROR;
    }
    
    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(uint8_t device_address,
                                                uint16_t register_address,
                                                uint8_t register_count,
                                                uint8_t* data)
    {
        // Send start condition
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
                    // Send restart condition
                    error = I2C_Master_MasterSendRestart(device_address, I2C_Master_READ_XFER_MODE);
                    if (error == I2C_Master_MSTR_NO_ERROR)
                    {
                        // Continue reading until we have register to read
                        uint8_t counter = register_count;
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
        // Send stop condition
        I2C_Master_MasterSendStop();
        // Return error code
        return error ? ERROR : NO_ERROR;
    }
    
    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(uint8_t device_address,
                                            uint16_t register_address,
                                            uint8_t data)
    {
        // Send start condition
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
        // Send stop condition
        I2C_Master_MasterSendStop();
        // Return error code
        return error ? ERROR : NO_ERROR;
    }
    
    ErrorCode I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(uint8_t device_address,
                                            uint16_t register_address,
                                            uint8_t register_count,
                                            uint8_t* data)
    {
        // Send start condition
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
                    // Continue writing until we have data to write
                    uint8_t counter = register_count;
                    while(counter > 0)
                    {
                         error =
                            I2C_Master_MasterWriteByte(data[register_count-counter]);
                        if (error != I2C_Master_MSTR_NO_ERROR)
                        {
                            // Send stop condition
                            I2C_Master_MasterSendStop();
                            // Return error code
                            return ERROR;
                        }
                        counter--;
                    }
                }
            }
        }
        // Send stop condition in case something didn't work out correctly
        I2C_Master_MasterSendStop();
        // Return error code
        return error ? ERROR : NO_ERROR;
    }
    
    
    uint8_t I2C_Peripheral_EXTERNAL_EEPROM_IsDeviceConnected(uint8_t device_address)
    {
        // Send a start condition followed by a stop condition
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
