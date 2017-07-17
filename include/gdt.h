#ifndef __FAWAZ__GDT_H
#define __FAWAZ__GDT_H

#include <common/types.h>

namespace fawaz {
    
	class GlobalDescriptorTable{

	public:
		class SegmentDescriptor{
			
		private:
			fawaz::common::uint16_t limit_lo;
			fawaz::common::uint16_t base_lo;
			fawaz::common::uint8_t base_hi;
			fawaz::common::uint8_t type;
			fawaz::common::uint8_t flags_limit_hi;
			fawaz::common::uint8_t base_vhi;
		public:
			SegmentDescriptor(fawaz::common::uint32_t base, fawaz::common::uint32_t limt, fawaz::common::uint8_t type);
			fawaz::common::uint32_t Base();
			fawaz::common::uint32_t Limit();

		} __attribute__((packed));


	private:
		SegmentDescriptor nullSegmentSelector;
		SegmentDescriptor unusedSegmentSelector;
		SegmentDescriptor codeSegmentSelector;
		SegmentDescriptor dataSegmentSelector;

	public:
		GlobalDescriptorTable();
		~GlobalDescriptorTable();

		fawaz::common::uint16_t CodeSegmentSelector();
		fawaz::common::uint16_t DataSegmentSelector();
};

}
#endif