#include<avr/io.h>
#include<avr/interrupt.h>
#include <stdlib.h>

float rpm=150;int count=0;
int clock_ms=0;
float time0 = 0, time1 = 0;
int overf = 0;

void usart_init(void)

{
	
	UCSR0B=(1<< RXEN0)|(1<< TXEN0);//TRANSMIT AND RECEIVE ENABLE
	
	UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);//ASYNCHRONOUS, 8 BIT TRANSFER
	
	UBRR0L= 0x67 ; //BAUD RATE 9600
	

}

void usart_send(int ch )

{
	
	while(UCSR0A!=(UCSR0A|(1<<UDRE0)));//waiting for UDRE to become high
	
	UDR0= ch;

}

int main(void)
{
  sei();
  usart_init();
  
  //unsigned char ch;
  
  DDRC|=0b00000000;
  DDRD|=0b11000000;
  PORTD|=0b01000000;
  DDRB|=0b00000010;
  
  Serial.begin(9600);
  
  //TCCR1B=(1<<WGM12)|(1<<CS11)|(1<<CS10);
  TCCR0A|=(1<<WGM01);
  TCCR0B|=(1<<CS01)|(1<<CS00);
  
  TCCR1A |= (1<<COM1A1)|(1<<WGM10);
  TCCR1B |= (1<<CS10) | (1<<WGM12);
  
  OCR0A=250;
  
  OCR1A=255;
  sei();
  TIMSK0=(1<<OCIE0A);
  
  EIMSK|=(1<<INT0);
  EICRA|=(1<<ISC01);
  
 
  
  while(1)
  {
   
  }
  
  return 0;
}
  ISR(TIMER0_COMPA_vect)
  {
    clock_ms++;
    if(clock_ms%1000==0) {Serial.print("Second is"); Serial.println(clock_ms/1000);} 
    if(clock_ms == 65535) {overf++; clock_ms=0;}
    
  }
  
  ISR(INT0_vect)
  {
    count++;
    //Serial.println(count);
     _delay_ms(200);
    if(count==1)
    {
      
     time0 = clock_ms; 
     
    }
    
    /*
    if(PIND == 0b00000100)
    {
     count++;
      _delay_ms(200); 
    }
    */
    else if(count==2)
    {
     time1 = clock_ms;
     Serial.print(time0);
     Serial.print("\t");
     Serial.println(time1);
     float time = time1+200 - time0;
      rpm = 1000*(60 / time);
      
      count=0;
      overf=0;
      clock_ms=0;
      Serial.print("RPM is ");
      Serial.println(rpm);
     //Serial.println(rpm);
     /*
      int temp = rpm;
    char sen[4] = {0};
    int i=0;
    if(rpm==0) usart_send(48);
    else{
    while(temp>0)
    {
      sen[i]= (temp%10) + 48;
      temp/=10;
      ++i;
    }
    int len = strlen(sen);
      for(i=len - 1;i>=0;i--)
         usart_send(sen[i]);
    } 
     usart_send('\n');
   */
     
     //ch = rpm;
    // usart_send(char(rpm));
   
    }
   
  }
  
      
  
  
  
  
  /*
     if(rpm>=200 && rpm<235)
    {
       OCR1A=180;
    }  
    else if(rpm<200)
    {
         
         if(OCR1A<=245)
          OCR1A  += 10;
         
         
       
    }
    else if(rpm>=23
    5)
    {
     if(OCR1A>=10)
      OCR1A -= 10; 
    }
        
        
    
  }
*/
