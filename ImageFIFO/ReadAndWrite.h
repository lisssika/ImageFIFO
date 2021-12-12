#pragma once
#include "ImageFIFO.h"

void reader(ImageFIFO& fifo, size_t n);
void writer(ImageFIFO& image_fifo, size_t n);

