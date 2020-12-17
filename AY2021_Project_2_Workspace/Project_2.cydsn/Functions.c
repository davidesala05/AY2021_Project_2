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

#include "Functions.h"


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
    isr_TIMESTAMP_StartEx(custom_TIMER_ISR);
    isr_BUTTON_PRESS_StartEx(custom_BUTTON_PRESS_ISR);
    isr_BUTTON_REL_StartEx(custom_BUTTON_REL_ISR);
    EEPROM_INTERNAL_Start();            //To let start the internal EEPROM
    Timer_TIMESTAMP_Start();
}

/*
Function used to change the RGB color
according to the acceleration value
*/
void Set_RGB(void){

    DC_R = abs((int16)((DC_100/(FS_range_value*G))*accX));
    DC_G = abs((int16)((DC_100/(FS_range_value*G))*accY));
    DC_B = abs((int16)((DC_100/(FS_range_value*G))*accZ));
    
    PWM_RG_WritePeriod(DC_100);
    PWM_B_WritePeriod(DC_100);
    
    PWM_RG_WriteCompare1(DC_R);
    PWM_RG_WriteCompare2(DC_G);
    PWM_B_WriteCompare(DC_B);
}

/*
Function used to let start the components
after double-click
*/
void HM_Start(void){
    
    
    Register_Initialization_after_Overth_Event();
    
    Initialize_Parameters();
    
    Register_to_value();
    
    PWM_RG_Start();
    PWM_B_Start();
    
//    count_overth_event = 0;
//    
//    hours = 0;
//    minutes = 0;
//    seconds = 0;
//    count_global = 0;
}

/*
Function used to stop the components
after double-click
*/
void HM_Stop(void){
    
    reg = LIS3DH_CTRL_REG1_INIT | MASK_DATARATE_0Hz;
    
    ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                   LIS3DH_CTRL_REG1,
                                                   reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    PWM_RG_Stop();
    PWM_B_Stop();
}

/*
Function used to initialize the parameters
(DataRate, Verbose flag and Full-scale Range)
when the device is started.
*/
void Initialize_Parameters(void){
    
    //Read the INTERNAL EEPROM
    reg = EEPROM_INTERNAL_ReadByte(EEPROM_INTERNAL_ADDRESS);
    
    //Save the EEPROM REGISTER content in the variables
    DataRate_reg = 0b00001111 & (reg >> 4);
    FS_range_reg = 0b00000011 & (reg >> 2);
    Verbose_flag = 0b00000001 & (reg >> 1);

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
    
    /*
    We read the register on which is saved the value of FS, depending on it the value inside the
    THS register is changes accordingly to a conversion so that we have a fixed threshold of 2G 
    for all the different full scale range selected through the menu
    */
    switch (FS_range_reg) //switch  FS_range_value 
    {
        case MASK_FS_RANGE_2G:
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_THS,              
                                         LIS3DH_INT2_THS_2G);
            break;
        
        case MASK_FS_RANGE_4G:
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_THS,              
                                         LIS3DH_INT2_THS_4G);
            break;
        
        case MASK_FS_RANGE_8G:
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_THS,              
                                         LIS3DH_INT2_THS_8G);
            break;
        
        case MASK_FS_RANGE_16G:
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_THS,              
                                         LIS3DH_INT2_THS_16G);
            break;
        
        default:
            break;
    }
    
    /*
    Check register for duration of event; duration time is: Content of duration register/ODR
    We check on ODR value on the register and re-write it basing on the frequency we have
    */
    switch (DataRate_reg)
    {
        case MASK_DATARATE_50Hz:
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_DURATION,              
                                         LIS3DH_INT2_DURATION_50HZ);
            break;
        
        case MASK_DATARATE_100Hz:
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_DURATION,              
                                         LIS3DH_INT2_DURATION_100HZ);
            break;
        
        case MASK_DATARATE_200Hz:
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_DURATION,              
                                         LIS3DH_INT2_DURATION_200HZ);
            break;
        
        default:
            break;

    }
}

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

