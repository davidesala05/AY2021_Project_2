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

    flag_ACC = 1;
    
    Pin_INT_AC_ClearInterrupt();
}

/* [] END OF FILE */
