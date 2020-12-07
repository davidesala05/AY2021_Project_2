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
uint8_t count_global = 0;
uint8_t seconds = 0;
uint8_t minutes = 0;
uint8_t hours = 0;
uint8_t count_button_press = 0;
uint8_t count_button_rel = 0;
uint8_t parameter_selected = FS_RANGE;
uint8_t potentiometer_value = 0;
uint8_t FS_range_reg = 0;
uint8_t DataRate_reg = 0;
uint8_t Verbose_flag = 0;
uint8_t start_release = 0;

// Initialisation of the flags
uint8_t flag_isbuttonpressed = 0;
int8_t flag_configurationmode = CM_EXIT;
uint8_t flag_sampling = 0;
uint8_t flag_blinking = 0;
uint8_t flag_error = 0;
uint8_t flag_singleclick = 0;
uint8_t flag_doubleclick = 0;
uint8_t flag_longpression = 0;
uint8_t flag_shortdistance = 0;

void Device_Initialisation()
{
    // Initialisation of the components
    ADC_DelSig_Init();
    EEPROM_INTERNAL_Start();
    I2C_Master_Init();
    Pin_ONBOARD_LED_Write(ONBOARD_LED_OFF);
    PWM_RG_Init();
    PWM_B_Init();
    Timer_TIMESTAMP_Start();
    UART_Start();
    
    // Declaration of the ISR functions
    isr_BUTTON_PRESS_StartEx(custom_BUTTON_PRESS_ISR);
    isr_BUTTON_REL_StartEx(custom_BUTTON_REL_ISR);
    isr_TIMESTAMP_StartEx(custom_TIMER_ISR);
}

/* [] END OF FILE */
