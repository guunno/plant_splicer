#pragma once

#include "Genome.h"

#include "Buffer.h"
#include "Vector2.h"

#include "BranchOrientation.h"

#include <memory>

struct FruitData
{
	int length = 2; // Length of the Fruit
	float dir = 0; // Direction of Fruit
	float width = 25; // Width of Fruit
	FloatColour colour{ 0, 0, 0 }; // Colour of Fruit

	float randomTurn = 0; // Variable amount branch rotates each step
	bool isDirPositive = 0; // Direction is Left or Right

	float colourAdoption = 0; // How much colour is inherited from the point it splits up from
	float widthAdoption = 0; // How much width is inherited from the point it splits up from

	FruitChangeGene<float> widthChanges[3]{ {0.0f}, {0.0f}, {0.0f} }; // Amount Width changes each step
	FruitChangeGene<FloatColour> colourChanges[3]{ // Amount Colour changes each step
		{ FloatColour{0, 0, 0} },
		{ FloatColour{0, 0, 0} },
		{ FloatColour{0, 0, 0} }
	};
	FruitChangeGene<float> dirChanges[3]{ {0.0f}, {0.0f}, {0.0f} }; // Amount branch rotates each step
};

// Needed to refer to Branch in Fruit File
class Branch;

class Fruit
{
public:
	Fruit() {}
	Fruit(FruitGenome& genomeData, Branch* parentBranch);
	void Create(FruitGenome& genomeData, Branch* parentBranch = nullptr);
	void Render(
		const std::unique_ptr<sf::CircleShape>& circle,
		sf::RenderWindow* window,
		const Buffer<Fruit>& allFruits,
		const BranchOrientation& offset
	) const;

public:
	FruitData data;
};