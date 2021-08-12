
//byte conArray[8];


#include<OBD2.h>
#include<DueTimer.h>
#include"can_msg.h"
#include"controller_parser.h"

// these classes were moved into can_msg.h file because I need them for the parser as well.
/*
class HRI_MESSAGE_RX_CAN0 : public cCANFrame {
	bool CallbackRx(Rx_CAN_FRAME *R);
};

class HRI_MESSAGE_RX_CAN1 : public cCAnFrame {
	bool Callback(RX_CAN_FRAME *R);
};
*/

//we are going to gateway 
/********** CAN0 messages *********/


HRI_MESSAGE_RX_CAN0 msg1_CAN0;		//_0x0CFDD633
HRI_MESSAGE_RX_CAN0 msg2_CAN0;		//_0x0CFDD733
HRI_MESSAGE_RX_CAN0 msg3_CAN0;		//_0x0CFDD834
HRI_MESSAGE_RX_CAN0 msg4_CAN0;      //_0x0CFDD934
HRI_MESSAGE_RX_CAN0 msg5_CAN0;      //_0x0CFDE801
HRI_MESSAGE_RX_CAN0 msg6_CAN0;      //_0x0CFDE900
HRI_MESSAGE_RX_CAN0 msg7_CAN0;      //_0x0CFDEA00

									
HRI_MESSAGE_RX_CAN1 msg1_CAN1;      //_0x0CFDD633
HRI_MESSAGE_RX_CAN1 msg2_CAN1;      //_0x0CFDD733
HRI_MESSAGE_RX_CAN1 msg3_CAN1;      //_0x0CFDD834
HRI_MESSAGE_RX_CAN1 msg4_CAN1;      //_0x0CFDD934
HRI_MESSAGE_RX_CAN1 msg5_CAN1;      //_0x0CFDE801
HRI_MESSAGE_RX_CAN1 msg6_CAN1;      //_0x0CFDE900
HRI_MESSAGE_RX_CAN0 msg7_CAN1;      //_0x0CFDEA00


//declare parser

ControllerParser parser;

//create the CANport acqisition schedulers
cAcquireCAN can_port0(CAN_PORT_0);
cAcquireCAN can_port1(CAN_PORT_1);



void setup() {



	Serial.begin(115200);

	Serial.println("System Start\reset ");

	pinMode(13, OUTPUT);

  pinMode(A1, INPUT);
  
	can_port0.initialize(_250K);
	can_port1.initialize(_250K);

	//initialize msg id
	msg1_CAN0.ID = 0x0CFDD633;
	msg2_CAN0.ID = 0x0CFDD733;
	msg3_CAN0.ID = 0x0CFDD834;
	msg4_CAN0.ID = 0x0CFDD934;
	msg5_CAN0.ID = 0x0CFDE801;
	msg6_CAN0.ID = 0x0CFDE900;
	msg7_CAN0.ID = 0x0CFDEA00;

  msg1_CAN1.ID = 0x0CFDD633;
  msg2_CAN1.ID = 0x0CFDD733;
  msg3_CAN1.ID = 0x0CFDD834;
  msg4_CAN1.ID = 0x0CFDD934;
  msg5_CAN1.ID = 0x0CFDE801;
  msg6_CAN1.ID = 0x0CFDE900;
  msg7_CAN1.ID = 0x0CFDEA00;

	
	//add message to rx (set RX filters)

	can_port0.addMessage(&msg1_CAN0, RECEIVE);
	can_port0.addMessage(&msg2_CAN0, RECEIVE);
	can_port0.addMessage(&msg3_CAN0, RECEIVE);
	can_port0.addMessage(&msg4_CAN0, RECEIVE);
	can_port0.addMessage(&msg5_CAN0, RECEIVE);
	can_port0.addMessage(&msg6_CAN0, RECEIVE);
	can_port0.addMessage(&msg7_CAN0, RECEIVE);

  can_port1.addMessage(&msg1_CAN0, RECEIVE);
  can_port1.addMessage(&msg2_CAN0, RECEIVE);
  can_port1.addMessage(&msg3_CAN0, RECEIVE);
  can_port1.addMessage(&msg4_CAN0, RECEIVE);
  can_port1.addMessage(&msg5_CAN0, RECEIVE);
  can_port1.addMessage(&msg6_CAN0, RECEIVE);
  can_port1.addMessage(&msg7_CAN0, RECEIVE);

  Serial.print("vsc_mode, auto_mode, estop,  X1_mag, X1_state, Y1_mag, Y1_state, Z1_mag, X2_, Y2, Z2, butt_set1, butt_set2,   ");
}