/*
Function used to convert the position of the potentiometer
in the correspondent value of the current parameter to set
To be called at the exit of the CONFIGURATION MODE
*/
void Potentiometer_to_Register(uint8_t parameter, int16_t value){

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

/*
Function used to convert the value read by the potentiometer in
a determined parameter to a feedback.
The feedback is different in blicking frequency and in colour of the RGB LED
according to the below references:

- FS_RANGE --> RED
    +- 2G  -->  1 Hz
    +- 4G  -->  2 Hz
    +- 8G  -->  5 Hz
    +- 16G --> 10 Hz

- DATARATE --> GREEN
    50Hz   --> 1 Hz
    100Hz  --> 2 Hz
    200Hz  --> 5 Hz

- VERBOSE FLAG
    ON  --> 1 Hz
    OFF --> 5 Hz
*/
void Set_Feedback(uint8_t parameter){

    PWM_RG_WritePeriod(DC_0);
    PWM_B_WritePeriod(DC_0);
    
    switch (parameter){
    
        case FS_RANGE : //RED feedback
            //Switch OFF GREEN and BLUE LED
            PWM_RG_WriteCompare2(DC_0);
            PWM_B_WriteCompare(DC_0);
            
            if (FS_range_reg == MASK_FS_RANGE_2G){ //BLINKING of 1Hz
                PWM_RG_WritePeriod(PERIOD_1Hz);
                PWM_RG_WriteCompare1(PERIOD_1Hz/2);
            }
            else if (FS_range_reg == MASK_FS_RANGE_4G){ //BLINKING of 2Hz
                PWM_RG_WritePeriod(PERIOD_2Hz);
                PWM_RG_WriteCompare1(PERIOD_2Hz/2);
            }
            else if (FS_range_reg == MASK_FS_RANGE_8G){ //BLINKING of 5Hz
                PWM_RG_WritePeriod(PERIOD_5Hz);
                PWM_RG_WriteCompare1(PERIOD_5Hz/2);
            }
            else if (FS_range_reg == MASK_FS_RANGE_16G){ //BLINKING of 10Hz
                PWM_RG_WritePeriod(PERIOD_10Hz);
                PWM_RG_WriteCompare1(PERIOD_10Hz/2);
            }
            break;
            
        case DATARATE : //GREEN feedback
            //Switch OFF RED and BLUE LED
            PWM_RG_WriteCompare1(DC_0);
            PWM_B_WriteCompare(DC_0);
            
            if (DataRate_reg == MASK_DATARATE_50Hz){ //BLINKING of 1Hz
                PWM_RG_WritePeriod(PERIOD_1Hz);
                PWM_RG_WriteCompare2(PERIOD_1Hz/2);
            }
            else if (DataRate_reg == MASK_DATARATE_100Hz){ //BLINKING of 2Hz
                PWM_RG_WritePeriod(PERIOD_2Hz);
                PWM_RG_WriteCompare2(PERIOD_2Hz/2);
            }
            else if (DataRate_reg == MASK_DATARATE_200Hz){ //BLINKING of 5Hz
                PWM_RG_WritePeriod(PERIOD_5Hz);
                PWM_RG_WriteCompare2(PERIOD_5Hz/2);
            }
            break;
            
        case VERBOSE_FLAG :  //BLUE feedback
            //Switch OFF RED and GREEN LED
            PWM_RG_WriteCompare1(DC_0);
            PWM_RG_WriteCompare2(DC_0);
            
            if(Verbose_flag == 0){ //BLINKING of 1Hz
                PWM_B_WritePeriod(PERIOD_1Hz);
                PWM_B_WriteCompare(PERIOD_1Hz/2);
            }
            else { //BLINKING of 5Hz
                PWM_B_WritePeriod(PERIOD_5Hz);
                PWM_B_WriteCompare(PERIOD_5Hz/2);
            }
            break;
            
        default :
            break;
    }
}

/*
Function used to save in the INTERNAL EEPROM the
register with the new parameters.
To be called at the exit of the CONFIGURATION MODE
*/
void Save_Parameters_on_INTERNAL_EEPROM(void){
    
    //Create the register to write with the new parameters
    reg = (DataRate_reg << 4) | (FS_range_reg << 2) | (Verbose_flag << 1);
    
    EEPROM_INTERNAL_UpdateTemperature();
    //Write the register in the INTERNAL EEPROM
    EEPROM_INTERNAL_WriteByte(reg, EEPROM_INTERNAL_ADDRESS);
}

/*
Function used to reset the stream_to_FIFO MODE after
that an overthreshold event is occured.
Is necessary to initialize the FIFO_CTRL register to the
BYPASS MODE and then go back to the Stream_to_FIFO MODE.
*/
void Register_Initialization_after_Overth_Event(void){
    
    //Set BYPASS MODE
    reg = LIS3DH_FIFO_CTRL_REG_BYPASS_MODE;
    ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                   LIS3DH_FIFO_CTRL_REG,
                                                   reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    //Turn back Stream to FIFO mode
    reg = LIS3DH_FIFO_CTRL_REG_Stream_to_FIFO_MODE;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_FIFO_CTRL_REG,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
}

void Write_EVENT_on_EXTERNAL_EEPROM(void){

    /******************************************/
    /*                WAVEFORM                */
    /******************************************/
    ErrorCode error;
    //FIRST 128 registers
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                  EEPROM_EXTERNAL_START_POINT_WAVEFORM + ((count_overth_event-1)*N_REG_2PAGE),
                                                                  N_REG_1PAGE,
                                                                  waveform_8bit);
    } while(error == ERROR);

    //REMAIN registers
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                  EEPROM_EXTERNAL_START_POINT_WAVEFORM + ((count_overth_event-1)*N_REG_2PAGE) + N_REG_1PAGE,
                                                                  N_REG_WAVEFORM - N_REG_1PAGE,
                                                                  &waveform_8bit[N_REG_1PAGE]);
    } while(error == ERROR);

    /******************************************/
    /*                TIMESTAMP               */
    /******************************************/
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_TIMESTAMP + 0 + ((count_overth_event-1)*N_REG_TIMESTAMP),
                                                             hours);
    } while(error == ERROR);
    
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_TIMESTAMP + 1 + ((count_overth_event-1)*N_REG_TIMESTAMP),
                                                             minutes);
    } while(error == ERROR);
    
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_TIMESTAMP + 2 + ((count_overth_event-1)*N_REG_TIMESTAMP),
                                                             seconds);
    } while(error == ERROR);
    
    /******************************************/
    /*              SENSITIVITY               */
    /******************************************/
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_SENSITIVITY + (count_overth_event-1),
                                                             Sensitivity);
    } while(error == ERROR);

    /******************************************/
    /*                DATARATE                */
    /******************************************/
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_DATARATE + (count_overth_event-1),
                                                             DataRate_reg);
    } while(error == ERROR);
}

