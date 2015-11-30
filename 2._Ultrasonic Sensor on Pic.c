#include <18F4520.h>
#fuses HS, PUT, NOWDT, NODEBUG, NOBROWNOUT, NOPROTECT, NOLVP, NOXINST
#device adc =10
#use delay(clock=4000000)
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7, bits=8, parity=N)

//  Variables Ultrasonico 2
float distancia, tiempo;
#define trig pin_D5
#define echo pin_D4
//  Variables Ultrasonico 2
float distancia2, tiempo2;
#define trig2 pin_D7
#define echo2 pin_D6


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
void main()
{
   delay_ms(500);
                            
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);

   while(1)
   {  
      Ultras();
      printf(" Distancia 1 = %2.2f     Distancia 2= %2.2f \n\r", distancia, distancia2 );

   }
}

