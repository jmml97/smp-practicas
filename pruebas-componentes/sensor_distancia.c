#include "../include/tarjeta.h"
#use delay (clock=48MHz)
#include "../include/srf02.h"

const long int N0 = 0;

unsigned int16 pwm = 128;
//int pwm2 = 64;
int aux = 0;
// int aux2 = 0;

#INT_TIMER0
void int_tmr0() {

   set_timer0(N0);
   
   if (aux <= pwm) {
      M1_A();
   } else {
      M1_P();
   }

   // if (aux <= pwm2) {
   //    output_high(PIN_A0);
   // } else {
   //    output_low(PIN_A0);
   // }
      
   aux++;
   // aux2++;
}


void main() {

   unsigned int16 m;

   // todo?
   set_tris_a(0x00);
   set_tris_b(0x13);
   set_tris_c(0x00);
   set_tris_d(0x1F);
   set_tris_e(0x00);

   setup_timer_0(RTCC_INTERNAL | RTCC_8_BIT);
   set_timer0(N0);
   
   enable_interrupts(INT_TIMER0);
   enable_interrupts(GLOBAL);

   while(1){
      delay_ms(250);
      pwm = srf_measure_cm() * 8;
   
   }
   
}
