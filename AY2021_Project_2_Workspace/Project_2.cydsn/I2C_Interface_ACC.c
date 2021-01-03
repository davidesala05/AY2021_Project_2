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
* This file includes all the required source code for the Accelerometer
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
ErrorCode I2C_Peripheral_Start(void) 
{
    // Start I2C peripheral
    I2C_Master_Start();  
    
    // Return no error since start function does not return any error
    return NO_ERROR;
} // I2C_Peripheral_Start()


/*
This function stops the I2C peripheral from working

It is needed to stop the communication between the components
according to the implementation of the transmission protocol -->
this command is the last one send
    */
ErrorCode I2C_Peripheral_Stop(void)
{
    // Stop I2C peripheral
    I2C_Master_Stop();
    
    // Return no error since stop function does not return any error
    return NO_ERROR;
} // I2C_Peripheral_Stop()


/*  
This function performs a complete reading operation over I2C from a single
register
- device_address: I2C address of the device to talk to
- register_address: address of the register to be read
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
*/
ErrorCode I2C_Peripheral_ReadRegister(uint8_t device_address, 
                                        uint8_t register_address,
                                        uint8_t* data)
{
    // Send START condition
    uint8_t error = I2C_Master_MasterSendStart(device_address,I2C_Master_WRITE_XFER_MODE);
    if (error == I2C_Master_MSTR_NO_ERROR)
    {
        // Write address of register to be read
        error = I2C_Master_MasterWriteByte(register_address);
        if (error == I2C_Master_MSTR_NO_ERROR)
        {
            // Send RESTART condition
            error = I2C_Master_MasterSendRestart(device_address, I2C_Master_READ_XFER_MODE);
            if (error == I2C_Master_MSTR_NO_ERROR)
            {
                // Read data without acknowledgement (NACK)
                *data = I2C_Master_MasterReadByte(I2C_Master_NAK_DATA);
            }
        }
    }
    
    // Send STOP condition
    I2C_Master_MasterSendStop();
    
    // Return error code
    return error ? ERROR : NO_ERROR;
} // I2C_Peripheral_ReadRegister()


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
*/  
ErrorCode I2C_Peripheral_ReadRegisterMulti(uint8_t device_address,
                                            uint8_t register_address,
                                            uint8_t register_count,
                                            uint8_t* data)
{
    // Send START condition
    uint8_t error = I2C_Master_MasterSendStart(device_address,I2C_Master_WRITE_XFER_MODE);
    if (error == I2C_Master_MSTR_NO_ERROR)
    {
        // Write address of register to be read with the MSb equal to 1
        register_address |= 0x80; // Datasheet indication for multi read --> autoincrement
        
        error = I2C_Master_MasterWriteByte(register_address);
        if (error == I2C_Master_MSTR_NO_ERROR)
        {
            // Send RESTART condition
            error = I2C_Master_MasterSendRestart(device_address, I2C_Master_READ_XFER_MODE);
            if (error == I2C_Master_MSTR_NO_ERROR)
            {
                /* Continue reading until we have register to read with the acknowledgement (ACK)
                --> the counter variable starts from the total number of data and it is 
                decremented at each operation until it reaches the last information */
                uint8_t counter = register_count;
                while(counter>1)
                {
                    data[register_count-counter] =
                        I2C_Master_MasterReadByte(I2C_Master_ACK_DATA);
                    counter--;
                }
                
                // Read last data without acknowledgement (NACK)
                data[register_count-1]
                    = I2C_Master_MasterReadByte(I2C_Master_NAK_DATA);
            }
        }
    }
    
    // Send STOP condition
    I2C_Master_MasterSendStop();
    
    // Return error code
    return error ? ERROR : NO_ERROR;
} // I2C_Peripheral_ReadRegisterMulti


/*   
This function performs a complete writing operation over I2C to a single 
register.
- device_address: I2C address of the device to talk to.
- register_address: address of the register to be written.
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
*/    
ErrorCode I2C_Peripheral_WriteRegister(uint8_t device_address,
                                        uint8_t register_address,
                                        uint8_t data)
{
    // Send START condition
    uint8_t error = I2C_Master_MasterSendStart(device_address, I2C_Master_WRITE_XFER_MODE);
    if (error == I2C_Master_MSTR_NO_ERROR)
    {
        // Write register address
        error = I2C_Master_MasterWriteByte(register_address);
        if (error == I2C_Master_MSTR_NO_ERROR)
        {
            // Write byte of interest
            error = I2C_Master_MasterWriteByte(data);
        }
    }
    // Send STOP condition
    I2C_Master_MasterSendStop();
    
    // Return error code
    return error ? ERROR : NO_ERROR;
} // I2C_Peripheral_WriteRegister()


/*  
This function performs a complete writing operation over I2C from multiple
registers
- device_address I2C address of the device to talk to
- register_address: address of the first register to be write
- register_count: number of registers we want to write
- data: pointer to an array where data will be saved

It is needed when a multi-writing operation has to be done
depending on the dimension of the data --> the implementation
of the I2C communication protocol for the multi-writing operations
expects:
1) Send the START condition
2) Writing the address of the register to be read
3) Write all the data
4) Send the STOP condition
*/    
ErrorCode I2C_Peripheral_WriteRegisterMulti(uint8_t device_address,
                                        uint8_t register_address,
                                        uint8_t register_count,
                                        uint8_t* data)
{
    // Send START condition
    uint8_t error = I2C_Master_MasterSendStart(device_address, I2C_Master_WRITE_XFER_MODE);
    if (error == I2C_Master_MSTR_NO_ERROR)
    {
        // Write address of register to be written with the MSB equal to 1
        register_address |= 0x80; // Datasheet indication for multi write -- autoincrement
        
        error = I2C_Master_MasterWriteByte(register_address);
        if (error == I2C_Master_MSTR_NO_ERROR)
        {
            /* Continue writing until we have register to write with the acknowledgement 
            (ACK) --> the counter variable starts from the total number of data and it is 
            decremented at each operation until it reaches the last information */
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
    
    // Send STOP condition in case something didn't work out correctly
    I2C_Master_MasterSendStop();
    
    // Return error code
    return error ? ERROR : NO_ERROR;
}


/*
This function checks if a device is connected over the I2C lines.
- device_address: I2C address of the device to be checked.
- retval: Returns true (>0) if device is connected.

It is necessary to control if there is a proper connection between
the components so it is possible to start the communication
*/
uint8_t I2C_Peripheral_IsDeviceConnected(uint8_t device_address)
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
