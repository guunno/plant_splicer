#include "SFML/Graphics.hpp"
#include "Vector2.h"
#include "Plant.h"
#include "Editor/Editor.h"
#include <chrono>

#include "Editor/FileDialoguePrompt.h"

#include <memory>

#ifdef DIST
#define main WinMain
#endif

#define IS_CONVERTING_FILE 0

int main()
{
#if IS_CONVERTING_FILE
	std::string path;
	AskUserForPath(path, FileSearchFilter{"Tree Files!", "*.genome"});
	std::cout << "Converting `" << path << "` " << (FileManager::ConvertFromHeaderless(path) ? "Succeeded" : "Failed") << "!\n";
	return 0;
#endif

	std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 800), "Render", sf::Style::Close);
	std::unique_ptr<Editor> editor = std::make_unique<Editor>();

	Vector2 move(1, 0);
	uint32_t t = (uint32_t)std::chrono::system_clock::now().time_since_epoch().count();
	Plant plant(Vector2(400.0f, 800.0f), window, t);

	editor->Create();

	window->clear((editor->settings.mainBG));
	plant.Render();
	window->display();

	sf::Event mainWindowEvent;
	while (window->isOpen())
	{
		while (window->pollEvent(mainWindowEvent))
		{
			if (mainWindowEvent.type == sf::Event::Closed)
				window->close();

			if (mainWindowEvent.type == sf::Event::MouseButtonPressed)
			{
				window->clear(editor->settings.mainBG);
				plant.branchGenes = editor->branchGenomes;
				t = (uint32_t)std::chrono::system_clock::now().time_since_epoch().count();
				plant.InitAllBranches(t);
				plant.Render();
				window->display();
			}

			if (mainWindowEvent.type == sf::Event::KeyPressed)
			{
				if (mainWindowEvent.key.code == sf::Keyboard::S)
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
