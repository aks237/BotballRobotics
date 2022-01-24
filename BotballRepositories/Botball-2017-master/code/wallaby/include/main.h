#ifndef MAIN_H
#define MAIN_H

#define BALE_PORT 0

#define BALE_UP 650
#define BALE_DOWN 1649

//void wallaby_path();
void servo_slow(int start, int servo, int end, int time);
void pick_up_bale();

#endif