#include "FileManager.h"
#include <fstream>
#include <chrono>
#include <string>

#include "Plant.h"

bool FileManager::SaveGenomes(Buffer<BranchGenome>& data)
{
    std::string path = std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".genome";
    return SaveGenomes(data, path);
}

bool FileManager::SaveGenomes(Buffer<BranchGenome>& data, const std::string& path)
{
	std::ofstream file(path, std::ios::out | std::ios::binary);

	if (!file.good())
		return false;

    uint32_t stride = sizeof(BranchGenome);
    file.write((char*)&stride, sizeof(uint32_t));
    file.write((char*)&data[0], sizeof(BranchGenome) * data.Size());

	file.close();
    return true;
}

bool FileManager::LoadGenomes(Buffer<BranchGenome>& outputData, const std::string& path)
{
	std::ifstream file(path, std::ios::out | std::ios::binary);

	if (!file.good())
		return false;

    uint32_t stride = 0;
    file.read((char*)&stride, sizeof(stride));

    for (int i = 0; i < outputData.Size(); i++)
        file.read((char*)&outputData[i], stride);

    file.close();
    return true;
}

bool FileManager::ConvertLegacy(const std::string& path)
{
    Buffer<BranchGenome> buffer(Plant::BRANCH_COUNT);

    // READ BYTES INTO BUFFER
    {
        std::ifstream file(path, std::ios::out | std::ios::binary);

        if (!file.good())
            return false;

        file.read((char*)&buffer[0], sizeof(BranchGenome) * buffer.Size());
        file.close();
    }

    return FileManager::SaveGenomes(buffer, path);
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

#undef SHUFFLE
}

void FileManager::CreateSplicedPlant(const sf::String& string0, const sf::String& string1, uint32_t randomSeed, Buffer<BranchGenome>& splicedPlant)
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
