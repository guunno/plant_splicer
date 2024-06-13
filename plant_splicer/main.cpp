#include "SFML/Graphics.hpp"
#include "Vector.h"
#include "Plant.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Plant Splicer", sf::Style::Close);
	sf::Event event;
	Vector move(1, 0);

	Plant plant;
	plant.CreatePlant({ 400, 600 }, &window);

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		plant.RenderPlant({ 400, 500 }, &(window));
		window.display();
	}
}