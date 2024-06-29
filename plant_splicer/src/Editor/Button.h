#pragma once
#include "../Vector2.h"
#include "../Buffer.h"
#include "../Genome.h"
#include <SFML/System.hpp>

#define NUM_GENES_IN_BRANCH 33

class ValueEditButton
{
public:
	union { float* floatVal; int* intVal; };
	float max = 100;
	float min = -100;
	int yPos = 0;
	int page = 0;
	bool floorToInt = false;
	sf::String label;

	void InitButton(sf::String name, int buttonIndex, bool floorToIntN = false, float maxN = 1000, float minN = -1000);
	void PointButton(float& value);
	void PointButton(int& value);

};

class StringEditButton
{
public:
	sf::String* value;
	int yPos = 0;
	int page = 0;
	sf::String label;

	void InitButton(sf::String name, int buttonIndex);
	void PointButton(sf::String& pointer);

};


class SoloBranchGenomeButtonManager 
{
public:
	Buffer<ValueEditButton> buttons{NUM_GENES_IN_BRANCH};
	ValueEditButton* activeButton = nullptr;
	sf::String stringifiedNum = "";

	void LinkButtons(BranchGenome& linkedGenome);
	void ActivateButton(int mouseX, int currPage);
	void ProcessInput(sf::Keyboard::Key key);
};

struct Settings
{
	FloatColour editorBG{ 2, 0, 5 };
	FloatColour mainBG{ sf::Color::Black };
};

struct SplicingSettings
{
	sf::String loadPath;
	sf::String splice0Path;
	sf::String splice1Path;
};

class SettingsButtonManager 
{
public:
	Buffer<ValueEditButton> buttons{6};
	ValueEditButton* activeButton = nullptr;
	sf::String stringifiedNum = "";

	void LinkButtons(Settings& editor);
	void ActivateButton(int mouseX, int currPage);
	void ProcessInput(sf::Keyboard::Key key);
};

class SplicingButtonManager
{
public:
	Buffer<StringEditButton> buttons{ 3 };
	StringEditButton* activeButton = nullptr;
	sf::String stringifiedNum = "";

	void LinkButtons(SplicingSettings& editor);
	void ActivateButton(int mouseX, int currPage);
	void ProcessInput(char key);
};
