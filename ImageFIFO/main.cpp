#include <iostream>
#include <thread>
#include <chrono>
#include "ReadAndWrite.h"
#include "ImageFIFO.h"
using namespace std;

void threadFunction()
{
	//
}

int main()
{
	ImageFIFO im_fifo(500, 2);
	reader(im_fifo, 1);
	writer(im_fifo, 1);
	//std::thread thr(threadFunction);
	//thr.join();
	return 0;
}