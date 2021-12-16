#include "ImageFIFO.h"
#include "ReadAndWrite.h"
#include <string>
#include <memory>

int main()
{
	ImageFIFO image_fifo(2548762, 1);
	const std::string inp("input1.BMP");
	const std::string out("output1.BMP");
	ReaderWriter rw (std::make_unique<ImageFIFO>(2548762, 1));
	rw.writer( inp);
	rw.reader( out);
}