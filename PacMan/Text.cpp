#include "Text.h"

Text::Text(const std::string& text, const unsigned size, const sf::Vector2f position, const bool visible) :
	m_visible(visible)
{
	m_text.setString(text);
	m_text.setCharacterSize(size);
	m_text.setPosition(position);
	m_text.setFillColor(sf::Color::White);
}

void Text::Render(sf::RenderWindow& window) const
{
	if (m_visible)
	{
		window.draw(m_text);
	}
}

void Text::SetString(const std::string& string)
{
	if (m_text.getString() != string) m_text.setString(string);
}

const sf::Vector2f& Text::GetPosition() const
{
	return m_text.getPosition();
}

void Text::SetPosition(const sf::Vector2f& position)
{
	m_text.setPosition(position);
}

void Text::SetFont(const sf::Font& font)
{
	m_text.setFont(font);
}

bool Text::GetVisible() const
{
	return m_visible;
}

void Text::SetVisible(const bool visibility)
{
	m_visible = visibility;
}
