#pragma once
#include <mutex>
#include <vector>
#include <memory>

class ImageFIFO final {
public:
	ImageFIFO(size_t blockSize, size_t blockCount);
	void* getFree();
	void addReady(void* data);
	void* getReady();
	void addFree(void* data);
	size_t get_blockSize() const;
	size_t get_blockCount() const;
private:
	std::mutex m_FifoMutex;
	std::unique_ptr<char[]> m_Data;
	std::vector<bool> flags;

	void* get_ptr(bool flag);
	void add(void* data, bool flag);
	size_t blockSize_;
	size_t blockCount_;
	bool free_flag = false;
	bool ready_flag = !free_flag;
};
