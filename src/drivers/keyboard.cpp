#include <drivers/keyboard.h>

using namespace fawaz::common;
using namespace fawaz::drivers;
using namespace fawaz::hardwarecom;

KeyboardEventHandler::KeyboardEventHandler(){

}
KeyboardEventHandler::~KeyboardEventHandler(){

}
void KeyboardEventHandler::OnKeyDown(char){

}
void KeyboardEventHandler::OnKeyUp(char){

}


KeyboardDriver::KeyboardDriver(InterruptManager* manager, KeyboardEventHandler* handler)
:	InterruptHandler(0x21, manager),
	dataport(0x60),
	commandport(0x64)
{
	this->handler = handler;
}
KeyboardDriver::~KeyboardDriver(){

}

void printf(char* str);
void printfHex(uint8_t key);

void KeyboardDriver::Activate(){

	//this is remove holding keys
	while(commandport.Read() & 0x1)
		dataport.Read();
	commandport.Write(0xAE); // active interrupt
	commandport.Write(0x20); // give us currunt state

	uint8_t status = (dataport.Read() | 1) & ~0x10;	// read that, then we set right bit new state and clear fifth bit (~0x10)
	commandport.Write(0x60); // tells PIC or Keyboard to change currunt status
	dataport.Write(status); // after changing write the status

	dataport.Write(0xF4); // finally activate the keyboard
}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp){

	uint8_t key = dataport.Read();

	if (handler == 0)
		return esp;

	if(key < 0x80){
		switch(key){
			case 0xFA: break;
			case 0x2A: break; // SHIFT
			case 0x0E: break; // BACKSPACE
			case 0x39: printf(" "); break; // space
			case 0x1C: printf("\n"); break;
			case 0x21: handler->OnKeyDown('f'); break;
			case 0x1E: handler->OnKeyDown('a'); break;
			case 0x11: handler->OnKeyDown('w'); break;
			case 0x2C: handler->OnKeyDown('z'); break;
			case 0x45: case 0xC5: break;
			default:
				printf("KEYBOARD 0x");
				printfHex(key);
				break;
		}
	}
	return esp;
}