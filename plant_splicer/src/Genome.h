#pragma once
#include "SFML/Graphics.hpp"

struct Genome
{
	// INITIAL VARIABLES
	float initDir = -0.1; //up is 0
	sf::Color initColour{ 105, 105, 105 };
	float initWidth = 5; //pixels
	int length = 200;
	int lengthVariation = 40; //length = length +/- variation


	// ADOPTION, how similar the colour or size is to the branch it split from
	float colourAdoption = 0.9f;
	float widthAdoption = 0.2f;
	float dirAdoption = 0.5f;

	// BRANCH GENETICS;
	int branch0 = 1;
	int branch1 = 2; // This is the genome of the branch it links to (0> = none, 1-10 = a branch gene, 11/12 = fruit gene)
	int branch2 = 3;

	float branch0Position = 0.25f;
	float branch1Position = 0.5f; // Where along the current branch the new branch stems from
	float branch2Position = 0.75f;
};

struct BranchGenome : Genome
{
	// CHANGE VARIABLES
	sf::Color colourChange{ 1, 0, 2 }; //change in color every step
	float widthChange = -0.001f; // can be negative

	float dirChange = 0.002f;
	float randTurn = 0.04f; // will change by dirChange and also the random change between -randTurn and positive randTurn

	bool fruitGene = false;
};

struct FruitChangeGene
{
	float valueChange = 0;
	float changeOffset = 0;
	float changeWidth = 0;

	// size change is how it changes
	// change offset is where the change begins to take place
	// change width is for how long the change takes place
};

struct FruitGenome : Genome
{
	// CHANGE VARIABLES
	FruitChangeGene widthChanges[3] {};
	FruitChangeGene colourChanges[9] {}; // there are nine for the three RGB values of each colour
	FruitChangeGene directionChanges[3] {};

	bool fruitGene = true;
};