#pragma once
#include "ImageFIFO.h"
std::condition_variable free_blocks_added;
std::condition_variable ready_blocks_added;
std::mutex mutex_;
void reader(ImageFIFO& fifo, size_t n);
void writer(ImageFIFO& image_fifo, size_t n);

