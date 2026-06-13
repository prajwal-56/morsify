#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct Morsemap {
    char symbol;
    char m_code[];
};

void turn_on(){
    system("echo 1 | sudo tee /sys/class/leds/tpacpi::lid_logo_dot/brightness");    
}

void turn_off(){
    system("echo 0 | sudo tee /sys/class/leds/tpacpi::lid_logo_dot/brightness");    
}


void blink_led( char state[1]){

    if(*state == '0'){
        turn_off();
        sleep(2);
        turn_on();

    } else  if( *state == '1' ) {
        system("echo 1 | sudo tee /sys/class/leds/tpacpi::lid_logo_dot/brightness");
        system("sleep 0.5");

    }

}

// to turn of the led
void init(){
    system("echo 1 | sudo tee /sys/class/leds/tpacpi::lid_logo_dot/brightness");
}


int main(){

    char phrase[1024];

    // while(phrase != '\q'){

    //     fgets( phrase, sizeof(phrase) , stdin );

    //     phrase[ strcspn() phrase , "\n"  ] = "\0";

    //     gen


    // }

    while(true) {
        turn_off();
        sleep(0.5);
        turn_on();
        sleep(0.5);
    }
    return 0;
}