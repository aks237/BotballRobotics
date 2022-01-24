#include <kipr/botball.h>
#include "main.h"
#include "arm.h"
#include "createDrive.h"
#include "sort.h"
#include "time.h"

#define STATE 1
#define LIGHT_PORT 0

int light_start()
{
	int state = -1;
	int min = 9999;
	int max = 0;
	int curr = 0;
	int avg = 9999;
	while(a_button()==0)
	{
		curr = analog(LIGHT_PORT);
		if(curr > max)
			max = curr;
		if(curr < min)
			min = curr;
		avg = (max+min)/2;
		printf("Min: %d\n Max: %d\n Curr: %d\n Avg: %d\n",min,max,curr,avg);
		msleep(1);
	}
	while(state == -1)
	{
		if(a_button()==1)
		{
            state = 1;
			printf("ARMED\nSTATE:%d", state);
		}
        else if(b_button()==1)
        {
            state = 2;
            printf("ARMED\nSTATE:%d",state);
        }
	}
	while(analog(LIGHT_PORT) > avg)
	{
		msleep(1);
	}
	return state;
}

void create_shake(int am, int spd1, int spd2, int fur){
    //am is the amount to move 
    //spd1 is the speed to move forward
    //spd2 is speed backward
    //fur is which furrow. 0 is the further one, 1 is the closer one
    int init;
    int final;
    if (fur == 0)
    {	
        init = BIG_RIGHT;
    	final = BIG_LEFT;
    } 
    else 
    {
        init = BIG_LEFT;
    	final = BIG_RIGHT;
    }
    
    set_servo_position(BIG, init);
    create_forward(am, spd1);
    msleep(200);
    set_servo_position(BIG, final);
    create_backward(am, spd2);
    msleep(200);
    set_servo_position(BIG, init);
    msleep(1000);
}

void arm_shake()
{
	set_servo_position(SHOULDER_PORT, SHOULDER_DUMP+100);
    msleep(100);
    set_servo_position(SHOULDER_PORT, SHOULDER_DUMP);
    msleep(100);
    set_servo_position(SHOULDER_PORT, SHOULDER_DUMP+100);
    msleep(100);
    set_servo_position(SHOULDER_PORT, SHOULDER_DUMP);
    msleep(100);
}

void create_shake_turn_forever()
{
	while(1)
    {
    	create_shake_turn();
    }
}

