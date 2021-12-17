#include "ImageFIFO.h"
#include <mutex>
#include <vector>
#include <new>


void* ImageFIFO::get_ptr(bool flag)
{
	char* ptr = static_cast<char*>( m_Data);
	std::lock_guard<std::mutex>guard(m_FifoMutex);
	for (size_t i = 0; i < flags.size(); i++)
	{
		if (flags[i] == flag)
		{
			return static_cast<void*>(ptr);
		}
		ptr += blockSize_;
	}
	return nullptr;
}

void ImageFIFO::add(void* data, bool flag)
{
	if (static_cast<char*>(data) - static_cast<char*>(m_Data) < 0)
	{
		throw std::runtime_error("out of bounds)");
	}
	const unsigned int n = (static_cast<char*>(data) - static_cast<char*>(m_Data)) / blockSize_;
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
	m_Data = ::operator new(blockSize*blockCount);
	for (size_t i = 0; i<blockCount; i++)
	{
		flags.push_back(free_flag);
	}
	
}

ImageFIFO::~ImageFIFO()
{
	operator delete(m_Data);
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
