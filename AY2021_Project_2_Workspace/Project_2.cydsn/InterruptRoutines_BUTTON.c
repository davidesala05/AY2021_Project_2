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
    File InterruptRoutines_BUTTON.c
    Source file aimed to update the flag variables related to the PushButton component
    Authors: Garofalo Daniela, Pedica Benedetta and Sala Davide
*/
    
#include <InterruptRoutines_BUTTON.h>
    
CY_ISR(custom_BUTTON_ISR)
{
    flag_isbuttonpressed = 1;
    count_clicks++;
}

/* [] END OF FILE */
