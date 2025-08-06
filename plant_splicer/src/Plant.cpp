#include "Plant.h"

#define MAX_RECUSION_DEPTH 5


Branch::Branch(BranchGenome& genomeData, Branch* parentBranch)
{
	Create(genomeData, parentBranch);
}

bool IsCloseEnough(float x, float y, float closeRange = 15)
{
	if (abs(x - y) <= closeRange)
	{
		return true;
	}
	return false;
}

void Branch::Create(BranchGenome& genomeData, Branch* parentBranch, int gIdx, int chInd)
{
	data.length = genomeData.length + (int)(genomeData.lengthVariation * ((rand() % 201) - 100) / 100.0f);

	data.branchIndexes[0] = genomeData.branch0;
	data.branchIndexes[1] = genomeData.branch1;
	data.branchIndexes[2] = genomeData.branch2;
	data.branchIndexes[3] = genomeData.branch3;
	data.branchIndexes[4] = genomeData.branch4;
	data.branchIndexes[5] = genomeData.branch5;

	data.branchingPoints[0] = (int)floor(genomeData.branch0Position * data.length) - 1;
	data.branchingPoints[1] = (int)floor(genomeData.branch1Position * data.length) - 1;
	data.branchingPoints[2] = (int)floor(genomeData.branch2Position * data.length) - 1;
	data.branchingPoints[3] = (int)floor(genomeData.branch3Position * data.length) - 1;
	data.branchingPoints[4] = (int)floor(genomeData.branch4Position * data.length) - 1;
	data.branchingPoints[5] = (int)floor(genomeData.branch5Position * data.length) - 1;

	data.rBranchIndexes[0] = genomeData.rBranch0;
	data.rBranchIndexes[1] = genomeData.rBranch1;
	data.rBranchIndexes[2] = genomeData.rBranch2;
	data.conBranchIndexes[0] = genomeData.cRBranch0;
	data.conBranchIndexes[1] = genomeData.cRBranch1;
	data.conBranchIndexes[2] = genomeData.cRBranch2;

	data.randomTurn = genomeData.randTurn;
	data.isDirPositive = rand() % 2;

	data.dirChange = genomeData.dirChange;
	data.widthChange = genomeData.widthChange;
	data.colourChange = genomeData.colourChange;

	data.colour = genomeData.initColour;
	data.colourAdoption = genomeData.colourAdoption;

	data.widthAdoption = genomeData.widthAdoption;
	data.dirAdoption = genomeData.dirAdoption;

	data.width = genomeData.initWidth;
	data.dir = genomeData.initDir;

	if (!parentBranch || genomeData.dirSpread < 0.01) { return; }

	int numOfSiblings = 0;
	int youngness = 0;
	int thisIdx = 0;

	for (int i = 0; i < 6; i++)
	{
		if (parentBranch->childIndices[i] == 0 && parentBranch->data.branchIndexes[i] == gIdx)
		{
			thisIdx = i;
			break;
		}
	}

	for (int i = 0; i < 6; i++)
	{
		if (parentBranch->data.branchIndexes[i] == gIdx && IsCloseEnough(parentBranch->data.branchingPoints[i], parentBranch->data.branchingPoints[thisIdx], genomeData.spreadMaxDistanceEff))
		{
			numOfSiblings++;
			if (thisIdx != 0 && i < thisIdx)
				youngness++;
		}
	}

	if (numOfSiblings < 2)
		return;

	data.spreadOff += ((genomeData.dirSpread * 1.0f / numOfSiblings * 1.0f) * youngness * 1.0f) - (float)(genomeData.dirSpread / PI);
}

