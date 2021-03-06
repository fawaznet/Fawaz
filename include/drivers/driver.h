#ifndef __FAWAZ__DRIVERS__DRIVER_H
#define __FAWAZ__DRIVERS__DRIVER_H

namespace fawaz {
	namespace drivers {

		class Driver
		{
		public:
			Driver();
			~Driver();

			virtual void Activate();
			virtual int Reset();
			virtual void Deactivate();
			
		};

		class DriverManager
		{
		private:
			Driver* drivers[255];
			int numDrivers;

		public:
			DriverManager();
			~DriverManager();

			void AddDriver(Driver* drv);
			void ActivateAll();
			
};
	}
}
#endif