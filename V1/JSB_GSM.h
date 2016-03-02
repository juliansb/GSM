/**
******************************************************
* @file JSB_GSM.h
* @brief Conjunto de funciones para control del modulo GSM
* @author Oscar Salas Mestres & Julian Salas Bartolome
* @version 1.0
* @date Agosto 2012
*
*******************************************************/



#ifndef _JSB_GSM
#define _JSB_GSM

#include <stdio.h>
#include <stdlib.h>
#include <stdlibm.h>

#include "JSB_Buffer.h"
#include "JSB_String.h"




/** @defgroup group1 Funciones para gestion de un modulo GSM
 *  @brief Funciones para gestion de un modulo GSM 
 *  @{
 */

/** @defgroup group2 Funciones de inicializacion 
 *  @brief Conjunto de funciones para inicializar el modulo GSM
 *  @{
 */

int JSB_IniciaGSM (char *cPin);
int1 JSB_TestGSM (void);
int1 JSB_CompruebaPIN (void);
int1 JSB_EnviaPIN ( char *cPin );
int1 JSB_TestRegistro (void);
int1 JSB_ResetGSM();

/** @} */ // end of group2

/** @defgroup group3 Funciones SMS
 *  @brief Conjunto de funciones para el tratamiento de los SMS
 *  @{
 */

int1 JSB_TestSMS (int nSms);
char* JSB_LeeSMS ( int nSms);
char* JSB_LeeRemitenteSMS (int nSMS);
int1 JSB_BorraSMS ( int nSms);
int1 JSB_EnviaSMS (char *cNumero, char *cMensaje);
int1 JSB_Borra_N_SMS (int nSMS);

/** @} */ // end of group3 

/** @defgroup group4 Funciones de dispositivo
 *  @brief Conjunto de funciones para para obtener informacion sobre caracteristcas del modulo
 *  @{
 */

char* JSB_LeeFabricante (void);
char* JSB_LeeModelo (void);
char* JSB_LeeRevision (void);

/** @} */ // end of group4

/** @} */ // end of group1

#include "JSB_Buffer.h"
#include "JSB_GSM.c"

#endif