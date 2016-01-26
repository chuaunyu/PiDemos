#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#define DHT11_PIN 12

#define SUC 1
#define ERROR_READ -1
#define ERROR_SUM -2

#define TIMEOUT_MS 5000

long read(unsigned long *pdata) {
  if(-1 == wiringPiSetup()){
     return ERROR_READ;
  }
  
  unsigned char checksum = 0;
  unsigned int timeout = 0;
  
  int sum = 0;
  int i=0;
  unsigned long data = 0;
  pinMode(DHT11_PIN, OUTPUT);
  
  // Set LOW to gpio 18ms
  digitalWrite(DHT11_PIN, LOW);
  delay(25);

  // Set HIGH to gpio 40us
  digitalWrite(DHT11_PIN, HIGH);
  pullUpDnControl(DHT11_PIN,PUD_UP);
  pinMode(DHT11_PIN, INPUT);
  delayMicroseconds(40);

  // Waitting for DHT11 response------------------

  // Receive a LOW signal
  if(digitalRead(DHT11_PIN) == LOW){
      timeout = millis() + TIMEOUT_MS;
  
      // Receive a HIGH signal
      while(digitalRead(DHT11_PIN) != HIGH){
        if (millis() > timeout){
            return ERROR_READ;     
        }
     }
        
    
      // Read 4*8bit=32bit data -->  8bit temeprature int + 8bit temperature decimal + 8bit humidity int + 8bit humidity decimal
      for(i=0;i<32;i++){
        // Receive a HIGH signal --> Receive a LOW signal --> Wait for 28 us
        while(digitalRead(DHT11_PIN) != LOW){
            if (millis() > timeout){
                return ERROR_READ;
            }
        }
           
        while(digitalRead(DHT11_PIN) != HIGH){
            if (millis() > timeout){
                return ERROR_READ;
            }
        }
          
        delayMicroseconds(32);
      
        data*=2;
        if(digitalRead(DHT11_PIN) == 1){
          data++;
        } 
      }
  
      // Read 8bit check sum
      for(i=0;i<8;i++){
        // Receive a HIGH signal --> Receive a LOW signal --> Wait for 28 us
        while(digitalRead(DHT11_PIN) != LOW){
            if (millis() > timeout){
                return ERROR_READ;
            }
        }

        while(digitalRead(DHT11_PIN) != HIGH){
            if (millis() > timeout){
                return ERROR_READ;
            }
        }
        delayMicroseconds(32);
      
        checksum*=2;
        if(digitalRead(DHT11_PIN) == 1){
          checksum++;
        }
      }

      // Valid checksum---------------------
      sum += (data>>24)&0xff;
      sum += (data>>16)&0xff;
      sum += (data>>8)&0xff;
      sum += data&0xff;
      sum = sum&0xff;   
  
      if(sum != checksum){
        return ERROR_SUM;
      }
      *pdata = data;
      return SUC;
    }else{
      return ERROR_READ;
    }
}

int main(int argc, char **argv){
	unsigned long data = 0;
	if(read(&data) == 1){
		printf("Success!\nHumidity:%d.%d%,\nTemperature:%d.%dC\n", (data>>24)&0xff, (data>>16)&0xff, (data>>8)&0xff, data&0xff);
	}else{
	   printf("Error!\n");
	}
}   
