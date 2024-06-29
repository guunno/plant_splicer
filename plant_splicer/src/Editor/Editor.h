#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "../Genome.h"
#include "../FileManager.h"

class Editor
{
public:
	std::unique_ptr<sf::RenderWindow> m_window;

	sf::Font font;
	sf::Text text;
	Buffer<BranchGenome> branchGenomes{ 10 };
	Buffer<SoloBranchGenomeButtonManager> managers{ 10 };
	SettingsButtonManager settingsManager;
	SplicingButtonManager splicingManager;
	int currPage = 0;
	int currBranch = 0;

	Settings settings;
	SplicingSettings splicingSettings;

	enum Mode {Menu, BranchEditor, Constants, Splicing};

	Mode mode = Menu;

	void Create();
	void DeActivateAll();
	bool IsAnythingActive();
	
	void Process();
private:
	FileManager m_FileManager;
};