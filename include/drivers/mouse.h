#ifndef __FAWAZ__DRIVERS__MOUSE_H
#define __FAWAZ__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwarecom/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecom/port.h>

namespace fawaz {
	namespace drivers {
		class MouseEventHandler
		{
			
		public:
			MouseEventHandler();
			~MouseEventHandler();

			virtual void OnActivate();
			virtual void OnMouseDown(fawaz::common::uint8_t button);
			virtual void OnMouseUp(fawaz::common::uint8_t button);
			virtual void OnMouseMove(int x, int y);
			
		};

		class MouseDriver : public fawaz::hardwarecom::InterruptHandler, public Driver {

			fawaz::hardwarecom::Port8Bit dataport;
			fawaz::hardwarecom::Port8Bit commandport;

			fawaz::common::uint8_t buffer[3];
			fawaz::common::uint8_t offset;
			fawaz::common::uint8_t buttons;

			MouseEventHandler* handler;
			
			
		public:
			MouseDriver(fawaz::hardwarecom::InterruptManager* manager, MouseEventHandler* handler);
			~MouseDriver();
			virtual fawaz::common::uint32_t HandleInterrupt(fawaz::common::uint32_t esp);
			virtual void Activate();


			
};


	}
}

#endif