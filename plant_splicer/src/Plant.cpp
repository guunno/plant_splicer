#include "Plant.h"

#define MAX_RECUSION_DEPTH 2

Fruit::Fruit(FruitGenome& genomeData, Branch* parentBranch)
{
	Create(genomeData, parentBranch);
}

void Fruit::Create(FruitGenome& genomeData, Branch* parentBranch)
{
	std::cout << genomeData.length << " Created!\n";
	data.length = genomeData.length;

	data.colourChanges[0] = genomeData.colourChanges[0];
	data.colourChanges[1] = genomeData.colourChanges[1];
	data.colourChanges[2] = genomeData.colourChanges[2];

	data.dirChanges[0] = genomeData.directionChanges[0];
	data.dirChanges[1] = genomeData.directionChanges[1];
	data.dirChanges[2] = genomeData.directionChanges[2];

	data.randomTurn = genomeData.randTurn;

	data.colour = genomeData.initColour;

	data.colourAdoption = genomeData.colourAdoption;
	data.widthAdoption = genomeData.widthAdoption;

	data.dir = LERP(genomeData.initDir, parentBranch->data.dir, genomeData.dirAdoption);
	data.width = LERP(genomeData.initWidth, parentBranch->data.width, genomeData.widthAdoption);
}

Branch::Branch(BranchGenome& genomeData, Branch* parentBranch)
{
	Create(genomeData, parentBranch);
}

