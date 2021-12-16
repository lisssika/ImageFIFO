#include "pch.h"
#include "../ImageFIFO/ImageFIFO.h"
#include "../ImageFIFO/ReadAndWrite.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <future>
#include <sstream>
#include <functional>
#include<memory>
#include <stdexcept>

std::string input_path("images/input/");
std::string sync_one_path("images/Sync_one/");
std::string sync_several_path("images/Sync_several/");
std::string async_one_path("images/Async_one/");
std::string async_several_path("images/Async_several/");

bool two_equal_files(std::string f1, std::string f2)
{
	std::ifstream file1(f1, std::ios::binary);
	std::ifstream file2(f2, std::ios::binary);
	if (file1.is_open()&&file2.is_open())
	{
		return std::equal(
			std::istreambuf_iterator<char>(file1),
			std::istreambuf_iterator<char>(),
			std::istreambuf_iterator<char>(file2)
		);
	}
	return false;
}

bool several_pairs_equal_files(const std::string& inp, const std::string& out)
{
	std::stringstream inp_ss(inp);
	std::stringstream out_ss(out);
	bool is_equal = true;

	for (std::string input; std::getline(inp_ss, input, ' '); ) {
		std::string output; std::getline(out_ss, output, ' ');
		is_equal*=two_equal_files(input, output);
	}
	return is_equal;
}

void generate_file_names(size_t n, std::string& inp, std::string& out)
{
	for (size_t i = 1; i <= n; i++)
	{
		inp += input_path + "input" + std::to_string(i) + ".BMP ";
		out += sync_several_path + "output" + std::to_string(i) + ".BMP ";
	}
}

TEST(TestEqualFunc, equal_files_) {
  EXPECT_TRUE(two_equal_files(input_path+"input1.BMP", "images/output/output1.BMP"));
}


TEST(TestEqualFunc, different_files_) {
	EXPECT_TRUE(!two_equal_files(input_path+"input2.BMP", "images/output/output1.BMP"));
}

TEST(Sync, one_file)
{
	ImageFIFO image_fifo(2548762, 1);
	const std::string inp = input_path + "input1.BMP";
	const std::string out = sync_one_path + "output1.BMP";
	ReaderWriter rw(std::make_unique<ImageFIFO>(2548762, 1));
	rw.writer( inp);
	rw.reader( out);
	EXPECT_TRUE(two_equal_files(inp, out));
}

TEST(Sync, sevral_files)
{
	std::string inp;
	std::string out;  
	generate_file_names(3, inp, out);
	ReaderWriter rw(std::make_unique<ImageFIFO>(2548762, 3));
	rw.writer(inp);
	rw.reader(out);
	EXPECT_TRUE(several_pairs_equal_files(inp, out));
}


TEST(Async, one_file)
{
	const std::string inp = input_path + "input1.BMP";
	const std::string out = async_one_path + "output1.BMP";
	ReaderWriter rw(std::make_unique<ImageFIFO>(2548762, 1));
	std::future<int> threadWriter = std::async(std::launch::async, &ReaderWriter::reader, &rw, std::ref(inp));
	std::future<int> threadReader = std::async(std::launch::async, &ReaderWriter::writer, &rw, std::ref(out));
	threadReader.get();
	threadWriter.get();
	EXPECT_TRUE(two_equal_files(inp, out));
}

TEST(Async, several_files)
{
	ImageFIFO image_fifo(2548762, 3);
	std::string inp;
	std::string out;
	generate_file_names(3, inp, out);

	ReaderWriter rw(std::make_unique<ImageFIFO>(2548762, 3));
	std::future<int> threadWriter = std::async(std::launch::async, &ReaderWriter::reader, &rw, std::ref(inp));
	std::future<int> threadReader = std::async(std::launch::async, &ReaderWriter::writer, &rw, std::ref(out));
	threadReader.get();
	threadWriter.get();

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