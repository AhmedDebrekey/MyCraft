#include "Chunk.h"
#include "glm/gtc/noise.hpp"

Chunk::Chunk(uint8_t ChunkSize)
	:mChunkSize(ChunkSize)
{
	GenChunk();

	GenChunkMesh();
}

Chunk::~Chunk()
{
}

void Chunk::GenChunk()
{
	for (size_t x = 0; x < mChunkSize; x++)
	{
		for (size_t z = 0; z < mChunkSize; z++)
		{
			float Height = glm::simplex(glm::vec2(x * 64.0f,z * 64.0f)); 

			Height = (Height + 1) / 2;
			Height *= 32 + 32;
			for (size_t y = 0; y < Height; y++)
			{
				Blocks.push_back({ 1, glm::vec3(x,y,z)});
			}
		}
	}
}

bool Chunk::CheckIfInChunkBounds(glm::vec3 pos)
{
	for (size_t i = 0; i < Blocks.size(); i++)
	{
		if (Blocks.at(i).pos == pos)
		{
			return true;
		}
	}
	return false;
}

uint8_t Chunk::FindBlockIndex(glm::vec3 pos)
{
	for(size_t i = 0; i < Blocks.size(); i++)
	{
		if (Blocks.at(i).pos == pos)
		{
			return i;
		}
	}
}



void Chunk::GenChunkMesh()
{
	for (size_t x = 0; x < mChunkSize; x++)
	{
		for (size_t z = 0; z < mChunkSize; z++)
		{
			float Height = glm::simplex(glm::vec2(x * 64.0f, z * 64.0f));  

			Height = (Height + 1) / 2;
			Height *= 32 + 32;
			for (size_t y = 0; y < Height; y++)
			{
				if (!CheckIfInChunkBounds(glm::vec3(x, y, z - 1)))
				{
					//add this cube at x , y , z;
					uint32_t data = uint32_t(x) | uint32_t(z) << 5;
					mvChunkBlocks.push_back({ data, float(y) });
					continue;
				}

				if (!CheckIfInChunkBounds(glm::vec3(x, y, z + 1)))
				{
					//add this cube at x , y , z;
					uint32_t data = uint32_t(x) | uint32_t(z) << 5;
					mvChunkBlocks.push_back({ data, float(y) });
					continue;
				}

				if (!CheckIfInChunkBounds(glm::vec3(x, y - 1, z)))
				{
					//add this cube at x , y , z;
					uint32_t data = uint32_t(x) | uint32_t(z) << 5;
					mvChunkBlocks.push_back({ data, float(y) });
					continue;
				}

				if (!CheckIfInChunkBounds(glm::vec3(x, y + 1, z)))
				{
					//add this cube at x , y , z;
					uint32_t data = uint32_t(x) | uint32_t(z) << 5;
					mvChunkBlocks.push_back({ data, float(y) });
					continue;
				}

				if (!CheckIfInChunkBounds(glm::vec3(x - 1, y, z)))
				{
					//add this cube at x , y , z;
					uint32_t data = uint32_t(x) | uint32_t(z) << 5;
					mvChunkBlocks.push_back({ data, float(y) });
					continue;
				}

				if (!CheckIfInChunkBounds(glm::vec3(x + 1, y, z)))
				{
					//add this cube at x , y , z;
					uint32_t data = uint32_t(x) | uint32_t(z) << 5;
					mvChunkBlocks.push_back({ data, float(y) });
					continue;
				}

			}
		}
	}
	Blocks.clear();
	Blocks.shrink_to_fit();
	//for (size_t i = 0; i < Blocks.size(); i++)
	//{
	//	mvChunkBlocks.push_back({ Blocks[i].pos });
	//}
}