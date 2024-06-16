#include "SFML/Graphics.hpp"
#include "Vector2.h"
#include "Plant.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Plant Splicer", sf::Style::Close);
	sf::Event event;
	Vector2 move(1, 0);

#if OG_CODE
	Plant plant;
	plant.CreatePlant({ 400, 600 }, &window);
#else
	Plant plant(Vector2(400.0f, 600.0f), &window);
#endif

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed)
			{
				window.clear();
				plant.Render();
				window.display();
			}
		}
	}
}
