#pragma once

#include "Genome.h"

#include "Buffer.h"
#include "Vector2.h"

#include "BranchOrientation.h"

#include <memory>

struct BranchData
{
	int length = 2; // Length of the Branch
	int branchingPoints[3]{ 0, 0, 0 }; // Where Along Each Branch do Child Branches begin
	int branchIndexes[3]{ -1, -1, -1 }; // Genome Indices of Child Branches
	int numBranches = 0; // Number of Child Branches
	bool isDirPositive = 0; // Direction is Left or Right

	float widthChange = 0; // Amount Width changes each step
	FloatColour colourChange{ 0, 0, 0 }; // Amount Colour changes each step
	float dirChange = 0; // Amount branch rotates each step
	float widthAdoption = 0; // How much colour is inherited from the point it splits up from
	float colourAdoption = 0; // How much colour is inherited from the point it splits up from

	float randomTurn = 0; // Variable amount branch rotates each step

	float dir = 0; // Direction of Branch
	float width = 25; // Width of Branch
	FloatColour colour{ 0, 0, 0 }; // Colour of Branch
};

// Needed to refer to Fruit in Branch File
class Fruit;

class Branch
{
public:
	Branch() {}
	Branch(BranchGenome& genomeData, Branch* parentBranch = nullptr);
	void Create(BranchGenome& genomeData, Branch* parentBranch = nullptr);
	void Render(
		const std::unique_ptr<sf::CircleShape>& circle,
		sf::RenderWindow* window, const Buffer<Branch>& allBranches, const Buffer<Fruit>& allFruit,
		const BranchOrientation& offset, uint32_t recursionDepth = 0
	) const;

public:
	BranchData data;
	uint32_t childIndices[3]{ 0, 0, 0 }; // Child Index of 0 would mean no child for that index
	bool isChildABranch[3]{ true, true, true }; // Is child a branch or Fruit?
};