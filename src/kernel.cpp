
#include <common/types.h>
#include <gdt.h>
#include <hardwarecom/interrupts.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

using namespace fawaz;
using namespace fawaz::common;
using namespace fawaz::hardwarecom;
using namespace fawaz::drivers;

void printf(char* str){
	static uint16_t* VideoMemory = (uint16_t*) 0xb8000;

	static uint8_t x = 0, y = 0; //the curser

	for (int i = 0; str[i] != '\0'; ++i){

		switch(str[i]){
			case '\n':
				y++;
				x=0;
				break;
			default:
				VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
				x++;
				break;
		}
		if (x >= 80)
		{
			y++;
			x=0;
		}

		if (y >= 25)
		{
			for(y=0; y < 25; y++)
				for(x=0; x < 80; x++)
					VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';

			x=0;
			y=0;
		}
	}
}

void printfHex(uint8_t key){
	char* foo = "00";
	char* hex = "0123456789ABCDEF";

	foo[0] = hex[(key >> 4) & 0xF];
	foo[1] = hex[key & 0xF];
	printf(foo);
}

class PrintKeyboardEventHandler : public KeyboardEventHandler {
public:
	void OnKeyDown(char c){
		char* foo = " ";
		foo[0] = c;
		printf(foo);
	}
};


class MouseToConsole : public MouseEventHandler{

	int8_t x, y;
public:

	MouseToConsole(){
		uint16_t* VideoMemory = (uint16_t*)0xb8000;
		x = 40;
		y = 12;
		VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xF000) >> 4
							| (VideoMemory[80*y+x] & 0x0F00) << 4
							| (VideoMemory[80*y+x] & 0x00FF);
	}


	void OnMouseDown(int xoffset, int yoffset){
		static uint16_t* VideoMemory = (uint16_t*)0xb8000;

			VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xF000) >> 4
								| (VideoMemory[80*y+x] & 0x0F00) << 4
								| (VideoMemory[80*y+x] & 0x00FF);

			x += xoffset;
			if(x < 0) x = 0;
			if(x >= 80) x = 79;

			y += yoffset;
			if(y < 0) y = 0;
			if(y >= 25)y = 24;

			VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xF000) >> 4
								| (VideoMemory[80*y+x] & 0x0F00) << 4
								| (VideoMemory[80*y+x] & 0x00FF);
	}
};




typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors(){
	for (constructor* i = &start_ctors; i != &end_ctors; i++)
	{
		(*i)();
	}
}


extern "C" void kernelMain(void* multiboot_structure, uint32_t magicNumber){
	printf("Hello World, this is Jabal Operating System (JabalOS).\n");
	//printf("this is my Operating System (FawazOS).");

	GlobalDescriptorTable gdt;
	InterruptManager interrupts(0x20, &gdt); //&gdt pointer to gdt

	printf("Initializing Hardware, Stage 1\n");
	DriverManager drvManager;

		PrintKeyboardEventHandler kbhandler;
		KeyboardDriver keyboard(&interrupts, &kbhandler); //&interrupt pointer to interrupt manager
		drvManager.AddDriver(&keyboard);

		MouseToConsole mousehandler;
		MouseDriver mouse(&interrupts, &mousehandler); //&interrupt pointer to interrupt manager
		drvManager.AddDriver(&mouse);

		printf("Initializing Hardware, Stage 2\n");
		drvManager.ActivateAll();

	printf("Initializing Hardware, Stage 3\n");
	printf("===============================================\n\n");
	interrupts.Activate();

	while(1);
}