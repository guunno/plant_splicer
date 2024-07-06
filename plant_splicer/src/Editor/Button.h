#pragma once
#include "../Vector2.h"
#include "../Buffer.h"
#include "../Genome.h"
#include <SFML/System.hpp>
#include <map>
#include <sstream>

inline std::string floatToString(float floatNumber)
{
	std::stringstream ss;
	ss << floatNumber;
	return ss.str();
}

#define NUM_GENES_IN_BRANCH 36

class ValueEditButton
{
public:
	union { float* floatVal = nullptr; int* intVal; };
	float max = 100;
	float min = -100;
	int yPos = 0;
	int page = 0;
	bool floorToInt = false;
	sf::String label;

	void InitButton(const sf::String& name, int buttonIndex, float* value, float maxN = 1000, float minN = -1000);
	void InitButton(const sf::String& name, int buttonIndex, int* value, float maxN = 1000, float minN = -1000);

private:
	void Initialise(const sf::String& name, int buttonIndex, float maxN = 1000, float minN = -1000);
};

class StringEditButton
{
public:
	sf::String* value;
	int yPos = 0;
	int page = 0;
	sf::String label;

	void InitButton(const sf::String& name, int buttonIndex, sf::String* valueEditText);
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
	float zoom{1};
};

struct SplicingSettings
{
	sf::String loadPath = "";
	sf::String splice0Path = "(CLICK TO CHANGE)";
	sf::String splice1Path = "(CLICK TO CHANGE)";
};

class SettingsButtonManager 
{
public:
	Buffer<ValueEditButton> buttons{7};
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
