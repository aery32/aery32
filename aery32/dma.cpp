#include <avr32/io.h>

class dma {
	public:
		dma() {}
		
		void write() {}
		
		void flush()
		{

		}

	protected:
		void *buffer;
		size_t bufsize;
		size_t datasize;

		avr32_pdca_channel_t channel;
};