#include <kipr/botball.h>
#include "createDrive.h"

//Motor Ports
#define RIGHT 1
#define LEFT 0
//Servo Ports
#define WRIST 3
#define CLAW 0 
//Sensor Ports
#define TOUCH_DOWN 1
#define TOUCH_UP 0
#define TOUCH_RIDGE_RIGHT 2
#define TOUCH_RIDGE_LEFT 9
#define SEE_COW 0
//Motor Constants
#define MAX_SPEED 1500
#define ARM_SPEED 100
//Claw Positions
#define CLAW_OPEN 2047
#define CLAW_CLOSE 400
#define CLAW_COW 1600
#define CLAW_MID 2000
//Wrist Positions
#define WRIST_UP 1300
#define WRIST_DOWN 335
#define WRIST_BOTGUY 475
//Camera Variables
#define CHANNEL 0
//Other Variables
int botguy_offset;

void servo_slow(int servo, int end, int time) //time is in MS
{
    int start = get_servo_position(servo);
    float poschange = 10 * (end - start) / time;
    float i;
    if (start < end)
    {
        for(i = start; i < end; i += poschange)
        {
            set_servo_position(servo, (int)i);
            msleep(10);
        }
    } 
    else 
    {
        for(i = start; i > end; i += poschange)
        {
            set_servo_position(servo, (int)i);
            msleep(10);
        }
    }
}

void arm_up()
{
    mav(RIGHT, -MAX_SPEED);
    mav(LEFT, -MAX_SPEED);
    while(digital(TOUCH_UP) != 1)
        msleep(1);
    ao();
}

void arm_down()
{
    mav(RIGHT, MAX_SPEED);
    mav(LEFT, MAX_SPEED);
    while(digital(TOUCH_DOWN) != 1)
        msleep(1);
    ao();
}

void claw_open(int wait) // Wait is in ms
{
    set_servo_position(CLAW, CLAW_OPEN);
    msleep(wait);
}

void claw_close(int wait) // Wait is in ms
{
    set_servo_position(CLAW, CLAW_CLOSE);
    msleep(wait);
}

void claw_close_cow(int wait) // Wait is in ms
{
    set_servo_position(CLAW, CLAW_COW);
    msleep(wait);
}

void arm_up_for_time(int time)// Time is in ms
{
    mav(RIGHT, -MAX_SPEED);
    mav(LEFT, -MAX_SPEED);
    msleep(time);
    ao();
}

void arm_down_for_time(int time)// Time is in ms
{
    mav(RIGHT, MAX_SPEED);
    mav(LEFT, MAX_SPEED);
    msleep(time);
    ao();
}

void arm_down_for_dist(int dist) // Dist is in Back-EMF's
{
    cmpc(RIGHT);
    cmpc(LEFT);
    mav(RIGHT, MAX_SPEED);
    mav(LEFT, MAX_SPEED);
    while(!(gmpc(RIGHT) > dist) && !(gmpc(LEFT) > dist))
    {
        msleep(1);
    }
    ao();
}

void arm_up_for_dist(int dist) // Dist is in Back-EMF's
{
    cmpc(RIGHT);
    cmpc(LEFT);
    mav(RIGHT, -MAX_SPEED);
    mav(LEFT, -MAX_SPEED);
    while(!(gmpc(RIGHT) < -dist) && !(gmpc(LEFT) < -dist))
    {
        msleep(1);
    }
    ao();
}

void arm_up_botguy() 
{
    msleep(200);
	arm_up_for_dist(200);
}

//Initialization. Do not modify without good reason!
void initialize()
{
    create_connect();
    while(!a_button() && !b_button() && !c_button()) 
    {
        if (a_button())
        {
            botguy_offset = -2;
        }
        else if (b_button())
        {
            botguy_offset = 0;
        }
        else if (c_button())
        {
            botguy_offset = 2;
        }
    }

    //enable_servos();
    cmpc(LEFT);
    cmpc(RIGHT);
}

void cow_dump_wrist()
{
    servo_slow(WRIST, WRIST_UP, 1000);
}

