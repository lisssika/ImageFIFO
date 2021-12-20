#include "pch.h"
#include "../ImageFIFO/ImageFIFO.h"
#include "../ImageFIFO/ReadAndWrite.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <future>
#include <functional>
#include<memory>
#include <vector>

class FileUtils:public ::testing::Test
{
public:
	std::string input_path="images/input/";
	std::string sync_one_path="images/Sync_one/";
	std::string sync_several_path="images/Sync_several/";
	std::string async_one_path="images/Async_one/";
	std::string async_several_path="images/Async_several/";
	bool async_exec = true;

	bool two_equal_files(const std::string& f1, const std::string& f2) const
	{
		std::ifstream file1(f1, std::ios::binary);
		std::ifstream file2(f2, std::ios::binary);
		if (file1 && file2)
		{
			return std::equal(
				std::istreambuf_iterator<char>(file1),
				std::istreambuf_iterator<char>(),
				std::istreambuf_iterator<char>(file2)
			);
		}
		return false;
	}

	bool several_pairs_equal_files(const std::vector<std::string>& inp, const std::vector<std::string>& out)
	{
		bool is_equal = true;

		for (size_t i = 0; i < inp.size(); i++) {
			is_equal *= two_equal_files(inp[i], out[i]);
		}
		return is_equal;
	}

	void generate_file_names(size_t n, std::vector<std::string>& inp, std::vector<std::string>& out, bool sync) const
	{
		for (size_t i = 1; i <= n; i++)
		{
			inp.push_back(input_path + "input" + std::to_string(i) + ".BMP ");
			if (sync)
			{
				out.push_back(sync_several_path + "output" + std::to_string(i) + ".BMP ");
			}
			else
			{
				out.push_back(async_several_path + "output" + std::to_string(i) + ".BMP ");
			}
		}
	}
};

class TestEqualFunc :public FileUtils {};
TEST_F(TestEqualFunc, equal_files_) {
  EXPECT_TRUE(two_equal_files(input_path+"input1.BMP", "images/output/output1.BMP"));
}

TEST_F(TestEqualFunc, different_files_) {
	EXPECT_TRUE(!two_equal_files(input_path+"input2.BMP", "images/output/output1.BMP"));
}
class Sync:public FileUtils{};
TEST_F(Sync, one_file)
{
	ImageFIFO image_fifo(2548762, 1);
	const std::string inp = input_path + "input1.BMP";
	const std::string out = sync_one_path + "output1.BMP";
	ReaderWriter rw(std::make_unique<ImageFIFO>(2548762, 1), {inp}, {out});
	rw.exec(!async_exec);
	EXPECT_TRUE(two_equal_files(inp, out));
}

TEST_F(Sync, sevral_files)
{
	std::vector<std::string> inp;
	std::vector<std::string> out;
	generate_file_names(3, inp, out, true);
	ReaderWriter rw(std::make_unique<ImageFIFO>(2548762, 3), inp, out);
	rw.exec(!async_exec);
	EXPECT_TRUE(several_pairs_equal_files(inp, out));
}

class Async:public FileUtils{};
TEST_F(Async, one_file)
{
	const std::string inp = input_path + "input1.BMP";
	const std::string out = async_one_path + "output1.BMP";
	ReaderWriter rw(std::make_unique<ImageFIFO>(2548762, 1), {inp}, {out});
	rw.exec(async_exec);
	EXPECT_TRUE(two_equal_files(inp, out));
}

TEST_F(Async, several_files)
{
	ImageFIFO image_fifo(2548762, 3);
	std::vector<std::string> inp;
	std::vector<std::string> out;
	generate_file_names(3, inp, out, false);

	ReaderWriter rw(std::make_unique<ImageFIFO>(2548762, 3), inp, out);
	rw.exec(async_exec);
	EXPECT_TRUE(several_pairs_equal_files(inp, out));
}
/*
void test_w_exception()
{
	
}

void catch_throw ()
{
	ImageFIFO image_fifo(2548762, 1);
	const std::string inp = input_path + "photo.BMP";
	const std::string out = async_one_path + "output1.BMP";
	std::future<int> threadWriter = std::async(std::launch::async, writer, std::ref(image_fifo), std::ref(inp));
	std::future<int> threadReader = std::async(std::launch::async, reader, std::ref(image_fifo), std::ref(out));
	if (threadReader.valid()&&threadWriter.valid())
	{
		try
		{
			threadWriter.get();
			threadReader.get();
		}
		catch (std::runtime_error& e)
		{
			std::cout << "Async task threw exception: " << e.what() << std::endl;
			// как остановать поток threadReader?
			throw e;
		}
	}
}

TEST(Async, exception_catch)
{
	EXPECT_THROW(catch_throw(), std::runtime_error);
}
*/