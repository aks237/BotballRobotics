#include "createDrive.h"
#include <math.h>
/**
 * create_line_follow makes the create line follow. It sticks to the line by slowing down to arc back to the line. 
 * Requires following to be defined:
 * RIGHT_LINE for the tophat sensor on the right side
 * LEFT_LINE for the tophat sensor on the left side
 * dist: distance to travel in centimeters
 * speed: speed to drive at on a range from 0-1000
 */
void create_line_follow(int dist, int speed)
{
    //create_line_follow makes the create line follow for "dist" centimeters at "speed" speed

    dist*=10; //Converts centimeters into millimeters
    set_create_distance(0); //Clears create position counter
    create_drive_straight(speed); //Initializes movement
    while(abs(get_create_distance())<abs(dist)) //while distance has not been achieved, perform the following:
    {
        if(analog(LEFT_LINE)>leftBlack-200)				  //if the left tophat sensor sees black
        {
            create_drive_direct(speed*.75,speed*1.25); //drive to the left
        }
        else if(analog(RIGHT_LINE)>rightBlack-200)		  //if the right tophat sensor sees black
        {
            create_drive_direct(speed*1.25,speed*.75); //drive to the right
        }
        else if(analog(LEFT_LINE)<leftWhite)
        {
			create_drive_direct(speed*1.75,speed*.25);
        }
        else if(analog(RIGHT_LINE)<rightWhite)
        {
			create_drive_direct(speed*.25,speed*1.75);
        }
        else									  //if none of the tophat sensors see black
        {
            create_drive_straight(speed);			  //drive straight forward
        }
        msleep(5);
    }
    create_stop();							  //stop the create once distance has been achieved
}

/**
 * create_forward makes the create go forward straight. 
 * Requires no predefined constants
 * dist: distance to travel in centimeters
 * speed: speed to drive at on a range from 0-1000
 */
void create_forward(int dist, int speed)
{
    dist*=10; //Converts centimeters into millimeters
    set_create_distance(0); //Clears create position counter
    create_drive_straight(speed); //Initializes movement
    while(get_create_distance()<dist) //Waits until distance traveled reaches distance
    {
        msleep(1);
    }
    create_stop(); //Stops movement
}
/**
 * create_backward makes the create go backward straight. 
 * Requires no predefined constants
 * dist: distance to travel in centimeters
 * speed: speed to drive at on a range from 0-1000
 */
void create_backward(int dist, int speed)
{
    dist*=10; //Converts centimeters into millimeters
    set_create_distance(0); //Clears create position counter
    create_drive_straight(-speed); //Initializes movement 
    while(get_create_distance()>-dist) //Waits until distance traveled reaches distance
    {
        msleep(1);
    }
    create_stop(); //Stops movement
}
/**
 * create_left makes the create turn left in place. 
 * Requires no predefined constants
 * degrees: angle to turn in degrees
 * speed: speed to drive at on a range from 0-1000
 */
void create_left(int degrees, int speed)
{
    set_create_total_angle(0); //Clears create angle counter
    create_drive_direct(-speed,speed); //Initializes movement
    while(get_create_total_angle()<degrees) //Waits until angle turned reaches degrees
    {
        msleep(5);
    }
    create_stop(); //Stops movement
}
/**
 * create_right makes the create turn right in place. 
 * Requires no predefined constants
 * degrees: angle to turn in degrees
 * speed: speed to drive at on a range from 0-1000
 */
void create_right(int degrees, int speed)
{
    set_create_total_angle(0); //Clears create angle counter
    create_drive_direct(speed,-speed); //Initializes movement
    while(get_create_total_angle()>-degrees) //Waits until angle turned reaches degrees
    {
        msleep(5);
    }
    create_stop(); //Stops movement
}

/**
 * create_square_up makes the create square up against a wall
 * Requires no predefined constants
 * time: time to square up for
 * speed: speed to drive at on a range from 0-1000
 */
void create_square_up(int time, int speed) 
{
    create_drive_direct(speed, speed);
    msleep(time);
    create_stop();
}

/**
 * create_square_up_line makes the create square up against a line using tophat sensors
 * Requires following to be defined:
 * RIGHT_LINE for the tophat sensor on the right side
 * LEFT_LINE for the tophat sensor on the left side
 * speed: speed to drive at on a range from 0-1000
 */
void create_square_up_line(int speed)
{
    int tolerance = 300;
    while (analog(LEFT_LINE) < leftBlack-tolerance || analog(RIGHT_LINE) < rightBlack-tolerance) 
    {
        if (analog(LEFT_LINE) < leftBlack-tolerance && analog(RIGHT_LINE) < rightBlack-tolerance) 
        {
            create_drive_direct(speed, speed);
        } 
        else if (analog(RIGHT_LINE) < rightBlack-tolerance) 
        {
            create_drive_direct(0, speed/2);
        }
        else if (analog(LEFT_LINE) < leftBlack-tolerance) 
        {
            create_drive_direct(speed/2, 0);
        }
    }
    create_stop();
}


/**
 * create_detect_bump makes the create go a certain speed until its bumpers are triggered.
 * Requires following to be defined:
 * speed: speed to drive at on a range from 0-1000
 * timeout: wait this amount of time before stopping the create, even if it hasn bumped anything
 */
void create_detect_bump(int speed, int timeout) 
{
	create_drive_direct(speed, speed);
    int ms = 0;
    while (ms < timeout) 
    {   
        if (get_create_lbump() || get_create_rbump())
        {
        	break;
        }
    	msleep(1);
        ms++;
    }
    create_stop();
}
