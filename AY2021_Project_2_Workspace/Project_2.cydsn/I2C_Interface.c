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

#include "I2C_Interface.h" 
#include "I2C_Master.h"

    ErrorCode I2C_Peripheral_Start(void) 
    {
        // Start I2C peripheral
        I2C_Master_Start();  
        
        // Return no error since start function does not return any error
        return NO_ERROR;
    }
    
    
    ErrorCode I2C_Peripheral_Stop(void)
    {
        // Stop I2C peripheral
        I2C_Master_Stop();
        // Return no error since stop function does not return any error
        return NO_ERROR;
    }

    ErrorCode I2C_Peripheral_ReadRegister(uint8_t device_address, 
                                            uint8_t register_address,
                                            uint8_t* data)  //l'ultimo è puntatore a pacchetto dati che riempiremo leggendo il registro 
    {
        // Start condition
        uint8_t error= I2C_Master_MasterSendStart(device_address, I2C_Master_WRITE_XFER_MODE); //così mandiamo lo start 
        if (error== I2C_Master_MSTR_NO_ERROR)
        { //scrivere indirizzo del registro che deve essere letto
            error=I2C_Master_MasterWriteByte(register_address);//scrivo sul bus l'indirizzo che voglio leggere dal device interrogato
            if (error== I2C_Master_MSTR_NO_ERROR)
            {
                //mando un restart 
                error=I2C_Master_MasterSendRestart(device_address,I2C_Master_READ_XFER_MODE);
                 if (error== I2C_Master_MSTR_NO_ERROR)
                {
                    //copio contenuto di indirizzo che sto leggendo nel data
                *data = I2C_Master_MasterReadByte(I2C_Master_NAK_DATA);                
                }
            }
        }
        I2C_Master_MasterSendStop(); //mando la stop condition
        return error ? ERROR : NO_ERROR; //error o è 0 o un valore qualsiasi che indica un errore
        //operatore ternario in C--> if else schiacciata in un unica riga
    }
    
    
    ErrorCode I2C_Peripheral_ReadRegisterMulti(uint8_t device_address,
                                                uint8_t register_address,
                                                uint8_t register_count,
                                                uint8_t* data)
    {
        // TODO
    }
    
    ErrorCode I2C_Peripheral_WriteRegister(uint8_t device_address,
                                            uint8_t register_address,
                                            uint8_t data)
    {
        // TODO
    }
    
    ErrorCode I2C_Peripheral_WriteRegisterMulti(uint8_t device_address,
                                            uint8_t register_address,
                                            uint8_t register_count,
                                            uint8_t* data)
    {
        // TODO
    }
    
    
    uint8_t I2C_IsDeviceConnected(uint8_t device_address)
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
