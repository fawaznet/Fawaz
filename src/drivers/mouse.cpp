#include <drivers/mouse.h>

using namespace fawaz::common;
using namespace fawaz::drivers;
using namespace fawaz::hardwarecom;

MouseEventHandler::MouseEventHandler(){

}
MouseEventHandler::~MouseEventHandler(){

}
void MouseEventHandler::OnActivate(){

}
void MouseEventHandler::OnMouseDown(uint8_t button){

}
void MouseEventHandler::OnMouseUp(uint8_t button){

}
void MouseEventHandler::OnMouseMove(int x, int y){

}

MouseDriver::MouseDriver(InterruptManager* manager, MouseEventHandler* handler)
:	InterruptHandler(0x2C, manager),
	dataport(0x60),
	commandport(0x64)
{
	this->handler = handler;
}
MouseDriver::~MouseDriver(){

}

void MouseDriver::Activate(){
	
	offset = 0; //maybe 0 maybe not
	buttons = 0;

	

	//this is remove holding keys
	commandport.Write(0xAB); // active mouse interrupt
	commandport.Write(0x20); // give us currunt state, command 0x60 = read controller command byte

	uint8_t status = dataport.Read() | 2;	// second bit to true
	commandport.Write(0x60); // tells PIC or Keyboard to change currunt status
	dataport.Write(status); // after changing write the status, command 0x60 = set controller command byte

	commandport.Write(0xD4);
	dataport.Write(0xF4); // finally activate the mouse
	dataport.Read();
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp){
	
	uint8_t status = commandport.Read();
	if (!(status & 0x20) || handler == 0)
		return esp;


	buffer[offset] = dataport.Read();
	offset = (offset + 1) % 3;

	if (offset == 0)
	{
		if (buffer[1] != 0 || buffer[2] != 0)
		{
			handler->OnMouseMove(buffer[1], -buffer[2]);
		}

		for (uint8_t i = 0; i < 3; i++)
		{
			if ((buttons & (0x1<<i)) != (buttons & (0x1<<i)))
			{
				if (buttons & (0x1<<i))
					handler->OnMouseUp(i+1);
				else
					handler->OnMouseDown(i+1);
			}
		}
		buttons = buffer[0];
	}
	return esp;
}