void cow_dump_claw()
{
    //servo_slow(CLAW, CLAW_OPEN-1000, 2400);
    set_servo_position(CLAW, CLAW_OPEN);
    msleep(200);
}

void cow_dump()
{
    arm_down_for_time(700);
    cow_dump_wrist();
    msleep(200);
    create_backward(1, 400);
    msleep(200);
    cow_dump_claw();
    set_servo_position(WRIST, WRIST_DOWN);
    msleep(750);
    //arm_up();
}

void cow_dump_second()
{
    arm_down_for_time(700);
    cow_dump_wrist();
    msleep(200);
    create_backward(1, 400);
    msleep(200);
    cow_dump_claw();
    set_servo_position(WRIST, WRIST_BOTGUY-200);
    msleep(750);
    arm_up();
}

int get_biggest_object()
{
    int object;
    int area;
    int greatest_area = 0;
    int biggest_object = 0;
	for (object = 0; object < get_object_count(CHANNEL); object++)
    {
    	area = get_object_area(CHANNEL, object);
        if (area > greatest_area)
        {
        	greatest_area = area;
            biggest_object = object;
        }
    }
    return biggest_object;
}

void straighten_ridge()
{
	// Straightens up on the ridge in the middle of the field
    while(!digital(TOUCH_RIDGE_LEFT))
    {
    	create_drive_direct(100, 100);
        msleep(1);
    }
    while(!digital(TOUCH_RIDGE_RIGHT))
    {
    	create_drive_direct(0, 100);
        msleep(1);
    }
}

