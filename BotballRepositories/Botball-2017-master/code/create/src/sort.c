#include <kipr/botball.h>
#include "main.h"
#include "sort.h"

void multicamupdate(int count) {
    int i;
    for(i = 0; i < count; i++)
        camera_update();
}

void sort_green() {
    sort(GREEN);
}

void sort_orange() {
    sort(ORANGE);
}

int area(int channel) {
    int i,count;
    int size = 0;
    count = get_object_count(channel);
    for(i = 0; i < count ; i++)
        size += get_object_area(channel,i);
    return size;
}

void spin_back_check() {
    if(gmpc(SPINNER_PORT) > 350) {
        cmpc(SPINNER_PORT);
        motor(SPINNER_PORT, -SPINNER_SPEED);
        msleep(250);
        motor(SPINNER_PORT, SPINNER_SPEED);
    }
}

void unjam() {
    while(1) {
        cmpc(SPINNER_PORT);
        msleep(250);
        //printf("SPINNER_CHANGE: %d\n", gmpc(SPINNER_PORT));
        if(gmpc(SPINNER_PORT)<10) {
            motor(SPINNER_PORT, -SPINNER_SPEED);
            msleep(750);
            motor(SPINNER_PORT, SPINNER_SPEED);
        }
    }
}

void create_shake_turn() {
    	create_left(5,150);
    	create_right(5,150);
}

void sort_orange_green() {
    int count = 0;
    thread unjam_thread = thread_create(unjam);
    
    thread_start(unjam_thread);
    multicamupdate(20);
    set_servo_position(GATE,GATE_CLOSE);
    set_servo_position(BIG,BIG_MIDDLE);
    msleep(400);
    motor(SPINNER_PORT,SPINNER_SPEED);
    
    
    while(count < 4) {
        multicamupdate(4);
        if(area(ORANGE)>CAMERA_TOLERANCE) {
            printf("seen orange\n");
            set_servo_position(BIG,BIG_RIGHT);
            msleep(SPINNER_DELAY);
            off(SPINNER_PORT);
            //thread_destroy(unjam_thread);
            //if (count == 7)
            //    thread_destroy(unjam_thread);
            msleep(300);
            /*if(count>=4) {
                set_servo_position(BIG,BIG_LEFT_LESS);
                msleep(450);
            }
            else
            {*/
                servo_slow(BIG,BIG_LEFT_LESS,450);
            //}
            set_servo_position(BIG, BIG_MIDDLE);
            msleep(300);
            multicamupdate(8);
            msleep(300);
            motor(SPINNER_PORT,SPINNER_SPEED);
            //unjam_thread = thread_create(unjam);
            //thread_start(unjam_thread);
            if(count%2 == 0)
	    {
	    	//arm_shake(); COMMENTED OUT SO IT DOESN"T INTERFERE WITH ARM
	    }
                
            count++;
            /*
            if(count == 4) {
                thread_destroy(create_shake_turn_thread);
                ao();
                create_left(10,150);
                set_servo_position(GATE,GATE_LEFT);
                msleep(500);
                create_right(15,150);
                int i;
                for(i = 0; i < 3; i++) {
                    //thread create_shake_turn_thread = thread_create(create_shake_turn);
                    //thread_start(create_shake_turn_thread);
                }
                create_left(5,150);
                create_forward(5, 150);
                motor(SPINNER_PORT,SPINNER_SPEED);
                thread_start(create_shake_turn_thread);
            }*/
        }
        camera_update();
        if(area(GREEN) > CAMERA_TOLERANCE) {
            printf("seen green\n");
            set_servo_position(BIG,BIG_LEFT_LESS);
            msleep(SPINNER_DELAY);
            off(SPINNER_PORT);
            //thread_destroy(unjam_thread);
            msleep(300);
            set_servo_position(BIG,BIG_RIGHT);
            msleep(450);
            set_servo_position(BIG, BIG_MIDDLE);
            msleep(300);
            multicamupdate(8);
            msleep(300);
            motor(SPINNER_PORT,SPINNER_SPEED);
            //unjam_thread = thread_create(unjam);
            //thread_start(unjam_thread);
        }
    }
    off(SPINNER_PORT);
    thread_destroy(unjam_thread);
}

