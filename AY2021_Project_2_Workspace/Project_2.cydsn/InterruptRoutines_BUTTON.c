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
#include <stdio.h>
    
CY_ISR(custom_BUTTON_PRESS_ISR)
{
    UART_PutString("PRESSING of the button");
//    count_button_press = 0;
////    flag_isbuttonpressed = 1;
//    if (flag_singleclick)
//    {
//        if (count_button_rel <= COUNTS_1_SECOND/2)
//        {
//            flag_shortdistance = 1;
//            flag_singleclick = 0;
//        }
//    }
}

CY_ISR(custom_BUTTON_REL_ISR)
{
    UART_PutString("RELEASING of the button");
//    count_button_rel = 0;
//    if (count_button_press >= 2*COUNTS_1_SECOND)
//    {
//        flag_longpression = 1;
//    } else if (count_button_press <= COUNTS_1_SECOND/2)
//    {
//        flag_singleclick = 1;
//    }
//    if(flag_shortdistance == 1 && flag_singleclick == 1)
//    {
//        flag_doubleclick = 1;
//        flag_shortdistance = 0;
//        flag_singleclick = 0;
//    }
}

/* [] END OF FILE */
