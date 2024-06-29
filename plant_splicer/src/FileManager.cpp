#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <sstream>

bool FileManager::SaveGenomes(Buffer<BranchGenome>& data)
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
		return false;
	}

	for (int i = 0; i < 10; i++)
	{
		file.write((char*)&data[i], sizeof(BranchGenome));
	}
	file.close();
    return true;
}

bool FileManager::LoadGenomes(Buffer<BranchGenome>& loadPos, sf::String path)
{
	std::ifstream file(path.toAnsiString(), std::ios::out | std::ios::binary);

	if (!file.good())
	{
		return false;
	}

	for (int i = 0; i < 10; i++)
	{
		file.read((char*)&loadPos[i], sizeof(BranchGenome));
	}
    return true;
}

void FileManager::CheckGenomeReferences(bool* list, uint8_t recursionDepth, Buffer<BranchGenome>& bg, uint8_t index)
{
    if (recursionDepth >= 5) return;

    if (bg[index].branch0 >= 0 && bg[index].branch0 < 10 && !list[bg[index].branch0])
    {
        list[bg[index].branch0] = true;
        CheckGenomeReferences(list, recursionDepth + 1, bg, index + 1);
    }
    if (bg[index].branch1 >= 0 && bg[index].branch1 < 10 && !list[bg[index].branch1])
    {
        list[bg[index].branch1] = true;
        CheckGenomeReferences(list, recursionDepth + 1, bg, index + 1);
    }
    if (bg[index].branch2 >= 0 && bg[index].branch2 < 10 && !list[bg[index].branch2])
    {
        list[bg[index].branch2] = true;
        CheckGenomeReferences(list, recursionDepth + 1, bg, index + 1);
    }
    if (bg[index].branch3 >= 0 && bg[index].branch3 < 10 && !list[bg[index].branch3])
    {
        list[bg[index].branch3] = true;
        CheckGenomeReferences(list, recursionDepth + 1, bg, index + 1);
    }
    if (bg[index].branch4 >= 0 && bg[index].branch4 < 10 && !list[bg[index].branch4])
    {
        list[bg[index].branch4] = true;
        CheckGenomeReferences(list, recursionDepth + 1, bg, index + 1);
    }
    if (bg[index].branch5 >= 0 && bg[index].branch5 < 10 && !list[bg[index].branch5])
    {
        list[bg[index].branch5] = true;
        CheckGenomeReferences(list, recursionDepth + 1, bg, index + 1);
    }
    if (bg[index].rBranch0 >= 0 && bg[index].rBranch0 < 10 && !list[bg[index].rBranch0])
    {
        list[bg[index].rBranch0] = true;
        CheckGenomeReferences(list, recursionDepth + 1, bg, index + 1);
    }
    if (bg[index].rBranch1 >= 0 && bg[index].rBranch1 < 10 && !list[bg[index].rBranch1])
    {
        list[bg[index].rBranch1] = true;
        CheckGenomeReferences(list, recursionDepth + 1, bg, index + 1);
    }
    if (bg[index].rBranch2 >= 0 && bg[index].rBranch2 < 10 && !list[bg[index].rBranch2])
    {
        list[bg[index].rBranch2] = true;
        CheckGenomeReferences(list, recursionDepth + 1, bg, index + 1);
    }
}

