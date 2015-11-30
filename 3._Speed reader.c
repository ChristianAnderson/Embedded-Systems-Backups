#include <18F4520.h>
//*********************************************************************************************************
// ******************************************* FUSIBLES **************************************************
//*********************************************************************************************************
#FUSES XT                       //Oscilador a cristal de cuarzo 
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)

#use delay(crystal=4000000)     // Velocidad del retardo
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1) // Envio de Datos por medio del Seriañ

//*********************************************************************************************************
//  *************************************** Variables globales *******************************************
//*********************************************************************************************************
int SEGUNDOS = 0;                // Acumula desbordamientos para completar el segundo
FLOAT VELOCIDAD = 0;               // Almacenamiento temporal de los pulsos acumulados en TIMER0
FLOAT DISTANCIA = 0;               // Almacenamiento temporal de los pulsos acumulados en TIMER0

//*********************************************************************************************************
#INT_TIMER3                      //**************************** ISR TIMER3 *******************************
//*********************************************************************************************************
void TIMER3_isr(void)
{
   set_timer3(3036);             // Precarga TMR3 con valor para siguiente desboradmiento en 500ms
      SEGUNDOS++;                // Contabiliza el número de desbordamientos de TMR3
         if (SEGUNDOS == 1)      // ¿Dos desbordamientos?
            {                    // Si, 
             output_toggle(PIN_D7); // Señalizador de actividad de la ISR
             VELOCIDAD = get_timer0(); // Obtiene la cantidad de marcas acumuladas en TMR0
             VELOCIDAD = VELOCIDAD * 1.25;
             DISTANCIA = DISTANCIA + VELOCIDAD;
             printf ("Velocidad - %f \n\r", VELOCIDAD);
             printf ("Distancia - %f \n\r", DISTANCIA);
             
             SEGUNDOS = 0;       // Reinicia el aacontador de desbordamientos
             set_timer0(0);  // Reinicia el contador de marcas en el codificador incremental
            }
}

//*********************************************************************************************************
void main()                      //************************ PROGRAMA PRINCIPAL****************************
//*********************************************************************************************************
{
   
  set_tris_a(0xFF);                               // RA7:RA0 - Entradas

  set_tris_d(0x00);                               // RD7:RD0 - Salidas
  output_d(0x00);                              // Señalizadores apagados

      setup_timer_0(T0_EXT_L_TO_H|T0_DIV_1);          // Configura TMR0 como contador de pulsos externos
   
      setup_timer_3(T3_INTERNAL|T3_DIV_BY_8);         // Configura TMR3 como temporizador, emplenado la frecuencia del ciclo de instrucción
      enable_interrupts(INT_TIMER3);               // Habilita permiso de interrupciones causadas por desbordamiento de TMR3
      enable_interrupts(GLOBAL);                   // Habilita permiso global de interrupciones - ***** NO funciona si uno en OR los argumentos de esta y la función anterior *****
      set_timer1(3036);                         // Precarga de TMR3 para que se desborde en 500ms

      while(TRUE)                                  // Ciclo infinito en espera de las interrupciones
      {

      }
}
