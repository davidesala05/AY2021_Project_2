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
#include "Global.h"

uint8_t reg = 0;
uint8_t  reg_INT2_SRC = 0;
uint8_t flag_ACC = 0;
int16 dataX = 0;
int16 dataY = 0;
int16 dataZ = 0;
float32 accX = 0;
float32 accY = 0;
float32 accZ = 0;
uint8_t FS_range_reg = 0;
uint8_t FS_range_value = 0;
uint8_t Sensitivity = 0;
uint8_t data[6] = {0};

uint16_t DC_R = 0;
uint16_t DC_G = 0;
uint16_t DC_B = 0;

uint8_t Register_Param = 0;

uint8_t flag_overth_event = 0;

uint8_t flag_send_timestamps = 0;


void Register_Initialization(void){

    /*Initialization of the LIS3DH_CTRL_REG0 register*/
    reg = LIS3DH_CTRL_REG0_INIT;
    ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                   LIS3DH_CTRL_REG0,
                                                   reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
     /*Initialization of the LIS3DH_CTRL_REG1 register*/
    reg = LIS3DH_CTRL_REG1_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG1,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    /*Initialization of the LIS3DH_CTRL_REG3 register*/
    reg = LIS3DH_CTRL_REG3_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG3,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    /*Initialization of the LIS3DH_CTRL_REG4 register*/
    reg = LIS3DH_CTRL_REG4_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG4,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    /*Initialization of the LIS3DH_CTRL_REG5 register*/
    reg = LIS3DH_CTRL_REG5_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG5,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    /*Initialization of the LIS3DH_CTRL_REG6 register*/
    reg = LIS3DH_CTRL_REG6_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG6,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    /*Initialization of the LIS3DH_FIFO_CTRL_REG register*/
    reg = LIS3DH_FIFO_CTRL_REG_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_FIFO_CTRL_REG,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    /*Initialization of the LIS3DH_INT2_CFG register*/
    reg = LIS3DH_INT2_CFG_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_CFG,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    /*Initialization of the LIS3DH_INT2_THS register*/
    reg = LIS3DH_INT2_THS_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_THS,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
   
    
    /*Initialization of the LIS3DH_INT2_DURATION register*/
    reg = LIS3DH_INT2_DURATION_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_DURATION,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
}


/*
Function used to initialize the parameters
(DataRate, Verbose flag and Full-scale Range)
when the device is started.
*/
void Initialize_Parameters(void){
    
    //Read the INTERNAL EEPROM
    Register_Param = EEPROM_INTERNAL_ReadByte(EEPROM_INTERNAL_ADDRESS);
    
    //Save the EEPROM REGISTER content in the variables
    DataRate_reg = 0b00001111 & (Register_Param >> 4);
    FS_range_reg = 0b00000011 & (Register_Param >> 2);
    

    //Write DataRate
    reg = LIS3DH_CTRL_REG1_INIT | (DataRate_reg << 4);
    
    ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                   LIS3DH_CTRL_REG1,
                                                   reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }

    //Write Full Scale Range
    reg = LIS3DH_CTRL_REG4_INIT | (FS_range_reg << 4);
    
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG4,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
}


/*
Function used to convert the register used to
change the parameter (for the EEPROM saving)
in the real value of the parameter (used for the conversions)
*/

/*
Function used to convert the register used to
change the parameter (for the EEPROM saving)
in the real value of the parameter (used for the conversions)
To be called at the exit of the CONFIGURATION MODE
*/
void Register_to_value(void){
    
    //FULL-SCALE RANGE CONVERSION
    if (FS_range_reg == MASK_FS_RANGE_2G){ //+- 2G
        FS_range_value = 2;
        Sensitivity = 1;
    }
    else if (FS_range_reg == MASK_FS_RANGE_4G){ //+- 4G
        FS_range_value = 4;
        Sensitivity = 2;
    }
    else if (FS_range_reg == MASK_FS_RANGE_8G){ //+- 8G
        FS_range_value = 8;
        Sensitivity = 4;
    }
    else if (FS_range_reg == MASK_FS_RANGE_16G){ //+- 16G
        FS_range_value = 16;
        Sensitivity = 12;
    }
    
    //DATARATE CONVERSION
    if (DataRate_reg == MASK_DATARATE_50Hz){ //50 Hz
        DataRate_value = 50;
    }
    else if (DataRate_reg == MASK_DATARATE_100Hz){ //100 Hz
        DataRate_value = 100;
    }
    else if (DataRate_reg == MASK_DATARATE_200Hz){ //200 Hz
        DataRate_value = 200;
    }
}


/* [] END OF FILE */
