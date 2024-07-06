#include "Button.h"
#include <sstream>

void ValueEditButton::InitButton(const sf::String& name, int buttonIndex, float* value, float maxN, float minN)
{
	floorToInt = false;
	floatVal = value;
	Initialise(name, buttonIndex, maxN, minN);
}

void ValueEditButton::InitButton(const sf::String& name, int buttonIndex, int* value, float maxN, float minN)
{
	floorToInt = true;
	intVal = value;
	Initialise(name, buttonIndex, maxN, minN);
}

void ValueEditButton::Initialise(const sf::String& name, int buttonIndex, float maxN, float minN)
{
	max = maxN;
	min = minN;
	yPos = buttonIndex % 15;
	page = floor(buttonIndex / 15);
	label = name;
};


void StringEditButton::InitButton(const sf::String& name, int buttonIndex, sf::String* textEdit)
{
	yPos = buttonIndex % 15;
	page = (int)floor(buttonIndex / 15);
	label = name;
	value = textEdit;
};


void SoloBranchGenomeButtonManager::ActivateButton(int mouseY, int currPage)
{
	// there is an offset for space for something at the top, could be anything :/
	int selectedRow = floor(mouseY / 30);
	selectedRow -= 2;

	if (selectedRow >= 0 && selectedRow < NUM_GENES_IN_BRANCH)
	{
		if (buttons[selectedRow].page != currPage)
		{
			stringifiedNum = "";
			return;
		}

		if (buttons[selectedRow].floorToInt)
			stringifiedNum = std::to_string(*buttons[selectedRow].intVal);
		else
			stringifiedNum = floatToString(*buttons[selectedRow].floatVal);

		activeButton = &buttons[selectedRow];
	}
};

