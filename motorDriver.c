#include <stdio.h>

//UART step signals using little endian format
#define STEP                    0b1111111111111110 //only use this one
#define TWO_STEP                0b1111111001111110
#define THREE_STEP              0b1111001111001110
#define FOUR_STEP               0b1110011100110010
#define FIVE_STEP_OVERDRIVE     0b1100110011011010
#define SIX_STEP_OVERDRIVE      0b1101101101101101

//conversion factors
const int REV_TO_STEP = 40000;
const double DEG_TO_STEP = 111.111;

//other constants
const int DELAY = 100;

struct Motor {
    int PU_PIN; //Pulse pin
    int DIR_PIN; //Direction pin
    int MF_PIN; //Motor-Free pin
    int pos;
    // int LOWER_LIMIT;
    // int UPPER_LIMIT;


    //Constructor
    void (*init)(struct Motor *);

    //Getters and Setters
    void (*set_home)(struct Motor *);
    int (*get_pos)(struct Motor *);

    //Utility
    void (*step)(struct Motor *, int);
    void (*spin_rev)(struct Motor *, int);
    void (*spin_deg)(struct Motor *, int);
    void (*home)(struct Motor *);
};


//Constructor
void init(struct Motor * motor) {
    motor->PU_PIN = 0;
    motor->DIR_PIN = 1;
    motor->MF_PIN = 2;
    motor->pos = 0;
}


//Setter / Getter
void set_home(struct Motor * motor) { //set current position to new 0 position
    motor->pos = 0;
}
int get_pos(struct Motor * motor) {
    return (motor->pos)%REV_TO_STEP;
}



//Utility
void step(struct Motor * motor, int steps) {
    motor->pos += steps;
    if(steps < 0) {
        _pinh(motor->DIR_PIN);
        steps*=-1;
    }
    else {
        _pinl(motor->DIR_PIN);
    }

    for(int i = 0; i < steps; i++) {
        _wypin(motor->PU_PIN,STEP);
        _waitus(DELAY);
    }
    _pinl(motor->DIR_PIN);
}

void spin_rev(struct Motor * motor, int revs) {
    int steps = revs*REV_TO_STEP;
    motor->pos += steps;
    if(steps < 0) {
        _pinh(motor->DIR_PIN);
        steps*=-1;
    }
    else {
        _pinl(motor->DIR_PIN);
    }

    for(int i = 0; i < steps; i++) {
        _wypin(motor->PU_PIN,STEP);
        _waitus(DELAY);
    }
    _pinl(motor->DIR_PIN);
}

// void spin_rev(struct Motor * motor, int revs) {
//     int steps = revs*REV_TO_STEP;
//     step(&motor, steps);
// }

void spin_deg(struct Motor * motor, int degs) {
    int steps = degs*DEG_TO_STEP;
    motor->pos += steps;
    if(steps < 0) {
        _pinh(motor->DIR_PIN);
        steps*=-1;
    }
    else {
        _pinl(motor->DIR_PIN);
    }

    for(int i = 0; i < steps; i++) {
        _wypin(motor->PU_PIN,STEP);
        _waitus(DELAY);
    }
    _pinl(motor->DIR_PIN);
}

void home(struct Motor * motor, int degs) {
    int steps = -1*((motor->pos)%40000); //calculate the steps needed to get back
    if(steps > 20000) {
        steps = -1*(40000-steps);
    }
    motor->pos += steps;
    if(steps < 0) {
        _pinh(motor->DIR_PIN);
        steps*=-1;
    }
    else {
        _pinl(motor->DIR_PIN);
    }

    for(int i = 0; i < steps; i++) {
        _wypin(motor->PU_PIN,STEP);
        _waitus(DELAY);
    }
    _pinl(motor->DIR_PIN);
}