void sort(int color) {
    printf("sorting %d poms\n", color);
    int count = 0;
    thread unjam_thread = thread_create(unjam);
    
   	thread_start(unjam_thread);
    multicamupdate(20);
    set_servo_position(GATE,GATE_CLOSE);
    set_servo_position(BIG,BIG_MIDDLE);
    msleep(400);
    printf("SPINN THE SPINNER\n");
    motor(SPINNER_PORT,SPINNER_SPEED);
    
    while(count < 4) {
        multicamupdate(4);
        //spin_back_check();
        if(area(color)>CAMERA_TOLERANCE) {
            set_servo_position(BIG,BIG_RIGHT);
            msleep(SPINNER_DELAY);
            off(SPINNER_PORT);
            //thread_destroy(unjam_thread);
            msleep(300);
            set_servo_position(BIG,BIG_LEFT_LESS);
            msleep(450);
            set_servo_position(BIG, BIG_MIDDLE);
            msleep(300);
            multicamupdate(8);
            msleep(300);
            printf("START SPINNER\n");
            motor(SPINNER_PORT,SPINNER_SPEED);
            //thread unjam_thread = thread_create(unjam);
            //thread_start(unjam_thread);
            if(count%2 == 0)
	    {
                //arm_shake(); COMMENTED OUT SO DOESN"T INTERFERE WITH ARM
	    }
            count++;
        }
    }
    off(SPINNER_PORT);
    if(color == ORANGE) {
        dump_left();
        create_shake(1, 50, 500, 0);
        msleep(1000);
    }
    set_servo_position(BIG,BIG_MIDDLE);
    msleep(400);
    motor(SPINNER_PORT,SPINNER_SPEED);
    while(count < 8) {
        multicamupdate(4);
        //spin_back_check();
        if(area(color)>CAMERA_TOLERANCE) {
            set_servo_position(BIG,BIG_LEFT_LESS);
            msleep(SPINNER_DELAY);
            off(SPINNER_PORT);
            //thread_destroy(unjam_thread);
            msleep(300);
            set_servo_position(BIG,BIG_RIGHT);
            msleep(450);
            set_servo_position(BIG, BIG_MIDDLE);
            msleep(300);
            multicamupdate(8);
            msleep(300);
            printf("START SPINNER\n");
            motor(SPINNER_PORT,SPINNER_SPEED);
            if (count != 7) {
                //thread unjam_thread = thread_create(unjam);
                //thread_start(unjam_thread);
            }
            count++;
        }
        cmpc(SPINNER_PORT);
    }
    off(SPINNER_PORT);
    thread_destroy(unjam_thread);
}

void dump_green() {
    //dumps extra green poms
    int time,count = 0;
    
    printf("DUMPING GREEN POMS!!!!\n");
    set_servo_position(GATE, GATE_UP);
    set_servo_position(BIG, BIG_LEFT_LESS);
    multicamupdate(20);
    motor(SPINNER_PORT, SPINNER_SPEED);


    while(time < DUMP_GREEN_TIME && count < 3)
    {
        camera_update();

        if(area(GREEN)>500)
        {
            set_servo_position(BIG, BIG_RIGHT);
            msleep(250);
            set_servo_position(BIG, BIG_LEFT_LESS);
            msleep(250);
            time = 0;
            msleep(2000);
            count++;
        }
        time++;
        printf("time: %d\n", time);
    }
    off(SPINNER_PORT);
}

void dump_left() {
    set_servo_position(GATE, GATE_LEFT);
    msleep(1000);
    set_servo_position(BIG, BIG_LEFT);
    msleep(500);
    set_servo_position(BIG, BIG_MIDDLE);
    printf("DUMP LEFT\n");
}

void dump_right() {
    servo_slow(GATE, GATE_RIGHT_SEMI,750);
    msleep(500);
    set_servo_position(GATE, GATE_RIGHT);
    msleep(500);
    set_servo_position(BIG, BIG_RIGHT);
    msleep(500);
    set_servo_position(BIG, BIG_MIDDLE);
    printf("DUMP RIGHT\n");
    
}

