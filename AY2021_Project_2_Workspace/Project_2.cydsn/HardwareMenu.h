/* ========================================
 *
 * Laboratorio di Tecnologie Elettroniche e Biosensori
 * Politecnico di Milano
 * AA 2020-2021 - I semester
 *
 * Final Projects:
 * Project 2 
 * Authors: Daniela Garofalo, Benedetta Pedica, Davide Sala
 * Date: 10/01/2021
 *
 * ========================================
*/

/*
*  \brief header file for Hardware Menu function definition
* 
*  In this header file is declared the main function used to 
*  define and change the parameters of the Hardware Menu implemented.
*   
*/
    
#ifndef __HARDWAREMENU_H__
    #define __HARDWAREMENU_H__
    
    #include "Global.h"
    #include "Functions_SETTINGS.h"
    #include "Functions_EVENTS.h"

    /* Declaration of the function that manages the different options
    related to the type of pressing allowed on the PushButton component 
    --> change the device state among three possibilities:
        -RUN
        -WAIT
        -CONFIGURATION MODE
    */
    void Hardware_Menu(void);
         
    
#endif

/* [] END OF FILE */
