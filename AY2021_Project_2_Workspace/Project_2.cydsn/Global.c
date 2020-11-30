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

uint8_t FS_range = 0;
uint8_t Sensitivity = 0;
uint8_t Verbose_flag = 0;
uint8_t DataRate = 0;
uint8_t System_state = 0;

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

void Set_RGB(void){

    DC_R = abs((int16)((DC_100/(FS_range*G))*accX));
    DC_G = abs((int16)((DC_100/(FS_range*G))*accY));
    DC_B = abs((int16)((DC_100/(FS_range*G))*accZ));
    
    PWM_RG_WriteCompare1(DC_R);
    PWM_RG_WriteCompare2(DC_G);
    PWM_B_WriteCompare(DC_B);
    
}

void Start_Components(void){
    
    //DA RAGIONARCI BENE!!! (accendere acc con registri)
    I2C_Peripheral_Start();
    UART_Start();
    isr_ACC_StartEx(Custom_ACC_ISR);
    PWM_RG_Start();
    PWM_B_Start();
    EEPROM_INTERNAL_Start();
}

void Stop_Components(void){
    
    //DA RAGIONARCI BENE!!!! (spegnere acc con registri)
    I2C_Peripheral_Stop();
    UART_Stop();
    PWM_RG_Stop();
    PWM_B_Stop();
    EEPROM_INTERNAL_Stop();
}

void Change_Parameters(uint8_t Parameter){
    
    //Read the INTERNAL EEPROM
    Register_Param = EEPROM_INTERNAL_ReadByte(EEPROM_INTERNAL_ADDRESS);
    
    //Save the EEPROM REGISTER content in the variables
    System_state  = 0b00000001 & (Register_Param >> 7);
    FS_range      = 0b00000011 & (Register_Param >> 4);
    DataRate      = 0b00000111 & (Register_Param >> 1);
    Verbose_flag  = 0b00000001 & (Register_Param >> 0);
    
    switch (Parameter){
        
        case DATA_RATE :
            //Write DataRate
            reg = LIS3DH_CTRL_REG1_INIT | (DataRate << 4);
            
            ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                           LIS3DH_CTRL_REG1,
                                                           reg);
            if(error == ERROR){
                UART_PutString("Error occurred during I2C comm\r\n");  
            }
            break;
        case FULL_SCALE :
            //Write Full Scale Range
            reg = LIS3DH_CTRL_REG4_INIT | (FS_range << 4);
            
            error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                 LIS3DH_CTRL_REG4,
                                                 reg);
            if(error == ERROR){
                UART_PutString("Error occurred during I2C comm\r\n");  
            }
            break;
            
        case SYSTEM_STATE :
            //System State Switching
            Switch_System_State(System_state);
            break;
            
        default:
            break;
    }
}

void Switch_System_State(uint8_t State){

    switch(State){
    
        case ON :
            Start_Components();
            break;
            
        case OFF:
            Stop_Components();
            break;
            
        default:
            break;
    }
}










/* [] END OF FILE */
