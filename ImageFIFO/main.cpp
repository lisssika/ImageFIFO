#include "ImageFIFO.h"
#include "ReadAndWrite.h"
#include <string>
int main()
{
	ImageFIFO image_fifo(2548762, 1);
	const std::string inp("input1.BMP");
	const std::string outp("output1.BMP");
	writer(image_fifo, inp);
	reader(image_fifo, outp);
}