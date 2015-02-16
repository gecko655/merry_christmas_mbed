#include "mbed.h"
#include "rtos.h"
#include "C12832.h"

DigitalOut led0(LED1);
DigitalOut led1(LED2);
DigitalOut led2(LED3);
DigitalOut led3(LED4);
C12832 lcd(p5,p7,p6,p8,p11);
BusOut colorled(p23,p24,p25);
Ticker ticker;

static int BRIGHTNESSPATTERN[] = {0,4,3,4,2,4,3,4,1,4,3,4,2,4,3,4};
int ledbrightness[] = {0,0,0,0};

void setBrightness(int led, int brightness){
	ledbrightness[led] = brightness;
}


int ledpattern[][4] = {
		{5,1,2,4},
		{4,5,1,2},
		{2,4,5,1},
		{1,2,4,5},
};

void ledManager(){
    static int count=0;
    led0=(BRIGHTNESSPATTERN[count]<ledbrightness[0])?1:0;
    led1=(BRIGHTNESSPATTERN[count]<ledbrightness[1])?1:0;
    led2=(BRIGHTNESSPATTERN[count]<ledbrightness[2])?1:0;
    led3=(BRIGHTNESSPATTERN[count]<ledbrightness[3])?1:0;
    count++;
    if(count==16){
        count=0;
    }
}
void colorled_thread(void const* args){
   colorled=0;
    while (true) {
        colorled=colorled+1;
        if(colorled==8){
            colorled=0;
        }
        Thread::wait(500);
    }
}

int main() {
	Thread thread(colorled_thread);
    ticker.attach_us(ledManager,1000);
    int count=0;
    while(true){
    	Thread::wait(100);
    	setBrightness(0,ledpattern[0][count]);
    	setBrightness(1,ledpattern[1][count]);
    	setBrightness(2,ledpattern[2][count]);
    	setBrightness(3,ledpattern[3][count]);
    	count++;
    	if(count>3){
    		count=0;
    	}
    }
}
