#include"controller_parser.h"

Stick::Stick() {
	mag = -1;
	pos_status = -1;
	neg_status = -1;
	neut_status = -1;

}

int Stick::get_magnitude() {
	return mag;
}
int Stick::get_neg_status() {
	return neg_status;
}
int Stick::get_neut_status() {
	return neut_status;

}
int Stick::get_pos_status() {
	return pos_status;
}
void Stick::set_magnitude(int mag) {
	this->mag = mag;
}
void Stick::set_neg_state(int state) {
	this->neg_status = state;
}
void Stick::set_neut_state(int state) {
	this->neg_status = state;
}

void Stick::set_pos_state(int state) {
	this->pos_status = state;
}

void Stick::set_states(int mag, int neut_state, int neg_state, int pos_state) {
	this->mag = mag;
	this->neut_status = neut_state;
	this->neg_status  = neg_state;
	this->pos_status  = pos_state;
}

int Stick::get_state(){
  if(pos_status != -1) return pos_status;
  if(neg_status != -1) return neg_status;
  if(neut_status != -1) return neut_status;
  return -1;  
}

void Stick::set_elapse_time(unsigned long elapse_time){

  this->elapse_time = elapse_time;
}
unsigned long Stick::get_elapse_time(){
  return elapse_time;
}
Button::Button() {
	up = 0;
	left = 0;
	right = 0;
	down = 0;

}
void Button::set_keys(int up, int left, int right, int down) {
	this->up = up;
	this->left = left;
	this->right = right;
	this->down = down;
}

int Button::get_up(){
  return up;
}

int Button::get_down(){
  return down;
}
int Button::get_left(){
  return left;
}
int Button::get_right(){
  return right;
}
void Button::set_elapse_time(unsigned long elaps_time){

  this->elapse_time = elapse_time; 
}

unsigned long Button::get_elapse_time(){
  return elapse_time;
}
//#########################################

int HeartBeat::get_vsc_mode(){

  return vsc_mode;
}
int HeartBeat::get_auto_mode(){
  return auto_mode;
}
int HeartBeat::get_estop_state(){
  return estop_state;
}

void HeartBeat::set_elapse_time(unsigned long elapse_time){
  this->elapse_time = elapse_time; 
}

unsigned long HeartBeat::get_elapse_time(){
  return elapse_time;
}

//ControllParser::Controll


int make_signed(int16_t magLsb, int16_t magMsb) {
	int value = (magMsb << 2) + ((magLsb >> 6)& 3);
	if ( (((magLsb >> 2) & 3) == 1)){
	  value = value * (-1);
	}
	return value;
}
// return 1 if conrol bit are set , 2 not_set, 3 error, 4u
//   status  : bit = set ,not set, 
//   neutral : bit1=  0  ,   1   ,
//   negative: bit3=  0  ,   1   ,
//   postive : bit5=  0  ,   2
int ControllerParser::status_field_parser(uint8_t state) {
	state = state & 0x03;
	switch (state) {
		case NOTSET: // 0x00
			return 1;
		case SET:    // 0x01
			return 2;
		case ERROR:  // 0x10
			return 3;
		default:    // 0x11 or anything
			return 4;

	}
	//this function should never retrun -1 something wrong in the logic.
	return -1;
}

//input: control_msg is byte5
uint8_t ControllerParser::button_parser(uint8_t byte5, SIDE side  ){
	// up_but status 
	uint8_t up_status_bits = byte5 >> 4;
	int up_state = status_field_parser(up_status_bits);
	// down_but status
	int down_state = status_field_parser(byte5);
	// left_but state
	uint8_t left_status_bits = byte5 >> 6 ;
	int left_state = status_field_parser(left_status_bits);

	//right_but status
	uint8_t right_status_bits = byte5 >> 2;
	int right_state = status_field_parser(right_status_bits);
	switch (side) {

	case left:
		pad_event.butt_set1.set_keys(up_state, left_state, right_state, down_state);
		break;
	case right:
		pad_event.butt_set2.set_keys(up_state, left_state, right_state, down_state);
	}
}

uint8_t ControllerParser::joy_stick_parser(uint8_t byte0, uint8_t byte1, SIDE side, AXIS axis) {
	
	int mag = make_signed(byte0, byte1);
	int neutral = status_field_parser(byte0);
	int negative = status_field_parser(byte0>>2);
	int positive = status_field_parser(byte0>>4); 

	if (side == left && axis == x) {
		pad_event.X1.set_states(mag, neutral, negative, positive);
		
	
	}else if(side == right && axis == x) {
		
		pad_event.X2.set_states(mag, neutral, negative, positive);
		
	}else if (side == left && axis == y) {
		
		pad_event.Y1.set_states(mag, neutral, negative, positive);
		
	}else if (side == right && axis == y) {
		
		pad_event.Y2.set_states(mag, neutral, negative, positive);
		
	}else if (side == left && axis == z) {
		pad_event.Z1.set_states(mag, neutral, negative, positive);
		
	}else if (side == right && axis == z) {
		pad_event.Z2.set_states(mag, neutral, negative, positive);
		
	}
}

