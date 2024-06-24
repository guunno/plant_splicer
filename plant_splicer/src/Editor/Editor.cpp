#include "Editor.h"
#include <sstream>

void Editor::Create(sf::RenderWindow& window)
{
	m_window = &window;
	window.create(sf::VideoMode(500, 800), "Editor", sf::Style::Titlebar);
	font.loadFromFile("src/Editor/verdana.ttf");
	text.setFont(font);
	text.setString("Editor");
	text.setCharacterSize(15);
	for (int i = 0; i < 10; i++)
	{
		managers[i].LinkButtons(branchGenomes[i]);
	}
}

void Editor::Process()
{
	if (!m_window->isOpen())
		return;


	sf::Event ev;
	while(m_window->pollEvent(ev))
	{
		if (ev.type == sf::Event::MouseButtonPressed && m_window->hasFocus())
		{
			managers[currBranch].ActivateButton((sf::Mouse::getPosition() - m_window->getPosition()).y, currPage);
		}
		if (ev.type == sf::Event::KeyPressed)
		{
			managers[currBranch].ProcessInput(ev.key.code);
			if (ev.key.code == sf::Keyboard::Left)
			{	
				if (currBranch > 0)
					currBranch -= 1;
			}
			if (ev.key.code == sf::Keyboard::Right)
			{
				if (currBranch < 9)
					currBranch += 1;
			}
		}
	}

	m_window->clear();

	text.setString(sf::String("(<- -> keys) Editing Branch num ") + char('0' + currBranch));
	text.setPosition(0, 0);
	m_window->draw(text);

	for (int i = 0; i < 28; i++)
	{
		text.setString(managers[currBranch].buttons[i].label);
		text.setPosition(0, (i + 1) * 30);
		m_window->draw(text);
		if (managers[currBranch].buttons[i].floorToInt)
		{
			std::ostringstream oss;
			oss << *managers[currBranch].buttons[i].intVal;
			text.setString(oss.str());
			if (managers[currBranch].activeButton == &managers[currBranch].buttons[i])
				text.setString(managers[currBranch].stringifiedNum);
			text.setPosition(250, (i + 1) * 30);
			m_window->draw(text);
		}
		else
		{
			std::ostringstream oss;
			oss << *managers[currBranch].buttons[i].floatVal;
			text.setString(oss.str());
			if (managers[currBranch].activeButton == &managers[currBranch].buttons[i])
				text.setString(managers[currBranch].stringifiedNum);
			text.setPosition(250, (i + 1) * 30);
			m_window->draw(text);
		}
	}

	m_window->display();

};