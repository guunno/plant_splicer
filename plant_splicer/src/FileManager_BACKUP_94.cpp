#include "FileManager.h"
#include <fstream>
#include <chrono>
#include <string>

bool FileManager::SaveGenomes(Buffer<BranchGenome>& data)
{
	std::string str;
	str += std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".genome";
	std::ofstream file(str, std::ios::out | std::ios::binary);

	if (!file.good())
		return false;

    file.write((char*)&data[0], sizeof(BranchGenome) * data.Size());

	file.close();
    return true;
}

bool FileManager::LoadGenomes(Buffer<BranchGenome>& outputData, sf::String path)
{
	std::ifstream file(path.toAnsiString(), std::ios::out | std::ios::binary);

	if (!file.good())
		return false;

    file.read((char*)&outputData[0], sizeof(BranchGenome) * outputData.Size());

    file.close();
    return true;
}

void FileManager::CheckGenomeReferences(bool* list, uint8_t recursionDepth, Buffer<BranchGenome>& bg, uint8_t index)
{
    if (recursionDepth >= 5) return;

#define CheckGenomeRef(attr) if (bg[index].attr >= 0 && bg[index].attr < 10 && !list[bg[index].attr]) {\
  list[bg[index].attr] = true; CheckGenomeReferences(list, recursionDepth + 1, bg, index + 1);}

    CheckGenomeRef(branch0);
    CheckGenomeRef(branch1);
    CheckGenomeRef(branch2);
    CheckGenomeRef(branch3);
    CheckGenomeRef(branch4);
    CheckGenomeRef(branch5);

    CheckGenomeRef(rBranch0);
    CheckGenomeRef(rBranch1);
    CheckGenomeRef(rBranch2);

#undef CheckGenomeRef
}

void FileManager::ShuffleGenome(BranchGenome gene0, BranchGenome gene1, BranchGenome& out)
{
#define SHUFFLE(attr) out.attr = (rand() % 2 == 0) ? gene0.attr : gene1.attr

    SHUFFLE(branch0);
    SHUFFLE(branch1);
    SHUFFLE(branch2);
    SHUFFLE(branch3);
    SHUFFLE(branch4);
    SHUFFLE(branch5);

    SHUFFLE(rBranch0);
    SHUFFLE(rBranch1);
    SHUFFLE(rBranch2);
    SHUFFLE(colourChange.r);
    SHUFFLE(colourChange.g);
    SHUFFLE(colourChange.b);
   
    SHUFFLE(widthChange);
    SHUFFLE(dirChange);
    SHUFFLE(randTurn);

    SHUFFLE(branch0Position);
    SHUFFLE(branch1Position);
    SHUFFLE(branch2Position);
    SHUFFLE(branch3Position);
    SHUFFLE(branch4Position);
    SHUFFLE(branch5Position);

    SHUFFLE(initDir);
    SHUFFLE(dirSpread);
    SHUFFLE(spreadMaxDistanceEff);

    SHUFFLE(initColour.r);
    SHUFFLE(initColour.g);
    SHUFFLE(initColour.b);

    SHUFFLE(initWidth);
    SHUFFLE(length);
    SHUFFLE(lengthVariation);

    SHUFFLE(colourAdoption);
    SHUFFLE(widthAdoption);
    SHUFFLE(dirAdoption);

<<<<<<< HEAD
#undef SHUFFLE
=======
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
>>>>>>> 22c02b4cca6730e2f83c3fd7e8b494e470550904
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