uint8_t ControllerParser::heart_beat_parser(uint8_t vscMode, uint8_t auto_mode, uint8_t estop){
	
	uint8_t vmode = vscMode & 0x0F; // extract the 4 last bits;
  int vsc_mode_state;
  int auto_mode_state;
  int estop_state;
  vscMode = (int)vscMode;
  switch (vmode){

    case 0x00:  // error 
      vsc_mode_state = 0;
      break;
    case 0x01:  // Sleep
      vsc_mode_state = 1;
      break;
    case 0x02:  // wakeup
      vsc_mode_state = 2;
      break;
    case  0x03: //Initialize 
      vsc_mode_state = 3;
      break;
    case 0x04: // local
      vsc_mode_state = 4;
      break;
    case 0x05:  // Remote Transition
      vsc_mode_state = 5;
      break;
    case 0x06: // Remote 
      vsc_mode_state = 6;
      break;
    case 0x07: // Vehicle On
      vsc_mode_state = 7;
      break;
    case 0x08: // stationary 
      vsc_mode_state = 8;
      break;
    case 0x09: //Mobile
      vsc_mode_state = 9;
      break;
    case 0x0A: // Menu
      vsc_mode_state = 10;
      break;
    case 0x0B: //Pause
      vsc_mode_state = 11;
      break;
  }
  //int auto_mode_state ;
	switch (auto_mode) {
		case 0x00: // user control
			auto_mode_state = 0;
			break;
		case 0x01: // shared control 
			auto_mode_state = 1;
			break;
		case 0x02: // autonomous 
			auto_mode_state = 2;
			break;
	}
								
	estop_state = (estop == 0x00) ? 0 : 1; // inactive 0 : active 1
   
  // set hearbeat value
  pad_event.HB.vsc_mode = vsc_mode_state;
  pad_event.HB.auto_mode = auto_mode_state;
  pad_event.HB.estop_state = estop_state; 
 
}
// this method is going to be call from 
//HRI_MESSAGE_RX_CAN0::CallbackRx(RX_CAN_FRAME *recieved_msg)
void ControllerParser::msg_parser(HRI_MESSAGE_RX_CAN0 *hri_msg) {
	//Basic
	if (hri_msg->ID == 0x0CFDD633) {
    unsigned long last = millis();
    unsigned long duration = last - msg1_prev_time;
     
    pad_event.X1.set_elapse_time(duration);
    pad_event.Y1.set_elapse_time(duration);
    pad_event.butt_set2.set_elapse_time(duration);
    msg1_prev_time = last;
    
		
		SIDE s = left;
		AXIS axis;
		joy_stick_parser(hri_msg->U.b[0], hri_msg->U.b[1], s, axis = x);
		joy_stick_parser(hri_msg->U.b[2], hri_msg->U.b[3], s, axis = y);
		button_parser(hri_msg->U.b[5], s);
		set_control_array();
    
	//Extended
	}else if(hri_msg->ID == 0x0CFDD733){
    unsigned long last = millis();
    unsigned long duration = last - msg2_prev_time;
    msg2_prev_time = last;
    pad_event.Z1.set_elapse_time(duration);
		// Z left joy stick
		AXIS axis;
		SIDE s = left; 
		joy_stick_parser(hri_msg->U.b[0], hri_msg->U.b[1], s, axis =z);
		set_control_array();
	//Basic
	}else if(hri_msg->ID == 0x0CFDD834){
		unsigned long last = millis();
		unsigned long duration = last - msg3_prev_time;
		msg3_prev_time = last;
		pad_event.X2.set_elapse_time(duration);
		pad_event.Y2.set_elapse_time(duration);
		pad_event.butt_set2.set_elapse_time(duration);
    
		SIDE s = right;
		AXIS axis; 
		// decode RIGHT X joystick
		joy_stick_parser(hri_msg->U.b[0], hri_msg->U.b[1], s, axis = x);
		// decode RIGHT Y joystick
		joy_stick_parser(hri_msg->U.b[2], hri_msg->U.b[3], s, axis = y);
		// decode RIGHT button
		SIDE side = right;
		button_parser(hri_msg->U.b[5],side);
		set_control_array();
 
	//Extended
	}else if(hri_msg->ID == 0x0CFDD934){
		// Z right joystick
    // setting up time
		unsigned long last = millis();
		unsigned long duration = last - msg4_prev_time;
		msg4_prev_time = last;
		pad_event.Z2.set_elapse_time(duration);

    // decode the message
    AXIS axis;
		SIDE s = right;
		joy_stick_parser(hri_msg->U.b[0], hri_msg->U.b[1], s, axis = z);
		set_control_array();
	}
	//Extended 
	else if (hri_msg->ID == 0x0CFDE801) {
    
		unsigned long last = millis();
		unsigned long duration = last - msg5_prev_time;
		pad_event.HB.set_elapse_time(duration);
		msg5_prev_time = last;
		heart_beat_parser(hri_msg->U.b[0], hri_msg->U.b[1], hri_msg->U.b[2]);
		set_control_array();
	}
	
}