int main()
{
    initialize();
    //light_start();
    shut_down_in(119);
    set_servo_position(WRIST, WRIST_UP);
    create_right(20, 50);
    arm_up();
    create_left(20, 50);
    enable_servos();

    //getting out of starting box
    claw_open(100);
    create_backward(5, 300);
    create_right(150, 300);
    create_backward(25,300);
    create_forward(44,300); //51
    create_right(90,300);
    create_backward(20, 300);

    //moving down middle of field towards botguy
    create_forward(50, 300); //114
    create_left(1, 300);
    create_forward(64, 300);
    straighten_ridge();
    
    // Move until botguy is in the center of camera's view
    /*
    camera_open_black();
    camera_update();
    point2 center = get_object_center(CHANNEL, get_biggest_object());
    while (center.x < (70) || center.x > (90)) 
    // While the center of botguy is not within 10 units of center of camera view
    {
        center = get_object_center(CHANNEL, get_biggest_object()); //Get the center of the biggest 
        camera_update();
        if (center.x < (70))
        {
        	create_backward(1, 100);
        } 
        else if (center.x > (90))
        {
        	create_forward(1, 100);
        }
        
    }
    camera_close();
    */
    //create_backward(5, 300);
	if (botguy_offset<0)
    {
    	create_backward(botguy_offset*-1, 200);
    }
    else 
    {
    	create_forward(botguy_offset, 200);
    }
    create_right(93,100);
    msleep(100);
    create_backward(60,300);
    create_forward(35,300);

    //positioning arm for botguy and grabbing him
    arm_down_for_dist(4800); //5200
    set_servo_position(CLAW, CLAW_MID);
    set_servo_position(WRIST, WRIST_DOWN+200);
    
   	thread arm_up_botguy_thread = thread_create(arm_up_botguy);
    thread_start(arm_up_botguy_thread);
    create_backward(18,100); //16, 300
    thread_wait(arm_up_botguy_thread);
    thread_destroy(arm_up_botguy_thread);
    
    arm_down_for_time(750);
    msleep(150);
    claw_close(200);
    arm_up_for_time(1750);
    create_forward(13,300);
    arm_up();
    set_servo_position(WRIST, WRIST_DOWN);
    create_backward(10,300);

    //dumping botguy on the ramp
    create_right(90,200);
    msleep(100);
    create_backward(42,300);
    msleep(100);
    create_right(90,200);
    msleep(100);
    create_backward(41,300); //38
    msleep(100);
    set_servo_position(WRIST, WRIST_UP-300);
    msleep(300);
    set_servo_position(CLAW, CLAW_OPEN);
    msleep(300);
    set_servo_position(WRIST, WRIST_DOWN);

    //moving away from the ramp and starting route to pick up the first blue cow
    msleep(200);
    create_forward(15,150);
    create_right(25,200);
    create_forward(19,150);
    create_left(70, 100);
    while (analog(SEE_COW) < 1300)
    {
    	create_left(1, 100);
    }
    
    //positioning arm for blue cow
    while (analog(SEE_COW) < 2900){
        create_backward(1, 100);
    }
    create_right(45, 100);
    while (analog(SEE_COW) < 2900){
        create_left(1, 100);
    }
    create_left(8, 100);
    msleep(100);
    create_forward(31,200);
    create_right(1,25);
    set_servo_position(WRIST, WRIST_UP);
    arm_down_for_dist(2200);
    set_servo_position(WRIST, WRIST_DOWN);
    arm_down_for_dist(4500);
    msleep(500);

    //grab blue cow and bring to ramp
    set_servo_position(CLAW, CLAW_CLOSE);
    msleep(200);
    arm_up_for_time(1000);
    set_servo_position(WRIST, WRIST_DOWN);
    arm_up();
    create_left(1, 25);
    create_backward(41,300); //move along platform to place the cow to the left of botguy
    create_right(90,100);
    msleep(100);
    create_backward(25,200);
    msleep(100);
    create_forward(5,100);
    msleep(100);

    cow_dump();

    create_forward(35,200);

    //turn and square up against far wall
    set_servo_position(WRIST, WRIST_UP);
    create_left(90, 300);
    msleep(100);
    create_backward(45, 400);

    //go towards 2nd cow
    create_forward(67, 400);
    create_left(90, 300);
    msleep(200);
    create_backward(25, 100);
    //turn a little to the right
    create_stop();
    create_drive_direct(-100, 0);
    msleep(1000);
    create_stop();
    /*
    while(analog(SEE_COW) > 1000)
    {
    	create_backward(1, 100);
    }*/
    //create_backward(40, 300);
    /*create_drive_direct(100, 0);
    msleep(500);
    while(analog(SEE_COW) > 1000)
    {
    	create_drive_direct(-100, 0);
    }
    */
    //create_left(4, 25);

    // position arm for 2nd cow
    set_servo_position(WRIST, WRIST_UP);
    arm_down_for_dist(3000);
    set_servo_position(WRIST, WRIST_DOWN);
    arm_down_for_dist(2200);
    msleep(500);
    set_servo_position(CLAW, CLAW_CLOSE);
    msleep(200);
    arm_up_for_time(1000);
    set_servo_position(WRIST, WRIST_DOWN);
    arm_up();

    //head towards ramp
    create_forward(20, 300);
    create_left(200,200);
    msleep(100);
    create_backward(25, 300);
    //create_forward(4,100);
    //msleep(500);

    cow_dump_second();

    //head towards water tank
    create_forward(20, 300);
    create_left(110, 300);
    msleep(100);
    create_forward(36, 300);
    straighten_ridge();
    create_backward(20, 300);
    create_left(86, 300);
    create_backward(45, 300);
    
    // Shake Water tank
    create_forward(26, 300);
    set_servo_position(WRIST, WRIST_UP);
    //set_servo_position(CLAW, CLAW_OPEN-600);
    set_servo_position(CLAW, CLAW_MID);
    arm_down_for_dist(5500);
    set_servo_position(WRIST, WRIST_DOWN);
    arm_down();
    set_servo_position(WRIST, 200);
    create_backward(24, 300); //24
    arm_up_for_dist(3500);
    create_forward(15, 200);
    arm_up_for_dist(2500);
    create_backward(10, 200);
    arm_up_for_dist(750);
    set_servo_position(CLAW, CLAW_CLOSE + 300);
    msleep(100);
    //create_forward(10, 100);
    create_left(90, 100);
    
    disable_servos();
    return 0;
}
