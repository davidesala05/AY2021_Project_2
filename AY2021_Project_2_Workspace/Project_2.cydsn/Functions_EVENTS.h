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

#ifndef _FUNCTIONS_EVENTS_H_
    
    #define _FUNCTIONS_EVENTS_H_
    
    #include "project.h"
    #include "cytypes.h"
    #include "Global.h"
    
    /*
    Function used to write all the events information o fthe External EEPROM
    The logic to write the waveforms (192 byte) is inverted according to the value of the current event:
        ODD event  --> 128+64 write operation
        EVEN event --> 64+128 write operation

    All the operation are under a do-while cycle, this allows us to maximize the write speed,
    the ACK polling strategy is used to send write commans until no error (ACK) is received
    This means that the previous write operation is over
    */
    void Write_EVENT_on_EXTERNAL_EEPROM(void);
    
    /*
    Function used to read the waveforms saved in the external EEPROM,
    convert them in int32 and send consequently by the UART
    */
    void Read_Waveform_from_EXTERNAL_EEPROM(void);
    
    /*
    Function used to read the timestamps saved in the external EEPROM
    and send them through the UART
    */
    void Read_Timestamp_from_EXTERNAL_EEPROM(void);
    
    /*
    This function is used to read, interpret and send all the info
    about the events to the serial port.
    Then a python file is used to re-interpret all the info and
    save them in a CSV file (used then to plot all the events info)
    */
    void Export_file_CSV(void);

#endif

/* [] END OF FILE */
