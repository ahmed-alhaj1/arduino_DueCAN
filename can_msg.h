#pragma once
#if !(__HRI_CAN_MESSAGE_H__)
#define __HRI_CAN_MESSAGE_H__

#include <CAN_Acquisition.h>
class HRI_MESSAGE_RX_CAN0 : public cCANFrame {
	bool CallbackRx(RX_CAN_FRAME *R);
};

class HRI_MESSAGE_RX_CAN1 : public cCANFrame {
	bool CallbackRx(RX_CAN_FRAME *R);
};

#endif 