void FileManager::ShuffleGenome(BranchGenome gene0, BranchGenome gene1, BranchGenome& out)
{
    if (rand() % 2)
        out.branch0 = gene0.branch0;
    else
        out.branch0 = gene1.branch0;

    if (rand() % 2)
        out.branch1 = gene0.branch1;
    else
        out.branch1 = gene1.branch1;

    if (rand() % 2)
        out.branch2 = gene0.branch2;
    else
        out.branch2 = gene1.branch2;

    if (rand() % 2)
        out.branch3 = gene0.branch3;
    else
        out.branch3 = gene1.branch3;

    if (rand() % 2)
        out.branch4 = gene0.branch4;
    else
        out.branch4 = gene1.branch4;

    if (rand() % 2)
        out.branch5 = gene0.branch5;
    else
        out.branch5 = gene1.branch5;

    if (rand() % 2)
        out.rBranch0 = gene0.rBranch0;
    else
        out.rBranch0 = gene1.rBranch0;

    if (rand() % 2)
        out.rBranch1 = gene0.rBranch1;
    else
        out.rBranch1 = gene1.rBranch1;

    if (rand() % 2)
        out.rBranch2 = gene0.rBranch2;
    else
        out.rBranch2 = gene1.rBranch2;

    if (rand() % 2)
        out.colourChange.r = gene0.colourChange.r;
    else
        out.colourChange.r = gene1.colourChange.r;

    if (rand() % 2)
        out.colourChange.g = gene0.colourChange.g;
    else
        out.colourChange.g = gene1.colourChange.g;

    if (rand() % 2)
        out.colourChange.b = gene0.colourChange.b;
    else
        out.colourChange.b = gene1.colourChange.b;

    if (rand() % 2)
        out.widthChange = gene0.widthChange;
    else
        out.widthChange = gene1.widthChange;

    if (rand() % 2)
        out.dirChange = gene0.dirChange;
    else
        out.dirChange = gene1.dirChange;

    if (rand() % 2)
        out.randTurn = gene0.randTurn;
    else
        out.randTurn = gene1.randTurn;

    out.branch0Position = (rand() % 2) ? gene0.branch0Position : gene1.branch0Position;
    out.branch1Position = (rand() % 2) ? gene0.branch1Position : gene1.branch1Position;
    out.branch2Position = (rand() % 2) ? gene0.branch2Position : gene1.branch2Position;
    out.branch3Position = (rand() % 2) ? gene0.branch3Position : gene1.branch3Position;
    out.branch4Position = (rand() % 2) ? gene0.branch4Position : gene1.branch4Position;
    out.branch5Position = (rand() % 2) ? gene0.branch5Position : gene1.branch5Position;

    out.initDir = (rand() % 2) ? gene0.initDir : gene1.initDir;
    out.dirSpread = (rand() % 2) ? gene0.dirSpread : gene1.dirSpread;
    out.spreadMaxDistanceEff = (rand() % 2) ? gene0.spreadMaxDistanceEff : gene1.spreadMaxDistanceEff;

    out.initColour = (rand() % 2) ? gene0.initColour : gene1.initColour;

    out.initWidth = (rand() % 2) ? gene0.initWidth : gene1.initWidth;
    out.length = (rand() % 2) ? gene0.length : gene1.length;
    out.lengthVariation = (rand() % 2) ? gene0.lengthVariation : gene1.lengthVariation;

    out.colourAdoption = (rand() % 2) ? gene0.colourAdoption : gene1.colourAdoption;
    out.widthAdoption = (rand() % 2) ? gene0.widthAdoption : gene1.widthAdoption;
    out.dirAdoption = (rand() % 2) ? gene0.dirAdoption : gene1.dirAdoption;
}

void FileManager::CreateSplicedPlant(sf::String string0, sf::String string1, unsigned int randomSeed, Buffer<BranchGenome>& splicedPlant)
{
	Buffer<BranchGenome> plant0{ 10 };
	Buffer<BranchGenome> plant1{ 10 };

	LoadGenomes(plant0, string0);
	LoadGenomes(plant1, string1);
	
	bool usedGenomes0[10]{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	bool usedGenomes1[10]{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	
    CheckGenomeReferences(usedGenomes0, 0, plant0, 0);
    CheckGenomeReferences(usedGenomes1, 0, plant1, 0);

    srand(randomSeed);
    for (int i = 0; i < 10; i++)
    {
        if (usedGenomes0[i] && usedGenomes1[i])
            ShuffleGenome(plant0[i], plant1[i], splicedPlant[i]);
        else if (usedGenomes0[i])
            splicedPlant[i] = plant0[i];
        else
            splicedPlant[i] = plant1[i];
    }
}