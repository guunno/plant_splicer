#include "SFML/Graphics.hpp"
#include "Vector2.h"
#include "Plant.h"
#include "Editor/Editor.h"
#include <thread>
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Plant Splicer", sf::Style::Close);
	sf::RenderWindow editorWindow;
	Editor editor;
	sf::Event event;
	Vector2 move(1, 0);
	Plant plant(Vector2(400.0f, 800.0f), &window);

	editor.Create(editorWindow);

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();


			if (event.type == sf::Event::MouseButtonPressed)
			{
				window.clear();
				plant.branchGenes = editor.branchGenomes;
				plant.ResetIntermediate();
				plant.InitBranches();
				plant.Render();
				window.display();
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::E)
				{
					sf::Texture scr;
					scr.create(window.getSize().x, window.getSize().y);
					scr.update(window);
					scr.copyToImage().saveToFile("plant.png");
				}
			}
		}
		editor.Process();
	}
}
