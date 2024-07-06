#pragma once
#include <fstream>
#include "Genome.h"
#include "Buffer.h"

class FileManager
{
public:
	static bool SaveGenomes(Buffer<BranchGenome>& data);
	static bool SaveGenomes(Buffer<BranchGenome>& data, const std::string& path);
	static bool LoadGenomes(Buffer<BranchGenome>& loadPos, const std::string& path);
	static void CheckGenomeReferences(bool* list, uint8_t recursionDepth, Buffer<BranchGenome>& bg, uint8_t index);
	static void ShuffleGenome(BranchGenome gene0, BranchGenome gene1, BranchGenome& out);
	static void CreateSplicedPlant(const sf::String& path0, const sf::String& path1, uint32_t randomSeed, Buffer<BranchGenome>& splicedPlant);

	static bool ConvertFromHeaderless(const std::string& path);
};