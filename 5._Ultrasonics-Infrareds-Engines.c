#include <18F4520.h>
#fuses HS, PUT, NOWDT, NODEBUG, NOBROWNOUT, NOPROTECT, NOLVP, NOXINST
#device adc =10
#use delay(clock=4000000)
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7, bits=8, parity=N)

float LeftSensor=0;  //Puerto a0
float RightSensor=0; //Puerto a1

float CenterSensor=0; //Puerto a2

long voltaje = 660;



int max_duty = 410;
int plusMotor = 5;
int min_duty = 0;
int souvenir = 0;

//  Variables Ultrasonico 1
float distancia, tiempo;
#define trig pin_D5
#define echo pin_D4
//  Variables Ultrasonico 2
float distancia2, tiempo2;
#define trig2 pin_D7
#define echo2 pin_D6
//----------------------------------------------
void Ultras(){
      output_high(trig);                   
      delay_us(20);                       
      output_low(trig);
      while(!input(echo))                     
      {}
      set_timer1(0);                         
      while(input(echo))                     
      {}
      tiempo=get_timer1();
      distancia=(tiempo*10)/(58.0);
      

      output_high(trig2);                   
      delay_us(20);                       
      output_low(trig2);
      while(!input(echo2))                     
      {}
      set_timer1(0);                         
      while(input(echo2))                     
      {}
      tiempo2=get_timer1();
      distancia2=(tiempo2*10)/(58.0);     
}
//----------------------------------------------------


//------------------------Sensores INFRAROJOS------------------------------------------------------
void Lectura()
{
 //Lectura del primer sensor (PIN_A0).
   set_adc_channel(0);   //Se establece el canal en 0 (N?mero del PIN)
   delay_us(10); //3ms para estabilizaciÛn de la seÒal

   LeftSensor=read_adc();    //C·lculo de los ?C.

   //Lectura del SEGUNDO sensor (PIN_A0).
   set_adc_channel(1);   //Se establece el canal en 1 (N?mero del PIN)
   delay_us(10); //3ms para estabilizaciÛn de la seÒal

   RightSensor=read_adc();    //C·lculo de los ?C.

   //LECTURA SENSOR DEL CENTRO (PIN_A2).
   set_adc_channel(2);   //Se establece el canal en 1 (N?mero del PIN)
   delay_us(10); //3ms para estabilizaciÛn de la seÒal

   CenterSensor=read_adc();    //C·lculo de los ?C.

//   printf(" Izquierdo = %2.2f   Centro =  %2.2f    Derecho = %2.2f \n\r", LeftSensor, CenterSensor, RightSensor );
}
void main()
{
   
   delay_ms(500);

      setup_adc(adc_clock_internal); //Reloj interno usado para conversiÛn
      setup_adc_ports(ALL_ANALOG);   //set pins AN0-AN7 to analog

      setup_CCP1(ccp_pwm);
      setup_CCP2(ccp_pwm);
      
      setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
      setup_timer_2(T2_DIV_BY_4,249,1);

      enable_interrupts(GLOBAL);                      
      setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);

   while(1)
   {
      
         Ultras();
        if (((int)(distancia > 20)) && ((int)distancia2 >20) )
         {
            Lectura();
         if (LeftSensor < voltaje && CenterSensor > voltaje && RightSensor < voltaje)
         {
            set_pwm1_duty(max_duty);  // Ciclos de trabajo al 60 por ciento
            set_pwm2_duty(max_duty);  // en ambos PWM Pulse wave modulation
            OUTPUT_B(5);  //Avanzan ambos 1010 0000
            OUTPUT_D(3);  // Led indicadores ambos encendidos
         }

         else if (LeftSensor > voltaje && CenterSensor < voltaje && RightSensor < voltaje)
         {
            set_pwm1_duty(min_duty);  // Ciclos de trabajo al 60 por ciento
            set_pwm2_duty(max_duty-plusMotor);  // en ambos PWM Pulse wave modulation
            OUTPUT_B(4);  //Avanzan ambos 1010 0000
            OUTPUT_D(2);  // Led indicadores ambos encendidos
            souvenir = 1;
         }    

         else if (LeftSensor < voltaje && CenterSensor < voltaje && RightSensor > voltaje)
         {
            set_pwm1_duty(max_duty-plusMotor);  // Ciclos de trabajo al 60 por ciento
            set_pwm2_duty(min_duty);  // en ambos PWM Pulse wave modulation
            OUTPUT_B(1);  //Avanzan ambos 1010 0000
            OUTPUT_D(1);  // Led indicadores ambos encendidos
            souvenir = 2;
         }

         else if (LeftSensor < voltaje && CenterSensor < voltaje && RightSensor < voltaje)
         {
            if (souvenir == 2)
            {
               set_pwm1_duty(max_duty-plusMotor);  // Ciclos de trabajo al 60 por ciento
               set_pwm2_duty(min_duty);  // en ambos PWM Pulse wave modulation
               OUTPUT_B(1);  //Avanzan ambos 1010 0000
               OUTPUT_D(1);  // Led indicadores ambos encendidos     
            }
            if (souvenir == 1)
            {
               set_pwm1_duty(min_duty);  // Ciclos de trabajo al 60 por ciento
               set_pwm2_duty(max_duty-plusMotor);  // en ambos PWM Pulse wave modulation
               OUTPUT_B(4);  //Avanzan ambos 1010 0000
               OUTPUT_D(2);  // Led indicadores ambos encendidos  
            }
         }
         
         else if (LeftSensor > voltaje && CenterSensor > voltaje && RightSensor > voltaje)
         {
            set_pwm1_duty(min_duty);  // Ciclos de trabajo al 60 por ciento
            set_pwm2_duty(min_duty);  // en ambos PWM Pulse wave modulation
            OUTPUT_B(1);  //Avanzan ambos 1010 0000
            OUTPUT_D(2);  // Led indicadores ambos encendidos
            
         }  
         }
         else
           {
            set_pwm1_duty(min_duty);  // Ciclos de trabajo al 60 por ciento
            set_pwm2_duty(min_duty);  // en ambos PWM Pulse wave modulation
            OUTPUT_B(0);  //Avanzan ambos 1010 0000
            OUTPUT_D(2);  // Led indicadores ambos encendidos 
           } 
         
   }
}
