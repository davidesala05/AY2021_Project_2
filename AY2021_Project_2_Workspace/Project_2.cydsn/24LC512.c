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
#include "24LC512.h"
#include "I2C_Master.h"

ErrorCode I2C_Peripheral_ReadEEPROMRegisterMulti(uint8_t device_address,
                                            uint8_t register_address_high,
                                            uint8_t register_address_low,
                                            uint8_t register_count,
                                            uint8_t* data)
{
    // Send start condition
    uint8_t error = I2C_Master_MasterSendStart(device_address,I2C_Master_WRITE_XFER_MODE);
    if (error == I2C_Master_MSTR_NO_ERROR)
    {
        error = I2C_Master_MasterWriteByte(register_address_high);
        if (error == I2C_Master_MSTR_NO_ERROR)
        {
            error = I2C_Master_MasterWriteByte(register_address_low);
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

/* [] END OF FILE */
