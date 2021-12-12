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

std::string input_path("images/input/");
std::string sync_one_path("images/Sync_one/");
std::string sync_several_path("images/Sync_several/");
std::string async_one_path("images/Async_one/");
std::string async_several_path("images/Async_several/");

bool equal_files(std::string f1, std::string f2)
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

TEST(TestEqualFunc, equal_files_) {
  EXPECT_TRUE(equal_files(input_path+"input1.BMP", "images/output/output1.BMP"));
}


TEST(TestEqualFunc, different_files_) {
	EXPECT_TRUE(!equal_files(input_path+"input2.BMP", "images/output/output1.BMP"));
}

TEST(Sync, one_file)
{
	ImageFIFO image_fifo(2548762, 1);
	const std::string inp = input_path + "input1.BMP";
	const std::string outp = sync_one_path + "output1.BMP";
	
	writer(image_fifo, inp);
	reader(image_fifo, outp);
	EXPECT_TRUE(equal_files(inp, outp));
}

TEST(Sync, sevral_files)
{
	ImageFIFO image_fifo(2548762, 3);
	std::string inp;
	std::string out;  
	for (int i = 1; i<4; i++)
	{
		inp += input_path + "input" +std::to_string(i)+".BMP ";
		out += sync_several_path + "output" + std::to_string(i) + ".BMP ";
	}
	writer(image_fifo, inp);
	reader(image_fifo, out);
	std::stringstream inp_ss(inp);
	std::stringstream out_ss(out);
	
	for (std::string input; std::getline(inp_ss, input, ' '); ) {
		std::string output; std::getline(out_ss, output, ' ');
		EXPECT_TRUE(equal_files(input, output));
	}
}


TEST(Async, one_file)
{
	ImageFIFO image_fifo(2548762, 1);
	const std::string inp = input_path + "input1.BMP";
	const std::string out = async_one_path + "output1.BMP";
	std::future<int> threadWriter = std::async(std::launch::async, writer, std::ref(image_fifo), std::ref(inp) );
	std::future<int> threadReader = std::async(std::launch::async, reader, std::ref(image_fifo), std::ref(out));
	threadReader.get();
	threadWriter.get();
	EXPECT_TRUE(equal_files(inp, out));
}

TEST(Async, several_files)
{
	ImageFIFO image_fifo(2548762, 3);
	std::string inp;
	std::string out;
	for (int i = 1; i < 4; i++)
	{
		inp += input_path + "input" + std::to_string(i) + ".BMP ";
		out += async_several_path + "output" + std::to_string(i) + ".BMP ";
	}
	std::future<int> threadWriter = std::async(std::launch::async, writer, std::ref(image_fifo), std::ref(inp));
	std::future<int> threadReader = std::async(std::launch::async, reader, std::ref(image_fifo), std::ref(out));
	threadReader.get();
	threadWriter.get();

	std::stringstream inp_ss(inp);
	std::stringstream out_ss(out);

	for (std::string input; std::getline(inp_ss, input, ' '); ) {
		std::string output; std::getline(out_ss, output, ' ');
		EXPECT_TRUE(equal_files(input, output));
	}
}