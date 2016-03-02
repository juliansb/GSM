/**
******************************************************
* @file JSB_GSM.c
* @brief Conjunto de funciones para control del modulo GSM
* @author Oscar Salas Mestres & Julian Salas Bartolome
* @version 1.0
* @date Agosto 2012
*
*******************************************************/











/**
******************************************************
* @brief Manda el PIN a un modulo GSM
*
* Antes de enviar el PIN temporiza 5 Sg para que el modulo pueda estabilizarse
*
* @param cPIN Pin a enviar al modulo
* @return Devuelve 1 si el modulo ha respondido Ok y 0 en caso contrario
*
* @post Se borra el buffer
*
*
* Ejemplo:
*    
*	  char cPIN[]="1234";
*	  int1 nResultado = JSB_EnviaPIN (cPIN);
*
* Resultado:
*
*	  nResultado = 1 	
*
* @see JSB_IniciaGSM(), JSB_TestGSM() , JSB_CompruebaPIN(), JSB_TestRegistro ()		
*/
int1 JSB_EnviaPIN ( char *cPin )
{
 	//-------------------------------------------------------------	
	//Definicion de variables
	//-------------------------------------------------------------	
	int1 nResultado;
  char cOk[]="OK";
 	int nOk;
	//-------------------------------------------------------------	

   	delay_ms(2000);                                             			                       //Esperamos 2 sg para que se inicilalice el modulo
   	BorraBuffer();                                              			                       //Borramos Buffer para asegurarnos de que esta limpio
    enable_interrupts(int_rda);                                 			                       // Habilita Interrupción RDA para recepcion de caracters
   	printf("AT+CPIN=%s\r",cPin);                                			                       //Mandamos secuencia de PIN
   	delay_ms(5000);									                                                         //Temporizamos 1 Sg	
  	nOk=JSB_StrPos(cBuffer, cOk, 0);						                                             //Comprobamos si en la respuesta aparece el texto OK
  	if ( nOk != -1 )								                                                         //Si existe el texto es que el modulo ha aceptado el PIN	
   	{
		  nResultado = 1;                                                                        //Y devolvemios 1
   	}
   	else
   	{
      nResultado = 0;							                                                           //En caso contrario devolvemos 0
   	} 
    BorraBuffer();                                              			                       //Borramos Buffer 
   	return (nResultado);
}
/**
******************************************************
* @brief Comprueba si el modulo ya tiene el PIN ( activado )
*
* @return Devuelve 1 si el modulo ha respondido Ok y 0 en caso contrario
*
* @post Se borra el buffer
*
* Ejemplo:
*
*   int1 nResultado = JSB_CompruebaPIN ();
*
* Resultado:
*
*   nResultado = 1 	
*
* @see JSB_IniciaGSM(), JSB_TestGSM() , JSB_EnviaPIN(), JSB_TestRegistro ()    
*/
int1 JSB_CompruebaPIN (void)
{
	//-------------------------------------------------------------	
	//Definicion de variables
	//-------------------------------------------------------------	
		int1 nResultado;
		char cPinCmp[]="READY"; 
		int nOk;
	//-------------------------------------------------------------	

    BorraBuffer();                                              			                       //Borramos Buffer 
   	enable_interrupts(int_rda);                                 			                       //Habilita Interrupción RDA para recepcion de caracters
   	printf("AT+CPIN?\r");                                       			                       //Mandamos secuencia AT+CPIN?, nos debe devolver +CPIN: READY en caso de que tennga el pin, si no tiene pin nos devovera ERROR
   	delay_ms(1000);					   				                                                       //Temporizamos 1 Sg	
   	nOk=JSB_StrPos(cBuffer, cPinCmp, 0);						                                         //Comprobamos si en la respuesta aparece el texto READY 	
  	if ( nOk != -1 	)								                                                         //Si existe el texto es que el modulo ya tiene PIN	
   	{
		  nResultado = 1;								                                                         //y devolvemos 1
   	}
   	else
   	{
		  nResultado=0;								                                                           //En caso contrario devolvemos 0
   	} 
  	BorraBuffer();                                               			                       //Borramos Buffer
   	return (nResultado); 
}
/**
******************************************************
* @brief Comprueba si el modulo GSM esta conectado
*
* @return Devuelve 1 si el modulo ha respondido Ok y 0 en caso contrario
*
* @post Se borra el buffer
*
* Ejemplo:
*
*	  int1 nResultado = JSB_TestGSM ();
*
* Resultado:
*
*	  nResultado = 1 	
*
* @see  JSB_TestGSM() , JSB_CompruebaPIN(), JSB_EnviaPIN()  , JSB_TestRegistro ()  
*/
int1 JSB_TestGSM (void)
{
	//-------------------------------------------------------------	
	//Definicion de variables
	//-------------------------------------------------------------	
   	int1 nResultado;
    char cOk[]="OK";
 		int nOk;
	//-------------------------------------------------------------	

   	BorraBuffer();                                              			                       //Borramos Buffer
   	enable_interrupts(int_rda);                                 			                       //Habilita Interrupción RDA para recepcion de caracters
   	printf("AT\r");                                             			                       //Mandamos secuencia AT, nos debe devolver OK
   	delay_ms(1000);															                                             //Temporizamos 1 Sg	
  	nOk=JSB_StrPos(cBuffer, cOk, 0);										                                     //Comprobamos si en la respuesta aparece el texto OK
  	if ( nOk != -1 )														                                             //Si existe es que el modulo esta conectado	
   	{
		  nResultado = 1;														                                             //Y devolvemos 1
   	}
   	else
   	{
       	nResultado = 0;													                                             //En caso contrario devolvemos 0
   	} 
   	BorraBuffer();                                               			                       //Borramos Buffer
   	return (nResultado);
}
/**
******************************************************
* @brief Inicia el modulo GSM
*
* Comprueba que el modulo esta conectado llamando a JSB_TestGSM()
* Comprueba si tiene numero PIN con JSB_CompruebaPIN()
* Si no lo tiene puesto, lo pone con JSB_EnviaPIN(), en ese caso temporiza 20 sg para estabilizar el modulo GSM
*
* @return Devuelve 1 si el modulo se ha inicializado correctamene, 0 en caso contrario
* @param cPIN Pin a enviar al modulo
*
*
* Ejemplo:
*
*	  char cPIN[]="1234";
*	  int1 nResultado = JSB_IniciaGSM (cPin);
*
* Resultado:
*
*	  nResultado = 1 	
*
* @see  JSB_TestGSM() , JSB_CompruebaPIN(), JSB_EnviaPIN()    
*/
int1 JSB_IniciaGSM (char *cPin)
{
	//-------------------------------------------------------------	
	//Definicion de variables
	//-------------------------------------------------------------	
   	int1 nResultado;
 	//-------------------------------------------------------------	
	
    BorraBuffer();                                                                           //Borramos Buffer
    printf("ATV1\r");                                           			                       //Nos aseguramos de la respuesta alfabetica ( la respuesta puede ser alfabetica como ERROR o numerica como 2)
    delay_ms(2000);						   									                                           //Temporizamos 1 sg

    printf("ATE0\r");                                           			                       //Nos aseguramos que no hay Eco
    delay_ms(2000);						   									                                           //Temporizamos 1 sg
    BorraBuffer(); 															                                             //Borramos el Buffer 
    nResultado=JSB_TestGSM();												                                         //Comprobamos si el modulo esta conectado
    if (nResultado==1)														                                        
    {
        nResultado=JSB_CompruebaPIN();									                                     //Si lo esta, comprobamos si tiene PIN
		    if (nResultado==0)													                                         //Si no lo tiene
		    {
    	     nResultado=JSB_EnviaPIN(cPin);								                                     //Se lo enviamos
			     delay_ms(1000);													                                         //Esparamos 1 sg a que se estabilice el modulo
		    }		
	  }
    return (nResultado);													                                           //Retornamos el resultado de la inicializacion, sera 1 si pasa todos los test
}
/**
******************************************************
* @brief Comprueba si el modulo GSM esta registrado en la red
*
* @return Devuelve 1 si el modulo esta registrado y 0 en caso contrario
*
* @post Se borra el buffer
*
* Ejemplo:
*
*   int1 nResultado = JSB_TestRegistro ();
*
* Resultado:
*
*   nResultado = 1  
*
* @see  JSB_TestGSM() , JSB_CompruebaPIN(), JSB_EnviaPIN()    
*/
int1 JSB_TestRegistro (void)
{

  //------------------------------------------------------------- 
  //Definicion de variables
  //------------------------------------------------------------- 
    int1 nResultado = 0;
    char cOk[]=",1";
    int nOk;
  //------------------------------------------------------------- 

    BorraBuffer();                                                                         //Borramos buffer
    printf("AT+CREG?\r");                                                                  //Interrogamos por el registro en la red
    delay_ms(2000);                                                                        //Temporizamos 2 sg
    nOk=JSB_StrPos(cBuffer, cOk, 0);                                                         //Comprobamos si en la respuesta aparece el texto OK
    if ( nOk != -1 )                                                                         //Si existe es que el modulo esta conectado  
    {
      nResultado = 1;                                                                        //Y devolvemos 1
    }
    else
    {
        nResultado = 0;                                                                      //En caso contrario devolvemos 0
    } 
    BorraBuffer();                                                                           //Borramos Buffer
    return (nResultado);
}
/**
******************************************************
* @brief Resetea el modulo GSM
*
* @return Devuelve 1 si el modulo esta reseteado y 0 en caso contrario
*
* @post Se borra el buffer
*
* Ejemplo:
*
*   int1 nResultado = JSB_ResetGSM ();
*
* Resultado:
*
*   nResultado = 1  
*
* @see  JSB_TestGSM() , JSB_CompruebaPIN(), JSB_EnviaPIN()    
*/
int1 JSB_ResetGSM (void)
{

  //------------------------------------------------------------- 
  //Definicion de variables
  //------------------------------------------------------------- 
    int1 nResultado = 0;
    char cOk[]="OK";
    int nOk;
  //------------------------------------------------------------- 

    BorraBuffer();                                                                         //Borramos buffer
    printf("AT+CFUN=1\r");                                                                  //Reseteamos el modulo
    delay_ms(2000);                                                                        //Temporizamos 2 sg
    nOk=JSB_StrPos(cBuffer, cOk, 0);                                                         //Comprobamos si en la respuesta aparece el texto OK
    if ( nOk != -1 )                                                                         //Si existe es que el modulo esta conectado  
    {
      nResultado = 1;                                                                        //Y devolvemos 1
    }
    else
    {
        nResultado = 0;                                                                      //En caso contrario devolvemos 0
    } 
    BorraBuffer();                                                                           //Borramos Buffer
    return (nResultado);
}
/**
******************************************************
* @brief Comprueba si existe un SMS determinado
*
* @param nSMS Numero de SMS que se quiere comprobar si existe
* @return Devuelve 1 si el SMS existe, 0 en caso contrario
*
* @post Se borra el buffer
*
* Ejemplo:
*
*	  int nSms=3;
*	  int1 nResultado = TestSMS (nSms);
*
* Resultado:
*
*	  nResultado = 1 	
*
* @see JSB_BorraSMS(), JSB_EnviaSMS (), JSB_LeeSMS(), JSB_LeeRemitenteSMS ()		
*/
int1 JSB_TestSMS (int nSMS)
{
 
	//-------------------------------------------------------------	
	//Definicion de variables
	//-------------------------------------------------------------	
   	int1 nResultado=0;
    char cOk[]="OK";
 		int nOk;
	//-------------------------------------------------------------	

 
  	BorraBuffer();                                 	                                         //Borramos Buffer 
   	enable_interrupts(int_rda);                    	                                         // Habilita Interrupción RDA para recepcion de caracters
  	printf("AT+CMGR=%U\r",nSMS);                   	                                         //Mandamos secuencia de leer mensaje
   	delay_ms(1000);									                                                         //Temporizamos 1 sg	
  	nOk=JSB_StrPos(cBuffer, cOk, 0);				                                                 //Comprobamos si en la respuesta aparece el texto OK
  	if ( nOk != -1 )								                                                         //Si la respuesta es Ok es que el mensaje existe	
   	{
		    nResultado = 1;								                                                       //En ese caso devolvemos 1
   	}else{
		    nResultado=0;								                                                         //En caso contrario devolvemos 0
   	}
   	BorraBuffer();                                               			                       //Borramos Buffer 
   	return (nResultado);
}
/**
******************************************************
* @brief Lee el contenido de un SMS determinado
*
* @param nSMS Numero del SMS que se desea Leer
* @return Devuelve el puntero al contenido del SMS
*
* @post Se borra el buffer
* @post Debe liberarse la memoria de la salida
*
* Ejemplo:
*
*	  int1 nSalida;
*	  int nSms=3;
*	  char cTexto[20];
*	  nSalida=JSB_TestSMS(nSms);
*	  if (nSalida==1)
*	  {
*		 char *cSms = JSB_LeeSMS (nSms);
*		 strcpy (cTexto, cSms);	
*		 free (cSms);
*	  }
*
* Resultado:
*
*	  cTexto = "CALDERA" 
*
* @see JSB_BorraSMS(), JSB_EnviaSMS (), JSB_TestSMS(), JSB_LeeRemitenteSMS (), JSB_Borra_N_SMS ()	
*/
char* JSB_LeeSMS (int nSMS)
{
 
	//-------------------------------------------------------------	
	//Definicion de variables
	//-------------------------------------------------------------	
    char cLimitadorLinea[]="\r\n";
 		char *cSms;
 	//-------------------------------------------------------------	

 
  	BorraBuffer();                                           			                           //Borramos Buffer
   	enable_interrupts(int_rda);                                 			                       // Habilita Interrupción RDA para recepcion de caracters
  	printf("AT+CMGR=%U\r",nSMS);                                			                       //Mandamos secuencia de leer mensaje
   	delay_ms(1000);									                                                         //Temporizamos 2 sg	
    cSms = JSB_ExtraeLinea(cBuffer, cLimitadorLinea, 2);				                             //Leemos el contenido de la linea 2 que es el contenido del SMS
   	BorraBuffer();                                               			                       //Borramos Buffer 
    return (cSms);									//Devolvemos el contenido del SMS
}
/**
******************************************************
* @brief Lee el numero del remitente de un sms
*
* @param nSMS Numero del SMS que se desea Leer
* @return Devuelve el puntero al numero de telefono del remitente del sms
*
* @post Se borra el buffer
* @post Debe liberarse la memoria de la salida
*

*
@see JSB_BorraSMS(), JSB_EnviaSMS (), JSB_TestSMS(), JSB_Borra_N_SMS (), JSB_LeeSMS()   
*/
char* JSB_LeeRemitenteSMS (int nSMS)
{
 
  //------------------------------------------------------------- 
  //Definicion de variables
  //------------------------------------------------------------- 
    char cLimitadorLinea[]="\r\n";
    char cLimitadorCampo[]=",";
    char *cSms;
    char *cTelefonoTmp;
    char *cTelefono = malloc(10);
  
  //------------------------------------------------------------- 

 
    BorraBuffer();                                                                           //Borramos Buffer  
    enable_interrupts(int_rda);                                                              // Habilita Interrupción RDA para recepcion de caracters
    printf("AT+CMGR=%U\r",nSMS);                                                             //Mandamos secuencia de leer mensaje
    delay_ms(1000);                 //Temporizamos 2 sg 
    cSms = JSB_ExtraeLinea(cBuffer, cLimitadorLinea, 1);                                     //Leemos el contenido de la linea 1 que contiene los datos identificativos del SMS
    cTelefonoTmp = JSB_ExtraeLinea(cSms, cLimitadorCampo, 1);                                //Leemos el campo donde esta el telefono del remitente
    cTelefono = JSB_SubStr ( cTelefonoTmp, 4, 12 );
    free (cSms);                                                                             //Liberamos memoria de variable cSms   
    free (cTelefonoTmp);                                                                     //Liberamos memoria de la variable cTelefonoTmp
    BorraBuffer();                                                                           //Borramos Buffer 
    return (cTelefono);                                                                      //Devolvemos el contenido del SMS
}




