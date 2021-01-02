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


#ifndef _INTERRUPTROUTINES_H_
    
    #define _INTERRUPTROUTINES_H_
    
    // Include related to all the variables implemented into the actual project
    #include "Global.h"
    
    // Include related to the functions implemented into the actual project
    #include "Functions_SETTINGS.h"
    #include "Functions_EVENTS.h"
    #include "Pin_INT_ACC.h"
    
    // Include related to all the cytypes functions 
    #include "cytypes.h"
    
    
    /*************************************/
    /*        INTERRUPT ROUTINES         */
    /*************************************/
    
    // Timer ISR 
    /* Interrupt generated when the counter register goes
    down to zero, indicating the end of a period
    
    This function is used to measure the passing of the time
    from the power on of the device, it is involved in the
    management of the frequencies needed in the project and 
    it coordinates all the timing requirements*/
    CY_ISR_PROTO(custom_TIMER_ISR);
    
    // Button pression ISR
    /* Interrupt generated at the pressing of the PushButton
    component measuring the time interval between two 
    consecutive pressings in order to identify the occurrence
    of a double click condition */
    CY_ISR_PROTO(custom_BUTTON_PRESS_ISR);
    
    // Button release ISR
    /* Interrupt generated at the releasing of the PushButton
    component measuring the duration of the pressing in order
    to identify the occurrence of a long pressure condition */
    CY_ISR_PROTO(custom_BUTTON_REL_ISR);
    
    // UART ISR
    /* Interrupt generated at the receiving of a data from the
    serial port through the UART communication protocol */
    CY_ISR_PROTO(Custom_UART_ISR);
    
    // Accelerometer ISR
    /* Interrupts generated in 2 different conditions:
       - INT1 goes high when a new data is ready at the output
         of the accelerometer component --> this happens at the
         same frequency of the datarate imposed by the user
       - INT2 goes high when an overthreshold event is detected
         so it is necessary to read the entire FIFO register and
         save the information into the external EEPROM --> this
         happens with an unpredictable timing characteristic due
         to the possible application of the device */
    CY_ISR_PROTO(Custom_ACC_ISR);
    
#endif // _INTERRUPTROUTINES_H_

/* [] END OF FILE */
