#pragma once
#if !(__Controller_Parser_H__)
#define __Controller_Parser_H__

//#include<CAN_Acquision.h>
#include "can_msg.h"
//#include<ctime>
//#include<time.h>
//#include<TimeLib.h>



const uint8_t NOTSET = 0x00;
const uint8_t SET    = 0x00000001;
const uint8_t ERROR   = 0x00000010;


enum SIDE {left =1, right =2};
enum State {not_set=1, set=2 , error=2, unavailable=3};
enum AXIS {x=1, y=2, z=3};

struct HeartBeat{
  
  int vsc_mode;
  int auto_mode;
  int estop_state;
  unsigned long elapse_time;
  
  void set_elapse_time(unsigned long elapse_time);
  unsigned long get_elapse_time();
  int get_vsc_mode();
  int get_auto_mode();
  int get_estop_state();
  
};



struct Button {
  Button();
  unsigned long elapse_time;
  int up, left, right, down;
  void set_keys(int up, int left, int right, int down);
 // int 1939j ouotput in the documentation it shows that only one button active at a time.
  //int get_last_clicked();
  int get_up();
  int get_down();
  int get_left();
  int get_right();
  void set_elapse_time(unsigned long elapse_time);
  unsigned long get_elapse_time(); 
};


struct Stick{
  Stick();
  unsigned long elapse_time;
  int mag , neut_status ,neg_status , pos_status;
  int get_magnitude();
  int get_neg_status();
  int get_pos_status();
  int get_neut_status();
  void set_magnitude(int mag);
  void set_neg_state(int state);
  void set_pos_state(int state);
  void set_neut_state(int state);
  void set_states(int mag, int neut_state, int neg_state, int pos_state);
  int get_state();
  void set_elapse_time(unsigned long int elapse_time);
  unsigned long get_elapse_time();
 
};

struct GamePadEventData {

	Stick X1, Y1, X2, Y2, Z1, Z2;
	Button butt_set1, butt_set2;
    HeartBeat HB;
};
 
//the is the actual parser;
class ControllerParser{
	
  public:

	int status_field_parser(uint8_t state  ); //size should be 4 according to python script j1939
	uint8_t button_parser(uint8_t byte5, SIDE side );
	uint8_t joy_stick_parser(uint8_t byte0 , uint8_t byte1, SIDE side, AXIS axis);
	uint8_t heart_beat_parser(uint8_t vscMode, uint8_t auto_mode, uint8_t estop);
	void msg_parser(HRI_MESSAGE_RX_CAN0 *hri_msg);
	int16_t control_array[8];

  int get_X1_magnitude();
  int get_X1_state();
  int get_Y1_magnitude();
  int get_Y1_state();
  int get_Z1_magnitude();
  int get_Z1_state();
  int get_X2_magnitude();
  int get_X2_state();
  int get_Y2_magnitude();
  int get_Y2_state();
  int get_Z2_magnitude();
  int get_Z2_state();
  int get_butt_set1_up();
  int get_butt_set1_down();
  int get_butt_set1_left();
  int get_butt_set1_right();
  int get_butt_set2_up();
  int get_butt_set2_down();
  int get_butt_set2_right();
  int get_butt_set2_left();
  int get_vsc_mode();
  int get_auto_mode();
  int get_estop();
  void set_control_array();
  
	GamePadEventData pad_event;
  //time_t hb_msg_time;
  unsigned long msg5_prev_time;
  unsigned long msg4_prev_time;
  unsigned long msg3_prev_time;
  unsigned long msg2_prev_time;
  unsigned long msg1_prev_time;
 
};

#endif
