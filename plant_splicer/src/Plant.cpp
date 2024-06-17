#include "Plant.h"

#define MAX_RECUSION_DEPTH 3

Branch::Branch(BranchGenome& genomeData, Branch* parentBranch, int branchLayer)
{
	Create(genomeData, parentBranch, branchLayer);
}

void Branch::Create(BranchGenome& genomeData, Branch* parentBranch, int branchLayer)
{
	data.length = genomeData.length + (int)(genomeData.lengthVariation * ((rand() % 201) - 100) / 100.0f);

	data.branchIndexes[0] = genomeData.branch0;
	data.branchIndexes[1] = genomeData.branch1;
	data.branchIndexes[2] = genomeData.branch2;

	data.branchingPoints[0] = (int)floor(genomeData.branch0Position * data.length);
	data.branchingPoints[1] = (int)floor(genomeData.branch1Position * data.length);
	data.branchingPoints[2] = (int)floor(genomeData.branch2Position * data.length);

	data.randomTurn = genomeData.randTurn;
	data.isDirPositive = rand() % 2;
	data.branchLayer = branchLayer;

	data.dirChange = genomeData.dirChange;
	data.widthChange = genomeData.widthChange;
	data.colourChange = genomeData.colourChange;

	data.colour = genomeData.initColour;
	data.colourAdoption = genomeData.colourAdoption;

	data.widthAdoption = genomeData.widthAdoption;

	if (parentBranch)
	{
		data.dir = LERP(genomeData.initDir, parentBranch->data.dir, genomeData.dirAdoption);
		data.width = LERP(genomeData.initWidth, parentBranch->data.width, genomeData.widthAdoption);
	}
	else
	{
		data.dir = genomeData.initDir;
		data.width = genomeData.initWidth;
	}
}

void Branch::RenderBranch(
	const std::unique_ptr<sf::CircleShape>& circle,
	sf::RenderWindow* window,
	const Branch::Orientation& offset = Branch::Orientation(),
	uint32_t recursionDepth
) {
	Vector2 pos = offset.pos;
	float dir = data.dir + offset.dir;
	float width = LERP((recursionDepth == 0 ? data.width : offset.width), data.width, data.widthAdoption);
	floatColour colour;
	colour.r = floor(LERP((recursionDepth == 0 ? data.colour.r : offset.colour.r), data.colour.r, data.colourAdoption));
	colour.g = floor(LERP((recursionDepth == 0 ? data.colour.g : offset.colour.g), data.colour.g, data.colourAdoption));
	colour.b = floor(LERP((recursionDepth == 0 ? data.colour.b : offset.colour.b), data.colour.b, data.colourAdoption));

	for (int i = 0; i < data.length; i++)
	{
		dir += (data.dirChange + ((((rand() % 201) - 100) / 100.0f) * data.randomTurn)) * ((int)data.isDirPositive * 2 - 1);
		pos += Vector2(0, -1).rotateNew(dir);
		width += data.widthChange;
		colour += data.colourChange;

		if (recursionDepth < MAX_RECUSION_DEPTH)
		{
			for (int j = 0; j < 3; j++)
			{
				if (data.branchIndexes[j] && i == data.branchingPoints[j] && data.branchIndexes[j] >= 0)
					RenderBranch(circle, window, { pos, dir, colour, width }, recursionDepth + 1);
			}
		}
		RenderBranchSegment(circle, pos, width, colour, window);
	}
}

void Branch::RenderBranchSegment(
	const std::unique_ptr<sf::CircleShape>& circle, 
	Vector2 position, float width, floatColour colour, 
	sf::RenderWindow* window
) {
	sf::Color sfColour = colour.sfCol();
	// Light
	circle->setFillColor(sfColour + sf::Color(25, 25, 25, 100));
	circle->setPosition({ position.x - 1, position.y - 1 });
	circle->setRadius(width);
	window->draw(*circle);

	// Shadow
	circle->setFillColor(sf::Color(floor(abs(sfColour.r - 25) + (sfColour.r - 25)), floor(abs(sfColour.g - 25) + (sfColour.g - 25)), floor(abs(sfColour.b - 25) + (sfColour.b - 25)), 100));
	circle->setPosition({ position.x + 1, position.y + 1 });
	window->draw(*circle);

	// Actual
	circle->setFillColor(sfColour);
	circle->setPosition({ position.x, position.y });
	window->draw(*circle);
}

Plant::Plant(Vector2 pos, sf::RenderWindow* window)
	:pos(pos), window(window)
{
	m_Branches.Create(GetBranchCount());
	InitBranches();
}

void Plant::Render()
{
	m_Branches[0].RenderBranch(
		m_BranchRenderShape, window, 
		Branch::Orientation{ pos, 0, floatColour{0, 0, 0} }
	);
}

uint32_t Plant::GetBranchCount(uint32_t genomeIdx, uint8_t recursionDepth)
{
	if (genomeIdx > 13) return 0;
	if (recursionDepth >= MAX_RECUSION_DEPTH) return 1;

	return (
		GetBranchCount(branchGenes[genomeIdx].branch0, recursionDepth + 1) +
		GetBranchCount(branchGenes[genomeIdx].branch1, recursionDepth + 1) +
		GetBranchCount(branchGenes[genomeIdx].branch2, recursionDepth + 1) +
		1
	);
}

uint32_t Plant::InitBranches(uint32_t genomeIdx, uint8_t recursionDepth)
{
	uint32_t currentIndex = m_IntermediateBranchCount;
	m_Branches[currentIndex].Create(branchGenes[genomeIdx], nullptr, 0);
	m_IntermediateBranchCount++;

	if (recursionDepth >= MAX_RECUSION_DEPTH)
		return currentIndex;

	for (int i = 0; i < 3; i++)
	{
		int index = m_Branches[currentIndex].data.branchIndexes[i];
		if (index >= 0)
			m_Branches[currentIndex].childIndices[i] = InitBranches(index, recursionDepth + 1);
	}

	return currentIndex;
}
