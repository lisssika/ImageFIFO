#pragma once
#include <mutex>
#include <list>

class ImageFIFO final {
public:
	ImageFIFO(size_t blockSize, size_t blockCount);
	~ImageFIFO();
	void* getFree();
	void addReady(void* data);
	void* getReady();
	void addFree(void* data);
	size_t get_blockSize() const;
private:
	std::mutex m_FifoMutex;
	std::list<void*> m_FreeData;
	std::list<void*> m_ReadyData;
	void* get_ptr(std::list<void*>& list);
	void add(std::list<void*>& list, void* data);
	size_t blockSize_;
};
