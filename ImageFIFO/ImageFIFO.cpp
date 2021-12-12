#include "ImageFIFO.h"
#include <mutex>
#include <list>

void* ImageFIFO::get_ptr(std::list<void*>& list)
{
	std::lock_guard<std::mutex>guard(m_FifoMutex);
	if (list.empty())
		return nullptr;
	void* ptr = list.front();
	if(ptr)
	{
		list.pop_front();
	}
	return ptr;
}

void ImageFIFO::add(std::list<void*>& list, void* data)
{
	std::lock_guard<std::mutex> guard(m_FifoMutex);
	list.push_back(data);
}


ImageFIFO::ImageFIFO(size_t blockSize, size_t blockCount) : blockSize_(blockSize)
{
	std::lock_guard<std::mutex> guard(m_FifoMutex);
	for (size_t i = 0; i< blockCount; ++i)
	{
		m_FreeData.push_back (std::malloc(blockSize));
	}
}

ImageFIFO::~ImageFIFO()
{
	for(auto& free_data:m_FreeData)
	{
		free (free_data);
	}
	for (auto& ready_data : m_ReadyData)
	{
		free(ready_data);
	}
}

void* ImageFIFO::getFree() // writer
{
	return get_ptr(m_FreeData);
}

void ImageFIFO::addReady(void* data)//writer
{
	add(m_ReadyData, data);
}

void* ImageFIFO::getReady()//reader
{
	return get_ptr(m_ReadyData);
}

void ImageFIFO::addFree(void* data)//writer
{
	add(m_FreeData, data);
}

size_t ImageFIFO::get_blockSize() const
{
	return blockSize_;
}