static void CapBranchSegment(
	const std::unique_ptr<sf::VertexArray>& light,
	const std::unique_ptr<sf::VertexArray>& shadow,
	const std::unique_ptr<sf::VertexArray>& actual,
	Vector2 position, float width, const FloatColour& colour, float dir,
	float closemultiple = 1, float zoom = 1
) {
	for (int i = 1; i < 5; i++)
	{
		float closingrotation = (float(PI) / 2.0) * (i / 4.0) * closemultiple;
		float r = (width * zoom);
		Vector2 right = { cos(dir - closingrotation) * r + position.x, sin(dir - closingrotation) * r + position.y };
		Vector2 left = { cos(dir + float(PI) + closingrotation) * r + position.x, sin(dir + float(PI) + closingrotation) * r + position.y };

		// Light
		sf::Vertex lrvtx;
		sf::Vertex llvtx;
		lrvtx.position = { right.x + cos(dir + float(PI)), right.y + sin(dir + float(PI)) };
		lrvtx.color = FloatColour{ colour.r + 60, colour.g + 60, colour.b + 60, 10 };
		llvtx.position = { left.x + cos(dir + float(PI)), left.y + sin(dir + float(PI)) };
		llvtx.color = lrvtx.color;
		light->append(llvtx);
		light->append(lrvtx);

		// Shadow
		sf::Vertex srvtx;
		sf::Vertex slvtx;
		srvtx.position = { right.x + cos(dir), right.y + sin(dir) };
		srvtx.color = (FloatColour(
			abs(colour.r - 25) + (colour.r - 25),
			abs(colour.g - 25) + (colour.g - 25),
			abs(colour.b - 25) + (colour.b - 25),
			10
		));
		slvtx.position = { left.x + cos(dir), left.y + sin(dir) };
		slvtx.color = srvtx.color;
		shadow->append(slvtx);
		shadow->append(srvtx);

		// Actual
		sf::Vertex rvtx;
		sf::Vertex lvtx;
		rvtx.position = { right.x, right.y };
		rvtx.color = colour;
		lvtx.position = { left.x, left.y };
		lvtx.color = rvtx.color;
		lvtx.color.a = 10;
		actual->append(lvtx);
		actual->append(rvtx);
	}
}

static void RenderBranchSegment(
	const std::unique_ptr<sf::VertexArray>& light,
	const std::unique_ptr<sf::VertexArray>& shadow,
	const std::unique_ptr<sf::VertexArray>& actual,
	Vector2 position, float width, const FloatColour& colour, float dir,
	float zoom = 1
) {
	float r = width * zoom;
	Vector2 right = { cos(dir) * r + position.x, sin(dir) * r + position.y};
	Vector2 left = { cos(dir + float(PI)) * r + position.x, sin(dir + float(PI)) * r + position.y };

	// Light
	sf::Vertex lrvtx;
	sf::Vertex llvtx;
	lrvtx.position = { right.x + cos(dir + float(PI)), right.y + sin(dir + float(PI))};
	lrvtx.color = FloatColour{ colour.r + 60, colour.g + 60, colour.b + 60, 10 };
	llvtx.position = { left.x + cos(dir + float(PI)), left.y + sin(dir + float(PI)) };
	llvtx.color = lrvtx.color;
	light->append(llvtx);
	light->append(lrvtx);

	// Shadow
	sf::Vertex srvtx;
	sf::Vertex slvtx;
	srvtx.position = { right.x + cos(dir), right.y + sin(dir)};
	srvtx.color = (FloatColour(
		abs(colour.r - 25) + (colour.r - 25),
		abs(colour.g - 25) + (colour.g - 25),
		abs(colour.b - 25) + (colour.b - 25),
		10
	));
	slvtx.position = { left.x + cos(dir), left.y + sin(dir)};
	slvtx.color = srvtx.color;
	shadow->append(slvtx);
	shadow->append(srvtx);

	// Actual
	sf::Vertex rvtx;
	sf::Vertex lvtx;
	rvtx.position = { right.x, right.y};
	rvtx.color = colour;
	lvtx.position = { left.x, left.y };
	lvtx.color = rvtx.color;
	lvtx.color.a = 10;
	actual->append(lvtx);
	actual->append(rvtx);
}

