#include "../include/tarjeta.h"
#use delay(clock=48MHz)
#include "../include/srf02.h"

const long int N0 = 0;

int vel = 60;
int aux = 0;
long int delay = 50;
long int delay2 = 600;

#define ADELANTE 0
#define ATRAS 1
#define DERECHA 2
#define IZQUIERDA 3
#define PARAR 4

int movimiento = PARAR, movimiento_anterior = ADELANTE;

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

int absoluto(int a, int b) {

   if (a >= b)
      return a - b;
   else 
      return b - a;
   
}

int media(int m1, int m2, int m3, int m4) {
   return (m1 + m2 + m3 + m4) / 4;
}

int desv_abs_media(int m1, int m2, int m3, int m4) {}


void main() {

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

   delay_ms(3000);
   
   int medida1, medida2, medida3, medida4, media_var;
   media_var = medida1 = medida2 = medida3 = medida4 = srf_measure_cm();

   while (1) {
      
      media_var = media(medida1, medida2, medida3, medida4);
      
      if ((absoluto(medida4, media_var) <3) 
            && (absoluto(medida3, media_var) <3)
            && (absoluto(medida2, media_var) <3)
            && (absoluto(medida1, media_var) <3)) {
            
         led_off();
         
         if (medida4 < 50) {
            movimiento = ADELANTE;
         } else {
            movimiento = DERECHA;
         }
         
      } else {
         
         led_on();
         
         if (IN1 && IN2) {
            movimiento = PARAR;
         } else if (IN1) {
            movimiento = DERECHA;
         } else if (IN2) {
            movimiento = IZQUIERDA;
         } else {
            movimiento = ADELANTE;
         }
      
      }
      
      medida1 = medida2;
      medida2 = medida3;
      medida3 = medida4;
      medida4 = srf_measure_cm();
      
      delay_ms(25);
   }
}
