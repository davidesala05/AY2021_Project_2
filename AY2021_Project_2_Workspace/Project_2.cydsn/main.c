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
    File main.c
    Source file aimed to allow the proper functioning of the device
    Authors: Garofalo Daniela, Pedica Benedetta and Sala Davide
*/

#include "project.h"
#include <Global.h>
#include <HardwareMenu.h>

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    // Initialisation of the device
    Device_Initialisation();

    for(;;)
    {
        /* Place your application code here. */

        /* Function that implements the functionalities associated to the
        hardware menu when the PushButton component is pressed by the user */
        Hardware_Menu();
    }
}

/* [] END OF FILE */
