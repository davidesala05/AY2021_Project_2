/* ========================================
 *
 * Laboratorio di Tecnologie Elettroniche e Biosensori
 * Politecnico di Milano
 * AA 2020/2021 - I semestre
 *
 * Progetti finali:
 * - Progetto 2 -
 * Autori: Daniela Garofalo, Benedetta Pedica, Davide Sala
 * Data: 10/01/2021
 *
 * ========================================
*/


#ifndef __INTERRUPTROUTINES_H__
    #define __INTERRUPTROUTINES_H__
    
    // Include related to all the variables implemented into the actual project
    #include "Global.h"
    
    // Include related to the functions implemented into the actual project
    #include "Functions_SETTINGS.h"
    #include "Functions_EVENTS.h"
    
    // Include related to all the cytypes functions 
    #include "cytypes.h"
    
    
    /*************************************/
    /*        INTERRUPT ROUTINES         */
    /*************************************/
    
    // Timer ISR
    CY_ISR_PROTO(custom_TIMER_ISR);
    
    // Button pression ISR
    CY_ISR_PROTO(custom_BUTTON_PRESS_ISR);
    
    // Button release ISR
    CY_ISR_PROTO(custom_BUTTON_REL_ISR);
    
    // UART ISR
    CY_ISR_PROTO(Custom_UART_ISR);
    
    // Accelerometer ISR
    CY_ISR_PROTO(Custom_ACC_ISR);
    
#endif

/* [] END OF FILE */
