#include <18F4520.h>
#fuses HS, PUT, NOWDT, NODEBUG, NOBROWNOUT, NOPROTECT, NOLVP, NOXINST
#device adc =10
#use delay(clock=4000000)
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7, bits=8, parity=N)

float LeftSensor=0;  //Puerto a0
float RightSensor=0; //Puerto a1

long voltaje = 3;

int max_duty = 360;
int mid_duty = 180;
int min_duty = 0;

//------------------------Sensores INFRAROJOS------------------------------------------------------
void Lectura()
{
 //Lectura del primer sensor (PIN_A0).
   set_adc_channel(0);   //Se establece el canal en 0 (N?mero del PIN)
   delay_us(20); //3ms para estabilizaci€n de la se“al

   LeftSensor=read_adc()*(0.004887);    //C∑lculo de los ?C.

   //Lectura del SEGUNDO sensor (PIN_A0).
   set_adc_channel(1);   //Se establece el canal en 1 (N?mero del PIN)
   delay_us(20); //3ms para estabilizaci€n de la se“al

   RightSensor=read_adc()*(0.004887);    //C∑lculo de los ?C.

   printf(" Vista 1 = %2.2f     Vista 2= %2.2f \n\r", LeftSensor, RightSensor );

}
void main()
{
   delay_ms(500);
   
   setup_adc(adc_clock_internal); //Reloj interno usado para conversi€n

      setup_adc_ports(ALL_ANALOG);   //set pins AN0-AN7 to analog

      setup_CCP1(ccp_pwm);
      setup_CCP2(ccp_pwm);
      
      setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
      setup_timer_2(T2_DIV_BY_4,249,1);

      enable_interrupts(GLOBAL);

                            
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);

   while(1)
   {
         Lectura();
         if (LeftSensor < voltaje && RightSensor < voltaje)
         {
            set_pwm1_duty(max_duty);  // Ciclos de trabajo al 60 por ciento
            set_pwm2_duty(max_duty);  // en ambos PWM Pulse wave modulation
            OUTPUT_B(5);  //Avanzan ambos 1010 0000
            OUTPUT_D(3);  // Led indicadores ambos encendidos
         }
         //    AVANZAR             DETENER
         else if (LeftSensor < voltaje && RightSensor > voltaje)
         {
            set_pwm1_duty(max_duty+8);  // en ambos PWM Pulse wave modulation
            set_pwm2_duty(min_duty);  // Ciclos de trabajo al 60 por ciento
            OUTPUT_B(5);  //Avanzar solo IZQUIERDO 1000 0000
            OUTPUT_D(1);  // Led indicadores Solo izquiero
         }
         //    DETENER            AVANZAR
         else if (LeftSensor > voltaje && RightSensor < voltaje)
         {
            set_pwm1_duty(min_duty);  // en ambos PWM Pulse wave modulation
            set_pwm2_duty(max_duty+8);  // Ciclos de trabajo al 60 por ciento
            OUTPUT_B(5);  //Avanzar solo Derecho 0010 0000
            OUTPUT_D(2);  // Led indicadores Solo Derecho
         }

         //    CUANDO NO SE ENCUENTRA LA LINEA NEGRA
         else if (LeftSensor >= voltaje && RightSensor >= voltaje)
         {
            OUTPUT_B(0); //Por ahora no mover
            OUTPUT_D(0);  // Led indicadores Ninguno
         }
         }
         else{
         OUTPUT_B(0); //Por ahora no mover
         OUTPUT_D(0);  // Led indicadores Ninguno
         } 
   }
}
