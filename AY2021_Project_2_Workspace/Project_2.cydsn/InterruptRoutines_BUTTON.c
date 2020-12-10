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
    
CY_ISR(custom_BUTTON_PRESS_ISR)
{
    count_button_press = 0;
    if (flag_fastclick == 1)
    {
        if (count_button_rel <= COUNTS_1_SECOND/2)
        {
            flag_shortdistance = 1;
            flag_fastclick = 0;
        }
        else
        {
            flag_shortdistance = 0;
        }
    }
}

CY_ISR(custom_BUTTON_REL_ISR)
{
    count_button_rel = 0;
    if (count_button_press >= COUNTS_1_SECOND*2)
    {
        flag_longpression = 1;
        flag_shortdistance = 0;
        flag_fastclick = 0;
        flag_singleclick = 0;
    } else if (count_button_press <= COUNTS_1_SECOND/4)
    {
        flag_fastclick = 1;
        flag_longpression = 0;
        flag_singleclick = 0;
        
    } else if (count_button_press <= COUNTS_1_SECOND)
    {
        flag_singleclick = 1;
        flag_fastclick = 0;
        flag_longpression = 0;
        flag_shortdistance = 0;
    }
    if(flag_shortdistance == 1 && flag_fastclick == 1)
    {
        flag_doubleclick = 1;
        flag_longpression = 0;
        flag_fastclick = 0;
        flag_shortdistance = 0;
    }
}

/* [] END OF FILE */