/*
Function used to read the waveforms saved in the external eeprom,
convert them in int32 and send consequently by the UART
*/
void Read_Waveform_from_EXTERNAL_EEPROM(void){
    
    uint8_t all_waveforms[count_overth_event*N_REG_2PAGE]; //da allocare spazio prima
    uint8_t all_sensitivity[count_overth_event];
    uint8_t all_datarate[count_overth_event];
    
    uint8_t Pause[TRANSMIT_BUFFER_SIZE] = {0xA0,0,0,0,0,0,0,0,0,0,0,0,0,0xC0};
    
    ErrorCode error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                       EEPROM_EXTERNAL_START_POINT_WAVEFORM,
                                                                       (N_REG_2PAGE * count_overth_event),
                                                                       all_waveforms);
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                 EEPROM_EXTERNAL_START_POINT_SENSITIVITY,
                                                                 count_overth_event,
                                                                 all_sensitivity);
    } while(error == ERROR);
    
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                 EEPROM_EXTERNAL_START_POINT_DATARATE,
                                                                 count_overth_event,
                                                                 all_datarate);
    } while(error == ERROR);
    
    for(uint8_t y = 0; y < count_overth_event; y++){
        
        if (all_datarate[y] == MASK_DATARATE_50Hz){
            count_for_plotting = 4;
        }
        else if (all_datarate[y] == MASK_DATARATE_100Hz){
            count_for_plotting = 2;
        }
        else if (all_datarate[y] == MASK_DATARATE_200Hz){
            count_for_plotting = 1;
        }
        
        count_waveform = 0;
        
        for(int16_t i = 0; i < N_REG_WAVEFORM - 5; i = i+6){
            
            if(count_waveform == count_for_plotting){
            
                uint32_t index = i + N_REG_2PAGE*y;
                
                dataX = (int16)((all_waveforms[0+index] | (all_waveforms[1+index]<<8)))>>4;
                dataY = (int16)((all_waveforms[2+index] | (all_waveforms[3+index]<<8)))>>4;
                dataZ = (int16)((all_waveforms[4+index] | (all_waveforms[5+index]<<8)))>>4;
                
                accX = (float32)(dataX)*mg_TO_g*G*all_sensitivity[y];
                accY = (float32)(dataY)*mg_TO_g*G*all_sensitivity[y];
                accZ = (float32)(dataZ)*mg_TO_g*G*all_sensitivity[y];
                
                //X-axis
                DataUnion.f = accX;
                        
                Buffer[1] = (uint8_t)((DataUnion.l & 0xFF000000) >> 24);
                Buffer[2] = (uint8_t)((DataUnion.l & 0x00FF0000) >> 16);
                Buffer[3] = (uint8_t)((DataUnion.l & 0x0000FF00) >> 8);
                Buffer[4] = (uint8_t)((DataUnion.l & 0x000000FF) >> 0);
                
                //Y-axis
                DataUnion.f = accY;
                
                Buffer[5] = (uint8_t)((DataUnion.l & 0xFF000000) >> 24);
                Buffer[6] = (uint8_t)((DataUnion.l & 0x00FF0000) >> 16);
                Buffer[7] = (uint8_t)((DataUnion.l & 0x0000FF00) >> 8);
                Buffer[8] = (uint8_t)((DataUnion.l & 0x000000FF) >> 0);
                
                //Z-axis
                DataUnion.f = accZ;
                
                Buffer[9]  = (uint8_t)((DataUnion.l & 0xFF000000) >> 24);
                Buffer[10] = (uint8_t)((DataUnion.l & 0x00FF0000) >> 16);
                Buffer[11] = (uint8_t)((DataUnion.l & 0x0000FF00) >> 8);
                Buffer[12] = (uint8_t)((DataUnion.l & 0x000000FF) >> 0);
                
                /*The BUFFER is sent by the UART*/
                UART_PutArray(Buffer,TRANSMIT_BUFFER_SIZE);
                    
                count_waveform = 0;
            }
            else{
                i = i-6;
            }
            if(flag_send_waveform == 0){
                break;
            }
        }
        
        if(flag_send_waveform == 0){
                break;
        }
        
        count_waveform = 0;
        
        for(int16_t i = 0; i < PAUSE_LENGHT; i++){
            
            if(count_waveform == 1){
            
                UART_PutArray(Pause,TRANSMIT_BUFFER_SIZE);
                count_waveform = 0;
            }
            else{
                i--;
            }
            if(flag_send_waveform == 0){
                break;
            }
        }
        
        if(flag_send_waveform == 0){
                break;
        }
    }
}