/**
******************************************************
* @brief Borra un SMS determinado
*
* @param nSMS Numero de SMS a borrar
* @return Devuelve 1 si se ha borrado el mensaje con exito, 0 en caso contrario
*
* @pre Debe comprobarse antes la existencia del mensaje
* @post Se borra el buffer
*
*
* Ejemplo:
*
*   int1 nSalida;
*   int nSms=3;
*   char cTexto[20];
*   nSalida=JSB_TestSMS(nSms);
*   if (nSalida==1)
*   {
*     nSalida = JSB_BorraSMS (nSms);
*   }
* 
* Resultado:
*
*   Borra el mensaje -->nSalida = 1 
*
@see JSB_EnviaSMS (), JSB_TestSMS(), JSB_LeeRemitenteSMS (), JSB_Borra_N_SMS (), JSB_LeeSMS()
*/

int1 JSB_BorraSMS (int nSMS)
{
  //------------------------------------------------------------- 
  //Definicion de variables
  //------------------------------------------------------------- 
    int1 nResultado=0;
    char cOk[]="OK";;
    int nOk;
  //------------------------------------------------------------- 
    BorraBuffer();                                                                           //Borramos Buffer 
    enable_interrupts(int_rda);                                                              // Habilita Interrupción RDA para recepcion de caracters
    printf ("AT+CMGD=%U\r",nSMS);                                                            //Mandamos la secuencia de borrado del mensaje
    delay_ms(2000);                                                                          //Temporizamos 2 sg
    nOk=JSB_StrPos(cBuffer, cOk, 0);                                                         //Comprobamos si en la respuesta aparece el texto OK
    if ( nOk != -1 )                                                                         //Si aparece, el mensaje se ha borrado correctamente
    {                   
        nResultado = 1;                                                                      //Y devolvemos 1
    }
    else
    {
        nResultado = 0;                                                                      //En caso contrario devolvemos 0
    } 
    BorraBuffer();                                                                           //Borramos Buffer 
    return (nResultado);    
}