void Branch::RenderBranch(
	const std::shared_ptr<sf::RenderWindow>& window,
	const Buffer<Branch>& allBranches, 
	const Branch::Orientation& offset = Branch::Orientation(), float zoom,
	uint32_t recursionDepth, bool conRec
) const {
	FloatColour colour;

	Vector2 pos = offset.pos;
	uint32_t trecurs = conRec ? 1 : recursionDepth;
	float dir = LERP(data.dir, offset.dir, data.dirAdoption) + data.spreadOff;
	float width = LERP((trecurs == 0 ? data.width : offset.width), data.width, data.widthAdoption);
	colour.r = floor(LERP((trecurs == 0 ? data.colour.r : offset.colour.r), data.colour.r, data.colourAdoption));
	colour.g = floor(LERP((trecurs == 0 ? data.colour.g : offset.colour.g), data.colour.g, data.colourAdoption));
	colour.b = floor(LERP((trecurs == 0 ? data.colour.b : offset.colour.b), data.colour.b, data.colourAdoption));

	std::unique_ptr<sf::VertexArray> light = std::make_unique <sf::VertexArray>(sf::PrimitiveType::TriangleStrip, 0);
	std::unique_ptr<sf::VertexArray> shadow = std::make_unique <sf::VertexArray>(sf::PrimitiveType::TriangleStrip, 0);
	std::unique_ptr<sf::VertexArray> actual = std::make_unique <sf::VertexArray>(sf::PrimitiveType::TriangleStrip, 0);

	for (int i = 0; i < data.length; i++)
	{
		dir += (data.dirChange + ((((rand() % 201) - 100) / 100.0f) * data.randomTurn)) * ((int)data.isDirPositive * 2 - 1);
		pos += Vector2(0, -1).rotateNew(dir) * zoom;
		width += data.widthChange * zoom;
		colour += data.colourChange;

		RenderBranchSegment(light, shadow, actual, pos, width, colour, dir, zoom);

		if (recursionDepth < MAX_RECUSION_DEPTH)
		{
			for (int j = 0; j < 6; j++)
			{
				if (i == data.branchingPoints[j] && data.branchIndexes[j] >= 0)
				{
					CapBranchSegment(light, shadow, actual, pos, width, colour, dir, 1, zoom);
					window->draw(*light);
					window->draw(*shadow);
					window->draw(*actual);
					light->clear();
					shadow->clear();
					actual->clear();
					allBranches[childIndices[j]].RenderBranch(window, allBranches, { pos, dir, colour, width }, zoom, recursionDepth + 1, conRec);
				}
			}
		}
		else if (recursionDepth == MAX_RECUSION_DEPTH)
		{
			for (int j = 0; j < 3; j++)
			{
				if (i == data.length - 1 && data.rBranchIndexes[j] >= 0)
				{
					CapBranchSegment(light, shadow, actual, pos, width, colour, dir, 1, zoom);
					window->draw(*light);
					window->draw(*shadow);
					window->draw(*actual);
					light->clear();
					shadow->clear();
					actual->clear();
					allBranches[childIndices[6 + j]].RenderBranch(window, allBranches, { pos, dir, colour, width }, zoom, recursionDepth + 1, conRec);
				}
			}

			if (!conRec)
			{
				for (int j = 0; j < 3; j++)
				{
					if (i == data.length - 1 && data.conBranchIndexes[j] >= 0)
					{
						CapBranchSegment(light, shadow, actual, pos, width, colour, dir, 1, zoom);
						window->draw(*light);
						window->draw(*shadow);
						window->draw(*actual);
						light->clear();
						shadow->clear();
						actual->clear();
						allBranches[childIndices[9 + j]].RenderBranch(window, allBranches, { pos, dir, colour, width }, zoom, 0, true);
					}
				}
			}
		}
	}

	if (light->getVertexCount() > 0)
	{
		CapBranchSegment(light, shadow, actual, pos, width, colour, dir, 1, zoom);
		window->draw(*light);
		window->draw(*shadow);
		window->draw(*actual);
	}
}

Plant::Plant(Vector2 pos, const std::shared_ptr<sf::RenderWindow>& window)
	:pos(pos), window(window)
{
	m_Branches.Create(GetBranchCount());
	InitBranches();
}

Plant::Plant(Vector2 pos, const std::shared_ptr<sf::RenderWindow>& window, uint16_t seed)
	:pos(pos), window(window)
{
	srand(seed);
	m_Branches.Create(GetBranchCount());
	InitBranches();
}

