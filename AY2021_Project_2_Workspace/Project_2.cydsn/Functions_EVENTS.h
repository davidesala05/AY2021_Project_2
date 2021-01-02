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
*  \brief header file for Events Saving definition
* 
*  In this header file are declared all the functions used to
*  save over-threshold events, read them, and export them as CSV 
*  in order to be plot through matplotlib library in Python.
*   
*/

#ifndef _FUNCTIONS_EVENTS_H_
    
    #define _FUNCTIONS_EVENTS_H_
    
    // Include related to all the variables implemented into the actual project
    #include "Global.h"
    
    // Include related to all the components related to the actual project
    #include "project.h"
    
    // Include related to all the cytypes functions 
    #include "cytypes.h"
    
    
    /*
    Function used to write all the events information on
    the external EEPROM
    The logic through which write the waveforms (192 byte) is inverted according to the 
    value of the current event:
        ODD event  --> 128+64 write operation
        EVEN event --> 64+128 write operation
    
    In this way it is possible to put in place a specific space-saving strategy to store the
    information inside the external EEPROM: it is necessary to take into consideration that a
    multi-write operation can be done for a maximum of 128 bytes each time (that are considered
    as a "page" inside the memory) --> according to this strategy, it is possible to write 2 
    events inside 3 complete pages avoiding unused space in between.
    
    All the operation are under a do-while cycle (so they are executed at least once), and this 
    allows us to maximize the writing speed applying the ACK polling strategy to send write 
    commands until no error (ACK) is received (meaning that the previous writing operation is
    over).
    */
    void Write_EVENT_on_EXTERNAL_EEPROM(void);

    
    /* 
    Function used to read the waveforms saved in the external EEPROM, convert them
    in int32 and send them sequentially through the UART communication protocol.
    
    The reading operation can be done without limitation for this memory, so there are no
    constraints in this case. The waveforms samples will be stored from the 0 address
    of the external EEPROM
    */
    void Read_Waveform_from_EXTERNAL_EEPROM(void);   

    
    /*
    Function used to read the timestamps saved in the external EEPROM
    and send them through the UART to the terminal.
    
    The timestamps data will be stored from the 60k address of the external EEPROM
    */       
    void Read_Timestamp_from_EXTERNAL_EEPROM(void);    


    /*
    This function is used to read, convert and send all the info about the events to the 
    computer through the serial port.
    Then a Python code file is used to interpret all the info and save them inside a CSV file 
    (used to successively plot all the overthreshold events data).
    
    This function can be called only if the device is in the WAIT mode in order to allow the
    monitoring of the entire events history saved for the application for which this device
    has been implemented --> sending a specific character ('W' or 'w') through the UART 
    communication protocol, the user will be able to obtain all the information related to the 
    overthreshold events detected during the normal functioning of the device
    */    
    void Export_file_CSV(void);
    
#endif // _FUNCTIONS_EVENTS_H_

/* [] END OF FILE */
