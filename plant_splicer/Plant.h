#pragma once
#include "Genome.h"
#include "Vector.h"

#define min(a, b) a < b ? a : b

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

	void InitBranch(int genome0, BranchGenome allGenomes[], Vector createPoint, Branch* lastBranch = nullptr, bool isFirstBranch = true, int branchLayerNew = 0)
	{
		data.pos = createPoint;
		data.genomeIndex = genome0;
		BranchGenome genome = allGenomes[data.genomeIndex];
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
			data.dir = abs(genome.initDir - lastBranch->data.dir) * genome.dirAdoption + min(genome.initDir, lastBranch->data.dir);
			data.colour.r = abs(genome.initColour.r - lastBranch->data.colour.r) * genome.colourAdoption + min(genome.initColour.r, lastBranch->data.colour.r);
			data.colour.g = abs(genome.initColour.g - lastBranch->data.colour.g) * genome.colourAdoption + min(genome.initColour.g, lastBranch->data.colour.g);
			data.colour.b = abs(genome.initColour.b - lastBranch->data.colour.b) * genome.colourAdoption + min(genome.initColour.b, lastBranch->data.colour.b);
			data.size = abs(genome.initSize - lastBranch->data.size) * genome.sizeAdoption + min(genome.initSize, lastBranch->data.size);
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

	bool UpdateData(BranchGenome allGenome[])
	{
		BranchGenome genome = allGenome[data.genomeIndex];
		data.dir += (genome.dirChange + ((((rand() % 201) - 100) / 100.0f) * genome.randTurn)) * ((data.dirPositive * 2) - 1);
		Vector v = { 0, -1 };
		data.pos += v.rotateNew(data.dir);
		data.colour += genome.colourChange;
		data.size += genome.sizeChange;
		if (data.branchLayer < 10)
		{
			if (step == data.branchingPoints[0])
			{
				step++;
				std::cout << data.pos.y << "\n";
				return true;
			}
		}
		step++;
		return false;
	}

	BranchData RenderWholeBranch(sf::RenderWindow* window, BranchGenome allGenome[])
	{
		BranchData dataToReturn;
		for (int i = 0; i < data.setLength; i++)
		{
			DrawBranch(window);
			if (UpdateData(allGenome) == true)
			{
				dataToReturn = data;
			}
		}
		return dataToReturn;
	}

	void Reset(Vector origin, BranchGenome allGenome[])
	{
		BranchGenome genome = allGenome[data.genomeIndex];
		step = 0;
		data.pos = origin;
		data.dir = genome.initDir;
		data.colour = genome.initColour;
		data.size = genome.initSize;
	}

	int AcknowledgeChildBranches(int nextIndex, BranchGenome allGenome[])
	{
		data.branchIndexes[numBranches] = nextIndex;
		numBranches++;

		return allGenome[data.genomeIndex].branch0;
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

	Branch branches[MAX_BRANCHES];
	int totalBranches = 1;

	void CreatePlant(Vector origin, sf::RenderWindow* window)
	{
		int nextAvailibleBranch = 0;
		for(int i = 0; i < totalBranches; i++)
		{
			BranchData out;
			if(i == 0)
			{
				branches[i].InitBranch(0, branchGenes, origin);
			}
			out = branches[i].RenderWholeBranch(window, branchGenes);


			if (out.nonBlank)
			{
				nextAvailibleBranch++;
				branches[nextAvailibleBranch].InitBranch(branches[i].AcknowledgeChildBranches(nextAvailibleBranch, branchGenes),
					branchGenes, out.pos, &branches[i], false, branches[i].data.branchLayer + 1
				);
				totalBranches++;
			}
		}
		window->display();
	}

	void RenderPlant(Vector origin, sf::RenderWindow* window)
	{
		branches[0].Reset(origin, branchGenes);
		for(int i = 0; i < totalBranches; i++)
		{
			branches[i].RenderWholeBranch(window, branchGenes);
		}
	}
};