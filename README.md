# GSM 


Funciones C manejo de un modulo GSM para telecontrol

V 1.0 Agosto 2012

Versión inicial generada para gestionar el modulo WMOD2 de Wavecom

Se ha tratado de que sean funciones genericas independientemente del modulo utilizado, sin embargo, al utilizar otros modulos se han detectado algunas disfunciones corregidas en posteriores versiones.

La documentacion de esta version se encuentra en  

http://serverpic.com/MIPIC/Rutinas/GSM/V1/

v 2.0 Diciembre 2016

 
Se realizan modificaciones necesarias para utilizacion del modulo M590 de NEOWAY
Ante la posibilidad de encontrar diversos modulos, se crea la variable de compliacion Modo que adquirira´los valores de Modo_0, Modo_1, ... para usar en la compilacion las funciones adecuadas

   - Se añade la funcion JSB_TestRegistro que se encarga de comrpobar que el modulo se ha registrado en la red

   - Se añade la funcion JSB_ResetGSM para resetear el modulo en caso de que se necesite reiniciarlo desde 0

   - Se incorpora una nueva version de JSN_EnviaSMS que adecua el envio de SMS multilinea al modulo NEOWAY M590
     Se observo que la respuesta del modulo a un retorno de carro era lenta y se perdian caracteres de la siguiente linea por lo que
     ha sido necesario separar las lineas y temporizar entre el envio de cada linea para no perder caracteres 

   - Se modifica la funcion JSB_IniciaGSM, ademas de comprobar/introducir el PIN, comprueba si el modulo se ha registrado en la red ( 15 intentos )
     y si se registra correctamente temporiza 30 sg para asegurar la estabilizacion del modulo 


La documentacion de esta version se encuentra en  

http://serverpic.com/MIPIC/Rutinas/GSM/V2/