void Branch::Create(BranchGenome& genomeData, Branch* parentBranch)
{
	data.length = genomeData.length + (int)(genomeData.lengthVariation * ((rand() % 201) - 100) / 100.0f);

	data.branchIndexes[0] = genomeData.branch0;
	data.branchIndexes[1] = genomeData.branch1;
	data.branchIndexes[2] = genomeData.branch2;

	data.branchingPoints[0] = (int)floor(genomeData.branch0Position * data.length) - 1;
	data.branchingPoints[1] = (int)floor(genomeData.branch1Position * data.length) - 1;
	data.branchingPoints[2] = (int)floor(genomeData.branch2Position * data.length) - 1;

	data.randomTurn = genomeData.randTurn;
	data.isDirPositive = rand() % 2;

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

static void RenderBranchSegment(
	const std::unique_ptr<sf::CircleShape>& circle,
	Vector2 position, float width, const FloatColour& colour,
	sf::RenderWindow* window
) {
	circle->setRadius(width);

	// Light
	circle->setFillColor(colour + FloatColour{ 25, 25, 25, 100 });
	circle->setPosition({ position.x - 1, position.y - 1 });
	window->draw(*circle);
	
	FloatColour ShadowCol{
		abs(colour.r - 25) + (colour.r - 25),
		abs(colour.g - 25) + (colour.g - 25),
		abs(colour.b - 25) + (colour.b - 25),
		100
	};

	// Shadow
	circle->setFillColor(ShadowCol);
	circle->setPosition({ position.x + 1, position.y + 1 });
	window->draw(*circle);

	// Actual
	circle->setFillColor(colour);
	circle->setPosition({ position.x, position.y });
	window->draw(*circle);
}

static float sigmoidDerivative(float input)
{
	const float n = 1 / (1 + exp(-input));
	return n * (1 - n);
}

void Fruit::Render(
	const std::unique_ptr<sf::CircleShape>& circle, 
	sf::RenderWindow* window, 
	const Buffer<Fruit>& allFruits, 
	const BranchOrientation& offset
) const {
	std::cout << "Rendering Fruit!\n";

	Vector2 pos = offset.pos;
	float dir = data.dir + offset.dir;
	float width = LERP(offset.width, data.width, data.widthAdoption);
	FloatColour colour;
	colour.r = floor(LERP(offset.colour.r, data.colour.r, data.colourAdoption));
	colour.g = floor(LERP(offset.colour.g, data.colour.g, data.colourAdoption));
	colour.b = floor(LERP(offset.colour.b, data.colour.b, data.colourAdoption));

	for (int i = 0; i < data.length; i++)
	{
		/*
#define FRUIT_CHANGE(attr) attr.valueChange * sigmoidDerivative((i - attr.changeOffset) / attr.changeLength)
		float dirChange = (
			FRUIT_CHANGE(data.dirChanges[0]) + 
			FRUIT_CHANGE(data.dirChanges[1]) + 
			FRUIT_CHANGE(data.dirChanges[2])
		);
		float widthChange = (
			FRUIT_CHANGE(data.widthChanges[0]) +
			FRUIT_CHANGE(data.widthChanges[1]) +
			FRUIT_CHANGE(data.widthChanges[2])
		);
		FloatColour colourChange = (
			FRUIT_CHANGE(data.colourChanges[0]) +
			FRUIT_CHANGE(data.colourChanges[1]) +
			FRUIT_CHANGE(data.colourChanges[2])
		);*/

		float dirChange = 0;
		float widthChange = 0;
		FloatColour colourChange;

		dir += (dirChange + ((((rand() % 201) - 100) / 100.0f) * data.randomTurn)) * ((int)data.isDirPositive * 2 - 1);
		pos += Vector2(0, -1).rotateNew(dir);
		width += widthChange;
		colour += colourChange;

		RenderBranchSegment(circle, pos, width, colour, window);
	}
}

void Branch::Render(
	const std::unique_ptr<sf::CircleShape>& circle, 
	sf::RenderWindow* window, 
	const Buffer<Branch>& allBranches, const Buffer<Fruit>& allFruit, 
	const BranchOrientation& offset = BranchOrientation(),
	uint32_t recursionDepth
) const
{
	//std::cout << "Rendering Branch!\n";

	Vector2 pos = offset.pos;
	float dir = data.dir + offset.dir;
	float width = LERP((recursionDepth == 0 ? data.width : offset.width), data.width, data.widthAdoption);
	FloatColour colour;
	colour.r = floor(LERP((recursionDepth == 0 ? data.colour.r : offset.colour.r), data.colour.r, data.colourAdoption));
	colour.g = floor(LERP((recursionDepth == 0 ? data.colour.g : offset.colour.g), data.colour.g, data.colourAdoption));
	colour.b = floor(LERP((recursionDepth == 0 ? data.colour.b : offset.colour.b), data.colour.b, data.colourAdoption));
	colour.a = 100;

	for (int i = 0; i < data.length; i++)
	{
		dir += (data.dirChange + ((((rand() % 201) - 100) / 100.0f) * data.randomTurn)) * ((int)data.isDirPositive * 2 - 1);
		pos += Vector2(0, -1).rotateNew(dir);
		width += data.widthChange;
		colour += data.colourChange;

		RenderBranchSegment(circle, pos, width, colour, window);

		if (recursionDepth < MAX_RECUSION_DEPTH)
		{
			for (int j = 0; j < 3; j++)
			{
				if (i == data.branchingPoints[j] && data.branchIndexes[j] >= 0)
				{
					if (isChildABranch[j])
						allBranches[childIndices[j]].Render(circle, window, allBranches, allFruit, { pos, dir, colour, width }, recursionDepth + 1);
					else
						allFruit[childIndices[j]].Render(circle, window, allFruit, { pos, dir, colour, width });
				}
			}
		}
	}
}

Plant::Plant(Vector2 pos, sf::RenderWindow* window)
	:pos(pos), window(window)
{
	m_Branches.Create(GetBranchCount());
	m_Fruits.Create(GetFruitCount());

	InitBranches();

	std::cout << "Creating " << m_Branches.Size() << " Branches and " << m_Fruits.Size() << " Fruits!\n";
}

void Plant::Render()
{
	m_Branches[0].Render(
		m_BranchRenderShape, window, 
		m_Branches, m_Fruits,
		BranchOrientation{ pos, 0, FloatColour{ 0, 0, 0 } }
	);
}

uint32_t Plant::GetBranchCount(int genomeIdx, uint8_t recursionDepth)
{
	if (genomeIdx < 0 || genomeIdx > 9) return 0;
	if (recursionDepth >= MAX_RECUSION_DEPTH) return 1;

	return (
		GetBranchCount(branchGenes[genomeIdx].branch0, recursionDepth + 1) +
		GetBranchCount(branchGenes[genomeIdx].branch1, recursionDepth + 1) +
		GetBranchCount(branchGenes[genomeIdx].branch2, recursionDepth + 1) +
		1
	);
}

uint32_t Plant::GetFruitCount(int genomeIdx, uint8_t recursionDepth)
{
	if (genomeIdx < 0 || recursionDepth >= MAX_RECUSION_DEPTH) return 0;
	if (genomeIdx >= 10) return 1;

	return (
		GetFruitCount(branchGenes[genomeIdx].branch0, recursionDepth + 1) +
		GetFruitCount(branchGenes[genomeIdx].branch1, recursionDepth + 1) +
		GetFruitCount(branchGenes[genomeIdx].branch2, recursionDepth + 1)
	);
}

uint32_t Plant::InitBranches(uint32_t genomeIdx, uint8_t recursionDepth, Branch* parent)
{
	uint32_t currentIndex = m_IntermediateBranchCount;
	m_Branches[currentIndex].Create(branchGenes[genomeIdx], parent);
	m_IntermediateBranchCount++;

	if (recursionDepth >= MAX_RECUSION_DEPTH)
		return currentIndex;

	for (int i = 0; i < 3; i++)
	{
		int index = m_Branches[currentIndex].data.branchIndexes[i];
		if (index < 0) continue;

		if (index < 10)
		{
			m_Branches[currentIndex].childIndices[i] = InitBranches(index, recursionDepth + 1, &m_Branches[currentIndex]);
		}
		else
		{
			m_Branches[currentIndex].childIndices[i] = InitFruit(index, &m_Branches[currentIndex]);
			m_Branches[currentIndex].isChildABranch[i] = false;
		}			
	}

	return currentIndex;
}

uint32_t Plant::InitFruit(uint32_t genomeIdx, Branch* parent)
{
	uint32_t currentIndex = m_IntermediateFruitCount;
	m_Fruits[currentIndex].Create(fruitGenes[genomeIdx - 10], parent);
	m_IntermediateFruitCount++;

	return currentIndex;
}
