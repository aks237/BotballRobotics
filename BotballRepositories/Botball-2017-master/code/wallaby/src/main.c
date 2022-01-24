#include <kipr/botball.h>
#include "drive.h"
#include "main.h"

int main()
{
    enable_servos();

    set_servo_position(0,BALE_UP);
    //if the robot takes too much time then reduce distance vars from square ups
    //forward(23); //22.5
    pick_up_bale(); //picks up first bale 
    //right(90,0);
    //backward(15);

    forward(10); //travel against all odds to the opposite half of the board
    left(6, 0);
    forward(20);
    left(2, 0);
    forward(70);
    //left(90,ROBOT_DIAMETER/2); //turn to have the end face the wall at the half
    right(90,0);
    backward(32);
    msleep(500);
    forward(2); //Finish squaring-up and go forward a tad
    right(80,0);
    backward(75);
    //forward(4);
    //left(160,0);
    int total_dist = 64; //distance covered by black tape
    int i;
    for(i = 0; i < 3; i++) //utilizing the stinger stacker
    {
        backward(total_dist / 4);
        pick_up_bale();
    }
    disable_servos();
    return 0;
}

void pick_up_bale(){
    servo_slow(BALE_UP, 0, BALE_DOWN, 5000);
    set_servo_position(0, BALE_UP);
    msleep(1000);
}

void servo_slow(int start, int servo, int end, int time){ //time is in MS
    set_servo_position(servo, start);

    int difference = end - start;
    float timeper = difference/(time/1000.0);
    printf("timeper: %f", timeper);
    int i;
    for (i = 1; i <= (time/1000); i++){
        set_servo_position(servo, start+i*timeper);
        msleep(1000);
    }
}