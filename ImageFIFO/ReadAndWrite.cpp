#include "ReadAndWrite.h"
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>

std::condition_variable free_blocks_added;
std::condition_variable ready_blocks_added;
std::mutex mutex_;

int writer(ImageFIFO& fifo, const std::string& file_names)
{
	std::stringstream file_names_(file_names);
	std::ifstream file;
	void* data;
	for (std::string file_name; std::getline(file_names_, file_name, ' ');)
	{
		data = fifo.getFree();
		if (!data)
		{
			std::unique_lock<std::mutex> lk(mutex_);
			free_blocks_added.wait(lk);
			data = fifo.getFree();
		}
		file.open(file_name, std::ios::binary);
		if (file.is_open())
		{
			file.read(static_cast<char*>(data), fifo.get_blockSize());
			fifo.addReady(data);
			ready_blocks_added.notify_all();
			file.close();
		}
		else
		{
			throw std::runtime_error("file can't open");
		}
	}
	return 0;
}

int reader(ImageFIFO& fifo, const std::string& file_names)
{
	std::stringstream files(file_names);
	std::ofstream file;
	void* data;
	for (std::string file_name; std::getline(files, file_name, ' ');)
	{
		data = fifo.getReady();
		if (!data)
		{
			std::unique_lock<std::mutex> lk(mutex_);
			ready_blocks_added.wait(lk);
			data = fifo.getReady();
		}

		file.open(file_name, std::ios::binary);
		if (file.is_open())
		{
			file.write(static_cast<char*>(data), fifo.get_blockSize());
			fifo.addFree(data);
			free_blocks_added.notify_all(); // или noify_one?
			file.close();
		}
		else
		{
			throw std::runtime_error("file can't open");
		}
	}
	return 0;
}
