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
    File HardwareMenu.h
    Header file aimed to the declaration of the functions needed to manage the hardware menu build with the potentiometer
    Authors: Garofalo Daniela, Pedica Benedetta and Sala Davide
*/
    
#ifndef __HARDWAREMENU_H__
    #define __HARDWAREMENU_H__
    
    #include "Global.h"
    #include "Functions.h"

    
    
    /* Declaration of the function that manages the different options related to the type of 
    pressing allowed on the PushButton component --> change the device state among the possibilities:
    RUN, WAIT or CONFIGURATION MODE */
    void Hardware_Menu();
    
    /* Declaration of the functions that manage the components inside the CONFIGURATION MODE step
    of the hardware menu */
    void HM_Configuration();
    
#endif

/* [] END OF FILE */
