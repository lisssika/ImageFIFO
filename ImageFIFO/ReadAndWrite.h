#pragma once
#include "ImageFIFO.h"
#include <memory>
#include <string>
#include <vector>

class ReaderWriter final
{
private:
	std::condition_variable free_blocks_added;
	std::condition_variable ready_blocks_added;
	std::mutex mutex_;
	std::unique_ptr<ImageFIFO> fifo_;
	std::vector<std::string> file_names_inp_;
	std::vector<std::string> file_names_out_;

	int reader();
	int writer();
public:
	ReaderWriter(
		std::unique_ptr<ImageFIFO> fifo, 
		std::vector<std::string> const& file_names_inp,
		std::vector<std::string> const& file_names_out
	);
	std::pair<int, int> exec(bool async);
};



