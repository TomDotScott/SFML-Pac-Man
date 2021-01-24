#pragma once
#include <string>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

class Text
{
public:
	Text(const std::string& text, const unsigned size, const sf::Vector2f position, const bool visible = true);
	
	void Render(sf::RenderWindow& window) const;

	void SetString(const std::string& string);

	const sf::Vector2f& GetPosition() const;
	void SetPosition(const sf::Vector2f& position);

	void SetFont(const sf::Font& font);

	bool GetVisible() const;
	void SetVisible(bool visibility);

private:
	sf::Text m_text;
	bool m_visible;
};
