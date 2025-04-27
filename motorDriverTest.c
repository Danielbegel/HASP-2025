#include <stdio.h>
#include <propeller.h>
#include "HASP_25_P2.h"
// #include "config.h"
#include "motorDriver.c"


// we only need to support P2, 
#ifndef __P2__
#error this demo is for P2 only 
#endif

/**
 * flexprop specific goofyness that c syntax doesn't like.
 * 
 * This line will import the SmartSerial.spin library for use in this file
 */  
struct __using("spin/SmartSerial") ser;

uint32_t _txmode = 0b00000000000000000000000001111100; //async tx mode, output enabled for smart output

int main() {
    _clkset(_SETFREQ, _CLOCKFREQ);

    //16 bit, 6 step per word, 18us per word, 1us per bit
    uint32_t bitperiod = (_CLOCKFREQ / 1000000);
    // calculate smartpin mode for 16 bits per character
    uint32_t bit_mode = 15 + (bitperiod << 16);

    _pinstart(0,_txmode,bit_mode, 0);

    /*
      Test Code
    */

    struct Motor m1;
    m1.init = init;
    m1.set_home = set_home;
    m1.get_pos = get_pos;
    m1.step = step;
    m1.spin_rev = spin_rev;
    m1.spin_deg = spin_deg;
    m1.home = home;


    m1.init(&m1);


    printf("\nInitial Position: %d", m1.get_pos(&m1));


    //Some test code

    m1.spin_rev(&m1, -1); //1 revolution CCW
    printf("\nPosition: %d", m1.get_pos(&m1));
    _waitms(3000);

    m1.spin_deg(&m1, 90); //90 degrees CW
    printf("\nPosition: %d", m1.get_pos(&m1));
    _waitms(3000);

    m1.step(&m1,-300); //300 steps CCW
    printf("\nPosition: %d", m1.get_pos(&m1));
    _waitms(3000);

    m1.home(&m1); //go to position 0
    printf("\nPosition: %d", m1.get_pos(&m1));
    _waitms(3000);

    // m1.spin_deg(&m1,-90); //90 degrees CCW
    // m1.set_home(&m1); //set home
    // m1.spin_deg(&m1,90); //90 degrees CW
    // m1.home(&m1); //this will go back 90 degrees CCW (because we set the new home)

    printf("\nFinal Position: %d", m1.get_pos(&m1));

    return 0;
}
