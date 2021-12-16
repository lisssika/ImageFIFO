#pragma once
#include "ImageFIFO.h"
#include <memory>

class ReaderWriter final
{
private:
	std::condition_variable free_blocks_added;
	std::condition_variable ready_blocks_added;
	std::mutex mutex_;
	std::unique_ptr<ImageFIFO> fifo_;

public:
	ReaderWriter(std::unique_ptr<ImageFIFO> fifo);
	int reader(const std::string& file_names);
	int writer(const std::string& file_names);
};