void ControllerParser::set_control_array() {
	  int16_t x1 = get_X1_magnitude();
	  int16_t x2 = get_X2_magnitude();
	  int16_t y1 = get_Y1_magnitude();
	  int16_t y2 = get_Y2_magnitude();
	  int16_t z1 = get_Z1_magnitude();
	  int16_t z2 = get_Z2_magnitude();
	  int16_t butt1, butt2;
	  // button_set1 
	  if (!get_butt_set1_up() && !get_butt_set1_down() && !get_butt_set1_left() && !get_butt_set1_right()) butt1 = 0;        // 0

	  else if (!get_butt_set1_up() && !get_butt_set1_down() && !get_butt_set1_left() && !!get_butt_set1_right()) butt1 = 1; // 1-right
	  else if (!get_butt_set1_up() && !get_butt_set1_down() && !!get_butt_set1_left() && !get_butt_set1_right()) butt1 = 2; // 2- left
	  else if (!get_butt_set1_up() && !!get_butt_set1_down() && !get_butt_set1_left() && !get_butt_set1_right()) butt1 = 3; // 3- down
	  else if (!!get_butt_set1_up() && !get_butt_set1_down() && !get_butt_set1_left() && !get_butt_set1_right()) butt1 = 4; // 4- up

	  else if (!get_butt_set1_up()  && !get_butt_set1_down()  && !!get_butt_set1_left() && !!get_butt_set1_right()) butt1 = 5; // 5 right + left 
	  else if (!!get_butt_set1_up() && !!get_butt_set1_down() && !get_butt_set1_left() && !!get_butt_set1_right()) butt1 =  6; // 6 right + down
	  else if (!!get_butt_set1_up() && !get_butt_set1_down()  && !get_butt_set1_left() && !!get_butt_set1_right()) butt1 =  7; // 7 right + up

	  else if (!get_butt_set1_up() && !!get_butt_set1_down() && !!get_butt_set1_left() && !get_butt_set1_right()) butt1 =   8; // 8 left + down 
	  else if (!!get_butt_set1_up() && !get_butt_set1_down() && !!get_butt_set1_left() && !get_butt_set1_right()) butt1 =   9;  // 9 left + up
	  else if (!!get_butt_set1_up() && !!get_butt_set1_down() && !get_butt_set1_left()  && !get_butt_set1_right()) butt1 =   10;	 // 10 down + up

	  else if (!get_butt_set1_up() && !!get_butt_set1_down() && !!get_butt_set1_left()  && !!get_butt_set1_right()) butt1 = 11; // 11 right +left+ down
	  else if (!!get_butt_set1_up() && !get_butt_set1_down() && !!get_butt_set1_left() && !!get_butt_set1_right()) butt1 = 12; // 12 right + left +  up  
	  else if (!!get_butt_set1_up() && !!get_butt_set1_down() && !!get_butt_set1_left() && !get_butt_set1_right()) butt1 =13; // 13 left +down + up 

	  
	  else if (!!get_butt_set1_up() && !!get_butt_set1_down() && !!get_butt_set1_left() && !!get_butt_set1_right()) butt1 =  14; // 14 right left down up
	   
	  
	  
	  
	  //set button_set2 

	  if (!get_butt_set2_up() && !get_butt_set2_down() && !get_butt_set2_left() && !get_butt_set2_right()) butt2 = 0;        // 0

	  else if (!get_butt_set1_up() && !get_butt_set1_down() && !get_butt_set1_left()   && !!get_butt_set1_right()) butt2 = 1; // 1-right
	  else if (!get_butt_set1_up() && !get_butt_set1_down() && !!get_butt_set1_left()   && !get_butt_set1_right()) butt2 = 2; // 2- left
	  else if (!get_butt_set1_up() && !!get_butt_set1_down() && !get_butt_set1_left()   && !get_butt_set1_right()) butt2 = 3; // 3- down
	  else if (!!get_butt_set1_up() && !get_butt_set1_down() && !get_butt_set1_left()   && !get_butt_set1_right()) butt2 = 4; // 4- up

	  else if (!get_butt_set1_up() && !get_butt_set1_down() && !!get_butt_set1_left()   && !!get_butt_set1_right()) butt2 = 5; // 5 right + left 
	  else if (!!get_butt_set1_up() && !!get_butt_set1_down() && !get_butt_set1_left()  && !!get_butt_set1_right()) butt2 = 6; // 6 right + down
	  else if (!!get_butt_set1_up() && !get_butt_set1_down() && !get_butt_set1_left()   && !!get_butt_set1_right()) butt2 = 7; // 7 right + up

	  else if (!get_butt_set1_up() && !!get_butt_set1_down() && !!get_butt_set1_left()  && !get_butt_set1_right()) butt2 = 8;  // 8 left + down 
	  else if (!!get_butt_set1_up() && !get_butt_set1_down() && !!get_butt_set1_left()  && !get_butt_set1_right()) butt2 = 9;  // 9 left + up
	  else if (!!get_butt_set1_up() && !!get_butt_set1_down() && !get_butt_set1_left()   && !get_butt_set1_right()) butt2 = 10; // 10 down + up

	  else if (!get_butt_set1_up() && !!get_butt_set1_down() && !!get_butt_set1_left()  && !!get_butt_set1_right()) butt2 = 11;// 11 right +left+ down
	  else if (!!get_butt_set1_up() && !get_butt_set1_down() && !!get_butt_set1_left() && !!get_butt_set1_right()) butt1 = 12; //  12 rgiht + up + left
	  else if (!!get_butt_set2_up() && !!get_butt_set2_down() && !!get_butt_set2_left() && !get_butt_set2_right()) butt2 = 13; // 13 left +down + up 

	  else if (!!get_butt_set2_up()  && !!get_butt_set2_down() && !!get_butt_set2_left()  && !!get_butt_set2_right()) butt2 = 14; // 14 right left down up
	 
	  control_array[0] = x1;
	  control_array[1] = x2;
	  control_array[2] = y1;
	  control_array[3] = y2;
	  control_array[4] = z1;
	  control_array[5] = z2;
	  control_array[6] = butt1;
	  control_array[7] = butt2;


  }

