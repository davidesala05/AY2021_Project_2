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

/*
    File Global.c
    Source file aimed to the initialisation of all the variables needed for the programming
    Authors: Garofalo Daniela, Pedica Benedetta and Sala Davide
*/

#include <Global.h>

// Initialisation of the variables
int8_t device_state = WAIT;
uint8_t seconds = 0;
uint8_t minutes = 0;
uint8_t hours = 0;
uint8_t parameter_selected = FS_RANGE;
int16_t potentiometer_value = 0;
uint8_t FS_range_reg = 0;
uint8_t DataRate_reg = 0;
uint8_t Verbose_flag = 0;

// Initialisation of the counters
uint8_t count_global = 0;
uint16_t count_button_press = 0;
uint16_t count_button_rel = 0;
uint8_t count_blinking = 0;

// Initialisation of the flags
uint8_t flag_configurationmode = IDLE;
uint8_t flag_sampling_pot = 0;
uint8_t flag_blinking = 0;
uint8_t flag_singleclick = 0;
uint8_t flag_doubleclick = 0;
uint8_t flag_longpression = 0;
uint8_t flag_shortdistance = 0;
uint8_t flag_fastclick = 0;

void Device_Initialisation()
{
    // Initialisation of the components
    ADC_DelSig_Init();
    EEPROM_INTERNAL_Start();
    I2C_Master_Start();
    Pin_ONBOARD_LED_Write(ONBOARD_LED_OFF);
    Timer_TIMESTAMP_Start();
    UART_Start();
    PWM_RG_Start();
    PWM_B_Start();
    
    // Declaration of the ISR functions
    isr_BUTTON_PRESS_StartEx(custom_BUTTON_PRESS_ISR);
    isr_BUTTON_REL_StartEx(custom_BUTTON_REL_ISR);
    isr_TIMESTAMP_StartEx(custom_TIMER_ISR);
}

void Potentiometer_to_Register(uint8_t parameter, int16_t value)
{
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

void Set_Feedback(uint8_t parameter)
{
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
            
        case VERBOSE_FLAG : //BLUE feedback
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

/* [] END OF FILE */
