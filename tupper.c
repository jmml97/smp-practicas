#include "include/tarjeta.h"
#include "include/srf02.h"

#use delay (clock=48MHz)

const long int N0 = 0;

int vel = 240;
int aux = 0;
long int delay = 50;
long int delay2 = 600;

#define ADELANTE 0
#define ATRAS 1
#define DERECHA 2
#define IZQUIERDA 3
#define PARAR 4

int movimiento = ADELANTE, movimiento_anterior = ADELANTE;

void adelante_() {
   M1_A();
   M2_A();
   M3_A();
   M4_A();
}

void atras_(){
   M1_H();
   M2_H();
   M3_H();
   M4_H();
}

void parar_() {
   M1_P();
   M2_P();
   M3_P();
   M4_P();
}

void mover_i_eje() {
   M1_A();
   M2_A();
   M3_H();
   M4_H();
}

void mover_d_eje() {
   M1_H();
   M2_H();
   M3_A();
   M4_A();
}

#INT_TIMER0
void int_tmr0()
{

   set_timer0(N0);

   if (aux <= vel)
   {
      switch (movimiento)
      {
      case ADELANTE:
         adelante_();
         break;
      case ATRAS:
         atras_();
         break;
      case DERECHA:
         mover_d_eje();
         break;
      case IZQUIERDA:
         mover_i_eje();
         break;
      case PARAR:
         parar_();
         break;

      default:
         break;
      }
   }
   else
   {
      parar_();
   }

   aux++;
}


void main() {

   setup_timer_0(RTCC_INTERNAL | RTCC_8_BIT);
   set_timer0(N0);

   enable_interrupts(INT_TIMER0);
   enable_interrupts(GLOBAL);

   while (1)
   {

      movimiento = ADELANTE;
      delay_ms(delay2);
      movimiento = PARAR;

      delay_ms(delay / 2);
      movimiento = ATRAS;
      delay_ms(delay2);
      movimiento = PARAR;
      delay_ms(delay / 2);

      movimiento = DERECHA;
      delay_ms(delay2);
      movimiento = PARAR;
      delay_ms(delay / 2);

      movimiento = IZQUIERDA;
      delay_ms(delay2);
      movimiento = PARAR;

      delay_ms(delay2);

      if (delay > 100)
         delay = delay - 100;
      else
         delay = delay / 2;
}

}
