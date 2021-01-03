/* ========================================
 *
 * Laboratorio di Tecnologie Elettroniche e Biosensori
 * Politecnico di Milano
 * AY 2020/2021 - I semester
 *
 * Final Projects:
 * - Project 2 - 
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
    
#ifndef _HARDWAREMENU_H_
    
    #define _HARDWAREMENU_H_
    
    // Include related to all the variables implemented into the actual project
    #include "Global.h"

    // Include related to the functions implemented into the actual project
    #include "Functions_SETTINGS.h"
    #include "Functions_EVENTS.h"

    
    /* Declaration of the function that manages the different options
    related to the type of pressing allowed on the PushButton component 
    --> change the device state among three possibilities:
        - RUN mode
        - WAIT mode (the device is able to pass from one state to the other with
          a double click on the PushButton component)
        - CONFIGURATION MODE (the device enters in this modality when a long
          pression on the PushButton component is detected)
    */
    
    void Hardware_Menu(void);
    
#endif // _HARDWARE_MENU_H_

/* [] END OF FILE */
