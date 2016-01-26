#!/bin/bash
mkdir target
gcc -Wall -o target/led demos/led/led.c -lwiringPi
gcc -Wall -o target/dht11 demos/dht11/dht11.c -lwiringPi