/**
******************************************************
* @brief Borra un numero determminado de SMS's ( nSMS )
*
* @param nSMS Numero de mensajes a borrar
* @return Devuelve 1 si se han podido borrar todos los mensajes, 0 en caso de que haya alguno que no se haya podido borrar
*
*
* Ejemplo:
*   
*   int1 nSalida;
*   int nSms=20;
*   nSalida = JSB_Borra_N_SMS (nSms)
*
* Resultado:
* 
*   Borra los primeros nSMS SMS's
*
@see JSB_BorraSMS(), JSB_EnviaSMS (), JSB_TestSMS(), JSB_LeeRemitenteSMS (), JSB_LeeSMS()
*/

JSB_Borra_N_SMS ( int nSMS )
{
  //------------------------------------------------------------- 
  //Definicion de variables
  //------------------------------------------------------------- 
    int1 lResultado=1;
    int nMensaje;
    int1 lBorrado;
  //------------------------------------------------------------- 
  
    for (nMensaje=1;nMensaje<nSMS+1;nMensaje++)                                              //Hacemos un bucle para tratar todos los SMS solicitados
    {
        if ( JSB_TestSMS (nMensaje) )                                                        //Si existe el SMS tratado 
        {
          lBorrado = JSB_BorraSMS (nMensaje);                                                //Lo intentamos borrar  
          if ( lBorrado )                                                                    //Tanto si se ha borrado como si no se ha borradoo actualizamos el parametro de retorno
          {
            lResultado &= 1;                                                                 
          }else{
            lResultado &= 0;
          }
        }else{
         lResultado &= 1;                                                                     //Si el mensaje no existe, actualizamos el parametro de retorno
        }
    } 
    return ( lResultado );
}
/**
******************************************************
* @brief Envia un SMS a un numero determinado
*
* @param cNumero Numero del movil al que se le envia el SMS 
* @param cMensaje Puntero al texto que se desea enviar
* @return Devuelve 1 si se ha enviado el mensaje con exito en un tiempo inferior a 5 Sg, 0 en caso contrario
*
*
* Ejemplo:
*
*   int1 lOk;
*   char  cNumSMS[10]="609090909";;
*   char *cMensaje[]="El modulo GSM  se ha puesto en marcha";
*   lOk=EnviaSMS ( cNumSMS, cMensaje );
*
* Resultado:
*
*   Envia SMS --> lOk=1
*
* @see JSB_LeeSMS(), JSB_BorraSMS(), JSB_TestSMS()    
*/

