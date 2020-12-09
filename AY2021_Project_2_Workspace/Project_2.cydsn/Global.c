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
uint8_t FS_range_value = 0;
uint8_t Sensitivity = 0;
uint8_t Verbose_flag = 0;
uint8_t DataRate_reg = 0;
uint8_t DataRate_value = 0;
uint8_t data[N_REG_ACC] = {0};
uint8_t Buffer[TRANSMIT_BUFFER_SIZE] = {0};
uint16_t DC_R = 0;
uint16_t DC_G = 0;
uint16_t DC_B = 0;
uint8_t Register_Param = 0;
uint8_t flag_overth_event = 0;
uint8_t ch_received = 0;
uint8_t flag_send_timestamps = 0;
uint8_t flag_send_waveform = 0;
uint8_t waveform_8bit[N_REG_WAVEFORM_8bit] = {0};
uint8_t waveform_8bit_to_write[N_REG_WAVEFORM_8bit] = {0};
uint8_t count_overth_event = 0;
uint8_t timestamp_to_write[3] = {0};

/******************************************/
    /*                DANIELA               */
    /******************************************/

// Initialisation of the variables
    int8_t device_state = WAIT;
    uint8_t count_global = 0;
    uint8_t seconds = 0;
    uint8_t minutes = 0;
    uint8_t hours = 0;
    uint8_t start_press = 0;
    uint8_t count_clicks = 0;
    uint8_t parameter_selected = FS_RANGE;
    int16_t potentiometer_value = 0;
    
    
    // Initialisation of the flags
    uint8_t flag_isbuttonpressed = 0;
    int8_t flag_configurationmode = IDLE;
    uint8_t flag_sampling = 0;
    uint8_t flag_blinking = 0;

    uint16_t count_button_press = 0;
    uint16_t count_button_rel = 0;
    uint8_t flag_doubleclick = 0;
    uint8_t flag_singleclick = 0;
    uint8_t flag_longpression = 0;
    uint8_t flag_shortdistance = 0;
    uint8_t flag_fastclick = 0;
    uint8_t flag_sampling_pot = 0;
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
    isr_TIMESTAMP_StartEx(custom_TIMER_ISR);
    isr_BUTTON_PRESS_StartEx(custom_BUTTON_PRESS_ISR);
    isr_BUTTON_REL_StartEx(custom_BUTTON_REL_ISR);
    EEPROM_INTERNAL_Start();            //To let start the internal EEPROM
    PWM_RG_Start();
    PWM_B_Start();
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
    
    reg = LIS3DH_CTRL_REG1_INIT | (MASK_DATARATE_50Hz << 4);
    
    ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                   LIS3DH_CTRL_REG1,
                                                   reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    Register_Initialization_after_Overth_Event();
    
    Initialize_Parameters();
    
    count_overth_event = 0;
    
    hours = 0;
    minutes = 0;
    seconds = 0;
    count_global = 0;
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
    
    PWM_RG_WriteCompare1(DC_0);
    PWM_RG_WriteCompare2(DC_0);
    PWM_B_WriteCompare(DC_0);
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
    reg = (DataRate_reg << 4) | (FS_range_reg << 2) | (Verbose_flag < 1);
    
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

/*
Function used to write all the waveform values with a multiread
in the external eeprom.
*/
void Write_Waveform_on_EXTERNAL_EEPROM(void){
    
    //Copy the waveform array in a new array of smaller dimension
    memcpy(waveform_8bit_to_write, waveform_8bit, 128);
    //FIRST 128 registers
    ErrorCode error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                        EEPROM_EXTERNAL_START_POINT_WAVEFORM + count_overth_event*N_REG_WAVEFORM_8bit,
                                                                        128,
                                                                        waveform_8bit_to_write);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm1\r\n");  
    }
    
    //To be decided
    //CyDelay(10);
    //Copy the waveform array in a new array of smaller dimension
    memcpy(waveform_8bit_to_write, &waveform_8bit[128], N_REG_WAVEFORM_8bit-128);
    //REMAIN registers
    error = ERROR;
    while(error == ERROR){
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                 (EEPROM_EXTERNAL_START_POINT_WAVEFORM + count_overth_event*N_REG_WAVEFORM_8bit) + 128,
                                                                  N_REG_WAVEFORM_8bit-128,
                                                                  waveform_8bit_to_write);
    }
}

/*
Function used to write the timestamp's three values (hours, minutes, seconds
with a multiread in the external eeprom.
*/
void Write_Timestamp_on_EXTERNAL_EEPROM(void){

    timestamp_to_write[0] = hours;
    timestamp_to_write[1] = minutes;
    timestamp_to_write[2] = seconds;
    
    ErrorCode error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                        EEPROM_EXTERNAL_START_POINT_TIMESTAMP + count_overth_event*N_REG_TIMESTAMP,
                                                                        N_REG_TIMESTAMP,
                                                                        timestamp_to_write);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm2\r\n");  
    }


}

/*
Function used to write the current sensitivity on the external
eeprom. This is necessary since the sensitivity change with
the full-scale range (parameter that could change in configuration
mode during the run time).
So, diffrent overtheshold waveform can be generated under different conditions
and must be interpreted using different sensitivy.
*/
void Write_Sensitivity_on_EXTERNAL_EEPROM(void){
    
    ErrorCode error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                                   EEPROM_EXTERNAL_START_POINT_SENSITIVITY + count_overth_event,
                                                                   Sensitivity);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm3\r\n");  
    }


}

/*
Function used to read the waveforms saved in the external eeprom,
convert them in int32 and send consequently by the UART
*/
void Read_Waveform_from_EXTERNAL_EEPROM(void){
    
    uint8_t all_waveforms[count_overth_event*N_REG_WAVEFORM_8bit]; //da allocare spazio prima
    uint8_t all_sensitivity[count_overth_event];
    
    ErrorCode error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                       EEPROM_EXTERNAL_START_POINT_WAVEFORM,
                                                                       N_REG_WAVEFORM_8bit * count_overth_event,
                                                                       all_waveforms);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_SENSITIVITY,
                                                             count_overth_event,
                                                             all_sensitivity);
    
    for(uint8_t y = 0; y < count_overth_event; y++){
        for(uint8_t i = 0; i < N_REG_WAVEFORM_8bit; i = i+6){
            
            uint8_t index = i + N_REG_WAVEFORM_8bit*y;
        
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
            
            CyDelay(100);
        }
        
        for(uint8_t j = 0; j < 10; j++){
        
            uint8_t Pause[TRANSMIT_BUFFER_SIZE] = {0xA0,0,0,0,0,0,0,0,0,0,0,0,0,0xC0};
            UART_PutArray(Pause,TRANSMIT_BUFFER_SIZE);
            CyDelay(100);
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
        
        sprintf(string,"N° event --> %d  Timestamp --> Hour: %d Minute: %d Second: %d\n\n", i, all_timestamp[0+i*N_REG_TIMESTAMP], all_timestamp[1+i*N_REG_TIMESTAMP], all_timestamp[2+i*N_REG_TIMESTAMP]);
        UART_PutString(string);
        flag_send_timestamps = 0;
    }
}


/* [] END OF FILE */
