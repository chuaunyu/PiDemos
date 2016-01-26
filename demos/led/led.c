#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc,char* argv[])
{
	int pin = 21;
	if(wiringPiSetup() == -1){
		printf("Setup failed.");
		return 1;
	}

	pinMode(pin, OUTPUT);
	while(1){
		digitalWrite(pin, 1); // high lv
		delay(500);
		digitalWrite(pin, 0); // low lv
		delay(500);
	} 
	return 0;
}
