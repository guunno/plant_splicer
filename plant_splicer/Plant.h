#pragma once
#include "Genome.h"
#include "Vector.h"

#define LERP(a, b, k) a * k + b * (1 - k)

const int MAX_BRANCHES = 1000;

struct BranchData
{
public:
	Vector pos = { 0, 0 };
	float dir = 0;
	float size = 25;
	sf::Color colour = {0, 0, 0};
	int setLength = 2;
	int branchingPoints[3];
	int branchIndexes[3];
	int numBranches = 0;;
	int branchLayer = 0;
	int genomeIndex = 0;
	bool dirPositive = 0;
	bool nonBlank = false;
};

class Branch
{
public:
	BranchData data;
	int step = 0;

	void InitBranch(int genome0, BranchGenome allGenomes[], Vector createPoint, const BranchData& bdata = BranchData(), bool isFirstBranch = true, int branchLayerNew = 0)
	{
		data.pos = createPoint;
		data.genomeIndex = genome0;
		BranchGenome& genome = allGenomes[data.genomeIndex];
		data.setLength = genome.length + (genome.lengthVariation * (((rand() % 201) - 100) / 100.0f));
		data.branchingPoints[0] = floor(genome.branch0Position * data.setLength);
		data.branchingPoints[1] = floor(genome.branch1Position * data.setLength);
		data.branchingPoints[2] = floor(genome.branch2Position * data.setLength);
		data.dirPositive = (rand() % 2);
		data.nonBlank = true;
		data.branchLayer = branchLayerNew;

		if (isFirstBranch)
		{
			data.dir = genome.initDir;
			data.colour = genome.initColour;
			data.size = genome.initSize;
		}
		else
		{
			data.dir = LERP(genome.initDir, bdata.dir, genome.dirAdoption);
			data.colour.r = LERP(genome.initColour.r, bdata.colour.r, genome.colourAdoption);
			data.colour.g = LERP(genome.initColour.g, bdata.colour.g, genome.colourAdoption);
			data.colour.b = LERP(genome.initColour.b, bdata.colour.b, genome.colourAdoption);
			data.size = LERP(genome.initSize, bdata.size, genome.sizeAdoption);
		}
	}

	void DrawBranch(sf::RenderWindow* window)
	{
		sf::CircleShape circle;

		//light
		circle.setFillColor(data.colour + sf::Color(25, 25, 25));
		circle.setRadius(data.size);
		circle.setPosition({ data.pos.x - 1, data.pos.y - 1 });
		window->draw(circle);

		//shadow
		circle.setFillColor(sf::Color(data.colour.r - 25, data.colour.g - 25, data.colour.b - 25));
		circle.setPosition({ data.pos.x + 1, data.pos.y + 1 });
		window->draw(circle);

		//actual
		circle.setFillColor(data.colour);
		circle.setPosition({ data.pos.x, data.pos.y });
		window->draw(circle);

	}

	bool* UpdateData(BranchGenome allGenome[])
	{
		bool branchesExistArray[3];

		branchesExistArray[0] = false;
		branchesExistArray[1] = false;
		branchesExistArray[2] = false;

		BranchGenome& genome = allGenome[data.genomeIndex];
		data.dir += (genome.dirChange + ((((rand() % 201) - 100) / 100.0f) * genome.randTurn)) * ((data.dirPositive * 2) - 1);
		Vector v = { 0, -1 };
		data.pos += v.rotateNew(data.dir);
		data.colour += genome.colourChange;
		data.size += genome.sizeChange;
		if (data.branchLayer < 10)
		{
			if (step == data.branchingPoints[0])
			{
				if(genome.branch0 > -1)
				{
					branchesExistArray[0] = true;
				}
			}
			if (step == data.branchingPoints[1])
			{
				if (genome.branch1 > -1)
				{
					branchesExistArray[1] = true;
				}
			}
			if (step == data.branchingPoints[2])
			{
				if (genome.branch2 > -1)
				{
					branchesExistArray[2] = true;
				}
			}
		}
		step++;
		return branchesExistArray;
	}

	void RenderWholeBranch(sf::RenderWindow* window, BranchGenome allGenome[], BranchData* dataToReturn[])
	{
		for (int i = 0; i < data.setLength; i++)
		{
			DrawBranch(window);
			bool vals[3] = { UpdateData(allGenome) };
			for (int j = 0; j < 3; j++)
			{
				if (vals[j] == true)
				{
					*dataToReturn[j] = data;
				}
			}
		}
	}

	void Reset(Vector origin, BranchGenome allGenome[], bool isFirst = true, BranchData bdata = BranchData())
	{
		BranchGenome& genome = allGenome[data.genomeIndex];
		step = 0;
		data.pos = origin;
		data.colour = genome.initColour;
		data.dir = genome.initDir;
		data.size = genome.initSize;

		if (!isFirst)
		{
			data.dir = LERP(genome.initDir, bdata.dir, genome.dirAdoption);
			data.colour.r = LERP(genome.initColour.r, bdata.colour.r, genome.colourAdoption);
			data.colour.g = LERP(genome.initColour.g, bdata.colour.g, genome.colourAdoption);
			data.colour.b = LERP(genome.initColour.b, bdata.colour.b, genome.colourAdoption);
			data.size = LERP(genome.initSize, bdata.size, genome.sizeAdoption);
		}
	}

	int* AcknowledgeChildBranches(int nextIndex, BranchGenome allGenome[])
	{
		int arr[3];
		arr[0] = -1;
		arr[1] = -1;
		arr[2] = -1;
		data.branchIndexes[numBranches] = nextIndex;
		if (allGenome[data.genomeIndex].branch0 > -1)
			arr[0] = allGenome[data.genomeIndex].branch0;
		if (allGenome[data.genomeIndex].branch1 > -1)
			arr[1] = allGenome[data.genomeIndex].branch1;
		if (allGenome[data.genomeIndex].branch2 > -1)
			arr[2] = allGenome[data.genomeIndex].branch2;
		return arr;
	}
private:
	int numBranches = 0;
	Branch* parent = nullptr;
};

class Plant
{
public:
	BranchGenome branchGenes[10];
	FruitGenome fruitGenes[2];

	Branch* branches = new Branch[MAX_BRANCHES];
	int totalBranches = 1;

	void CreatePlant(Vector origin, sf::RenderWindow* window)
	{
		int nextAvailibleBranch = 0;
		for(int i = 0; i < totalBranches; i++)
		{
			BranchData* out = new BranchData[3];
			if(i == 0)
			{
				branches[i].InitBranch(0, branchGenes, origin);
			}
			branches[i].RenderWholeBranch(window, branchGenes, &out);

			for (int i = 0; i < 3; i++)
			{
				if (out[i].nonBlank)
				{
					nextAvailibleBranch++;
					branches[nextAvailibleBranch].InitBranch(branches[i].AcknowledgeChildBranches(nextAvailibleBranch, branchGenes)[0],
						branchGenes, out[i].pos, out[i], false, branches[i].data.branchLayer + 1
					);
					totalBranches++;
				}
			}
		}
		window->display();
	}

	void RenderPlant(Vector origin, sf::RenderWindow* window)
	{
		branches[0].Reset(origin, branchGenes);
		for(int i = 0; i < totalBranches; i++)
		{
			BranchData* out = new BranchData[3];
			branches[i].RenderWholeBranch(window, branchGenes, &out);
			for(int j = 0; j < 3; j++)
			{
				if (out[j].nonBlank)
					branches[out[j].branchIndexes[0]].Reset(out[j].pos, branchGenes, false, out[j]);
			}
		}
	}
};