/*
int isbreak;
int green_count	= 0;
int orange_count = 0;
int pink_count = 0;
int blue_count = 0;

void shake_arm(int amount)
{	
    int x = get_servo_position(BIG);
	set_servo_position(BIG, x+amount);
    msleep(200);
    set_servo_position(BIG, x-amount);
    msleep(200);
    set_servo_position(BIG, x);
    msleep(200);
    printf("shook %d\n", amount);
}

void timer()
{
    printf("Timer started.\n");
    msleep(SORT_TIME);
    isbreak = 1;
    printf("timer done! \n");
}

void sort_main()
{
    isbreak = 0;
    //time milliseconds
    thread timer_thread = thread_create(timer);
    thread_start(timer_thread);

    int color;

    motor(SPINNER_PORT, 150);

    while (1)
    {
        printf("in while loop....\n");
        if (isbreak)
        {
            off(SPINNER_PORT);
            thread_destroy(timer_thread);
            printf("breaking out of while loop\n");
            break;
        }
        color = find_color();
        printf("Color: %d\n",color);
        if (color != -1)
        {
            printf("found a color\n");
            //found a color
            if (think(color)) 
            {
                //continue using motor
                printf("think() told robot to keep using motor\n");
                msleep(500);
            }
            else
            {
                printf("think() told robot to stop sorting\n");
                off(SPINNER_PORT);
                break;
            }

        }
        else
        {
        	printf("did not find a color...\n");
        }
    }
}

// ========================== //
//     Find current color     //
// ========================== //
int find_color()
{   
    int current_highest_area = 0;   //current highest number of detected objects
    int current_index = -1;          //current channel with highest number of detected objects
    int x, area_max;               //declares loop variable as well as a variable used for
                                    //storing current object count,
    int i;
    for (i = 0; i < CHANNEL_COUNT; i++) 
    {
        area_max = 0;
        //finds channel with largest area
        int k;
        for (k=0; k < 5; k++)
        {
            camera_update();
        }
        x = get_object_count(i);
        printf("x=%d i=%d\n", x, i);
        if ( x != 0)
        {   
            //for loop for getting the total area of the objects
            int j; 
            for(j = 0; j < x; j++)
            {
                if (get_object_area(i,j) > area_max)
                {
                    area_max = get_object_area(i,j);
                }
            } 
            printf("index: %d, area_max: %d\n", i, area_max);
            //for every object there is, run once and add to total area
            if (area_max > current_highest_area)
            {
                current_highest_area = area_max;
                current_index = i;
            }
        }
    }

    return current_index;
}

// ====================================	//
// Decides what to do with the pom	 	//
// Return value of 1 continues sorting 	//
// 0 stops sorting 						//
// ================================= 	//
int think(int color){
    //int greencount = 0;
    //int orangecount = 0;
    //int pinkcount = 0;
    //int bluecount = 0;
    int colorlist[4] = {green_count, orange_count, pink_count, blue_count};

    //assuming first pom is green
    if (colorlist[color] >= colorlist[GREEN] && color != GREEN) 
    {
        //has reached equillibrium among poms
        //stop sorting
        return 0;
    }
    else
    {

        switch(color)
        {
            case GREEN:
                green_count++;
                printf("detected GREEN pom\n");
                break;
            case PINK:
                pink_count++;
                printf("detected GREEN pom\n");
                break;
            case ORANGE:
                orange_count++;
                printf("detected GREEN pom\n");
                break;
            case BLUE:
                blue_count++;
                printf("detected GREEN pom\n");
                break;
        }

        if (colorlist[color] % 2 == 0) 
        {
        //even amount of poms
            off(SPINNER_PORT);
            set_servo_position(SWITCH_PORT, SWITCH_RIGHT);
            msleep(500);
            motor(SPINNER_PORT, 150);

        } 
        else
        {
        //odd amount of poms
            off(SPINNER_PORT);
            set_servo_position(SWITCH_PORT, SWITCH_RIGHT);
            msleep(500);
            motor(SPINNER_PORT, 150);
        }
    }
    return 1;

}

// ========================== //
//      Sorts the colors      //
// ========================== //

//(unused)
void sort_colors(){
    //reset counts of poms
    blue_count = 0; green_count = 0; orange_count = 0; pink_count = 0;

    stopSort = 0;
    while (!stopSort)
    {
        //detect color, return color. (-1=none,0=green,1=pink,2=orange,3=blue)
        int result = find_color();
        if (result != -1)
        {
            switch (result)
            {
                case GREEN:
                    //set_servo_position(FURROW_SERVO_PORT,FURROW_NEAR_POS);
                    printf("green detected. not discard? %d", think(GREEN));
                    green_count += 1;
                    break;
                case PINK:
                    //set_servo_position(FURROW_SERVO_PORT,FURROW_MIDDLE_POS);
                    printf("pink detected. not discard? %d", think(PINK));
                    pink_count += 1;
                    break;
                case ORANGE:
                    //set_servo_position(FURROW_SERVO_PORT,FURROW_FAR_POS);
                    printf("orange detected. not discard? %d", think(ORANGE));
                    orange_count += 1;
                    break;
                case BLUE:
                    //set_servo_position(FURROW_SERVO_PORT,FURROW_NEAR_POS);
                    printf("blue detected. not discard? %d", think(BLUE));
                    blue_count += 1;
                    break;
            }

            msleep(1300);
        }
        else
        {
            printf("no pom found");
            //maybe will "go to sleep" and update less often if it detects that it has neither 
            //seen the collection arm move nor has it seen a pom a certain number of times
            //in a row whilst in regular update mode. if the arm moves or it DOES see a pom on  
            //one of the infrequent checks, it will return to a regular update mode
            msleep(1300);
        }
    } 
}
*/
