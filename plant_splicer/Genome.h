#pragma once
#include "SFML/Graphics.hpp"

struct Genome
{
public:

	// INITIAL VARIABLES
	float initDir = 0; //up
	sf::Color initColour{105, 105, 105};
	float initSize = 5; //pixels
	int length = 200;
	int lengthVariation = 120; //length = length +/- variation


	// ADOPTION, how similar the colour or size is to the branch it split from
	float colourAdoption = 1;
	float sizeAdoption = 0.5;
	float dirAdoption = 0.5;

	// BRANCH GENETICS;
	int branch0 = 0;
	int branch1 = 0; // This is the genome of the branch it links to (0> = none, 1-10 = a branch gene, 11/12 = fruit gene)
	int branch2 = 0;

	float branch0Position = 0.25;
	float branch1Position = 0.5; // Where along the current branch the new branch stems from
	float branch2Position = 0.75;
};

struct BranchGenome : Genome
{
public:
	// CHANGE VARIABLES
	sf::Color colourChange{ 1, 0, 1 }; //change in color every step
	float sizeChange = 0.01; // can be negative
	float dirChange = .001;
	float randTurn = 0.01; // will change by dirChange and also the random change between -randTurn and positive randTurn

	bool fruitGene = false;
};

struct FruitChangeGene
{
public:
	float valueChange = 0;
	float changeOffset = 0;
	float changeWidth = 0;

	// size change is how it changes
	// change offset is where the change begins to take place
	// change width is for how long the change takes place
};

struct FruitGenome : Genome
{
public:
	// CHANGE VARIABLES
	FruitChangeGene sizeChanges[3];
	FruitChangeGene colourChanges[9]; // there are nine for the three RGB values of each colour
	FruitChangeGene directionChanges[3];

	bool fruitGene = true;
};