#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Game.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML Pac-Man");

    Game game;

    sf::Clock clock;
	
	// Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        game.Input();

    	// Game tick every 100 ms
        while (clock.getElapsedTime() >= sf::milliseconds(200))
        {
            game.Update();
            clock.restart();
        }
    	
        window.clear();
        game.Render(window);
        window.display();
    }
    return EXIT_SUCCESS;
}
