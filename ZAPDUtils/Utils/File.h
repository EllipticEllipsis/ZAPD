#pragma once

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "Directory.h"
#include "Utils/StringHelper.h"

class File
{
public:
	bool Exists(const fs::path& filePath);
	std::vector<uint8_t> ReadAllBytes(const fs::path& filePath);
	std::string ReadAllText(const fs::path& filePath);
	std::vector<std::string> ReadAllLines(const fs::path& filePath);
	void WriteAllBytes(const fs::path& filePath, const std::vector<uint8_t>& data);
	void WriteAllBytes(const std::string& filePath, const std::vector<char>& data);
	void WriteAllBytes(const std::string& filePath, const char* data, int dataSize);
	void WriteAllText(const fs::path& filePath, const std::string& text);
};
