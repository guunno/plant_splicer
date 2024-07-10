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
	static float GetRandomFromBases(float max, float min, bool extended = false, float emax = 0, float emin = 0);
	static void MutateRandomGenome(Buffer<BranchGenome>& out, int genome, BranchGenome parent0, BranchGenome parent1);
	static void CreateSplicedPlant(const sf::String& path0, const sf::String& path1, uint32_t randomSeed, Buffer<BranchGenome>& splicedPlant, bool mutate = false);

	static bool ConvertLegacy(const std::string& path);
};