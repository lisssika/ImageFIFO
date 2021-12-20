#include "ReadAndWrite.h"
#include <fstream>
#include <future>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>

ReaderWriter::ReaderWriter(
	std::unique_ptr<ImageFIFO> fifo, 
	std::vector<std::string> const& file_names_inp,
	std::vector<std::string> const& file_names_out
) :
	fifo_(std::move(fifo)),
	file_names_inp_(file_names_inp),
	file_names_out_(file_names_out)
{
	if(file_names_out.size()!=file_names_inp.size())
	{
		throw std::runtime_error("count of input and output files isn't equal");
	}
}

std::pair<int, int> ReaderWriter::exec(bool async)
{
	std::future<int> threadWriter, threadReader;
	auto policy = std::launch::async;
	if (!async)
	{
		if (fifo_->get_blockCount() < file_names_inp_.size())
		{
			throw std::runtime_error(
				"Sync mode requires the count of blocks to be greater or equal the number of input files");
		}
		policy = std::launch::deferred;
	}
	threadWriter = std::async(policy, &ReaderWriter::writer, this);
	threadReader = std::async(policy, &ReaderWriter::reader, this);
	return { threadWriter.get() , threadReader.get()};
}


int ReaderWriter::writer()
{
	for (auto& file_name : file_names_inp_)
	{
		void*  data = fifo_->getFree();
		if (!data)
		{
			std::unique_lock<std::mutex> lk(mutex_);
			free_blocks_added.wait(lk);
			data = fifo_->getFree();
		}
		std::ifstream file(file_name, std::ios::binary);
		if (file)
		{
			file.read(static_cast<char*>(data), fifo_->get_blockSize());
			fifo_->addReady(data);
			ready_blocks_added.notify_one();
		}
		else
		{
			throw std::runtime_error("file can't open");
		}
	}
	return 0;
}


int ReaderWriter::reader()
{
	for(auto& file_name:file_names_out_)
	{
		void* data = fifo_->getReady();
		if (!data)
		{
			std::unique_lock<std::mutex> lk(mutex_);
			ready_blocks_added.wait(lk);
			data = fifo_->getReady();
		}
		std::ofstream file(file_name, std::ios::binary);
		if (file)
		{
			file.write(static_cast<char*>(data), fifo_->get_blockSize());
			fifo_->addFree(data);
			free_blocks_added.notify_one();
		}
		else
		{
			throw std::runtime_error("file can't open");
		}
	}
	return 0;
}