int1 JSB_EnviaSMS(char *cNumero, char *cMensaje)
{
  //------------------------------------------------------------- 
  //Definicion de variables
  //------------------------------------------------------------- 
    int1 nResultado=0;
    char cOk[]="OK";
    char cPermiso = ">";
    int nOk;
  //------------------------------------------------------------- 

    BorraBuffer();                                                                           //Borramos Buffer
    enable_interrupts(int_rda);                                                              //Habilita Interrupción RDA para recepcion de caracters
    printf("AT+CMGS=\"+34%s\"\r", cNumero);                                                  //Enviamos la primera secuencia
    delay_ms(1200);                                                                          //Temporizamos 1,2 Sg
    nOk=JSB_StrPos(cBuffer, cOk, 0);                                                         //Comprobamos si en la respuesta del modulo aparece '>'
    if ( nOk )
    {  
        BorraBuffer();                                                                           //Borramos Buffer
        printf ("%s",cMensaje);                                                                  //Enviamos el texto 
        putc(0x1A);                                                                              //Enviamos el caracter fin de mensaje 
        delay_ms(5000);                                                                          //Retardo de precaucion para que el modulo GSM se estabililce y pueda seguir recibiendo ordenes   
        nOk=JSB_StrPos(cBuffer, cOk, 0);                                                         //Comprobamos si en la respuesta del modulo aparece el texto OK
        if ( nOk != -1 )                                                                         //Si existe es que el envio ha sido correcto  
        {
            nResultado = 1;                                                                      //Y ponemos la salida a 1
        }else{
            nResultado = 0;                                                                      //En caso contrario lo ponemos a 0
        }
    }
    return nResultado;

}
/**
******************************************************
* @brief Lee el fabricante del modulo GSM
*
* @return Devuelve el fabricante del modulo
*
* @post Se borra el buffer
*
*
* Ejemplo:
*
*	  char *cFabricante;
*   cFabricante = JSB_LeeFabricante ();	
*
* Resultado:
*
*	  cFabricante = " WAVECOM MODEM" 
*
* @see 	JSB_LeeModelo(), JSB_LeeRevision	
*/
char* JSB_LeeFabricante (void)
{
 
	//-------------------------------------------------------------	
	//Definicion de variables
	//-------------------------------------------------------------	
    char cLimitadorLinea[]="\r\n";
		char *cFabricante;
	//-------------------------------------------------------------	

  	BorraBuffer();                                           			                           //Borramos Buffer 
   	enable_interrupts(int_rda);                                 			                       //Habilita Interrupción RDA para recepcion de caracters
  	printf("AT+CGMI\r");                                			                               //Mandamos secuencia de leer mensaje
   	delay_ms(1000);									                                                         //Temporizamos 2 sg	
	  cFabricante = JSB_ExtraeLinea(cBuffer, cLimitadorLinea, 1);				                       //Leemos el contenido de la linea 2 que es el contenido del SMS
	  BorraBuffer();                                               			                       //Borramos Buffer
    return (cFabricante);									                                                   //Devolvemos el contenido del fabricante
}
/**
******************************************************
* @brief Lee el modelo del modulo GSM
*
* @return Devuelve el modelo del modulo
*
* @post Se borra el buffer
*
*
* Ejemplo:
*
*	  char *cModelo;
*   cModelo = JSB_LeeModelo ();	
*
* Resultado:
*
*	  cModelo = " 900P" 
*
* @see	JSB_LeeFabricante(), JSB_LeeRevision 		
*/
char* JSB_LeeModelo (void)
{
 
	//-------------------------------------------------------------	
	//Definicion de variables
	//-------------------------------------------------------------	
    char cLimitadorLinea[]="\r\n";
		char *cModelo;
	//-------------------------------------------------------------	
 
  	BorraBuffer();                                           			                           //Borramos Buffer 
   	enable_interrupts(int_rda);                                 			                       // Habilita Interrupción RDA para recepcion de caracters
  	printf("AT+CGMM\r");                                			                               //Mandamos secuencia de leer mensaje
   	delay_ms(1000);									                                                         //Temporizamos 2 sg	
	  cModelo = JSB_ExtraeLinea(cBuffer, cLimitadorLinea, 1);			                             //Leemos el contenido de la linea 2 que es el contenido del SMS
	  BorraBuffer();                                            			                         //Borramos Buffer
    return (cModelo);									                                                       //Devolvemos el modelo del modulo
}
/**
******************************************************
* @brief Lee la Revision del modulo GSM
*
* @return Devuelve la revision del modulo
*
* @post Se borra el buffer
*
*
* Ejemplo:
*
*	  char *cRevision;
*   cRevision = JSB_LeeRevision ();	
*
* Resultado:
*
*   cModelo = "322_G250.53838032 011000 10:50" 
*
* @see	JSB_LeeFabricante(), JSB_LeeModelo 		
*/
char* JSB_LeeRevision (void)
{
 
	//-------------------------------------------------------------	
	//Definicion de variables
	//-------------------------------------------------------------	
    char cLimitadorLinea[]="\r\n";
 		char *cRevision;
	//-------------------------------------------------------------	

 
  	BorraBuffer();                                           			                           //Borramos Buffer
   	enable_interrupts(int_rda);                                 			                       //Habilita Interrupción RDA para recepcion de caracters
  	printf("AT+CGMR\r");                                			                               //Mandamos secuencia de leer mensaje
   	delay_ms(1000);									                                                         //Temporizamos 2 sg	
	  cRevision = JSB_ExtraeLinea(cBuffer, cLimitadorLinea, 1);				                         //Leemos el contenido de la linea 2 que es el contenido del SMS
   	delay_us(1);
	  BorraBuffer();                                               			                       //Borramos Buffer
    return (cRevision);									                                                     //Devolvemos la revision
}




