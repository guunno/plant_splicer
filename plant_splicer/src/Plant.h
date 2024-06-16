#pragma once

#include "Genome.h"

#include "Buffer.h"
#include "Vector2.h"

#include <memory>

#define OG_CODE false
#if !OG_CODE

#define LERP(a, b, k) a * k + b * (1 - k)

struct BranchData
{
	Vector2 pos{ 0, 0 }; // Position of Branch
	int length = 2; // Length of the Branch
	int branchingPoints[3] {}; // Where Along Each Branch do Child Branches begin
	int branchIndexes[3] {-1, -1, -1}; // Genome Indices of Child Branches
	int numBranches = 0; // Number of Child Branches
	int branchLayer = 0; // How many Parent Branches of it exists
	int genomeIndex = 0; // Genome Index
	bool isDirPositive = 0; // Direction is Left or Right
	
	float widthChange = 0; // Amount Width changes each step
	sf::Color colourChange {0, 0, 0}; // Amount Colour changes each step
	float dirChange = 0; // Amount branch rotates each step
	float randomTurn = 0; // Variable amount branch rotates each step

	float dir = 0; // Direction of Branch
	float width = 25; // Width of Branch
	sf::Color colour { 0, 0, 0 }; // Coour of Branch
};

class Branch
{
public:
	struct Orientation
	{
		Vector2 pos{ 0, 0 }; // Position of Branch
		float dir = 0; // Direction of Branch
		sf::Color colour{ 0, 100, 0 }; // Colour of Branch
	};

public:
	Branch() {}
	Branch(BranchGenome& genomeData, Branch* parentBranch = nullptr, int branchLayer = 0);
	void Create(BranchGenome& genomeData, Branch* parentBranch = nullptr, int branchLayer = 0);
	Orientation RenderBranch(const std::unique_ptr<sf::CircleShape>& circle, sf::RenderWindow* window, const Branch::Orientation& offset);

public:
	BranchData data;
	uint32_t childIndices[3] {0, 0, 0}; // Child Index of 0 would mean no child for that index

private:
	void RenderBranchSegment(const std::unique_ptr<sf::CircleShape>& circle, Vector2 position, float width, sf::Color colour, sf::RenderWindow* window);
};

class Plant
{
public:
	Plant() {}
	Plant(Vector2 pos, sf::RenderWindow* window);

	void Render(uint32_t BranchIndex = 0, const Branch::Orientation& orientation = Branch::Orientation());
	
public:
	Vector2 pos;
	sf::RenderWindow* window = nullptr;

	static const uint32_t BRANCH_COUNT = 10;
	static const uint32_t FRUIT_COUNT = 2;
	Buffer<BranchGenome> branchGenes { BRANCH_COUNT };
	Buffer<FruitGenome> fruitGenes { FRUIT_COUNT };

private:
	uint32_t GetBranchCount(uint32_t genomeIdx = 0, uint8_t recursionDepth = 0);
	uint32_t InitBranches(uint32_t genomeIdx = 0, uint8_t recursionDepth = 0);

private:
	std::unique_ptr<sf::CircleShape> m_BranchRenderShape = std::make_unique<sf::CircleShape>();
	uint32_t m_IntermediateBranchCount = 0;
	Buffer<Branch> m_Branches{};
};

#else

#define min(a, b) a < b ? a : b

const int MAX_BRANCHES = 1000;

struct BranchData
{
public:
	Vector2 pos = { 0, 0 };
	float dir = 0;
	float size = 25;
	sf::Color colour = { 0, 0, 0 };
	int setLength = 2;
	int branchingPoints[3];
	int branchIndexes[3];
	int numBranches = 0;
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

	void InitBranch(int genome0, BranchGenome allGenomes[], Vector2 createPoint, Branch* lastBranch = nullptr, bool isFirstBranch = true, int branchLayerNew = 0)
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
			data.size = genome.initWidth;
		}
		else
		{
			data.dir = abs(genome.initDir - lastBranch->data.dir) * genome.dirAdoption + min(genome.initDir, lastBranch->data.dir);
			data.colour.r = abs(genome.initColour.r - lastBranch->data.colour.r) * genome.colourAdoption + min(genome.initColour.r, lastBranch->data.colour.r);
			data.colour.g = abs(genome.initColour.g - lastBranch->data.colour.g) * genome.colourAdoption + min(genome.initColour.g, lastBranch->data.colour.g);
			data.colour.b = abs(genome.initColour.b - lastBranch->data.colour.b) * genome.colourAdoption + min(genome.initColour.b, lastBranch->data.colour.b);
			data.size = abs(genome.initWidth - lastBranch->data.size) * genome.widthAdoption + min(genome.initWidth, lastBranch->data.size);
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
		BranchGenome& genome = allGenome[data.genomeIndex];
		data.dir += (genome.dirChange + ((((rand() % 201) - 100) / 100.0f) * genome.randTurn)) * ((data.dirPositive * 2) - 1);
		data.pos += Vector2(0, -1).rotateNew(data.dir);
		data.colour += genome.colourChange;
		data.size += genome.sizeChange;
		if (data.branchLayer < 10)
		{
			if (step == data.branchingPoints[0])
			{
				step++;
				// std::cout << data.pos.y << "\n";
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

	void Reset(Vector2 origin, BranchGenome allGenome[])
	{
		BranchGenome& genome = allGenome[data.genomeIndex];
		step = 0;
		data.pos = origin;
		data.dir = genome.initDir;
		data.colour = genome.initColour;
		data.size = genome.initWidth;
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

	Branch* branches;
	int totalBranches = 1;

	void CreatePlant(Vector2 origin, sf::RenderWindow* window)
	{
		branches = new Branch[MAX_BRANCHES];

		int nextAvailibleBranch = 0;
		for (int i = 0; i < totalBranches; i++)
		{
			BranchData out;
			if (i == 0)
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

	void RenderPlant(Vector2 origin, sf::RenderWindow* window)
	{
		for (int i = 0; i < totalBranches; i++)
		{
			branches[i].Reset(origin, branchGenes);
			branches[i].RenderWholeBranch(window, branchGenes);
		}
	}
};

#endif
