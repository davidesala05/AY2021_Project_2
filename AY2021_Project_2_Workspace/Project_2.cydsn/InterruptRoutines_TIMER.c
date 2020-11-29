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
    File InterruptRoutines_TIMER.c
    Source file aimed to update the flag variable related to the Timer component
    Authors: Garofalo Daniela, Pedica Benedetta and Sala Davide
*/
    
#include <InterruptRoutines_TIMER.h>
    
CY_ISR(custom_TIMER_ISR)
{
    // Allowing the generation of other interrupts by reading the Status Register of the Timer component
    Timer_TIMESTAMP_ReadStatusRegister();
    
    // Increment count_global variable in order to detect the passing of time 
    count_global++;
    if (count_global == 60)
    {
        count_global = 0;
        minutes++;
    }
    if (minutes == 60)
    {
        minutes = 0;
        hours++;
    }
    if (hours == 24)
    {
        hours = 0;
    }
    
    // Increment count_button variable only when the PushButton component is pressed
    if (flag_isbuttonpressed)
    {
        count_button++;
    }
    
    // Sampling the ADC_DelSig component with a frequency of 1 Hz
    if (flag_configurationmode == CM_SETPARAMETERS)
    {
        potentiometer_value = ADC_DelSig_Read8();
        flag_sampling = 1;
    }
    
    // Blinking of the OnBoardLED component in the CONFIGURATION MODE
    if (flag_blinking)
    {
        Pin_ONBOARD_LED_Write(!Pin_ONBOARD_LED_Read());
    }
}

/* [] END OF FILE */
