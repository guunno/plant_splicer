#include "SFML/Graphics.hpp"
#include "Vector2.h"
#include "Plant.h"
#include "Editor/Editor.h"
#include <thread>
#include <iostream>

#include <memory>

int main()
{
	std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 800), "Render", sf::Style::Close);
	std::unique_ptr<Editor> editor = std::make_unique<Editor>();
	sf::Event event;
	Vector2 move(1, 0);
	Plant plant(Vector2(400.0f, 800.0f), window);

	editor->Create();

	window->clear((editor->settings.mainBG));
	plant.Render();
	window->display();


	unsigned int t = std::chrono::system_clock::now().time_since_epoch().count();

	while (window->isOpen())
	{
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				window->clear(editor->settings.mainBG);
				plant.branchGenes = editor->branchGenomes;
				t = std::chrono::system_clock::now().time_since_epoch().count();
				plant.InitAllBranches(t);
				plant.Render();
				window->display();
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::E)
				{
					window->clear(editor->settings.mainBG);
					plant.InitAllBranches(t);
					plant.Render();
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
