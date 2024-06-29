#include "Button.h"
#include <sstream>

void ValueEditButton::InitButton(sf::String name, int buttonIndex, bool floorToIntN, float maxN, float minN)
{
	max = maxN;
	min = minN;
	yPos = buttonIndex % 15;
	page = floor(buttonIndex / 15);
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


void StringEditButton::InitButton(sf::String name, int buttonIndex)
{
	yPos = buttonIndex % 15;
	page = floor(buttonIndex / 15);
	label = name;
};
void StringEditButton::PointButton(sf::String& pointer)
{
	value = &pointer;
};


void SoloBranchGenomeButtonManager::ActivateButton(int mouseY, int currPage)
{
	// there is an offset for space for something at the top, could be anything :/
	int selectedRow = floor(mouseY / 30);
	selectedRow -= 2;
	if (selectedRow >= 0 && selectedRow < NUM_GENES_IN_BRANCH)
	{
		stringifiedNum = "";
		if (buttons[selectedRow].page != currPage)
			return;
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

void SettingsButtonManager::ProcessInput(sf::Keyboard::Key key)
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
	}

	if (key == sf::Keyboard::Period)
	{
		if (stringifiedNum.find(".") == sf::String::InvalidPos)
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
	buttons[5].InitButton("Length", 5, true, 0.0f, 1.0f);
	buttons[5].PointButton(linkedGenome.length);
	buttons[6].InitButton("LengthVariation", 6, true);
	buttons[6].PointButton(linkedGenome.lengthVariation);

	buttons[7].InitButton("ColourAdoption", 7, false);
	buttons[7].PointButton(linkedGenome.colourAdoption);
	buttons[8].InitButton("WidthAdoption", 8, false);
	buttons[8].PointButton(linkedGenome.widthAdoption);
	buttons[9].InitButton("DirAdoption", 9, false);
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
	buttons[16].InitButton("Branch0Gene", 16, true);
	buttons[16].PointButton(linkedGenome.branch0);
	buttons[17].InitButton("Branch1Gene", 17, true);
	buttons[17].PointButton(linkedGenome.branch1);
	buttons[18].InitButton("Branch2Gene", 18, true);
	buttons[18].PointButton(linkedGenome.branch2);
	buttons[19].InitButton("Branch3Gene", 19, true);
	buttons[19].PointButton(linkedGenome.branch3);
	buttons[20].InitButton("Branch4Gene", 20, true);
	buttons[20].PointButton(linkedGenome.branch4);
	buttons[21].InitButton("Branch5Gene", 21, true);
	buttons[21].PointButton(linkedGenome.branch5);

	buttons[22].InitButton("Branch0Pos", 22, false, 1.0f, 0.0f);
	buttons[22].PointButton(linkedGenome.branch0Position);
	buttons[23].InitButton("Branch1Pos", 23, false, 1.0f, 0.0f);
	buttons[23].PointButton(linkedGenome.branch1Position);
	buttons[24].InitButton("Branch2Pos", 24, false, 1.0f, 0.0f);
	buttons[24].PointButton(linkedGenome.branch2Position);
	buttons[25].InitButton("Branch3Pos", 25, false, 1.0f, 0.0f);
	buttons[25].PointButton(linkedGenome.branch3Position);
	buttons[26].InitButton("Branch4Pos", 26, false, 1.0f, 0.0f);
	buttons[26].PointButton(linkedGenome.branch4Position);
	buttons[27].InitButton("Branch5Pos", 27, false, 1.0f, 0.0f);
	buttons[27].PointButton(linkedGenome.branch5Position);

	buttons[28].InitButton("Branch0Rec", 28, true);
	buttons[28].PointButton(linkedGenome.rBranch0);
	buttons[29].InitButton("Branch1Rec", 29, true);
	buttons[29].PointButton(linkedGenome.rBranch1);
	buttons[30].InitButton("Branch2Rec", 30, true);
	buttons[30].PointButton(linkedGenome.rBranch2);

	buttons[31].InitButton("DirSpread", 31);
	buttons[31].PointButton(linkedGenome.dirSpread);
	buttons[32].InitButton("SpreadCloseness", 32, true);
	buttons[32].PointButton(linkedGenome.spreadMaxDistanceEff);
}

void SettingsButtonManager::LinkButtons(Settings& editor)
{
	buttons[0].InitButton("BGColourR", 0);
	buttons[0].PointButton(editor.mainBG.r);
	buttons[1].InitButton("BGColourG", 1);
	buttons[1].PointButton(editor.mainBG.g);
	buttons[2].InitButton("BGColourB", 2);
	buttons[2].PointButton(editor.mainBG.b);
	buttons[3].InitButton("EditorColourR", 3);
	buttons[3].PointButton(editor.editorBG.r);
	buttons[4].InitButton("EditorColourG", 4);
	buttons[4].PointButton(editor.editorBG.g);
	buttons[5].InitButton("EditorColourB", 5);
	buttons[5].PointButton(editor.editorBG.b);
}

void SettingsButtonManager::ActivateButton(int mouseY, int currPage)
{
	// there is an offset for space for something at the top, could be anything :/
	int selectedRow = floor(mouseY / 30);
	selectedRow -= 2;
	if (selectedRow >= 0 && selectedRow < 6)
	{
		stringifiedNum = "";
		if (buttons[selectedRow].page != currPage)
			return;
		activeButton = &buttons[selectedRow];
	}
};

void SplicingButtonManager::LinkButtons(SplicingSettings& editor)
{
	buttons[0].InitButton("LoadPath", 0);
	buttons[0].PointButton(editor.loadPath);
	buttons[1].InitButton("SpliceGenome0Path", 1);
	buttons[1].PointButton(editor.splice0Path);
	buttons[2].InitButton("SpliceGenome1Path", 2);
	buttons[2].PointButton(editor.splice1Path);
}

void SplicingButtonManager::ActivateButton(int mouseY, int currPage)
{
	// there is an offset for space for something at the top, could be anything :/
	int selectedRow = floor(mouseY / 30);
	selectedRow -= 2;
	if (selectedRow >= 0 && selectedRow < 3)
	{
		stringifiedNum = "";
		if (buttons[selectedRow].page != currPage)
			return;
		activeButton = &buttons[selectedRow];
	}
};

void SplicingButtonManager::ProcessInput(char key)
{
	if (activeButton)
	{
		if (key == sf::Keyboard::Enter)
		{
			*activeButton->value = stringifiedNum;
			stringifiedNum = "";
			activeButton = nullptr;

			return;
		}

		stringifiedNum += key;
	}
}