#include <stdlib.h>
#include <avr/io.h>

int XAxis = 0;
int YAxis = 0;
char comp = 0;
char sw = 0;

int main(){
  
  //PWM Setup
  DDRB |= 1<<1;
  TCCR1A = (1<<COM1A1) | (1<<COM1A0) | (1<<WGM11);
  TCCR1B = (1<<WGM12) | (1<<WGM13) | (1<<CS12) | (1<<CS10);
  ICR1 = 313; //definition top
  OCR1A = 289; //comparaison registre a gauche: 297 (305)    milieu: 289     droite: 282 (268)      formule: 313-t*15625    t€[1,2]
  TIMSK1 |= (1<<OCIE1A); //active comparaison registre a

  //PWM Setp
  DDRB &= ~(1<<4);
  setup_adc();
  
  sei();
  
  
  
  
  while(1){
    
   
  }
  return 0;
}

ISR(TIMER1_COMPA_vect) {
  OCR1A = 289+(int)(((double)(YAxis-512)/1023)*32);
}

void setup_adc(){
  ADMUX |= (1<<REFS0);
  ADMUX |= (1<<MUX0);
  ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);
  DIDR0 = (1<<ADC1D);
  start_conversion();

}

void start_conversion(){
  ADCSRA |= (1<<ADSC);
}


void axis_handler(){ //Attribue la valeur au bon axe
  if(sw){
    YAxis=ADC;
  }
  else{
    XAxis=ADC;
  }
  ADMUX ^= (1<<MUX0); //switch entre input ADC0 et ADC1
  sw=!sw;
}

ISR(ADC_vect){
  axis_handler();
  start_conversion();
}
