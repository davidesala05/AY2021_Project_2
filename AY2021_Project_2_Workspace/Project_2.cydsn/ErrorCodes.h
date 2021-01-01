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
*  \brief Error codes definition
* 
*  This definition identifies several error codes that will
*  be used throughout the project to be consistent with the written firwmare.
*  Variables of the ErrorCode type are used as output of the functions devoted 
*  to the reading and the writing operations done using the I2C protocol.
*/

#ifndef __ERRORCODES_H
    #define __ERRORCODES_H
    
    typedef enum {
        NO_ERROR,           /// No error generated
        ERROR               /// Error generated
    } ErrorCode;
#endif

/* [] END OF FILE */
