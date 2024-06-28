#include "Editor.h"
#include <sstream>

#include <memory>

void Editor::Create()
{
	m_window = std::make_unique<sf::RenderWindow>();
	m_window->create(sf::VideoMode(500, 470), "Editor", sf::Style::Titlebar);
	font.loadFromFile("src/Editor/verdana.ttf");
	text.setFont(font);
	text.setString("Editor");
	for (int i = 0; i < 10; i++)
	{
		managers[i].LinkButtons(branchGenomes[i]);
	}
	settingsManager.LinkButtons(settings);
}

void Editor::DeActivateAll()
{
	for (int i = 0; i < 10; i++)
	{
		managers[i].activeButton = nullptr;
		managers[i].stringifiedNum = "";
	}
	settingsManager.activeButton = nullptr;
	settingsManager.stringifiedNum = "";
}

void Editor::Process()
{
	if (!m_window->isOpen())
		return;


	sf::Event ev;
	while(m_window->pollEvent(ev))
	{
		if (mode != Menu)
		{
			if (ev.type == sf::Event::KeyPressed)
				if (ev.key.code == sf::Keyboard::Escape)
				{
					mode = Menu;
					DeActivateAll();
				}
		}

		if(mode == BranchEditor)
		{
			if (ev.type == sf::Event::MouseButtonPressed && m_window->hasFocus())
			{
				managers[currBranch].ActivateButton((sf::Mouse::getPosition() - m_window->getPosition()).y + (30 * 15 * currPage), currPage);
			}
			if (ev.type == sf::Event::KeyPressed)
			{
				managers[currBranch].ProcessInput(ev.key.code);
				if (ev.key.code == sf::Keyboard::Left)
				{
					if (currBranch > 0)
						currBranch -= 1;
					currPage = 0;
					DeActivateAll();
				}
				if (ev.key.code == sf::Keyboard::Right)
				{
					if (currBranch < 9)
						currBranch += 1;
					currPage = 0;
					DeActivateAll();
				}
				if (ev.key.code == sf::Keyboard::Up)
				{
					if (currPage < 2)
						currPage += 1;
					DeActivateAll();
				}
				if (ev.key.code == sf::Keyboard::Down)
				{
					if (currPage > 0)
						currPage -= 1;
					DeActivateAll();
				}
			}
		}
		if (mode == Constants)
		{
			if (ev.type == sf::Event::MouseButtonPressed && m_window->hasFocus())
			{
				settingsManager.ActivateButton((sf::Mouse::getPosition() - m_window->getPosition()).y + (30 * 15 * currPage), currPage);
			}
			if (ev.type == sf::Event::KeyPressed)
			{
				settingsManager.ProcessInput(ev.key.code);
				if (ev.key.code == sf::Keyboard::Up)
				{
					if (currPage < 1)
						currPage += 1;
					DeActivateAll();
				}
				if (ev.key.code == sf::Keyboard::Down)
				{
					if (currPage > 0)
						currPage -= 1;
					DeActivateAll();
					
				}
			}
		}
		if (mode == Menu)
		{
			if (ev.type == sf::Event::MouseButtonPressed)
			{
				int mY = (sf::Mouse::getPosition() - m_window->getPosition()).y;
				if (180 < mY && mY < 240)
				{
					mode = BranchEditor;
				}

				if (280 < mY && mY < 340)
				{
					mode = Constants;
				}
			}
		}
	}

	m_window->clear(settings.editorBG);

	if (mode == Menu)
	{
		text.setCharacterSize(20);

		text.setString("Editor Menu");
		text.setPosition(250 -
			(text.getLocalBounds().width / 2), 20);
		m_window->draw(text);

		text.setString("Genetics Editor");
		text.setPosition(250 -
			(text.getLocalBounds().width / 2), 180);
		m_window->draw(text);

		text.setString("Settings");
		text.setPosition(250 -
			(text.getLocalBounds().width / 2), 280);
		m_window->draw(text);
	}

	if (mode == BranchEditor)
	{
		text.setCharacterSize(11);
		text.setString(sf::String("(Left/Right arrow keys) Editing Branch num ") + char('0' + currBranch));
		text.setPosition(250 -
			(text.getLocalBounds().width / 2), 0);
		m_window->draw(text);
		text.setString(sf::String("(Up/Down arrow keys) Page ") + char('0' + currPage + 1) + sf::String("/3"));
		text.setPosition(250 -
			(text.getLocalBounds().width / 2), 15);
		m_window->draw(text);

		text.setCharacterSize(15);
		for (int i = 0; i < NUM_GENES_IN_BRANCH; i++)
		{
			if (managers[currBranch].buttons[i].page != currPage) { continue; }
			text.setString(managers[currBranch].buttons[i].label);
			text.setPosition(0, (managers[currBranch].buttons[i].yPos + 1) * 30);
			m_window->draw(text);
			if (managers[currBranch].buttons[i].floorToInt)
			{
				std::ostringstream oss;
				oss << *managers[currBranch].buttons[i].intVal;
				text.setString(oss.str());
				if (managers[currBranch].activeButton == &managers[currBranch].buttons[i])
					text.setString(managers[currBranch].stringifiedNum);
				text.setPosition(250, (managers[currBranch].buttons[i].yPos + 1) * 30);
				m_window->draw(text);
			}
			else
			{
				std::ostringstream oss;
				oss << *managers[currBranch].buttons[i].floatVal;
				text.setString(oss.str());
				if (managers[currBranch].activeButton == &managers[currBranch].buttons[i])
					text.setString(managers[currBranch].stringifiedNum);
				text.setPosition(250, (managers[currBranch].buttons[i].yPos + 1) * 30);
				m_window->draw(text);
			}
		}
	}

	if (mode == Constants)
	{
		text.setCharacterSize(11);
		text.setString(sf::String("(Up/Down arrow keys) Page ") + char('0' + currPage + 1) + sf::String("/1"));
		text.setPosition(250 -
			(text.getLocalBounds().width / 2), 7);
		m_window->draw(text);

		text.setCharacterSize(15);
		for (int i = 0; i < 6; i++)
		{
			if (settingsManager.buttons[i].page != currPage) { continue; }
			text.setString(settingsManager.buttons[i].label);
			text.setPosition(0, (settingsManager.buttons[i].yPos + 1) * 30);
			m_window->draw(text);
			if (settingsManager.buttons[i].floorToInt)
			{
				std::ostringstream oss;
				oss << *settingsManager.buttons[i].intVal;
				text.setString(oss.str());
				if (settingsManager.activeButton == &settingsManager.buttons[i])
					text.setString(settingsManager.stringifiedNum);
				text.setPosition(250, (settingsManager.buttons[i].yPos + 1) * 30);
				m_window->draw(text);
			}
			else
			{
				std::ostringstream oss;
				oss << *settingsManager.buttons[i].floatVal;
				text.setString(oss.str());
				if (settingsManager.activeButton == &settingsManager.buttons[i])
					text.setString(settingsManager.stringifiedNum);
				text.setPosition(250, (settingsManager.buttons[i].yPos + 1) * 30);
				m_window->draw(text);
			}
		}

	}
	m_window->display();
};