/*
Function used to read the timestamps saved in the external eeprom
and send them through the UART
*/
void Read_Timestamp_from_EXTERNAL_EEPROM(void){
    
    uint8_t all_timestamp[count_overth_event*N_REG_TIMESTAMP];
    char string[50];
    
    ErrorCode error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                       EEPROM_EXTERNAL_START_POINT_TIMESTAMP,
                                                                       N_REG_TIMESTAMP * count_overth_event,
                                                                       all_timestamp);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }

    for (uint8_t i = 0; i < count_overth_event; i++){
        
        sprintf(string,"\n# event --> %d  Timestamp --> Hour: %d Minute: %d Second: %d\n\n", i+1, all_timestamp[0+i*N_REG_TIMESTAMP], all_timestamp[1+i*N_REG_TIMESTAMP], all_timestamp[2+i*N_REG_TIMESTAMP]);
        UART_PutString(string);
    }
}

void Reset_PWM_for_CONF_MODE(void){

    PWM_RG_WriteCounter(DC_0);
    PWM_B_WriteCounter(DC_0);

    PWM_RG_Start();
    PWM_B_Start();
}

void Export_file_CSV(void){

    uint8_t all_waveforms[count_overth_event*N_REG_2PAGE]; //da allocare spazio prima
    uint8_t all_sensitivity[count_overth_event];
    uint8_t all_datarate[count_overth_event];
    uint8_t all_timestamp[count_overth_event*N_REG_TIMESTAMP];
    
    //READ DATAT FROM  EXTERNAL EEPROM
    
    ErrorCode error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                       EEPROM_EXTERNAL_START_POINT_WAVEFORM,
                                                                       (N_REG_2PAGE * count_overth_event),
                                                                       all_waveforms);
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                 EEPROM_EXTERNAL_START_POINT_SENSITIVITY,
                                                                 count_overth_event,
                                                                 all_sensitivity);
    } while(error == ERROR);
    
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                 EEPROM_EXTERNAL_START_POINT_DATARATE,
                                                                 count_overth_event,
                                                                 all_datarate);
    } while(error == ERROR);
    
    do {
    
        error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                 EEPROM_EXTERNAL_START_POINT_TIMESTAMP,
                                                                 N_REG_TIMESTAMP * count_overth_event,
                                                                 all_timestamp);
    } while(error == ERROR);
    
    
    //CONVERT DATA IN FLOAT32
    
    float32 X_axis[32*count_overth_event];
    float32 Y_axis[32*count_overth_event];
    float32 Z_axis[32*count_overth_event];
    
    uint32_t index_ax = 0;
    
    for(uint8_t y = 0; y < count_overth_event; y++){
        
        for(int16_t i = 0; i < N_REG_WAVEFORM - 5; i = i+6){
            
            uint32_t index = i + N_REG_2PAGE*y;
                
            dataX = (int16)((all_waveforms[0+index] | (all_waveforms[1+index]<<8)))>>4;
            dataY = (int16)((all_waveforms[2+index] | (all_waveforms[3+index]<<8)))>>4;
            dataZ = (int16)((all_waveforms[4+index] | (all_waveforms[5+index]<<8)))>>4;
            
            X_axis[index_ax] = (float32)(dataX)*mg_TO_g*G*all_sensitivity[y];
            Y_axis[index_ax] = (float32)(dataY)*mg_TO_g*G*all_sensitivity[y];
            Z_axis[index_ax] = (float32)(dataZ)*mg_TO_g*G*all_sensitivity[y];
            
            index_ax ++;
        }
    }
    
    //WRITE CSV FILE
    
    FILE *fp;
    
    fp = fopen("Events.csv", "w+");
    
    if (fp == NULL) UART_PutString("ERROR OPENING THE FILE");
    
    fprintf(fp, "%d OVERTHRESHOLD EVENTS\n\n\n",count_overth_event);
    
    for(uint8_t i = 0; i < count_overth_event; i++){
        
        fprintf(fp, "EVENT , HOUR , MINUTE , SECOND , DATARATE , SENSITIVITY\n");
        
        fprintf(fp, "%d,%d,%d,%d,%d,%d\n ", i+1, all_timestamp[0+i*N_REG_TIMESTAMP], all_timestamp[1+i*N_REG_TIMESTAMP], all_timestamp[2+i*N_REG_TIMESTAMP], all_datarate[i], all_sensitivity[i]);
        
        for(uint8_t y = 0; y < 32; y++){
        
            fprintf(fp, "%f,",X_axis[y+((count_overth_event-1)*32)]);
        }
        
        fprintf(fp, "\n");
            
        for(uint8_t y = 0; y < 32; y++){
        
            fprintf(fp, "%f,",Y_axis[y+((count_overth_event-1)*32)]);
        }
        
        fprintf(fp, "\n");
        
        for(uint8_t y = 0; y < 32; y++){
        
            fprintf(fp, "%f,",Z_axis[y+((count_overth_event-1)*32)]);
        }
        
        fprintf(fp, "\n");
    }
    
    fprintf(fp, "END OF THE FILE\n");

    fclose(fp);
    
    UART_PutString("\nCSV FILE WRITTEN\n");
    
//    
//    
//    char * buffer;
//    char * ptr;
//    
//    getcwd(buffer, (size_t)1000);
//    char string[1000];
//    
//    sprintf(string, "%s\n", ptr);
//    UART_PutString(string);
    

}



/* [] END OF FILE */
