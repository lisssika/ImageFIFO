#pragma once
#include "ImageFIFO.h"

void reader(ImageFIFO& fifo, const std::string& file_names);
void writer(ImageFIFO& image_fifo, const std::string& file_names);

