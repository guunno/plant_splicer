#include "Plant.h"

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

	data.isDirPositive = rand() % 2;
	data.branchLayer = branchLayer;

	data.dirChange = genomeData.dirChange;
	data.widthChange = genomeData.widthChange;
	data.colourChange = genomeData.colourChange;

	if (parentBranch)
	{
		data.dir = LERP(genomeData.initDir, parentBranch->data.dir, genomeData.dirAdoption);
		data.colour.r = LERP(genomeData.initColour.r, parentBranch->data.colour.r, genomeData.colourAdoption);
		data.colour.g = LERP(genomeData.initColour.g, parentBranch->data.colour.g, genomeData.colourAdoption);
		data.colour.b = LERP(genomeData.initColour.b, parentBranch->data.colour.b, genomeData.colourAdoption);
		data.width = LERP(genomeData.initWidth, parentBranch->data.width, genomeData.widthAdoption);
	}
	else
	{
		data.dir = genomeData.initDir;
		data.colour = genomeData.initColour;
		data.width = genomeData.initWidth;
	}
}

Branch::Orientation Branch::RenderBranch(
	const std::unique_ptr<sf::CircleShape>& circle,
	sf::RenderWindow* window,
	const Branch::Orientation& offset = Branch::Orientation()
) {
	Vector2 pos = data.pos + offset.pos;
	float dir = data.dir + offset.dir;
	float width = data.width;
	sf::Color colour = data.colour + offset.colour;
	for (int i = 0; i < data.length; i++)
	{
		dir += data.dirChange;// +float((rand() % 180) * PI / 180.0f) * data.randomTurn * ((int)data.isDirPositive * 2 - 1);
		pos += Vector2(0, -1).rotateNew(dir);
		//std::cout << pos << "\n";
		width += data.widthChange;
		colour += data.colourChange;
		RenderBranchSegment(circle, pos, width, colour, window);
	}
	std::cout << "!!! " << pos - offset.pos << "\n";
	return { pos - offset.pos, dir - offset.dir, colour - offset.colour };
}

void Branch::RenderBranchSegment(
	const std::unique_ptr<sf::CircleShape>& circle, 
	Vector2 position, float width, sf::Color colour, 
	sf::RenderWindow* window
) {
	// Light
	circle->setFillColor(data.colour + sf::Color(25, 25, 25));
	circle->setPosition({ position.x - 1, position.y - 1 });
	circle->setRadius(data.width);
	window->draw(*circle);

	// Shadow
	circle->setFillColor(sf::Color(data.colour.r - 25, data.colour.g - 25, data.colour.b - 25));
	circle->setPosition({ position.x + 1, position.y + 1 });
	window->draw(*circle);

	// Actual
	circle->setFillColor(data.colour);
	circle->setPosition({ position.x, position.y });
	window->draw(*circle);
}

Plant::Plant(Vector2 pos, sf::RenderWindow* window)
	:pos(pos), window(window)
{
	m_Branches.Create(GetBranchCount());
	InitBranches();
}

void Plant::Render(uint32_t BranchIndex, const Branch::Orientation& orientation)
{
	Branch::Orientation childOrientations = m_Branches[BranchIndex].RenderBranch(
		m_BranchRenderShape, window, 
		{
			orientation.pos + pos, 
			orientation.dir,
			orientation.colour
		}
	);

	for (int i = 0; i < 3; i++)
	{
		uint32_t idx = m_Branches[BranchIndex].childIndices[i];
		if (idx >  0)
			Render(idx, childOrientations);
	}
}

uint32_t Plant::GetBranchCount(uint32_t genomeIdx, uint8_t recursionDepth)
{
	if (genomeIdx > 13) return 0;
	if (recursionDepth >= 10) return 1;

	return (
		GetBranchCount(branchGenes[genomeIdx].branch0, recursionDepth + 1) +
		GetBranchCount(branchGenes[genomeIdx].branch1, recursionDepth + 1) +
		GetBranchCount(branchGenes[genomeIdx].branch2, recursionDepth + 1) +
		1
	);
}

uint32_t Plant::InitBranches(uint32_t genomeIdx, uint8_t recursionDepth)
{
	std::cout << m_Branches.Size() << ": " << m_IntermediateBranchCount << ": " << genomeIdx << "\n";

	uint32_t currentIndex = m_IntermediateBranchCount;
	m_Branches[currentIndex].Create(branchGenes[genomeIdx], nullptr, 0);
	m_IntermediateBranchCount++;

	if (recursionDepth >= 10)
		return currentIndex;

	for (int i = 0; i < 3; i++)
	{
		int index = m_Branches[currentIndex].data.branchIndexes[i];
		if (index >= 0)
			m_Branches[currentIndex].childIndices[i] = InitBranches(index, recursionDepth + 1);
	}

	return currentIndex;
}
