#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>

#define min_value(a, b) a < b ? a : b

struct FloatColour
{
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 0.0f;

	FloatColour() {}
	FloatColour(float r, float g, float b, float a = 255.0f) :r(r), g(g), b(b), a(a) {}
	FloatColour(const sf::Color& colour) :r(colour.r), g(colour.g), b(colour.b), a(colour.a) {}

	operator sf::Color() const
	{
		return sf::Color { uint8_t(min_value(r, 255)), uint8_t(min_value(g, 255)), uint8_t(min_value(b, 255)) };
	}

	FloatColour operator-(const FloatColour& o) const
	{
		return FloatColour { this->r - o.r, this->g - o.g, this->b - o.b };
	}
	FloatColour operator+(const FloatColour& o) const
	{
		return FloatColour { this->r + o.r, this->g + o.g, this->b + o.b };
	}
	void operator+=(const FloatColour& o)
	{
		this->r += o.r;
		this->g += o.g;
		this->b += o.b;
	}
	void operator-=(const FloatColour& o)
	{
		this->r -= o.r;
		this->g -= o.g;
		this->b -= o.b;
	}
};

struct Genome
{
	// INITIAL VARIABLES
	float initDir = -0.1f; //up is 0
	FloatColour initColour{ 1, 1, 1 };
	float initWidth = 5; //pixels
	int length = 200;
	int lengthVariation = 40; //length = length +/- variation


	// ADOPTION, how similar the colour or size is to the branch it split from
	float colourAdoption = 0.0f;
	float widthAdoption = 1.0f;
	float dirAdoption = 0.0f;

	// BRANCH GENETICS;
	int branch0 = 1;
	int branch1 = 2; // This is the genome of the branch it links to (< 0 = no branch, 1-10 = a branch gene, 11/12 = fruit gene)
	int branch2 = 3;

	float branch0Position = 0.5f;
	float branch1Position = 0.75f; // Where along the current branch the new branch stems from (if out of range, then no branch is made)
	float branch2Position = 0.99f;
};

struct BranchGenome : Genome
{
	// CHANGE VARIABLES
	FloatColour colourChange{ 1, 1, 1 }; //change in color every step
	float widthChange = -0.005f; // can be negative

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