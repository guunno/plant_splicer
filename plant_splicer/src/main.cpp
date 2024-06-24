#include "SFML/Graphics.hpp"
#include "Vector2.h"
#include "Plant.h"
#include "Editor/Editor.h"
#include <thread>
#include <iostream>

#include <memory>

int main()
{
	std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 800), "Plant Splicer", sf::Style::Close);
	std::unique_ptr<Editor> editor = std::make_unique<Editor>();
	sf::Event event;
	Vector2 move(1, 0);
	Plant plant(Vector2(400.0f, 800.0f), window);

	window->clear();
	plant.Render();
	window->display();

	editor->Create();

	while (window->isOpen())
	{
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();


			if (event.type == sf::Event::MouseButtonPressed)
			{
				window->clear();
				plant.branchGenes = editor->branchGenomes;
				plant.ResetIntermediate();
				plant.InitBranches();
				plant.Render();
				window->display();
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::E)
				{
					sf::Texture screenShot;
					screenShot.create(window->getSize().x, window->getSize().y);
					screenShot.update(*window);
					screenShot.copyToImage().saveToFile("plant.png");
				}
			}
		}
		editor->Process();
	}
}