//this is our timer interrupt handler, called at XmS interval
void CAN_RxTx()
{
	//receive messages, don't periodically transmit (but you can transmit asynchronously via CANportx.TXmsg(xxx) in our overloaded messages below)	 
	can_port0.run(POLLING_noTx);
	can_port1.run(POLLING_noTx);
	//cam_port1.run(POLLING_noTx);
}

UINT8 i, CAN0_rxCtr, CAN1_rxCtr;
void loop(){

	// handle can port RX in tight loop
	CAN_RxTx();
  //bool passFail =  true;

//very import don't delete the following print statements  
/*
    Serial.print( parser.get_vsc_mode()) ; Serial.print(" , ");  Serial.print( parser.get_auto_mode()); Serial.print(" , "); Serial.print( parser.get_estop());
    Serial.print(" | "); Serial.print( parser.get_X1_magnitude()); Serial.print(" , "); Serial.print( parser.get_X1_state());Serial.print(" , "); 
    Serial.print(parser.get_Y1_magnitude()); Serial.print(" , "); Serial.print( parser.get_Y1_state());
    Serial.print(" , "); Serial.print( parser.get_Z1_magnitude()); Serial.print(" , ");  Serial.print( parser.get_Z1_state());
    Serial.print(" | ");
    Serial.print( parser.get_X2_magnitude()); Serial.print(" , "); Serial.print( parser.get_X2_state());Serial.print(" , "); 
    Serial.print(parser.get_Y2_magnitude()); Serial.print(" , "); Serial.print( parser.get_Y2_state());
    Serial.print(" , "); Serial.print( parser.get_Z2_magnitude()); Serial.print(" , ");Serial.print( parser.get_Z2_state());

    Serial.print(" , ");
    
    Serial.print("{ ") ;Serial.print(parser.get_butt_set1_up()); Serial.print(" , "); Serial.print(parser.get_butt_set1_down()); 
    Serial.print(" , "); Serial.print(parser.get_butt_set1_left()); Serial.print(" , "); Serial.print(parser.get_butt_set1_right());
    Serial.print(" } ");
    
    Serial.print("{ ") ; Serial.print(parser.get_butt_set2_up()); Serial.print(" , "); Serial.print(parser.get_butt_set2_down()); 
    Serial.print(" , ");Serial.print(parser.get_butt_set2_left()); Serial.print(" , "); Serial.print(parser.get_butt_set2_right());
    Serial.println(" } ");

*/
    Serial.print(" | "); Serial.print( parser.control_array[0]); Serial.print(" , "); Serial.print( parser.control_array[1]);Serial.print(" , "); 
    Serial.print(parser.control_array[2]); Serial.print(" , "); Serial.print( parser.control_array[3]);
    Serial.print(" , "); Serial.print( parser.control_array[4]); Serial.print(" , ");  Serial.print( parser.control_array[5]);
    Serial.print(" | ");
    Serial.print( parser.control_array[6]); Serial.print(" , "); Serial.print( parser.control_array[7]); Serial.println(" } ");

    
  //Serial.print("hear beat msg time elapse = ");   Serial.println(parser.pad_event.HB.get_elapse_time()); 
  //Serial.print("msg1 X time elapse     = "); Serial.println(parser.pad_event.X1.get_elapse_time());
  //Serial.print("msg1 Y time elapse     = "); Serial.println(parser.pad_event.Y1.get_elapse_time());
  //Serial.print("msg2 stick X time elapse     = "); Serial.println(parser.pad_event.X2.get_elapse_time());
  //Serial.print("msg2 stick Y time elapse     = "); Serial.println(parser.pad_event.Y2.get_elapse_time());
  //Serial.print("msg1_ex  Z time elapse    = "); Serial.println(parser.pad_event.Z1.get_elapse_time());
  //Serial.print("msg2_ex Z time elapse    = "); Serial.println(parser.pad_event.Z2.get_elapse_time());
}
    
bool HRI_MESSAGE_RX_CAN0::CallbackRx(RX_CAN_FRAME *recieved_msg) {

	// parameter "this" is  the HRI_MESSAGE_CAN0 which is a wrapper class for cCanFrame identical to cGateway
	// implemetation is controller_parser.cpp file
  //HRI_MESSAGE_RX_CAN0 *msg = this;
  bool ret_val = false;
  
  U8 i;
  if(recieved_msg){
    for(i = 0; i < 8 ;i++){
      this->U.b[i] = recieved_msg->data.bytes[i];
      
    }
	  parser.msg_parser(this);
    can_port1.TXmsg(this); 
  }
  return(ret_val);
}
bool HRI_MESSAGE_RX_CAN1::CallbackRx(RX_CAN_FRAME * recieved_msg){
  bool retval = false;
  U8 i;
  if(!!recieved_msg){
    for(i =0; i < 8 ;i++){    
      this->U.b[i] = recieved_msg->data.bytes[i];
    }
    can_port0.TXmsg(this);
  }
  return(retval);
}
