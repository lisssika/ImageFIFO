#include "ImageFIFO.h"
#include <mutex>
#include <vector>

void* ImageFIFO::get_ptr(bool flag)
{
	eq_size_data_flags(true);//  вот вроде странно каждый раз проверять, а вроде если не проверить, то можно выйти за границы вектора
	std::lock_guard<std::mutex>guard(m_FifoMutex);
	for (size_t i = 0; i < flags.size(); i++)
	{
		if (flags[i] == flag)
		{
			return m_Data[i];
		}
	}
	return nullptr;
}

bool ImageFIFO::eq_size_data_flags(bool to_throw) const
{
	if (m_Data.size() != flags.size())
	{
		if (to_throw)
			throw std::runtime_error("size of data vector is not equal size of data flags vector!");
		return false;
	}
	return true;
}


ImageFIFO::ImageFIFO(size_t blockSize, size_t blockCount) : blockSize_(blockSize)
{
	std::lock_guard<std::mutex> guard(m_FifoMutex);
	for (size_t i = 0; i < blockCount; ++i)
	{
		m_Data.push_back(std::malloc(blockSize));
	}
}

ImageFIFO::~ImageFIFO()
{
	for (auto& free_data : m_Data)
	{
		free(free_data);
	}
}

void* ImageFIFO::getFree() // writer
{
	return get_ptr(free_flag);
}

void ImageFIFO::addReady(void* data)//writer
{
	eq_size_data_flags(true); //  вот вроде странно каждый раз проверять, а вроде если не проверить, то можно выйти за границы вектора
	std::lock_guard<std::mutex> guard(m_FifoMutex);
	for (size_t i = 0; i < flags.size(); i++)
	{
		if (flags[i] == free_flag)
		{
			m_Data[i] = data;
			return;
		}
	}
	throw std::runtime_error("no free blocks!");
}

void* ImageFIFO::getReady()//reader
{
	return get_ptr(ready_flag);
}

void ImageFIFO::addFree(void* data)//writer
{
	int i = 0;
	for (const auto& block : m_Data)
	{
		i++;
		if (block == data)
		{
			flags[i] = free_flag;
			return;
		}
	}
	throw std::runtime_error("no block with this data!");
}

size_t ImageFIFO::get_blockSize() const
{
	return blockSize_;
}
