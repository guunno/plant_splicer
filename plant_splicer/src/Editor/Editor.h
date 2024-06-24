#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "../Genome.h"

class Editor
{
public:
	std::unique_ptr<sf::RenderWindow> m_window;

	sf::Font font;
	sf::Text text;
	Buffer<BranchGenome> branchGenomes{ 10 };
	Buffer<SoloBranchGenomeButtonManager> managers{ 10 };
	int currPage = 0;
	int currBranch = 0;

	void Create();

	void Process();
};