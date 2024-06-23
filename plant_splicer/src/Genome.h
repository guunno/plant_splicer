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
		return sf::Color { uint8_t(min_value(r, 255)), uint8_t(min_value(g, 255)), uint8_t(min_value(b, 255)), 255Ui8 };
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

	FloatColour operator*(float sf) const
	{
		return FloatColour{ r * sf, g * sf, b * sf };
	}
};

struct Genome
{
	Genome() { initDir = (rand() % 100 - 49) / 50.0f; }

	// INITIAL VARIABLES
	float initDir = 0.5f; //up is 0 // (rand() % 100) / 100 - 0.5f
	FloatColour initColour{ 60, 30, 0 };
	float initWidth = 12; //pixels
	int length = 100;
	int lengthVariation = 40; //length = length +/- variation


	// ADOPTION, how similar the colour or size is to the branch it split from
	float colourAdoption = 1.0f;
	float widthAdoption = 1.0f;
	float dirAdoption = 0.0f;
};

struct BranchGenome : Genome
{
	// CHANGE VARIABLES
	FloatColour colourChange{ 0.5, 0.2, 0.1 }; //change in color every step
	float widthChange = -0.016f; // can be negative

	float dirChange = 0.003f;
	float randTurn = 0.04f; // will change by dirChange and also the random change between -randTurn and positive randTurn

	// BRANCH GENETICS;
	int branch0 = 2;
	int branch1 = 11; // This is the genome of the branch it links to (< 0 = no branch, 0-9 = a branch gene, 10/11 = fruit gene)
	int branch2 = -1;

	float branch0Position = 0.5f;
	float branch1Position = 0.75f; // Where along the current branch the new branch stems from (if out of range, then no branch is made)
	float branch2Position = 0.999999999f;
};

template <typename ChangeType>
struct FruitChangeGene
{
	ChangeType valueChange; // How the value changes
	float changeOffset = 0; // Where the change takes place
	float changeLength = 1; // For how long the change takes place
};

struct FruitGenome : Genome
{
	// CHANGE VARIABLES
	FruitChangeGene<float> widthChanges[3] {};
	FruitChangeGene<FloatColour> colourChanges[3] {};
	FruitChangeGene<float> directionChanges[3] {};

	float randTurn = 0.04f; // will change by dirChange and also the random change between -randTurn and positive randTurn
};