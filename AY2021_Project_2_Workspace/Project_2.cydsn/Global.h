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

#include "project.h"
#include "cytypes.h"
#include "ErrorCodes.h"
#include "I2C_Interface.h"
#include "stdlib.h"


/******************************************/
/*        CONFIGURATION REGISTERS         */
/******************************************/

#define LIS3DH_DEVICE_ADDRESS    0x18

#define LIS3DH_CTRL_REG0 0x1E
#define LIS3DH_CTRL_REG0_INIT 0b00010000

#define LIS3DH_TEMP_CFG_REG 0x1F
#define LIS3DH_TEMP_CFG_REG_INIT 0b10000000

#define LIS3DH_CTRL_REG1 0x20
#define LIS3DH_CTRL_REG1_INIT 0b01000111

#define LIS3DH_CTRL_REG3 0x22
#define LIS3DH_CTRL_REG3_INIT 0b00010000

#define LIS3DH_CTRL_REG4 0x23
#define LIS3DH_CTRL_REG4_INIT 0b00001000

#define LIS3DH_CTRL_REG5 0x24
#define LIS3DH_CTRL_REG5_INIT 0b01001010

#define LIS3DH_CTRL_REG6 0x25
#define LIS3DH_CTRL_REG6_INIT 0b00101000

#define LIS3DH_FIFO_CTRL_REG 0x2E
#define LIS3DH_FIFO_CTRL_REG_INIT 0b10000000

#define LIS3DH_INT2_CFG 0x34
#define LIS3DH_INT2_CFG_INIT 0b00111111

#define LIS3DH_INT2_THS 0x36
#define LIS3DH_INT2_THS_INIT 0b00001111

#define LIS3DH_INT2_DURATION 0x37
#define LIS3DH_INT2_DURATION_INIT 0b00001111

#define OUT_X_L 0x28

/******************************************/
/*              OTHER MACROS              */
/******************************************/

#define N_REG_ACC 6
#define G           9.80665
#define mg_TO_g     0.001
#define DC_100 65535

/******************************************/
/*            GLOBAL VARIABLES            */
/******************************************/

extern uint8_t reg;
extern uint8_t flag_ACC;

extern uint8_t data[6];              //Used to save the acceleration values READ by the MULTIREAD
extern int16   dataX;                //Used to store the X-axis acceleration in 16bit
extern int16   dataY;                //Used to store the Y-axis acceleration in 16bit
extern int16   dataZ;                //Used to store the Z-axis acceleration in 16bit
extern float32 accX;
extern float32 accY;
extern float32 accZ;

extern uint8_t FS_range;
extern uint8_t Sensitivity;
extern uint16_t DC_R;
extern uint16_t DC_G;
extern uint16_t DC_B;


/******************************************/
/*                FUNCTIONS               */
/******************************************/

void Register_Initialization(void);

void Set_RGB(void);





/* [] END OF FILE */
