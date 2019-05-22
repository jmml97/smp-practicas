#include "include/tarjeta.h"
#include "include/srf02.h"

#use delay (clock=48MHz)

const long int N0 = 0;

// Variables para el control de la velocidad mediante PWM.
// Durante el movimiento a izquierda y derecha se tiene en
// cuenta la variable giro para determinar la diferencia entre
// velocidad de unas ruedas y otras.
int vel = 240;
int giro = 30;
int pwm_cnt = 0;

long int delay = 50;
long int delay2 = 600;

#define ADELANTE 0
#define ATRAS 1
#define ROTAR_DERECHA 2
#define ROTAR_IZQUIERDA 3
#define DERECHA 4
#define IZQUIERDA 5
#define PARAR 6

/*****************************************************************************/
/*                             Estados del robot                             */
/*****************************************************************************/
#define EST_EXPLORACION 0
#define EST_ATAQUE 1
#define EST_EJEMPLO 2

int estado_actual = EST_EXPLORACION;
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

void mover_i() {
  M1_P();
  M2_P();
  M3_H();
  M4_H();
}

void mover_d() {
   M1_H();
   M2_H();
   M3_P();
   M4_P();
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
      case ROTAR_DERECHA:
         mover_d_eje();
         break;
      case ROTAR_IZQUIERDA:
         mover_i_eje();
         break;
      case DERECHA:
	if (aux <= (vel - giro))
	  adelante_();
	else
	  mover_d();
	break;
      case IZQUIERDA:
	if (aux <= (vel - giro))
	  adelante_();
	else
	  mover_i();
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

     // En cada case, leer sensores, determinar velocidad, movimiento
     // y posiblemente cambiar a otro estado.
     switch (estado_actual) {
       case EST_EXPLORACION: {
       
       break;
       }
       default:
         break;
     }
   }

}
