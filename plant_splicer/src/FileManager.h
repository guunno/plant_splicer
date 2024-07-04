#pragma once
#include <fstream>
#include "Genome.h"
#include "Buffer.h"

class FileManager
{
public:
	bool SaveGenomes(Buffer<BranchGenome>& data);
	bool SaveGenomes(Buffer<BranchGenome>& data, sf::String path);
	bool LoadGenomes(Buffer<BranchGenome>& loadPos, sf::String path);
	bool ConvertLegacy(sf::String path);
	void CheckGenomeReferences(bool* list, uint8_t recursionDepth, Buffer<BranchGenome>& bg, uint8_t index);
	void ShuffleGenome(BranchGenome gene0, BranchGenome gene1, BranchGenome& out);
	void CreateSplicedPlant(sf::String path0, sf::String path1, unsigned int randomSeed, Buffer<BranchGenome>& splicedPlant);
};