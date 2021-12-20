#include "ImageFIFO.h"
#include <mutex>
#include <vector>

void* ImageFIFO::get_ptr(bool flag)
{
	char* ptr = m_Data.get();
	std::lock_guard<std::mutex>guard(m_FifoMutex);
	for (size_t i = 0; i < flags.size(); i++)
	{
		if (flags[i] == flag)
		{
			return m_Data.get() + i * blockSize_;
		}
		ptr += blockSize_;
	}
	return nullptr;
}

void ImageFIFO::add(void* data, bool flag)
{
	if (static_cast<char*>(data) -m_Data.get() < 0)
	{
		throw std::runtime_error("out of bounds)");
	}
	const unsigned int n = (static_cast<char*>(data) - m_Data.get()) / blockSize_;
	if (n > blockCount_)
	{
		throw std::runtime_error("out of bounds)");
	}
	std::lock_guard<std::mutex>guard(m_FifoMutex);
	flags[n] = flag;
}


ImageFIFO::ImageFIFO(size_t blockSize, size_t blockCount) : blockSize_(blockSize), blockCount_(blockCount)
{
	std::lock_guard<std::mutex> guard(m_FifoMutex);
	m_Data.reset(new char [blockSize * blockCount]);
	for (size_t i = 0; i<blockCount; i++)
	{
		flags.push_back(free_flag);
	}
}

void* ImageFIFO::getFree() // writer
{
	return get_ptr(free_flag);
}

void ImageFIFO::addReady(void* data)//writer
{
	add(data, ready_flag);
}

void* ImageFIFO::getReady()//reader
{
	return get_ptr(ready_flag);
}

void ImageFIFO::addFree(void* data)//writer
{
	add(data, free_flag);
}

size_t ImageFIFO::get_blockSize() const
{
	return blockSize_;
}

size_t ImageFIFO::get_blockCount() const
{
	return  blockCount_;
}