void SoloBranchGenomeButtonManager::ProcessInput(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Enter)
	{
		if (!activeButton) return;
		if (stringifiedNum.getSize() < 1)
		{
			activeButton = nullptr;
			return;
		}

		bool hasDecimal = stringifiedNum.find(".") != sf::String::InvalidPos;
		float num = 0;

		bool positive = true;
		if (stringifiedNum.find("-") != sf::String::InvalidPos)
		{
			positive = false;
			stringifiedNum[0] = '0';
		}

		if (!hasDecimal)
		{
			for (int i = 0; i < stringifiedNum.getSize(); i++)
				num += ((int)stringifiedNum[i] - 48) * (float)pow(10, stringifiedNum.getSize() - i - 1);

			if (activeButton->floorToInt)
				*activeButton->intVal = floor(num * (((int)positive * 2) - 1));
			else
				*activeButton->floatVal = num * (((int)positive * 2) - 1);
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
					num += ((int)stringifiedNum[i] - 48) * (float)pow(10, stringifiedNum.getSize() - i - 1);
				else
					num += ((int)stringifiedNum[i] - 48) * (float)pow(10, stringifiedNum.getSize() - i - 2);
			}

			num /= (float)pow(10, (stringifiedNum.getSize() - stringifiedNum.find(".") - 1));
			if (activeButton->floorToInt)
				*activeButton->intVal = floor(num * (((int)positive * 2) - 1));
			else
				*activeButton->floatVal = num * (((int)positive * 2) - 1);
		}
		stringifiedNum = "";
		activeButton = nullptr;
	}

	switch (key)
	{
	case sf::Keyboard::Period:
		if (stringifiedNum.find(".") == sf::String::InvalidPos)
			stringifiedNum += ".";
		return;

	case sf::Keyboard::Dash:
		if (stringifiedNum.find("-") == sf::String::InvalidPos)
			stringifiedNum.insert(0, "-");
		return;

	case sf::Keyboard::Backspace:
		if (stringifiedNum.getSize() > 0)
			stringifiedNum.erase(stringifiedNum.getSize() - 1, 1);
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
			hasDecimal = false;
		else
			hasDecimal = true;

		bool positive = true;
		if (stringifiedNum.find("-") != sf::String::InvalidPos)
		{
			positive = false;
			stringifiedNum[0] = '0';
		}

		if (!hasDecimal)
		{
			for (int i = 0; i < stringifiedNum.getSize(); i++)
				num += ((int)stringifiedNum[i] - 48) * (float)pow(10, stringifiedNum.getSize() - i - 1);

			if (activeButton->floorToInt)
				*activeButton->intVal = floor(num * (((int)positive * 2) - 1));
			else
				*activeButton->floatVal = num * (((int)positive * 2) - 1);
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
					num += ((int)stringifiedNum[i] - 48) * (float)pow(10, stringifiedNum.getSize() - i - 1);
				else
					num += ((int)stringifiedNum[i] - 48) * (float)pow(10, stringifiedNum.getSize() - i - 2);
			}

			num /= (float)pow(10, (stringifiedNum.getSize() - stringifiedNum.find(".") - 1));

			if (activeButton->floorToInt)
				*activeButton->intVal = (int)floor(num * (((int)positive * 2) - 1));
			else
				*activeButton->floatVal = num * (((int)positive * 2) - 1);
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
	// Initial Orientations
	buttons[0].InitButton("InitDir", 0, &linkedGenome.initDir);
	buttons[1].InitButton("InitColourR", 1, &linkedGenome.initColour.r);
	buttons[2].InitButton("InitColourG", 2, &linkedGenome.initColour.g);
	buttons[3].InitButton("InitColourB", 3, &linkedGenome.initColour.b);
	buttons[4].InitButton("InitWidth", 4, &linkedGenome.initWidth);

	// Length of Branch!
	buttons[5].InitButton("Length", 5, &linkedGenome.length, 0.0f, 1.0f);
	buttons[6].InitButton("LengthVariation", 6, &linkedGenome.lengthVariation);

	// How much its Parent Branch affeects its orientation
	buttons[7].InitButton("ColourAdoption", 7, &linkedGenome.colourAdoption);
	buttons[8].InitButton("WidthAdoption", 8, &linkedGenome.widthAdoption);
	buttons[9].InitButton("DirAdoption", 9, &linkedGenome.dirAdoption);

	// Changes taking place along the branch
	buttons[10].InitButton("ColourChangeR", 10, &linkedGenome.colourChange.r);
	buttons[11].InitButton("ColourChangeG", 11, &linkedGenome.colourChange.g);
	buttons[12].InitButton("ColourChangeB", 12, &linkedGenome.colourChange.b);
	buttons[13].InitButton("WidthChange", 13, &linkedGenome.widthChange);
	buttons[14].InitButton("DirChange", 14, &linkedGenome.dirChange);
	buttons[15].InitButton("RandTurn", 15, &linkedGenome.randTurn);

	// Child Branches
	buttons[16].InitButton("Branch0Gene", 16, &linkedGenome.branch0);
	buttons[17].InitButton("Branch1Gene", 17, &linkedGenome.branch1);
	buttons[18].InitButton("Branch2Gene", 18, &linkedGenome.branch2);
	buttons[19].InitButton("Branch3Gene", 19, &linkedGenome.branch3);
	buttons[20].InitButton("Branch4Gene", 20, &linkedGenome.branch4);
	buttons[21].InitButton("Branch5Gene", 21, &linkedGenome.branch5);

	// Where each Child Branch Begins
	buttons[22].InitButton("Branch0Pos", 22, &linkedGenome.branch0Position, 1.0f, 0.0f);
	buttons[23].InitButton("Branch1Pos", 23, &linkedGenome.branch1Position, 1.0f, 0.0f);
	buttons[24].InitButton("Branch2Pos", 24, &linkedGenome.branch2Position, 1.0f, 0.0f);
	buttons[25].InitButton("Branch3Pos", 25, &linkedGenome.branch3Position, 1.0f, 0.0f);
	buttons[26].InitButton("Branch4Pos", 26, &linkedGenome.branch4Position, 1.0f, 0.0f);
	buttons[27].InitButton("Branch5Pos", 27, &linkedGenome.branch5Position, 1.0f, 0.0f);

	// At final recursion, Create these Child Branch
	buttons[28].InitButton("Branch0Rec", 28, &linkedGenome.rBranch0);
	buttons[29].InitButton("Branch1Rec", 29, &linkedGenome.rBranch1);
	buttons[30].InitButton("Branch2Rec", 30, &linkedGenome.rBranch2);

	// Spreading apart to avoid excess collisions between Child Branches
	buttons[31].InitButton("DirSpread", 31, &linkedGenome.dirSpread);
	buttons[32].InitButton("SpreadCloseness", 32, &linkedGenome.spreadMaxDistanceEff);

	buttons[33].InitButton("ConBranch0", 33, &linkedGenome.cRBranch0);
	buttons[34].InitButton("ConBranch0", 34, &linkedGenome.cRBranch1);
	buttons[35].InitButton("ConBranch2", 35, &linkedGenome.cRBranch2);
}

void SettingsButtonManager::LinkButtons(Settings& editor)
{
	buttons[0].InitButton("BGColourR", 0, &editor.mainBG.r);
	buttons[1].InitButton("BGColourG", 1, &editor.mainBG.g);
	buttons[2].InitButton("BGColourB", 2, &editor.mainBG.b);
	buttons[3].InitButton("EditorColourR", 3, &editor.editorBG.r);
	buttons[4].InitButton("EditorColourG", 4, &editor.editorBG.g);
	buttons[5].InitButton("EditorColourB", 5, &editor.editorBG.b);
	buttons[6].InitButton("Zoom", 6, &editor.zoom);
}

void SettingsButtonManager::ActivateButton(int mouseY, int currPage)
{
	// there is an offset for space for something at the top, could be anything :/
	int selectedRow = floor(mouseY / 30);
	selectedRow -= 2;
	if (selectedRow >= 0 && selectedRow < 7)
	{
		stringifiedNum = "";
		if (buttons[selectedRow].page != currPage)
			return;
		activeButton = &buttons[selectedRow];
	}
};

void SplicingButtonManager::LinkButtons(SplicingSettings& editor)
{
	buttons[0].InitButton("SpliceGenome0Path", 0, &editor.splice0Path);
	buttons[1].InitButton("SpliceGenome1Path", 1, &editor.splice1Path);
	// buttons[2].InitButton("LoadPath", 0, &editor.loadPath);
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