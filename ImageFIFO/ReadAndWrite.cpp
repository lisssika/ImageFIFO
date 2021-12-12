#include "ReadAndWrite.h"
#include <fstream>
#include <string>

void writer(ImageFIFO& fifo, size_t n)
{
	std::ifstream file;
	size_t i = 0;
	void* data;
	while (i < n)
	{
		i++;
		data = fifo.getFree();
		if (data)
		{
			file.open("input" + std::to_string(i), std::ios::binary);
			if (file.is_open())
			{
				file.read(static_cast<char*>(data), fifo.get_blockSize());
				fifo.addReady(data);
				ready_blocks_added.notify_all();
			}
			throw std::runtime_error("file can't open");
		}
		else
		{
			std::unique_lock<std::mutex> lk(mutex_);
			free_blocks_added.wait(lk);
		}
	}
}

void reader(ImageFIFO& fifo, size_t n)
{
	std::ofstream file;
	size_t i = 0;
	void* data;
	while (i < n)
	{
		i++;
		data = fifo.getReady();
		if (data)
		{
			file.open("output" + std::to_string(i), std::ios::binary);
			if (file.is_open())
			{
				file.write(static_cast<char*>(data), fifo.get_blockSize());
				fifo.addFree(data);
				free_blocks_added.notify_all(); // noify_one???
			}
			else
			{
				throw std::runtime_error("file can't open");
			}
		}
		else
		{
			std::unique_lock<std::mutex> lk(mutex_);
			ready_blocks_added.wait(lk);
		}
	}
}