int ControllerParser::get_X1_magnitude(){
      return pad_event.X1.get_magnitude();
}

int ControllerParser::get_X1_state(){
      return pad_event.X1.get_pos_status();
}

int ControllerParser::get_Y1_magnitude(){
      return pad_event.Y1.get_magnitude();
}

int ControllerParser::get_Y1_state(){
      return pad_event.Y1.get_pos_status();
    
}
 
int ControllerParser::get_Z1_magnitude(){
      return pad_event.Z1.get_magnitude();
}

int ControllerParser::get_Z1_state(){
      return pad_event.Z1.get_pos_status();
}

int ControllerParser::get_X2_magnitude(){
      return pad_event.X2.get_magnitude();
}

int ControllerParser::get_X2_state(){
      return pad_event.X2.get_pos_status();
}

int ControllerParser::get_Y2_magnitude(){
      return pad_event.Y2.get_magnitude();
}

int ControllerParser::get_Y2_state(){
      return pad_event.Y2.get_pos_status();
}

int ControllerParser::get_Z2_magnitude(){
      return pad_event.Z2.get_magnitude();
}

int ControllerParser::get_Z2_state(){
      return pad_event.Z2.get_pos_status();

}

int ControllerParser::get_butt_set1_up(){
      return (pad_event.butt_set1.get_up()==1)? 0 :1;

}

int ControllerParser::get_butt_set1_down(){
      return (pad_event.butt_set1.get_down()==1)?0 : 1;

}

int ControllerParser::get_butt_set1_left(){
      return (pad_event.butt_set1.get_left() == 1) ? 0 : 1;

}

int ControllerParser::get_butt_set1_right(){
      return (pad_event.butt_set1.get_right() ==1 ) ? 0 :1;

}

  

int ControllerParser::get_butt_set2_up(){
      return (pad_event.butt_set2.get_up() == 1)? 0 : 1;
}

int ControllerParser::get_butt_set2_down(){
      return( pad_event.butt_set2.get_down() == 1)? 0 : 1;
}

int ControllerParser::get_butt_set2_right(){
      return (pad_event.butt_set2.get_right()==1) ? 0 : 1;
}

int ControllerParser::get_butt_set2_left(){
      return (pad_event.butt_set2.get_left()== 1)? 0 : 1;
}

int ControllerParser::get_vsc_mode(){
      return pad_event.HB.get_vsc_mode();
}

int ControllerParser::get_auto_mode(){
      return pad_event.HB.get_auto_mode();
}

int ControllerParser::get_estop(){
      return pad_event.HB.get_estop_state();
}
