/**
******************************************************
* @file Documentacion.h
* @brief Conjunto de funciones para manejo de dispositivos GSM
* @author Julian Salas Bartolome
* @version 2.0
* @date Septiembre 2015
*
*
*******************************************************/




/**
 * \mainpage GSM 2
 *
 * \section introduccion_seccion Introduccion
 *
 * Estas funciones perimiten acciones basicas para poder gestionar un telecontrol mediante GSM ( Envio SMS, Recepcion SMS, etc)
 *  
 * El modulo se comunica con el microcontrolador por un puerto serie
 *
 * @warning Estas funciones necesitan los paquetes JSB_String y JSB_Buffer
 *
 * cBuffer en un array definido como gloabal en JSB_Buffer y que contiene la informacion que se recibe desde el modulo GSM. cBuffer se gestiona con las funciones JSB_Buffer y
 * es utilizado directamente por las funciones GSM
 *
 *
 * Para llenar el buffer se utilizara normalmente el servicio de interrupcion de una comunicacion serie
 *
 *		#int_rda
 *		void Rx232() 
 *		{ 
 *			// Interrupcion USART
 *			// Recibe dato Rs232 y lo almacena en el Buffer
 *			char cRx;
 *			cRx=0; 
 *			cRx=getchar();
 *			AddCharBuff(cRx); 
 *		} 
 *
 * Para tratar la informacion del buffer se recurre a las funciones JSB_String
 *
 *
 */


/*! \page  page1  Paginas locales
  	\tableofcontents
  		* Enlaces a informacion local
		* @brief Enlaces a informacion sobre los comandos AT utilizados en la mayoria de modulos

  		\section sec1 Informacion tecnica 
	   		<a href="..\Documentacion\gsm822w.pdf">Comandos AT</a> 
*/





/** 
 *
 * @brief En proyectos PIC aparecen varios proyectos realizdos con estas funciones
 *
 * 
*/
/*! \example Ejemplos
 */

