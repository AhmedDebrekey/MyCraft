#pragma once
#include <iostream>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct BBlock
{
	uint8_t ID;
	glm::vec3 pos; //Inside the chunk that is
};

struct ChunkBlocks
{
	uint32_t blockPos;
	float height;
};

class Chunk
{
public:
	Chunk(uint8_t ChunkSize);

	~Chunk();

	void GenChunk();

	void GenChunkMesh();

	uint32_t GetBlockPositions(uint32_t index) { return mvChunkBlocks.at(index).blockPos; }
	float	 GetBlockHeight(uint32_t index) { return mvChunkBlocks.at(index).height; }
	uint32_t GetChunkSize() { return mvChunkBlocks.size(); }

	std::vector<ChunkBlocks>& GetBufferData() { return mvChunkBlocks; }
	auto GetFirstElement() const { return &mvChunkBlocks[0]; }

	void Clearmem() { mvChunkBlocks.clear(); mvChunkBlocks.shrink_to_fit(); }

private: 
	uint8_t mChunkSize;
	std::vector<BBlock> Blocks;
	std::vector<ChunkBlocks> mvChunkBlocks;
private:
	bool CheckIfInChunkBounds(glm::vec3 pos);
	uint8_t FindBlockIndex(glm::vec3 pos);
};
