#include <kipr/botball.h>
#include "createDrive.h"

#define GREEN 1
#define RED 0

//Menu constants
#define GOLD_TOUCH 4
#define BLACK_TOUCH 1
#define LIGHT_PORT 2

//Servo constants
#define SERV_ARM 3
#define ARM_DOWN 1450
#define ARM_UP 0
#define ARM_HIGHER 500
#define ARM_MIDDLE 800
#define ARM_FLOAT 1180

#define SERV_CLAW 1
#define CLAW_CLOSED 915
#define CLAW_OPEN 1601
#define CLAW_SUPER_OPEN 1750

#define SERV_SHOULDER 2
#define SHOULDER_DOWN 2000
#define SHOULDER_UP 0

#define DEFAULT_SPEED 200

#define REGULAR 0
#define DEALT 1


void servo_slow(int initial, int final,int port)
{
  int i;
  if(initial < final)
  {
    for(i = initial; i < final; i++)
    {
      set_servo_position(port,i);
      msleep(1);
    }
  }
  else
  {
    for(i = initial; i > final; i--)
    {
      set_servo_position(port,i);
      msleep(1);
    }
  }
}

int is_green(int threshold)
{
  int i = 50;
  while (i>0)
  {
    camera_update();
    if(get_object_area(GREEN,0)>threshold)
    {
      printf("green");
      return 1;
    }
    if(get_object_area(RED,0)>threshold)
    {
      printf("red");
      return 0;
    }
    i--;
  }
  return 0;
}

int light_start()
{
  int state = -1;
  int min = 9999;
  int max = 0;
  int curr = 0;
  int avg = 9999;
  while(digital(GOLD_TOUCH)==0&&digital(BLACK_TOUCH)==0)
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
    if(digital(GOLD_TOUCH)==1)
    {
      printf("ARMED FOR DOUBLE ELIM ALT\n");
      state = DEALT;
    }
    else if(digital(BLACK_TOUCH==1))
    {
      printf("ARMED\n");
      state = REGULAR;
    }
  }
  while(analog(LIGHT_PORT) > avg)
  {
    msleep(1);
  }
  return state;
}
void arm_float()
{
  set_servo_position(SERV_ARM,ARM_FLOAT);
}
void arm_down()
{
  set_servo_position(SERV_ARM,ARM_DOWN);
}
void arm_middle()
{
  set_servo_position(SERV_ARM,ARM_MIDDLE);
}
void arm_up()
{
  servo_slow(ARM_DOWN,ARM_UP,SERV_ARM);
  set_servo_position(SERV_CLAW,CLAW_CLOSED+200);
  msleep(1000);

}
void claw_open()
{
  set_servo_position(SERV_CLAW,CLAW_OPEN);
}
void claw_close()
{
  servo_slow(CLAW_OPEN,CLAW_CLOSED,SERV_CLAW);
}
void claw_close_fast()
{
  set_servo_position(SERV_CLAW,CLAW_CLOSED);
}
void claw_super_open()
{
  set_servo_position(SERV_CLAW,CLAW_SUPER_OPEN);
}
void init_pos()
{
  set_servo_position(SERV_SHOULDER,SHOULDER_UP);
  set_servo_position(SERV_ARM,470);
  claw_super_open();
}
void grabber_down()
{
  set_servo_position(SERV_SHOULDER,SHOULDER_DOWN);
  msleep(500);
}

