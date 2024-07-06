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
	float initDir = 0.0f; //up is 0 (radians)
	float dirSpread = 0.0f; // spread in radians
	int spreadMaxDistanceEff = 50; // how close branches have to be to spread
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
	FloatColour colourChange{ 0.5f, 0.2f, 0.1f }; //change in color every step
	float widthChange = -0.016f; // can be negative

	float dirChange = 0.003f;
	float randTurn = 0.04f; // will change by dirChange and also the random change between -randTurn and positive randTurn

	// BRANCH GENETICS;
	int branch0 = -1;
	int branch1 = -1; // This is the genome of the branch it links to (< 0 = no branch, 1-10 = a branch gene, 11/12 = fruit gene)
	int branch2 = -1;
	int branch3 = -1;
	int branch4 = -1;
	int branch5 = -1;

	float branch0Position = 1;
	float branch1Position = 1; // Where along the current branch the new branch stems from (if out of range, then no branch is made)
	float branch2Position = 1;
	float branch3Position = 1;
	float branch4Position = 1;
	float branch5Position = 1;

	int rBranch0 = -1; // Branches that only spawn at the end of a recursion limit (e.g if genome 0 creates branch with genome0, it will repeat until it hits the limit),
	int rBranch1 = -1; // Always stems off the very end of the branch
	int rBranch2 = -1; // can help reduce genome usage
};