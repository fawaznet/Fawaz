#ifndef __FAWAZ__HARDWARECOM__INTERRUPTS_H
#define __FAWAZ__HARDWARECOM__INTERRUPTS_H

#include <gdt.h>
#include <common/types.h>
#include <hardwarecom/port.h>

namespace fawaz {
	namespace hardwarecom {	
		class InterruptManager;


		class InterruptHandler
		{
		protected:
			fawaz::common::uint8_t interruptNumber;
			InterruptManager* interruptManager; //pointer for the InterruptManager to connect to.

			InterruptHandler(fawaz::common::uint8_t interruptNumber, InterruptManager* interruptManager);
			~InterruptHandler();

		public:
			virtual fawaz::common::uint32_t HandleInterrupt(fawaz::common::uint32_t esp);
			
		};

		class InterruptManager{
		friend class InterruptHandler;
		protected:
			// one Active InterruptManager
			static InterruptManager* ActiveInterruptManager;

			// this is something like Interrupt Descriptor Table but in a higher level.
			InterruptHandler* handlers[256];

			struct GateDescriptor
			{
				fawaz::common::uint16_t handlerAddressLowBits;
				fawaz::common::uint16_t gdt_codeSegmentSelector;
				fawaz::common::uint8_t reserved;
				fawaz::common::uint8_t access;
				fawaz::common::uint16_t handlerAddressHighBits;

			} __attribute__((packed));

			// array with 256 Entries
			static GateDescriptor InterruptDescriptorTable[256];

			// this is will create the table
			struct InterruptDescriptorTablePointer
			{
				fawaz::common::uint16_t size;
				fawaz::common::uint32_t base;	// the address of the tabel
			}__attribute__((packed));

			fawaz::common::uint16_t hardwareInterruptOffset;
			

			static void SetInterruptDescriptorTableEntry(
				fawaz::common::uint8_t interruptnumber,
				fawaz::common::uint16_t codeSegmentSelectorOffset,
				void (*handler)(),
				fawaz::common::uint8_t DescriptorPrivilegeLevel, 
				fawaz::common::uint8_t DescriptorType 
				);

			static fawaz::common::uint32_t HandleInterrupt(fawaz::common::uint8_t interruptNumber, fawaz::common::uint32_t esp);	//esp is stack pointer

			Port8BitSlow picMasterCommand;
			Port8BitSlow picMasterData;
			Port8BitSlow picSlaveCommand;
			Port8BitSlow picSlaveData;

		public:
			InterruptManager(fawaz::common::uint16_t hardwareInterruptOffset, fawaz::GlobalDescriptorTable* gdt);
			~InterruptManager();
			fawaz::common::uint16_t HardwareInterruptOffset();
			void Activate();
			void Deactivate();

			fawaz::common::uint32_t DoHandleInterrupt(fawaz::common::uint8_t interruptNumber, fawaz::common::uint32_t esp);	//esp is stack pointer


			
			static void HandleInterruptRequest0x00(); // Timer Interrupt
			static void HandleInterruptRequest0x01(); // Keyboard Interrupt
			static void HandleInterruptRequest0x02(); // Mouse Interrupt
			static void HandleInterruptRequest0x03(); // Timer Interrupt
			static void HandleInterruptRequest0x04(); // Keyboard Interrupt
			static void HandleInterruptRequest0x05(); // Mouse Interrupt
			static void HandleInterruptRequest0x06(); // Timer Interrupt
			static void HandleInterruptRequest0x07(); // Keyboard Interrupt
			static void HandleInterruptRequest0x08(); // Mouse Interrupt
			static void HandleInterruptRequest0x09(); // Timer Interrupt
			static void HandleInterruptRequest0x0A(); // Keyboard Interrupt
			static void HandleInterruptRequest0x0B(); // Mouse Interrupt
			static void HandleInterruptRequest0x0C(); // Mouse Interrupt
			static void HandleInterruptRequest0x0D(); // Keyboard Interrupt
			static void HandleInterruptRequest0x0E(); // Mouse Interrupt
			static void HandleInterruptRequest0x0F(); // Timer Interrupt
			static void HandleInterruptRequest0x31(); // Keyboard Interrupt
			

			static void InterruptIgnore();

			static void HandleException0x00();
			static void HandleException0x01();
			static void HandleException0x02();
			static void HandleException0x03();
			static void HandleException0x04();
			static void HandleException0x05();
			static void HandleException0x06();
			static void HandleException0x07();
			static void HandleException0x08();
			static void HandleException0x09();
			static void HandleException0x0A();
			static void HandleException0x0B();
			static void HandleException0x0C();
			static void HandleException0x0D();
			static void HandleException0x0E();
			static void HandleException0x0F();
			static void HandleException0x10();
			static void HandleException0x11();
			static void HandleException0x12();
			static void HandleException0x13();

		};
	}
}

#endif