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

#include "Global.h"

/******************************************/
/*              INIZIALIZATION            */
/******************************************/

//FLAGS
uint8_t     flag_ACC                = 0;
uint8_t     flag_overth_event       = 0;
uint8_t     flag_send_timestamps    = 0;
uint8_t     flag_send_waveform      = 0;
uint8_t     flag_blinking           = 0;
uint8_t     flag_doubleclick        = 0;
uint8_t     flag_singleclick        = 0;
uint8_t     flag_longpression       = 0;
uint8_t     flag_shortdistance      = 0;
uint8_t     flag_fastclick          = 0;
uint8_t     flag_sampling_pot       = 0;

//COUNTERS
uint8_t     count_overth_event      = 0;
uint16_t    count_button_press      = 0;
uint16_t    count_button_rel        = 0;
uint8_t     count_waveform          = 0;
uint8_t     count_for_plotting      = 0;
uint8_t     count_global            = 0;

//REGISTERS
uint8_t     reg                     = 0;
uint8_t     reg_INT2_SRC            = 0;
uint8_t     FS_range_reg            = 0;
uint8_t     DataRate_reg            = 0;

//PARAMETERS
uint8_t     FS_range_value          = 0;
uint8_t     Sensitivity             = 0;
uint8_t     Verbose_flag            = 0;
uint8_t     DataRate_value          = 0;
uint8_t     parameter_selected      = FS_RANGE;
int16_t     potentiometer_value     = 0;

//COLOUR
uint16_t    DC_R                    = 0;
uint16_t    DC_G                    = 0;
uint16_t    DC_B                    = 0;

//TIMESTAMP
uint8_t     timestamp_to_write[3]   = {0};
uint8_t     seconds                 = 0;
uint8_t     minutes                 = 0;
uint8_t     hours                   = 0;

//OTHERS
uint8_t     ch_received             = 0;
int8_t      device_state            = WAIT;
uint8_t     configurationmode_state = IDLE;

//ACCELERATION
uint8_t     data[N_REG_ACC]              = {0};
int16       dataX                        = 0;
int16       dataY                        = 0;
int16       dataZ                        = 0;
float32     accX                         = 0;
float32     accY                         = 0;
float32     accZ                         = 0;
uint8_t     Buffer[TRANSMIT_BUFFER_SIZE] = {0};

//WAVEFORM
uint8_t     waveform_8bit[N_REG_WAVEFORM]          = {0};

/* [] END OF FILE */
