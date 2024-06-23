#pragma once

#include "Genome.h"

#include "Buffer.h"
#include "Vector2.h"

#include "Branch.h"
#include "Fruit.h"

#include <memory>

#define LERP(a, b, k) a * k + b * (1 - k)

class Plant
{
public:
	Plant() {}
	Plant(Vector2 pos, sf::RenderWindow* window);

	void Render();
	
public:
	Vector2 pos;
	sf::RenderWindow* window = nullptr;

	static const uint32_t BRANCH_COUNT = 10;
	static const uint32_t FRUIT_COUNT = 2;
	Buffer<BranchGenome> branchGenes { BRANCH_COUNT };
	Buffer<FruitGenome> fruitGenes { FRUIT_COUNT };

private:
	uint32_t GetBranchCount(int genomeIdx = 0, uint8_t recursionDepth = 0);
	uint32_t GetFruitCount(int genomeIdx = 0, uint8_t recursionDepth = 0);

	uint32_t InitBranches(uint32_t genomeIdx = 0, uint8_t recursionDepth = 0, Branch* parent = nullptr);
	uint32_t InitFruit(uint32_t genomeIdx, Branch* parent);

private:
	std::unique_ptr<sf::CircleShape> m_BranchRenderShape = std::make_unique<sf::CircleShape>();
	uint32_t m_IntermediateBranchCount = 0;
	uint32_t m_IntermediateFruitCount = 0;
	Buffer<Branch> m_Branches{};
	Buffer<Fruit> m_Fruits{};
};