void grabber_up()
{
  set_servo_position(SERV_SHOULDER,SHOULDER_UP);
  msleep(500);
}
int pick(int seengreen)
{
  if(seengreen==1)
  {
    claw_open();
    msleep(500);
    set_servo_position(SERV_CLAW,473);
    msleep(500);
    servo_slow(473,CLAW_OPEN,SERV_CLAW);
    create_backward(7,DEFAULT_SPEED/5);
    servo_slow(CLAW_OPEN,CLAW_CLOSED,SERV_CLAW);
    msleep(750);
    seengreen++;
    servo_slow(ARM_DOWN,ARM_MIDDLE,SERV_ARM);
    msleep(500);
    create_forward(7,DEFAULT_SPEED/5);
    return 1;
  }
  return 0;
}
int main()
{
  camera_open();
  int seengreen = 0;
  int state;
  create_connect();
  create_safe();
  //set servos to initial positions now
  init_pos();
  set_servo_position(0,114);
  arm_up();
  grabber_up();
  enable_servos();
  state = REGULAR;
  shut_down_in(119.9);
  printf("started");
  create_backward(30,DEFAULT_SPEED/2);

  create_forward(24,DEFAULT_SPEED/2);
  set_servo_position(0,860);
  msleep(500);
  mav(0,1000);
  msleep(3000);
  ao();
    arm_down();
  msleep(500);
  claw_open();
  msleep(500);
  create_forward(10,DEFAULT_SPEED);
  claw_close();
  msleep(500);
  arm_middle();
  msleep(1000);

  //pile 1
  create_forward(30,DEFAULT_SPEED);
  if(state == DEALT)
  {
    claw_close();
    msleep(500);
    arm_middle();
    create_line_follow(21,DEFAULT_SPEED);
  }
  create_line_follow(35,DEFAULT_SPEED);
  if(is_green(250)==1)
    seengreen++;
  if(seengreen==0)
  {
    create_forward(5,DEFAULT_SPEED/2);
    claw_open();
    msleep(500);
    claw_close();
    msleep(500);
    create_backward(5,DEFAULT_SPEED/2);
  }
  create_right(95,DEFAULT_SPEED);
  create_backward(20,DEFAULT_SPEED/2);
  create_forward(10,DEFAULT_SPEED/2);
  create_left(27,DEFAULT_SPEED/2);
  if(seengreen==1)
  {
    create_backward(5,DEFAULT_SPEED/2);
    claw_open();
    msleep(500);
    claw_close();
    msleep(500);
    create_forward(5,DEFAULT_SPEED/2);
  }
  create_left(78,DEFAULT_SPEED/2);
  create_backward(3,DEFAULT_SPEED/5);
  servo_slow(ARM_MIDDLE,ARM_DOWN,SERV_ARM);
  claw_super_open();
  msleep(1000);
  create_line_follow(13,DEFAULT_SPEED/5);
  if(seengreen==0)
  {
    create_forward(5,DEFAULT_SPEED/5);
    claw_close();
    msleep(500);
    create_backward(5,DEFAULT_SPEED/5);
  }
  else
    claw_close();
  msleep(750);
  seengreen += pick(seengreen);
  arm_middle();
  msleep(500);
  create_line_follow(55,DEFAULT_SPEED);
  create_forward(5,DEFAULT_SPEED);
  mav(0,-1000);
  msleep(3000);
  set_servo_position(0,114);
  msleep(500);
  mav(0,1000);
  msleep(1000);
  create_forward(18,DEFAULT_SPEED);
  create_backward(15,DEFAULT_SPEED);
  create_right(90,DEFAULT_SPEED);
  create_backward(15,DEFAULT_SPEED/2);
  create_forward(6,DEFAULT_SPEED);
  create_right(93,DEFAULT_SPEED/2);
  ao();

  //bin
  grabber_down();
  msleep(1000);
  create_backward(16,DEFAULT_SPEED/2);
  create_left(2,DEFAULT_SPEED/2);
  create_backward(16,DEFAULT_SPEED/3);
  grabber_up();
  create_forward(10,DEFAULT_SPEED/2);
  create_right(10,DEFAULT_SPEED/2);
  create_backward(40,DEFAULT_SPEED/2);
  create_forward(35,DEFAULT_SPEED);
  create_line_follow(43,DEFAULT_SPEED);
  servo_slow(ARM_MIDDLE,ARM_DOWN,SERV_ARM);
  servo_slow(ARM_DOWN,ARM_UP,SERV_ARM);
  set_servo_position(SERV_CLAW,CLAW_CLOSED+200);
  msleep(1000);
  create_left(90,DEFAULT_SPEED);

  //pile 2
  claw_super_open();
  msleep(500);
  create_backward(18,DEFAULT_SPEED);
  create_forward(2,DEFAULT_SPEED);
  create_left(50,DEFAULT_SPEED);
  servo_slow(ARM_UP,ARM_DOWN,SERV_ARM);
  create_forward(2,DEFAULT_SPEED/2);
  claw_close();
  msleep(500);
  create_forward(5,DEFAULT_SPEED/2);
  create_right(50,DEFAULT_SPEED);
  create_backward(10,DEFAULT_SPEED);
  create_backward(5,DEFAULT_SPEED/5);
  arm_up();

  //pile 3
  create_forward(28,DEFAULT_SPEED);
  create_right(90,DEFAULT_SPEED);
  claw_open();
  create_backward(12,DEFAULT_SPEED);
  if(is_green(250)==1)
    seengreen++;
  servo_slow(ARM_UP,ARM_FLOAT,SERV_ARM);
  arm_down();
  msleep(250);
  claw_close();
  msleep(500);
  printf("%d",seengreen);
  seengreen += pick(seengreen);
  if(seengreen==2)
    servo_slow(ARM_MIDDLE,ARM_DOWN,SERV_ARM);
  servo_slow(ARM_DOWN,ARM_UP,SERV_ARM);
  set_servo_position(SERV_CLAW,CLAW_CLOSED+200);
  msleep(1000);

  //last pile
  msleep(500);
  create_left(90,DEFAULT_SPEED);
  create_backward(25,DEFAULT_SPEED);
  create_backward(10,DEFAULT_SPEED/2);
  create_forward(10,DEFAULT_SPEED);
  create_left(45,DEFAULT_SPEED/2);
  arm_middle();
  msleep(500);
  arm_down();
  claw_open();
  msleep(1000);
  create_forward(30,DEFAULT_SPEED);
  if(seengreen==0)
    seengreen++;
  create_forward(4,DEFAULT_SPEED/2);
  create_stop();
  claw_close();
  msleep(500);
  printf("%d",seengreen);
  seengreen+=pick(seengreen);
  arm_middle();
  msleep(500);
  create_backward(40,DEFAULT_SPEED);
  create_right(45,DEFAULT_SPEED/2);
  create_backward(20,DEFAULT_SPEED/2);
  servo_slow(ARM_MIDDLE,ARM_DOWN,SERV_ARM);
  servo_slow(ARM_DOWN,ARM_UP,SERV_ARM);
  set_servo_position(SERV_CLAW,CLAW_CLOSED+200);
  msleep(1000);
  //end
  create_forward(8,DEFAULT_SPEED/2);
  create_right(95,DEFAULT_SPEED);
  create_line_follow(85,DEFAULT_SPEED+50);
  create_left(200,DEFAULT_SPEED);
  servo_slow(SHOULDER_UP,SHOULDER_DOWN,SERV_SHOULDER);
  msleep(5000);
  disable_servos();
  create_disconnect();
  return 0;
}