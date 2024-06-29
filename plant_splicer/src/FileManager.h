#pragma once
#include <fstream>
#include <iostream>
#include "Genome.h"
#include "Buffer.h"

class FileManager
{
public:
	void SaveGenomes(Buffer<BranchGenome>& data);
	void LoadGenomes(Buffer<BranchGenome>& loadPos, sf::String path);
};