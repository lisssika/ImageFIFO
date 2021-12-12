#pragma once
#include "ImageFIFO.h"

int reader(ImageFIFO& fifo, const std::string& file_names);
int writer(ImageFIFO& image_fifo, const std::string& file_names);