void Plant::Render(float zoom)
{
	m_Branches[0].RenderBranch(window, 
		m_Branches,
		Branch::Orientation { pos, 0, FloatColour{ 0, 0, 0 } }, zoom
	);
}

uint32_t Plant::GetBranchCount(uint32_t genomeIdx, uint8_t recursionDepth, bool conRec)
{
	if (genomeIdx > 9) return 0;

	uint16_t c = 0;

	if (recursionDepth == MAX_RECUSION_DEPTH)
	{
		c += GetBranchCount(branchGenes[genomeIdx].rBranch0, recursionDepth + 1, conRec);
		c += GetBranchCount(branchGenes[genomeIdx].rBranch1, recursionDepth + 1, conRec);
		c += GetBranchCount(branchGenes[genomeIdx].rBranch2, recursionDepth + 1, conRec);
		if (!conRec)
		{
			c += GetBranchCount(branchGenes[genomeIdx].cRBranch0, 0, true);
			c += GetBranchCount(branchGenes[genomeIdx].cRBranch1, 0, true);
			c += GetBranchCount(branchGenes[genomeIdx].cRBranch2, 0, true);
		}
	}
	
	if (recursionDepth >= MAX_RECUSION_DEPTH) return 1 + c;

	return (
		GetBranchCount(branchGenes[genomeIdx].branch0, recursionDepth + 1, conRec) +
		GetBranchCount(branchGenes[genomeIdx].branch1, recursionDepth + 1, conRec) +
		GetBranchCount(branchGenes[genomeIdx].branch2, recursionDepth + 1, conRec) +
		GetBranchCount(branchGenes[genomeIdx].branch3, recursionDepth + 1, conRec) +
		GetBranchCount(branchGenes[genomeIdx].branch4, recursionDepth + 1, conRec) +
		GetBranchCount(branchGenes[genomeIdx].branch5, recursionDepth + 1, conRec) +
		1 + c
	);
}

uint32_t Plant::InitBranches(uint32_t genomeIdx, uint8_t recursionDepth, Branch* parent, bool conRec)
{
	uint32_t currentIndex = m_IntermediateBranchCount;
	m_Branches[currentIndex].Create(branchGenes[genomeIdx], parent, genomeIdx, currentIndex);
	m_IntermediateBranchCount++;

	if (recursionDepth == MAX_RECUSION_DEPTH)
	{
		for (int i = 0; i < 3; i++)
		{
			int index = m_Branches[currentIndex].data.rBranchIndexes[i];
			if (index >= 0)
				m_Branches[currentIndex].childIndices[i + 6] = InitEndBranches(index, recursionDepth + 1, &m_Branches[currentIndex]);
		}
		if (!conRec)
		{
			for (int i = 0; i < 3; i++)
			{
				int index = m_Branches[currentIndex].data.conBranchIndexes[i];
				if (index >= 0)
				{
					m_Branches[currentIndex].childIndices[i + 9] = InitBranches(index, 0, &m_Branches[currentIndex], true);
				}
			}
		}
	}

	if (recursionDepth >= MAX_RECUSION_DEPTH)
		return currentIndex;

	for (int i = 0; i < 6; i++)
	{
		int index = m_Branches[currentIndex].data.branchIndexes[i];
		if (index >= 0)
			m_Branches[currentIndex].childIndices[i] = InitBranches(index, recursionDepth + 1, &m_Branches[currentIndex], conRec);
	}


	return currentIndex;
}

uint32_t Plant::InitEndBranches(uint32_t genomeIdx, uint8_t recursionDepth, Branch* parent)
{
	uint32_t currentIndex = m_IntermediateBranchCount;
	m_Branches[currentIndex].Create(branchGenes[genomeIdx], parent);
	m_IntermediateBranchCount++;

	return currentIndex;
}

void Plant::InitAllBranches(uint32_t seed)
{
	srand(seed);
	m_IntermediateBranchCount = 0;
	m_Branches.ReCreate(GetBranchCount());
	InitBranches();
}
