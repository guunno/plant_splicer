#include "Button.h"
#include <sstream>

void ValueEditButton::InitButton(sf::String name, int buttonIndex, int buttonsPerPage, bool floorToIntN, float maxN, float minN)
{
	std::cout << buttonsPerPage << "\n";
	max = maxN;
	min = minN;
	yPos = buttonIndex % buttonsPerPage;
	page = floor(buttonIndex / buttonsPerPage);
	floorToInt = floorToIntN;
	label = name;
};

void ValueEditButton::PointButton(float& pointer)
{
	floatVal = &pointer;
};
void ValueEditButton::PointButton(int& pointer)
{
	intVal = &pointer;
};

void SoloBranchGenomeButtonManager::ActivateButton(int mouseY, int currPage)
{
	// there is an offset for space for something at the top, could be anything :/
	int selectedRow = floor(mouseY / 30);
	selectedRow -= 2;
	if (selectedRow >= 0 && selectedRow < 22)
	{
		activeButton = &buttons[selectedRow];
	}
};

void SoloBranchGenomeButtonManager::ProcessInput(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Enter)
	{
		if (!activeButton) return;
		bool hasDecimal = false;
		float num = 0;
		if (stringifiedNum.find(".") == sf::String::InvalidPos)
		{
			hasDecimal = false;
		}
		else
		{
			hasDecimal = true;
		}

		bool positive = true;
		if (stringifiedNum.find("-") != sf::String::InvalidPos)
		{
			positive = false;
			stringifiedNum[0] = '0';
		}

		if (!hasDecimal)
		{
			for (int i = 0; i < stringifiedNum.getSize(); i++)
			{
				num += ((int)stringifiedNum[i] - 48) * pow(10, stringifiedNum.getSize() - i - 1);
			}
			if (activeButton->floorToInt)
			{
				*activeButton->intVal = floor(num * (((int)positive * 2) - 1));
			}
			else
			{
				*activeButton->floatVal = num * (((int)positive * 2) - 1);
			}
		}
		else
		{
			bool beforeDecimal = false;
			for (int i = 0; i < stringifiedNum.getSize(); i++)
			{
				if (stringifiedNum[i] == '.')
				{
					beforeDecimal = true;
					continue;
				}
				if (beforeDecimal)
				{
					num += ((int)stringifiedNum[i] - 48) * pow(10, stringifiedNum.getSize() - i - 1);
				}
				else
				{
					num += ((int)stringifiedNum[i] - 48) * pow(10, stringifiedNum.getSize() - i - 2);
				}
			}
			num /= pow(10, (stringifiedNum.getSize() - stringifiedNum.find(".") - 1));
			if (activeButton->floorToInt)
			{
				*activeButton->intVal = floor(num * (((int)positive * 2) - 1));
			}
			else
			{
				*activeButton->floatVal = num * (((int)positive * 2) - 1);
			}
		}
		stringifiedNum = "";
		activeButton = nullptr;
		std::cout << ((int)positive * 2) - 1 << "\n";
	}

	if (key == sf::Keyboard::Period)
	{
		if(stringifiedNum.find(".") == sf::String::InvalidPos)
			stringifiedNum += ".";
		return;
	}

	if (key == sf::Keyboard::Dash)
	{
		if (stringifiedNum.find("-") == sf::String::InvalidPos)
			stringifiedNum.insert(0, "-");
		return;
	}

	if (key >= 26 && key <= 35)
		stringifiedNum += char('0' + (key - 26));
}

void SoloBranchGenomeButtonManager::LinkButtons(BranchGenome& linkedGenome)
{
	buttons[0].InitButton("InitDir", 0);
	buttons[0].PointButton(linkedGenome.initDir);
	buttons[1].InitButton("InitColourR", 1);
	buttons[1].PointButton(linkedGenome.initColour.r);
	buttons[2].InitButton("InitColourG", 2);
	buttons[2].PointButton(linkedGenome.initColour.g);
	buttons[3].InitButton("InitColourB", 3);
	buttons[3].PointButton(linkedGenome.initColour.b);
	buttons[4].InitButton("InitWidth", 4);
	buttons[4].PointButton(linkedGenome.initWidth);

// integer type buttons are handled differently
	buttons[5].InitButton("Length", 5, 38, true, 1000.0f, 1.0f);
	buttons[5].PointButton(linkedGenome.length);
	buttons[6].InitButton("LengthVariation", 6, 38, true);
	buttons[6].PointButton(linkedGenome.lengthVariation);

	buttons[7].InitButton("ColourAdoption", 7, 38, false);
	buttons[7].PointButton(linkedGenome.colourAdoption);
	buttons[8].InitButton("WidthAdoption", 8, 38, false);
	buttons[8].PointButton(linkedGenome.widthAdoption);
	buttons[9].InitButton("DirAdoption", 9, 38, false);
	buttons[9].PointButton(linkedGenome.dirAdoption);
	buttons[10].InitButton("ColourChangeR", 10);
	buttons[10].PointButton(linkedGenome.colourChange.r);
	buttons[11].InitButton("ColourChangeG", 11);
	buttons[11].PointButton(linkedGenome.colourChange.g);
	buttons[12].InitButton("ColourChangeB", 12);
	buttons[12].PointButton(linkedGenome.colourChange.b);
	buttons[13].InitButton("WidthChange", 13);
	buttons[13].PointButton(linkedGenome.widthChange);
	buttons[14].InitButton("DirChange", 14);
	buttons[14].PointButton(linkedGenome.dirChange);
	buttons[15].InitButton("RandTurn", 15);
	buttons[15].PointButton(linkedGenome.randTurn);

	// integer type buttons are handeled differently
	buttons[16].InitButton("Branch0Gene", 16, 38, true);
	buttons[16].PointButton(linkedGenome.branch0);
	buttons[17].InitButton("Branch1Gene", 17, 38, true);
	buttons[17].PointButton(linkedGenome.branch1);
	buttons[18].InitButton("Branch2Gene", 18, 38, true);
	buttons[18].PointButton(linkedGenome.branch2);

	buttons[19].InitButton("Branch0Pos", 19, 38, false, 1.0f, 0.0f);
	buttons[19].PointButton(linkedGenome.branch0Position);
	buttons[20].InitButton("Branch1Pos", 20, 38, false, 1.0f, 0.0f);
	buttons[20].PointButton(linkedGenome.branch1Position);
	buttons[21].InitButton("Branch2Pos", 21, 38, false, 1.0f, 0.0f);
	buttons[21].PointButton(linkedGenome.branch2Position);
}