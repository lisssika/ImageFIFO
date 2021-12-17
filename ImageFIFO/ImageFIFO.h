#pragma once
#include <mutex>
#include <vector>

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
	void* m_Data;
	std::vector<bool> flags;

	void* get_ptr(bool flag);
	void add(void* data, bool flag);
	size_t blockSize_;
	size_t blockCount_;
	bool free_flag = false;
	bool ready_flag = !free_flag;
};
