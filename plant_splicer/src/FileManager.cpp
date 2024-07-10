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

    CheckGenomeRef(cRBranch0);
    CheckGenomeRef(cRBranch1);
    CheckGenomeRef(cRBranch2);

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

    SHUFFLE(cRBranch0);
    SHUFFLE(cRBranch1);
    SHUFFLE(cRBranch2);

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

    SHUFFLE(initColour);

    SHUFFLE(initWidth);
    SHUFFLE(length);
    SHUFFLE(lengthVariation);

    SHUFFLE(colourAdoption);
    SHUFFLE(widthAdoption);
    SHUFFLE(dirAdoption);

#undef SHUFFLE
}
float FileManager::GetRandomFromBases(float max, float min, bool extended, float emax, float emin)
{
    float r = rand() % 1001;
    r /= 1000;

    if (extended)
        if (rand() % 4 == 0)
            max = emax; min = emin;

    std::cout << r * (max - min) + min << "\n";

    return r * (max - min) + min;
}

void FileManager::MutateRandomGenome(Buffer<BranchGenome>& out, int genome, BranchGenome parent0, BranchGenome parent1)
{
    int gene = rand() % 36; // THIS MUST BE THE NUMBER OF GENOMES
    std::cout << gene << "," << genome << "\n";
    int currGene = 0;
#define MUTATE(val, mn, mx, ex, emn, emx) {if (gene == currGene) out[genome].val = GetRandomFromBases(mx, mn, ex, emx, emn);\
    }

    MUTATE(branch0, -1, 9, false, 0, 0); currGene++;
    MUTATE(branch1, -1, 9, false, 0, 0); currGene++;
    MUTATE(branch2, -1, 9, false, 0, 0); currGene++;
    MUTATE(branch3, -1, 9, false, 0, 0); currGene++;
    MUTATE(branch4, -1, 9, false, 0, 0); currGene++;
    MUTATE(branch5, -1, 9, false, 0, 0); currGene++;

    MUTATE(rBranch0, -1, 9, false, 0, 0); currGene++;
    MUTATE(rBranch1, -1, 9, false, 0, 0); currGene++;
    MUTATE(rBranch2, -1, 9, false, 0, 0); currGene++;

    MUTATE(cRBranch0, -1, 9, false, 0, 0); currGene++;
    MUTATE(cRBranch1, -1, 9, false, 0, 0); currGene++;
    MUTATE(cRBranch2, -1, 9, false, 0, 0); currGene++;

    MUTATE(colourChange.r, -0.2, 1.5, true, -2, 2); currGene++;
    MUTATE(colourChange.g, -0.2, 1.5, true, -2, 2); currGene++;
    MUTATE(colourChange.b, -0.2, 1.5, true, -2, 2); currGene++;

    MUTATE(widthChange, -0.05, 0.05, true, -0.08, 0.08); currGene++;
    MUTATE(dirChange, -0.05, 0.05, true, -0.1, 0.1); currGene++;
    MUTATE(randTurn, -0.05, 0.05, true, 0, 0); currGene++;

    MUTATE(branch0Position, 0.1, 1, false, 0, 0); currGene++;
    MUTATE(branch1Position, 0.1, 1, false, 0, 0); currGene++;
    MUTATE(branch2Position, 0.1, 1, false, 0, 0); currGene++;
    MUTATE(branch3Position, 0.1, 1, false, 0, 0); currGene++;
    MUTATE(branch4Position, 0.1, 1, false, 0, 0); currGene++;
    MUTATE(branch5Position, 0.1, 1, false, 0, 0); currGene++;

    MUTATE(initDir, -0.1, 0.1, false, 0, 0); currGene++;
    MUTATE(dirSpread, 0, 7, false, 0, 0); currGene++;
    MUTATE(spreadMaxDistanceEff, 50, 500, false, 0, 0); currGene++;

    MUTATE(initColour.r, 0, 255, false, 0, 0); currGene++;
    MUTATE(initColour.g, 0, 255, false, 0, 0); currGene++;
    MUTATE(initColour.b, 0, 255, false, 0, 0); currGene++;

    MUTATE(initWidth, 3, 25, true, 1, 50); currGene++;
    MUTATE(length, 10, 500, true, 1, 800); currGene++;
    MUTATE(lengthVariation, 0, 40, true, 0, 100); currGene++;

    MUTATE(colourAdoption, 0, 1, false, 0, 0); currGene++;
    MUTATE(widthAdoption, 0, 1, false, 0, 0); currGene++;
    MUTATE(dirAdoption, 0, 1, false, 0, 0); currGene++;
#undef MUTATE
}

void FileManager::CreateSplicedPlant(const sf::String& string0, const sf::String& string1, uint32_t randomSeed, Buffer<BranchGenome>& splicedPlant, bool mutate)
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

    if (!mutate) return;

    bool usedGenomesPreM[10]{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    CheckGenomeReferences(usedGenomesPreM, 0, splicedPlant, 0);

    int genome = rand() % 10;
    if (!usedGenomesPreM[genome])
    {
        for (int i = genome; i > -1; i--)
        {
            if (usedGenomesPreM[i])
            {
                genome = i;
                break;
            }
        }
    }

    MutateRandomGenome(splicedPlant, genome, plant0[genome], plant1[genome]);

    bool usedGenomesM[10]{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    CheckGenomeReferences(usedGenomesM, 0, splicedPlant, 0);

    for (int i = 1; i < 10; i++)
    {
        if (usedGenomesM[i] != usedGenomesPreM[i])
        {
            for (int j = 9; j > -1; j--)
            {
                if (usedGenomesM[j])
                {
                    splicedPlant[i] = splicedPlant[j];
                    break;
                }
            }
        }
    }
}


