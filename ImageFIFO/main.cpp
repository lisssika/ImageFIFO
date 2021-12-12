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
	ImageFIFO im_fifo(2548762, 2);
	writer(im_fifo, 1);
	reader(im_fifo, 1);
	
	//std::thread thr(threadFunction);
	//thr.join();
	return 0;
}