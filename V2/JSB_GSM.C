/**
******************************************************
* @file JSB_GSM.c
* @brief Conjunto de funciones para control del modulo GSM
* @author Oscar Salas Mestres & Julian Salas Bartolome
* @version 1.0
* @date Agosto 2012
*
* Historia
* 
* Version 1.0 Agosto 2012
*
*  - Comandos basicos aplicados al modulo WMOD2 de WAVECOM
*
* Version 2.0 Diciembre 2016
*
* Se realizan modificaciones necesarias para utilizacion del modulo M590 de NEOWAY
* Ante la posibilidad de encontrar diversos modulos, se crea la variable de compliacion Modo que adquirira´los valores de Modo_0, Modo_1, ... para usar en la compilacion las funciones adecuadas
*
*   - Se añade la funcion JSB_TestRegistro que se encarga de comrpobar que el modulo se ha registrado en la red
*
*   - Se añade la funcion JSB_ResetGSM para resetear el modulo en caso de que se necesite reiniciarlo desde 0
*
*   - Se incorpora una nueva version de JSN_EnviaSMS que adecua el envio de SMS multilinea al modulo NEOWAY M590
*     Se observo que la respuesta del modulo a un retorno de carro era lenta y se perdian caracteres de la siguiente linea por lo que
*     ha sido necesario separar las lineas y temporizar entre el envio de cada linea para no perder caracteres 
*
*   - Se modifica la funcion JSB_IniciaGSM, ademas de comprobar/introducir el PIN, comprueba si el modulo se ha registrado en la red ( 15 intentos )
*     y si se registra correctamente temporiza 30 sg para asegurar la estabilizacion del modulo 
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
* @see JSB_IniciaGSM(),  JSB_CompruebaPIN()		
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

   	BorraBuffer();                                              			                       //Borramos Buffer para asegurarnos de que esta limpio
    enable_interrupts(int_rda);                                 			                       // Habilita Interrupción RDA para recepcion de caracters
   	printf("AT+CPIN=\"%s\"\r",cPin);                                			                   //Mandamos secuencia de PIN
   	delay_ms(4000);									                                                         //Temporizamos 4 Sg esperando la respuesta	
  	nOk=JSB_StrPos(cBuffer, cOk, 0);						                                             //Comprobamos si en la respuesta aparece el texto OK
  	if ( nOk != -1 )								                                                         //Si existe el texto OK es que el modulo ha aceptado el PIN	
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
* @see JSB_IniciaGSM(),  JSB_EnviaPIN()    
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
* @brief Comprueba si el modulo GSM esta conectado AL puerto serie
*
* @return Devuelve 1 si el modulo ha respondido Ok y 0 en caso contrario
*
* @post Se borra el buffer
*
* Ejemplo:
*
*
*	  int1 nResultado = JSB_TestGSM ();
*
* Resultado:
*
*	  nResultado = 1 	
*
* @see  JSB_IniciaGSM()  
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
* @brief Comprueba si el modulo GSM esta registrado en la red
*
* Comprueba la conexion hasta 15 veces dejando un tiempo de 2 sg entre comprobacion y comprobacion
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
* @see  JSB_InicioGSM()
*/
int1 JSB_TestRegistro (void)
{

  //------------------------------------------------------------- 
  //Definicion de variables
  //------------------------------------------------------------- 
    int1 nResultado = 0;
    char cOk[]=",1";
    int nOk;
    int nIntentos = 0;
  //------------------------------------------------------------- 

    while ( nIntentos < 15 & !nResultado )
    {  
        BorraBuffer();                                                                       //Borramos buffer
        printf("AT+CREG?\r");                                                                //Interrogamos por el registro en la red
        delay_ms(2000);                                                                      //Temporizamos 2 sg
        nOk=JSB_StrPos(cBuffer, cOk, 0);                                                     //Comprobamos si en la respuesta aparece el texto OK
        if ( nOk != -1 )                                                                     //Si existe es que el modulo esta conectado  
        {
            nResultado = 1;                                                                  //Y devolvemos 1
        }
        else
        {
            nResultado = 0;                                                                  //En caso contrario devolvemos 0 y hacemos un nuevo intento si no se han superado los 15
        } 
        nIntentos++;
        delay_ms(2000);                                                                      //Temporizamos 2 sg
    }
    BorraBuffer();                                                                           //Borramos Buffer
    return (nResultado);
}
/**
******************************************************
* @brief Inicia el modulo GSM
*
* Comprueba que el modulo esta conectado llamando a JSB_TestGSM()
* Comprueba si tiene numero PIN con JSB_CompruebaPIN()
* Si no lo tiene puesto, lo pone con JSB_EnviaPIN()
* Si se pasa el proceso del PIN, se comprueba si esta registrado en la red y, si lo esta, temporiza 30 Sg para asegurar la estabilizacion dle modulo
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
* @see  JSB_TestGSM() , JSB_TestRegistro (), JSB_CompruebaPIN(), JSB_EnviaPIN()    
*/
int1 JSB_IniciaGSM (char *cPin)
{
	//-------------------------------------------------------------	
	//Definicion de variables
	//-------------------------------------------------------------	
   	int1 nResultado;
	char cComando[25];
 	//-------------------------------------------------------------	
	
    BorraBuffer();                                                                           //Borramos Buffer
    printf("ATV1\r");                                           			                       //Nos aseguramos de la respuesta alfabetica ( la respuesta puede ser alfabetica como ERROR o numerica como 2)
    delay_ms(2000);						   									                                           //Temporizamos 2 sg

    printf("ATE0\r");                                           			                       //Nos aseguramos que no hay Eco
    delay_ms(2000);						   									                                           //Temporizamos 2 sg

    BorraBuffer(); 		                                                                       //Borramos el Buffer 
    nResultado=JSB_TestGSM();												                                         //Comprobamos si el modulo esta conectado
    if (nResultado==1)			                                                                 //Si esta conecetado											                                        
    {
        delay_ms(2000);                                                                      //Esperamos 2 sg para que se inicilalice el modulo
        nResultado=JSB_CompruebaPIN();									                                     // comprobamos si tiene PIN
		    if (nResultado==0)													                                         //Si no lo tiene
		    {
    	     nResultado=JSB_EnviaPIN(cPin);								                                     //Se lo enviamos
		    }		
	  }
    if (nResultado==1)                                                                       //Si se ha pasado satisfactoriamente el paso del PIN esperamos a que el modulo se registre en la red
    {
        nResultado = JSB_TestRegistro();                                                     //Comprobamos si el modulo se ha registrado en la red
    }

    if (nResultado==1)                                                                       //Si se ha pasado satisfactoriamente el paso del PIN y esta conectado a la red temporizamos 30 sg para estabilizar el modulo
    {
        delay_ms(30000);                                                                     //Temporizamos 30 sg para que se estabilice el modulo
    }

    return (nResultado);													                                           //Retornamos el resultado de la inicializacion, sera 1 si pasa todos los test
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
* @see  JSB_IniciaGSM()
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

    BorraBuffer();                                                                           //Borramos buffer
    printf("AT+CFUN=1\r");                                                                   //Enviamos al modulo el comando de reset
    delay_ms(2000);                                                                          //Temporizamos 2 sg
    nOk=JSB_StrPos(cBuffer, cOk, 0);                                                         //Comprobamos si en la respuesta aparece el texto OK
    if ( nOk != -1 )                                                                         //Si existe es que el modulo esta reseteado  
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
* @brief Inicializa el modulo para el envio/recepcion de SMS en los Modulos catalogados como MODO_0
*
*
* @post Se borra el buffer
*
* Ejemplo:
*
*   JSB_IniciaSMS ();
*
* Resultado:
*
*   Se inicia el envio/recepcion de SMS con las sigientes condiciones
*
*     CMGF = 1            SMS modo Texto
*
* @see JSB_BorraSMS(), JSB_Borra_N_SMS ( ), JSB_EnviaSMS (), JSB_LeeSMS(), JSB_LeeRemitenteSMS (), JSB_TestSMS (), JSB_IniciaSMS ()    
*/
void JSB_IniciaSMS (void )
{
  #ifdef MODO_0
  //  Se inicia el envio/recepcion de SMS con las sigientes condiciones
  //
  //    CMGF = 1            SMS modo Texto

      printf("AT+CMGF=1\r");                                                                 //Nos aseguramos que no hay Eco
      delay_ms(2000);                                                                        //Temporizamos 1 sg
      BorraBuffer();  
  #endif

  #ifdef MODO_1
  //  Se inicia el envio/recepcion de SMS con las sigientes condiciones
  //
  //    CMGF = 1            SMS modo Texto
  //    CSCS = "PCP437"     Tabla de caracteres utilizada
  //    CNMI = 2,1,0,0,0    Fuerza a enviar por el puerto serie el codigo de recepcion de SMS CMTI : <index>

      printf("AT+CMGF=1\r");                                                                 //Nos aseguramos que no hay Eco
      delay_ms(2000);                                                                        //Temporizamos 1 sg
      BorraBuffer();  

      printf("AT+CSCS=\"PCCP437\"\r");                                                       //Nos aseguramos que no hay Eco
      delay_ms(2000);                                                                        //Temporizamos 1 sg
      BorraBuffer();    
  
      printf("AT+CNMI=2,1,0,0,0\r");                                                         //Nos aseguramos que no hay Eco
      delay_ms(2000);                                                                        //Temporizamos 1 sg
      BorraBuffer();       
  #endif
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
    return (cSms);									                                                         //Devolvemos el contenido del SMS
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
* @return Devuelve 1 si se han podido borrar todos los mensajes, 0 en caso de que haya alguno que no se haya podido borrar o simplemente que no exista
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
* @brief Envia un SMS a un numero determinado en los modulos catalogados como MODULO_0
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

  #ifdef MODO_0

  // Se envia el mensaje como un bloque de lineas

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
        BorraBuffer();                                                                       //Borramos Buffer
        printf ("%s",cMensaje);  		                                                         //Enviamos el texto 
        putc(0x1A);                                                                          //Enviamos el caracter fin de mensaje 
        delay_ms(5000);                                                                      //Retardo de precaucion para que el modulo GSM se estabililce y pueda seguir recibiendo ordenes   
        nOk=JSB_StrPos(cBuffer, cOk, 0);                                                     //Comprobamos si en la respuesta del modulo aparece el texto OK
        if ( nOk != -1 )                                                                     //Si existe es que el envio ha sido correcto  
        {
            nResultado = 1;                                                                  //Y ponemos la salida a 1
        }else{
            nResultado = 0;                                                                  //En caso contrario lo ponemos a 0
        }
    }
    return nResultado;
#endif
#ifdef MODO_1

  // Se envia el mensaje linea a linea con retardo entre ellas

  //------------------------------------------------------------- 
  //Definicion de variables
  //------------------------------------------------------------- 
    int1 nResultado=0;
    char cOk[]="OK";
    char cPermiso = ">";
    char cSeparador[2] = "\r\0";
    int nOk;
    int nLineas = JSB_StrCount (cMensaje, &cSeparador, 0);                                   //Contamos el numero de lineas del mensaje
    int nLinea=0;
  //------------------------------------------------------------- 

    BorraBuffer();                                                                           //Borramos Buffer
    enable_interrupts(int_rda);                                                              //Habilita Interrupción RDA para recepcion de caracters
    printf("AT+CMGS=\"+34%s\"\r", cNumero);                                                  //Enviamos la primera secuencia
    delay_ms(1200);                                                                          //Temporizamos 1,2 Sg
    nOk=JSB_StrPos(cBuffer, cOk, 0);                                                         //Comprobamos si en la respuesta del modulo aparece '>'
    if ( nOk )
    {  
        BorraBuffer();                                                                       //Borramos Buffer
        for ( nLinea = 0; nLinea< nLineas; nLinea++ )                                        //Recorremos las lineas del mensaje  
        {
           char *cLinea= JSB_ExtraeLinea ( cMensaje, cSeparador, nLinea );                   //cogemos una linea
           printf ("%s",cLinea);                                                             //Enviamos el texto  de la linea
           delay_ms(200);                                                                    //temporizamos 200 msg
           if ( cLinea > 0 )                                                                 //Si existia la llinea, liberamos la variable utilizada
           {
              free(cLinea);                                                                  //Liberamso la variable cLinea
           } 
        }
        delay_ms(200);                                                                       //Una vez enviadas todas las lineastemporizamos 200 msg mas
        putc(0x1A);                                                                          //Enviamos el caracter fin de mensaje 
        delay_ms(5000);                                                                      //Retardo de precaucion para que el modulo GSM se estabililce y pueda seguir recibiendo ordenes   
        nOk=JSB_StrPos(cBuffer, cOk, 0);                                                     //Comprobamos si en la respuesta del modulo aparece el texto OK
        if ( nOk != -1 )                                                                     //Si existe es que el envio ha sido correcto  
        {
            nResultado = 1;                                                                  //Y ponemos la salida a 1
        }else{
            nResultado = 0;                                                                  //En caso contrario lo ponemos a 0
        }
    }
    return nResultado;
  #endif
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




