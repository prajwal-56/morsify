#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>

#define LID_LED_FILE_PATH "/sys/devices/platform/thinkpad_acpi/leds/tpacpi::lid_logo_dot/brightness"

struct Morsemap {
    char symbol;
    char m_code[10];
};

struct Morsemap morse_table[] = {
    {'A', "01"},
    {'B', "1000"},
    {'C', "1010"},
    {'D', "100"},
    {'E', "0"},
    {'F', "0010"},
    {'G', "110"},
    {'H', "0000"},
    {'I', "00"},
    {'J', "0111"},
    {'K', "101"},
    {'L', "0100"},
    {'M', "11"},
    {'N', "10"},
    {'O', "111"},
    {'P', "0110"},
    {'Q', "1101"},
    {'R', "010"},
    {'S', "000"},
    {'T', "1"},
    {'U', "001"},
    {'V', "0001"},
    {'W', "011"},
    {'X', "1001"},
    {'Y', "1011"},
    {'Z', "1100"},
    {'0', "11111"},
    {'1', "01111"},
    {'2', "00111"},
    {'3', "00011"},
    {'4', "00001"},
    {'5', "00000"},
    {'6', "10000"},
    {'7', "11000"},
    {'8', "11100"},
    {'9', "11110"},
    {'.', "010101"},
    {',', "110011"},
    {'?', "001100"},
    {'!', "101011"},
    {'/', "10010"},
    {'@', "011010"},
    {'=', "10001"},
    {'-', "100001"},
    {'+', "01010"}
};

int morse_table_size = sizeof(morse_table) / sizeof(morse_table[0]);


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



// takes the binary string (m_code) -> iterate through the symbols (0/1) -> blink according to which symbol it is 
void blink_letter(char letter[10]){
    
    for( int i = 0; letter[i] != '\0'; i++){
        if (letter[i] == '0'){
            dot();
            nanosleep(&sym_gap , NULL); // pause time after each symbol
        } else if (letter[i] == '1'){
            dash();
            nanosleep(&sym_gap , NULL); // pause time after each symbol
        }
    }
    nanosleep(&char_gap , NULL); // pause time after each letter
    return;
}

// finds the morse code of a given character , blinks the led.
void blink_char(char c ){
    
    if( toupper(c) == ' '){
        nanosleep(&word_gap, NULL);
    } 
    
    for(int i =0; i < morse_table_size; i++){
        if(toupper(c) == morse_table[i].symbol){
            blink_letter(morse_table[i].m_code);
            return;
        } 
    }
}


// takes the whole phrase -> blinks the led for each character
void blink_phrase(char phrase[1024]){

    for( int i = 0; i < strlen(phrase); i++){
        blink_char(phrase[i]);
    }
}


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

    while( strcmp(phrase , "\\q") != 0){

        fgets( phrase, sizeof(phrase) , stdin );
        phrase[ strcspn( phrase , "\n")  ] = '\0';
        blink_phrase(phrase);

    }

    dot();
    sleep(2);
    // dash();
    dash();
    // dot();


    return 0;
}