int main()
{
    int old_time = time(NULL);
    create_connect();

    //initialise servo positions
    //set_servo_position(FURROW_PORT, FURROW_MIDDLE_POS);
    set_servo_position(SHOULDER_PORT, 810);
    set_servo_position(BIG, BIG_RIGHT);
    set_servo_position(GATE,GATE_CLOSE);
    enable_servos();
    light_start();
    set_servo_position(SHOULDER_PORT, SHOULDER_DUMP);
    //msleep(300);
    
    //for testing SORTING
    /*
    camera_open_black();
    sort_orange_green();
    msleep(1000);
    dump_green();
    return 0;
    dump_left();
    create_forward(2, 150);
    create_backward(2, 150);
    create_drive_direct(-150, -150);
    msleep(500);
    create_forward(8, 150);
    msleep(1500);
    dump_right();
    create_forward(2, 150);
    create_backward(2, 150);
   	return 0;
    //*/
    
    //for testing DUMPING POMS
    /*
    set_servo_position(GATE,GATE_CLOSE);
    create_drive_direct(-50, -50);
    msleep(6000);
    create_stop();
    dump_left();
    create_shake(1, 200, 0);
    create_forward(8, 150);
    msleep(1500);
    dump_right();
    create_shake(1, 150, 1);
    create_forward(5, 150);
    
    
    return 0;
    */

    // Robot starts facing balcony
    //Back of robot alligned to wall and left side of robot is away from left wall
	shut_down_in(119);
    mrp(3,1400 / 12,100); //1400 is bemf per rotation, / 12 indicates that it is a 30 degree turn
    create_drive_direct(-300, -300);
    msleep(1000);
    create_stop();
    msleep(1000);

    create_forward(50, 200);

    /*
    while(!get_create_lbump()){
        create_drive_straight(100);
    }*/

    //turn towards main field
    create_right(100, 150);

    //square up against right wall
    create_drive_direct(-300, -300);
    msleep(500);

    //go to green poms 
    create_forward(114, 200);
    create_right(100, 150);

    //square up
    create_drive_direct(-300,-300);
    msleep(1500);
    set_servo_position(SHOULDER_PORT, SHOULDER_TEMP);

    //pick up bin with green poms and dump
    create_forward(34,450);
    pick_up_bin(10);
    create_backward(15,450);
    thread dump_bin_thread = thread_create(dump_bin);
    thread_start(dump_bin_thread);
	
	//start sorting
    camera_open_black();
    thread sort_green_thread = thread_create(sort_green);
    thread_start(sort_green_thread);
    
    create_forward(20, 100);

    //backup on the wall again
    create_left(90, 150);
    create_backward(70,450);
    create_drive_direct(-200, -200);
    msleep(3000);

    //go forward, turn, and square up against starting wall. 
    create_forward(19, 150);
    create_left(100, 150);
    thread dump_dregs_thread = thread_create(dump_dregs);
    thread_start(dump_dregs_thread);
    create_drive_direct(-200, -200);
    msleep(1000);
    dump_dregs();
    create_stop();
    thread_wait(dump_dregs_thread);
    thread_destroy(dump_dregs_thread);
    msleep(200);
    dump_dregs();
    msleep(250);
	
	//bring arm down and move bin under orange poms
    set_servo_position(SHOULDER_PORT, SHOULDER_PICK_UP);
    msleep(250);
    create_forward(40, 150);
    
	//drop bin and align arm with device
    set_servo_position(WRIST_PORT, WRIST_OPEN);
    msleep(1000);
    set_servo_position(SHOULDER_PORT, SHOULDER_DUMP);
    msleep(300);
    create_forward(10,150);
    create_left(39, 150);

    //spin arm
    motor(3, -100);
    msleep(3200);
    off(3);
	
	//turn back and pick up bin
    create_right(39, 150);
    msleep(500);
    create_backward(9, 150);
    msleep(500);
    pick_up_bin(5);
    msleep(500);
    set_servo_position(SHOULDER_PORT, SHOULDER_TEMP);
    msleep(300);
	
    if(STATE==1)
    {
		//square up with left wall
        create_forward(5,150);
        create_right(90,150);
        create_drive_direct(-150,-150);
        msleep(3000);
        create_forward(3,150);
        create_right(95,150);
        create_drive_direct(-150,-150);
        msleep(3000);
        thread_wait(sort_green_thread);
        thread_destroy(sort_green_thread);
        
        //dump left while shaking create
        create_right(10,150);
        dump_left();
        create_left(15,150);
        int i;
        for(i = 0; i < 3; i++) {
        create_left(5,150);
        create_right(5,150);
        }
        
        //square up against furrow and dump the poms in the right
        create_drive_direct(-150,-150);
        msleep(500);
        create_forward(5,150);
        dump_right();
        create_shake(1, 150, 250, 1);
        
        //dump bin and start sorting orange poms
        dump_bin();
        thread sort_orange_thread = thread_create(sort_orange_green);
   		thread_start(sort_orange_thread);
        
        //square up against close wall
        printf("SORT ORANGE POMS thread started\n");
        create_forward(20,150);
        create_left(90,150);
        create_drive_direct(-150,-150);
        msleep(1000);
        
        //go forward and square up against furrows
        create_forward(41,450);
        create_right(90,150);
        create_drive_direct(-150,-150);
        msleep(3000);
        
        //sort orange while having them fall down into the furrows while shaking
        thread create_shake_turn_thread = thread_create(create_shake_turn_forever);
        thread_start(create_shake_turn_thread);
	set_servo_position(GATE,GATE_LEFT);
        create_stop();
        msleep(2000);
        dump_dregs();
	
	//when done sorting first 4 poms, stop shaking
        thread_wait(sort_orange_thread);
        thread_destroy(sort_orange_thread);
        printf("destroy orange thread\n");
        thread_destroy(create_shake_turn_thread);
	create_stop();
        printf("stop turn thread\n");
        msleep(300);
	    
	//square up against furrows and align with 2nd set of furrows
        create_drive_direct(-150,-150);
        msleep(600);
        create_forward(5, 150);
	    
	//start shaking thread
	thread create_shake_turn_thread = thread_create(create_shake_turn_forever);
        thread_start(create_shake_turn_thread);
	
	//sort the last 3 orange poms
	sort_orange_green(); 
        thread_destroy(create_shake_turn_thread);
        printf("stop turn thread second time\n");
        
	    //flatten poms in furrows
        create_forward(10,150);
        create_left(90,150);
        set_servo_position(SHOULDER_PORT,SHOULDER_FLATTEN);
        create_left(110,150);
        create_right(60,150);
        set_servo_position(SHOULDER_PORT,SHOULDER_DUMP);
        create_right(45,150);
        create_backward(20,300);
        create_drive_direct(-150,-150);
        msleep(2000);
        create_forward(3,150);
        create_right(90,150);
        set_servo_position(SHOULDER_PORT,SHOULDER_FLATTEN);
        create_right(210,150);
        create_left(60,150);
    }
    else {
    create_right(50,150);
    create_forward(9, 150);
    create_right(50, 150);
    create_drive_direct(-200, -200);
    msleep(2000);

    create_drive_direct(400,430);
    msleep(5500);
    
    create_drive_direct(140,150);
    msleep(2400);
    
	create_backward(23,200);
    create_right(90, 150);
    create_drive_direct(-150, -150);
    msleep(1800);
    create_drive_direct(-50, -50);
    msleep(2600);
    
    //STOP sorting
    thread_destroy(sort_green_thread);
    
    //green poms into furrows
    msleep(500);
    dump_left();
    create_shake(1, 150, 250, 0);
    create_drive_direct(-150, -150);
    msleep(500);
    create_forward(8, 150);
    msleep(1500);
    dump_right();
    create_shake(1, 150, 250, 1);
    create_forward(5, 150);
    
    //dump and start sorting orange poms
    dump_bin();
    thread sort_orange_thread = thread_create(sort_orange_green);
    thread_start(sort_orange_thread);
    //dump_dregs();
    
    //thread dump_green_thread = thread_create(dump_green);
    //thread_start(dump_green_thread);
    
    create_forward(5, 150);
    create_left(180, 150);
    
    //thread_wait(dump_green_thread);
    //thread_destroy(dump_green_thread);
    
    //flatten the furrows
    //flatten_furrows();
    
    //turn and square up against far wall
    create_left(120, 150);
    create_drive_direct(-150, -150);
    msleep(3500);
    
    //turn towards furrows
    create_forward(10, 150);
    create_left(100, 150);
    //square up with furrows
    create_drive_direct(-150, -150);
    msleep(800);
    create_drive_direct(-50, -50);
    msleep(3200);
    create_stop();
    set_servo_position(GATE,GATE_LEFT);
    msleep(2000);
    dump_dregs();
    thread_wait(sort_orange_thread);
    thread_destroy(sort_orange_thread);
    create_drive_direct(-50, -50);
    msleep(2500);
    create_forward(10,150);
    set_servo_position(SHOULDER_PORT,SHOULDER_FLATTEN);
    create_left(270,150);
    create_right(90,150);
    create_backward(10,150);
    create_left(45,150);
    create_right(70,150);
    printf("Run took %f seconds", time(NULL)-old_time);
    }
    /*

    //collect green poms

    create_backward(20, 300);
    create_left(87, 300);
    //create_forward(45, 300);
    while(!get_create_lbump()){
    create_drive_straight(100);

    }
    create_backward(5,100);
    create_left(88, 300);

    //create_forward(52, 300);
    while(!get_create_lbump()){
    create_drive_straight(100);
    }
    create_backward(5,100);
    create_left(90, 300);
    //sort poms
    //Drop-off all poms at the furrow
    */
    disable_servos();
    create_disconnect();
    return 0;

}

/*
void servo_slow(int port, int final_pos, int ms)
{ //ms is the amount of ms to wait
    int x = get_servo_position(port);
    int diff = final_pos - x;
    int change = 0;

    if (diff > 0) 
    {
    	change = 1;
    }
    else 
    {
    	change = -1;
    }

	while (get_servo_position(port) != final_pos) 
    {
    	set_servo_position(port, x);
        msleep(ms);
        x += change;
    }
}
*/
