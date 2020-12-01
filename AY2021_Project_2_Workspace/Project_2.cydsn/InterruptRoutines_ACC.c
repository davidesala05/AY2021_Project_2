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

#include "InterruptRoutines_ACC.h"

CY_ISR(Custom_ACC_ISR){

    flag_ACC = 1; //Used in the main.c to sampling or for overthreshold event detection
    
    Pin_INT_AC_ClearInterrupt(); //necessary for the correct use of isr linked to the pin
}

/* [] END OF FILE */
