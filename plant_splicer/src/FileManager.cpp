#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <sstream>
#include <windows.h>

void FileManager::SaveGenomes(Buffer<BranchGenome>& data)
{

	std::string str;
	std::stringstream strstr;
	unsigned int t = std::chrono::system_clock::now().time_since_epoch().count();
	strstr << t;
	strstr >> str;
	str += ".genome";
	std::ofstream file(str, std::ios::out | std::ios::binary);

	if (!file.good())
	{
		return;
	}

	for (int i = 0; i < 10; i++)
	{
		file.write((char*)&data[i], sizeof(BranchGenome));
	}
	file.close();
}

void FileManager::LoadGenomes(Buffer<BranchGenome>& loadPos, sf::String path)
{
	std::ifstream file(path.toAnsiString(), std::ios::out | std::ios::binary);

	if (!file.good())
	{
		return;
	}

	for (int i = 0; i < 10; i++)
	{
		file.read((char*)&loadPos[i], sizeof(BranchGenome));
	}
}