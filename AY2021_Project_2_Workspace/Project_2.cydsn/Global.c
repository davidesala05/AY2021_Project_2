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

/******************************************/
/*            GLOBAL VARIABLES            */
/******************************************/

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
uint8_t Sensitivity = 0;
uint8_t Verbose_flag = 0;
uint8_t DataRate_reg = 0;
uint8_t data[6] = {0};
uint8_t Buffer[TRANSMIT_BUFFER_SIZE] = {0};
uint16_t DC_R = 0;
uint16_t DC_G = 0;
uint16_t DC_B = 0;
uint8_t Register_Param = 0;
uint8_t flag_overth_event = 0;
uint8_t ch_received = 0;
uint8_t flag_send_timestamps = 0;




/******************************************/
/*                FUNCTIONS               */
/******************************************/

/*
Function used to initialize the register
of the accelerometer when the device is started.
*/
void Register_Initialization(void){

    /*Initialization of the LIS3DH_CTRL_REG0 register*/
    reg = LIS3DH_CTRL_REG0_INIT;
    ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                   LIS3DH_CTRL_REG0,
                                                   reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    /*Initialization of the LIS3DH_TEMP_CFG_REG register*/
    reg = LIS3DH_TEMP_CFG_REG_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_TEMP_CFG_REG,
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
Function used to let start the components when
the device is powered ON.
*/
void Start_Components_powerON(void){
    
    I2C_Peripheral_Start();             //To comunicate with the accelerometer and the external EEPROM
    UART_Start();                       //To comunicate with the microcontroller via serial port
    isr_ACC_StartEx(Custom_ACC_ISR);    //To abilitate the isr called by the accelerometer
    isr_UART_StartEx(Custom_UART_ISR);  //To abilitate the isr called by the UART
    EEPROM_INTERNAL_Start();            //To let start the internal EEPROM
}

/*
Function used to change the RGB color
according to the acceleration value
*/
void Set_RGB(void){

    DC_R = abs((int16)((DC_100/(FS_range_value*G))*accX));
    DC_G = abs((int16)((DC_100/(FS_range_value*G))*accY));
    DC_B = abs((int16)((DC_100/(FS_range_value*G))*accZ));
    
    PWM_RG_WriteCompare1(DC_R);
    PWM_RG_WriteCompare2(DC_G);
    PWM_B_WriteCompare(DC_B);
    
}

/*
Function used to let start the components
after double-click
*/
void Start_Components(void){
    
    //DA RAGIONARCI BENE!!! (accendere acc con registri)
    I2C_Peripheral_Start();
    UART_Start();
    isr_ACC_StartEx(Custom_ACC_ISR);
    isr_UART_StartEx(Custom_UART_ISR);
    PWM_RG_Start();
    PWM_B_Start();
    EEPROM_INTERNAL_Start();
}

/*
Function used to stop the components
after double-click
*/
void Stop_Components(void){
    
    //DA RAGIONARCI BENE!!!! (spegnere acc con registri)
    I2C_Peripheral_Stop();
    UART_Stop();
    PWM_RG_Stop();
    PWM_B_Stop();
    EEPROM_INTERNAL_Stop();
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
    Verbose_flag = 0b00000001 & (Register_Param >> 1);

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

void Register_to_value(void){
    
    //FULL-SCALE RANGE CONVERSION
    if (FS_range_reg & MASK_FS_RANGE_2G){ //+- 2G
        FS_range_value = 2;
        Sensitivity = 1;
    }
    else if (FS_range_reg & MASK_FS_RANGE_4G){ //+- 4G
        FS_range_value = 4;
        Sensitivity = 2;
    }
    else if (FS_range_reg & MASK_FS_RANGE_8G){ //+- 8G
        FS_range_value = 8;
        Sensitivity = 4;
    }
    else if (FS_range_reg & MASK_FS_RANGE_16G){ //+- 16G
        FS_range_value = 16;
        Sensitivity = 12;
    }
    
    //DATARATE CONVERSION
    if (DataRate_reg & MASK_DATARATE_50Hz){ //50 Hz
        DataRate_value = 50;
    }
    else if (DataRate_reg & MASK_DATARATE_100Hz){ //100 Hz
        DataRate_value = 100;
    }
    else if (DataRate_reg & MASK_DATARATE_200Hz){ //200 Hz
        DataRate_value = 200;
    }
}

/*
Function used to change the colour of the RGB led
according to the parameter that we are changing in the
CONFIGURATION MODE
(for the COLOUR FEEDBACK)
*/
void Set_Colour_Parameter(uint8_t parameter){
    
    switch(parameter){
    
        case FS_RANGE : //RED feedback
            PWM_RG_WriteCompare1(DC_100);
            PWM_RG_WriteCompare2(DC_0);
            PWM_B_WriteCompare(DC_0);
            break;
            
        case DATARATE : //GREEN feedback
            PWM_RG_WriteCompare1(DC_0);
            PWM_RG_WriteCompare2(DC_100);
            PWM_B_WriteCompare(DC_0);
            break;
            
        case VERBOSE_FLAG :  //BLUE feedback
            PWM_RG_WriteCompare1(DC_0);
            PWM_RG_WriteCompare2(DC_0);
            PWM_B_WriteCompare(DC_100);
            break;
            
        default :
            break;
    }
}

/*
Function used to convert the position of the potentiometer
in the correspondent value of the current parameter to set
*/
void Potentiometer_to_value(uint8_t parameter, uint8_t value){

    switch (parameter){
    
        case FS_RANGE :
            if (value <= 64){
                FS_range_reg = MASK_FS_RANGE_2G;
            }
            else if (value > 64 && value <= 128){
                FS_range_reg = MASK_FS_RANGE_4G;
            }
            else if (value > 128 && value <= 192){
                FS_range_reg = MASK_FS_RANGE_8G;
            }
            else if (value > 192){
                FS_range_reg = MASK_FS_RANGE_16G;
            }
        break;
            
        case DATARATE :
            if (value <= 85){
                DataRate_reg = MASK_DATARATE_50Hz;
            }
            else if (value > 85 && value <= 170){
                DataRate_reg = MASK_DATARATE_100Hz;
            }
            else if (value > 170){
                DataRate_reg = MASK_DATARATE_200Hz;
            }
        break;
            
        case VERBOSE_FLAG :
            if(value <= 128){
                Verbose_flag = 0;
            }
            else {
                Verbose_flag = 1;
            }
        break;
            
        default :
            break;
    }
}


//OMG!!!!!! we have to add three more PWMs!!!!
//MAYBE WE CAN USE A MULTIPLEXER IN ORDER TO USE TWO DIFFERENT CLOCKs
void Set_Feedback(uint8_t parameter, uint8_t value){

    switch (parameter){
    
        case FS_RANGE : //RED feedback
            //Switch OFF GREEN and BLUE LED
            PWM_RG_WriteCompare2(DC_50);
            PWM_RG_WriteCompare2(DC_0);
            PWM_B_WriteCompare(DC_0);
            
            if (value == MASK_FS_RANGE_2G){
                
            }
            else if (value == MASK_FS_RANGE_4G){
                
            }
            else if (value == MASK_FS_RANGE_8G){
                
            }
            else if (value == MASK_FS_RANGE_16G){
                
            }
        break;
            
        case DATARATE : //GREEN feedback
            //Switch OFF RED and BLUE LED
            PWM_RG_WriteCompare2(DC_0);
            PWM_RG_WriteCompare2(DC_50);
            PWM_B_WriteCompare(DC_0);
            
            if (value == MASK_DATARATE_50Hz){
                
            }
            else if (value == MASK_DATARATE_50Hz){
                
            }
            else if (value == MASK_DATARATE_50Hz){
                
            }
        break;
            
        case VERBOSE_FLAG :  //BLUE feedback
            //Switch OFF RED and GREEN LED
            PWM_RG_WriteCompare2(DC_0);
            PWM_RG_WriteCompare2(DC_0);
            PWM_B_WriteCompare(DC_50);
            
            if(Verbose_flag == 0){
                
            }
            else {
                
            }
        break;
            
        default :
            break;
    }
}









/* [] END OF FILE */
