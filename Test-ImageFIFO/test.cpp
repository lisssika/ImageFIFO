#include "pch.h"
#include "../ImageFIFO/ImageFIFO.h"
#include "../ImageFIFO/ReadAndWrite.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <iterator>

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
  EXPECT_TRUE(equal_files("input1.BMP", "output1.BMP"));
}


TEST(TestEqualFunc, different_files_) {
	EXPECT_TRUE(!equal_files("input2.BMP", "output1.BMP"));
}

TEST(Sync, one_file)
{
	ImageFIFO image_fifo(2548762, 1);
	const std::string inp("input1.BMP");
	const std::string outp("output1.BMP");
	writer(image_fifo, inp);
	reader(image_fifo, outp);
	EXPECT_TRUE(equal_files(inp, outp));
}

TEST(Sync, sevral_files)
{
	ImageFIFO image_fifo(2548762, 3);
	const std::string inp("input1.BMP input2.BMP input3.BMP");
	const std::string outp("output1.BMP output2.BMP output3.BMP");
	writer(image_fifo, inp);
	reader(image_fifo, outp);
	EXPECT_TRUE(equal_files("input1.BMP", "output1.BMP"));
	EXPECT_TRUE(equal_files("input2.BMP", "output2.BMP"));
	EXPECT_TRUE(equal_files("input3.BMP", "output3.BMP"));
}

TEST(Async, one_file)
{
	
}