#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define LID_LED_FILE_PATH "/sys/devices/platform/thinkpad_acpi/leds/tpacpi::lid_logo_dot/brightness"

struct Morsemap {
    char symbol;
    char m_code[10];
};

struct timespec char_gap = {0 ,  300000000};     // off time between letters
struct timespec word_gap = {0 ,  500000000};     // off time between words
struct timespec sym_gap  = {0 ,  500000000};     // off time between dots/dashes within a letter
struct timespec dot_duration  = {0 ,  200000000};     // how long a the light stays ON for a dot
struct timespec dash_duration  = {0 ,  600000000};     // how long a the light stays ON for a dash


void turn_on(){
    // system("echo 1 | sudo tee /sys/class/leds/tpacpi::lid_logo_dot/brightness");    

    // much faster than system()
    FILE *f = fopen(LID_LED_FILE_PATH , "w");
    
    if(!f) {
       perror("failed to open LED file");
        return;
    }
    
    fprintf(f , "1");
    fclose(f);

}

void turn_off(){
    // system("echo 0 | sudo tee /sys/class/leds/tpacpi::lid_logo_dot/brightness");    

    FILE *f = fopen(LID_LED_FILE_PATH , "w");
    
    if(!f) {
       perror("failed to open LED file");
        return;
    }

    fprintf(f , "0");
    fclose(f);

}

// a single short blink (on and off)
void dot(){
    turn_on();
    nanosleep(&dot_duration , NULL);
    turn_off();
}

void dash(){
    turn_on();
    nanosleep(&dash_duration, NULL);
    turn_off();

}

// void blink_led( char state[1]){

//     if(*state == '0'){
//         turn_off();
//         nanosleep();
//         turn_on();

//     } else  if( *state == '1' ) {
//         system("echo 1 | sudo tee /sys/class/leds/tpacpi::lid_logo_dot/brightness");
//         system("nanosleep 0.5");

//     }

// }

// to turn of the led
void init(){
    printf("prepping up...");
    // system("echo 0 | sudo tee /sys/class/leds/tpacpi::lid_logo_dot/brightness");

    FILE *f = fopen(LID_LED_FILE_PATH , "w");

    if(!f) {
       perror("failed to open LED file");
        return;
    }


    fprintf(f , "0");
    fclose(f);

}


int main(){

    char phrase[1024];

    init(); 

    // while(phrase != '\q'){

    //     fgets( phrase, sizeof(phrase) , stdin );

    //     phrase[ strcspn() phrase , "\n"  ] = "\0";

    //     gen


    // }

    dot();
    sleep(2);
    // dash();
    dash();
    // dot();


    return 0;
}