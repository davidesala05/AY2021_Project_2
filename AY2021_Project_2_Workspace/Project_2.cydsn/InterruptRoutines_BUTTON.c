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
    // Initialisation of the counter aimed to measure the pressing duration
    count_button_press = 0;
    
    /* Verify that a fast click is happened because it could be releated to the occurrence of the
    DOUBLE CLICK condition */ 
    if (flag_fastclick == 1)
    {
        /* Verify that after a fast click is occurred, the successive pressing is occurred within
        a given temporal window of 0.5 seconds */
        if (count_button_rel <= COUNTS_1_SECOND/2)
        {
            // Detection of a fast click followed by another click at a short distance
            flag_shortdistance = 1;
            
            /* Initialisation of the flag variable to the initial condition in order to identify
            the occurrence of the second fast click which defines a DOUBLE CLICK condition */
            flag_fastclick = 0;
        }
        else
        {
            // Initialisation of the flag variable to the initial condition
            flag_shortdistance = 0;
        }
    }
}

CY_ISR(custom_BUTTON_REL_ISR)
{
    /* Initialisation of the counter aimed to measure the time between the release of the 
    PushButton component and the next pressing event */
    count_button_rel = 0;
    
    // Verify that the pressing is longer than 2 seconds 
    if (count_button_press >= COUNTS_1_SECOND*2)
    {
        //****** LONG PRESSION CONDITION ******//
        flag_longpression = 1;
    // Verify that the pressing is shorter than 0.5 seconds
    } else if (count_button_press <= COUNTS_1_SECOND/2)
    {
        // Fast click condition 
        flag_fastclick = 1;
    // Verify that the pressing is shorter than 1 second
    } else if (count_button_press <= COUNTS_1_SECOND)
    {
        //******* SINGLE CLICK CONDITION ******//
        flag_singleclick = 1;
    }
    // Verify that there is an occurrence of 2 fast clicks at a short distance one another
    if(flag_shortdistance == 1 && flag_fastclick == 1)
    {
        //****** DOUBLE CLICK CONDITION ******//
        flag_doubleclick = 1;
    }
}

/* [] END OF FILE */
