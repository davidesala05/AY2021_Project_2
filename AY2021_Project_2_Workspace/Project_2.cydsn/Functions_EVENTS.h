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
*  \brief header file for Events Saving definition
* 
*  In this header file are declared all the functions used to
*  save over-threshold events, read them, and export them as CSV 
*  in order to be plot through matplotlib library in Python.
*   
*/

#ifndef _FUNCTIONS_EVENTS_H_
    
    #define _FUNCTIONS_EVENTS_H_
    
    #include "project.h"
    #include "cytypes.h"
    #include "Global.h"
    
    /*
    Function used to write all the events information of the External EEPROM
    The logic through which write the waveforms (192 byte) is inverted according to the 
    value of the current event:
        ODD event  --> 128+64 write operation
        EVEN event --> 64+128 write operation

    All the operation are under a do-while cycle, and this allows us to maximize
    the writing speed, the ACK polling strategy is used to send write commands 
    until no error (ACK) is received (meaning that the previous "write" operation is over).
    */
    void Write_EVENT_on_EXTERNAL_EEPROM(void);

    
    /* 
    Function used to read the waveforms saved in the external EEPROM, convert them
    in int32 and send them sequentially through the UART communication protocol.
    */
    void Read_Waveform_from_EXTERNAL_EEPROM(void);   

    
    /*
    Function used to read the timestamps saved in the external eeprom
    and send them through the UART
    */       
    void Read_Timestamp_from_EXTERNAL_EEPROM(void);    


    /*
    This function is used to read, interpret and send all the info
    about the events to the serial port.
    Then a Python code file is used to interpret all the info and
    save them in a CSV file (used then to plot all the events info)
    */    
    void Export_file_CSV(void);       

    
#endif

/* [] END OF FILE */
