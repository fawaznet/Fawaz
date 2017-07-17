#ifndef __FAWAZ__DRIVERS__KEYBOARD_H
#define __FAWAZ__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardwarecom/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecom/port.h>

namespace fawaz {
	namespace drivers {
		class KeyboardEventHandler
		{
		public:
			KeyboardEventHandler();
			~KeyboardEventHandler();

			virtual void OnKeyDown(char);
			virtual void OnKeyUp(char);
			
		};

		class KeyboardDriver : public fawaz::hardwarecom::InterruptHandler, public Driver{

			fawaz::hardwarecom::Port8Bit dataport;
			fawaz::hardwarecom::Port8Bit commandport;

			KeyboardEventHandler* handler;
		public:
			KeyboardDriver(fawaz::hardwarecom::InterruptManager* manager, KeyboardEventHandler* handler);
			~KeyboardDriver();
			virtual fawaz::common::uint32_t HandleInterrupt(fawaz::common::uint32_t esp);
			virtual void Activate();
		};
	}
}

#endif