#include "include/tarjeta.h"
#use delay(clock = 48MHz)
#include "include/srf02.h"


const long int N0 = 0;

// Variables para el control de la velocidad mediante PWM.
// Durante el movimiento a izquierda y derecha se tiene en
// cuenta la variable giro para determinar la diferencia entre
// velocidad de unas ruedas y otras.
int vel = 240;
int vel_ant = 240;
int giro = 30;
int pwm_cnt = 0;
int aux = 0;

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
#define EST_INICIAL 0
#define EST_EXPLORAR 1
#define EST_ATACAR 2
#define EST_LAPA 3
#define EST_HUIR 4
#define EST_EVITAR_LD 5
#define EST_EJEMPLO 9

int estado_actual = EST_LAPA;
int movimiento = PARAR, movimiento_anterior = PARAR;

/*****************************************************************************/
/*                                  Sensores                                 */
/*****************************************************************************/

#define BUMPER_D IN1
#define BUMPER_I IN2
#define CNY_A IN3
#define CNY_I IN4
#define CNY_D IN5

void adelante_() {
  M1_A();
  M2_A();
  M3_A();
  M4_A();
}

void atras_() {
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

void cambiar_velocidad(int nueva_vel) {
  vel_ant = vel;
  vel = nueva_vel;
}

#INT_TIMER0
void int_tmr0() {
  set_timer0(N0);

  if (aux <= vel) {
    switch (movimiento) {
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
  } else {
    parar_();
  }

  aux++;
}

void main() {
  set_tris_a(0x00);
  set_tris_b(0x13);
  set_tris_c(0x00);
  set_tris_d(0x1F);
  set_tris_e(0x00);

  setup_timer_0(RTCC_INTERNAL | RTCC_8_BIT);
  set_timer0(N0);

  enable_interrupts(INT_TIMER0);
  enable_interrupts(GLOBAL);

  while (1) {
    // En cada case, leer sensores, determinar velocidad, movimiento
    // y posiblemente cambiar a otro estado.
    switch (estado_actual) {
      case EST_INICIAL: {
        delay_ms(3000);
        estado_actual = EST_EXPLORAR;
        break;
      }
      case EST_EXPLORAR: {

        // Recuperamos el valor inicial de la velocidad
        cambiar_velocidad(120);

        // Detectamos línea negra atrás
        if (!CNY_A) {
          estado_actual = EST_HUIR;
          continue;
        } 
        
        // Detectamos línea negra delante
        if (!(CNY_D && CNY_I)) {
          estado_actual = EST_EVITAR_LD;
          continue;
        }

        if (BUMPER_D || BUMPER_I) {
          estado_actual = EST_LAPA;
          continue;
        }

        movimiento = PARAR;
        delay_ms(200);
        int distancia_objeto = srf_measure_cm();

        if (distancia_objeto < 30) {
          estado_actual = EST_LAPA;
        } else if (distancia_objeto < 100) {
          movimiento = ADELANTE;
        } else {
          movimiento = ROTAR_DERECHA;
          delay_ms(300);
        }
        
        break;
      }
      case EST_EJEMPLO: {
        movimiento = ADELANTE;
        delay_ms(1000);
        led_on();
        giro = 50;
        delay_ms(300);
        led_off();
        movimiento = IZQUIERDA;
        delay_ms(2000);
        led_on();
        delay_ms(300);
        led_off();
        movimiento = DERECHA;
        delay_ms(2000);
        movimiento = PARAR;
        delay_ms(500);
        break;
      }
      case EST_LAPA: {
      
        //if (!CNY_A) {
          //estado_actual = EST_HUIR;
          //continue;
        //}
        
        movimiento = ADELANTE;

        if (BUMPER_D && BUMPER_I) {
          // Comprobar si antes estaba a velocidad máxima
          if (vel_ant != 255) {
            cambiar_velocidad(240);
            delay_ms(100);
            cambiar_velocidad(255);
          }

          movimiento = ADELANTE;
        } else if (BUMPER_I) {
          cambiar_velocidad(120);
          movimiento = ROTAR_IZQUIERDA;
        } else if (BUMPER_D) {
          cambiar_velocidad(120);
          movimiento = ROTAR_DERECHA;
        } else {
          cambiar_velocidad(120);
          movimiento = ADELANTE;
          //delay_ms(500);
          //estado_actual = EST_EXPLORAR;
        }
        break;
      }
      case EST_EVITAR_LD: {
        // Estamos asumiendo que Tupper está siendo empujado por otro robot
        if (!CNY_D && !CNY_I) {
          cambiar_velocidad(240);
          movimiento = ROTAR_IZQUIERDA;
          delay_ms(300);
          giro = 90;
          movimiento = IZQUIERDA;
        } else if (!CNY_D) {
          cambiar_velocidad(240);
          movimiento = ROTAR_IZQUIERDA;
          delay_ms(300);
          giro = 90;
          movimiento = IZQUIERDA;
        } else if (!CNY_I) {
          cambiar_velocidad(240);
          movimiento = ROTAR_DERECHA;
          delay_ms(300);
          giro = 90;
          movimiento = DERECHA;
        }
        delay_ms(300);
        estado_actual = EST_EXPLORAR;
        break;
      }
      case EST_HUIR: {
        cambiar_velocidad(240);
        
        if (!CNY_D) {
          movimiento = ROTAR_IZQUIERDA;
        } else {
          movimiento = ROTAR_DERECHA;
        }

        delay_ms(300);
        movimiento = ADELANTE;
        estado_actual = EST_EXPLORAR;
        break;
      }
      default:
        break;
    }
  }
}
