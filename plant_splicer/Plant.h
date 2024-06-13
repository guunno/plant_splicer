#pragma once
#include "Genome.h"
#include "Vector.h"

#define min(a, b) a < b ? a : b

class Branch
{
public:
	Vector currentPos;
	float currentDir;
	float currentSize;
	sf::Color currentColour;
	int setLength;
	int branchingPoints[3];
	int step = 0;
	int branchLayer = 0;
	int genomeIndex;

	void InitBranch(int genome0, BranchGenome* allGenomes[], Vector createPoint, Branch* lastBranch = nullptr, bool isFirstBranch = true, int branchLayer = 0)
	{
		currentPos = createPoint;
		genomeIndex = genome0;
		BranchGenome* genome = allGenomes[genomeIndex];

		setLength = genome->length + (genome->lengthVariation * (((rand() % 201) - 100) / 100.0f));
		branchingPoints[0] = floor(genome->branch0Position * setLength);
		branchingPoints[1] = floor(genome->branch1Position * setLength);
		branchingPoints[2] = floor(genome->branch2Position * setLength);

		if (isFirstBranch)
		{
			currentDir = genome->initDir;
			currentColour = genome->initColour;
			currentSize = genome->initSize;
		}
		else
		{
			currentDir = abs(genome->initDir - lastBranch->currentDir) * genome->dirAdoption + min(genome->initDir, lastBranch->currentDir);
			currentColour.r = abs(genome->initColour.r - lastBranch->currentColour.r) * genome->colourAdoption + min(genome->initColour.r, lastBranch->currentColour.r);
			currentColour.g = abs(genome->initColour.g - lastBranch->currentColour.g) * genome->colourAdoption + min(genome->initColour.g, lastBranch->currentColour.g);
			currentColour.b = abs(genome->initColour.b - lastBranch->currentColour.b) * genome->colourAdoption + min(genome->initColour.b, lastBranch->currentColour.b);
			currentSize = abs(genome->initSize - lastBranch->currentSize) * genome->sizeAdoption + min(genome->initSize, lastBranch->currentSize);
		}
	}

	void DrawBranch(sf::RenderWindow* window)
	{
		sf::CircleShape circle;
		circle.setFillColor(currentColour);
		circle.setRadius(currentSize);
		circle.setPosition({ currentPos.x, currentPos.y });

		window->draw(circle);
	}

	void UpdateData(BranchGenome* allGenome[])
	{
		BranchGenome* genome = allGenome[genomeIndex];
		if(branchLayer < 2)
		{
			if (step == branchingPoints[0])
			{
				n = new Branch;
				n->InitBranch(genome->branch0, allGenome, currentPos, this, false, branchLayer + 1);
				myBranches[numBranches] = n;
				numBranches++;
				std::cout <<branchLayer << "\n";
			}
		}
		currentDir += genome->dirChange + ((((rand() % 201) - 100) / 100.0f)*genome->randTurn);
		Vector v = { 0, -1 };
		currentPos += v.rotateNew(currentDir);
		currentColour += genome->colourChange;
		currentSize += genome->sizeChange;
		step++;
	}

	void RenderWholeBranch(sf::RenderWindow* window, BranchGenome* allGenome[])
	{
		for (int i = 0; i < setLength; i++)
		{
			DrawBranch(window);
			UpdateData(allGenome);
			if (numBranches == 0)
			{
				continue;
			}
			//myBranches[0]->RenderWholeBranch(window, allGenome);
		}
	}

	void Reset(Vector origin, BranchGenome* allGenome[])
	{
		BranchGenome* genome = allGenome[genomeIndex];
		step = 0;
		currentPos = origin;
		currentDir = genome->initDir;
		currentColour = genome->initColour;
		currentSize = genome->initSize;
	}
private:
	Branch* n;
	Branch* myBranches[3];
	FruitGenome* plantFruitGenomes[2];
	int numBranches = 0;
	Branch* parent = nullptr;
};

class Plant
{
public:
	BranchGenome* branchGenes[10];
	FruitGenome fruitGenes[2];

	Branch test;
	void CreatePlant(Vector origin, sf::RenderWindow* window)
	{
		for (int i = 0; i < 10; i++)
		{
			BranchGenome n;
			branchGenes[i] = &n;
		}
		test.InitBranch(0, branchGenes, origin);
		test.RenderWholeBranch(window, branchGenes);
	}

	void RenderPlant(Vector origin, sf::RenderWindow* window)
	{
		test.Reset(origin, branchGenes);
		test.RenderWholeBranch(window, branchGenes);
	}
};