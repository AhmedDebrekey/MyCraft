#pragma once
#include <cstdlib> //abs()
#include <map>
#include <unordered_map>
#include <optional>

#include "Block.h"

#include "Renderer.h"

#include "BlockData.h"
#include "glm/gtc/noise.hpp"
#include "glm/gtx/hash.hpp"
using u32 = unsigned int;


struct ChunkBounds
{
	uint32_t Index;
	bool inBounds;
};

class ChunkMesh
{
public:
	ChunkMesh()
	{}

	void GenChunk(float* HeightMap);

	~ChunkMesh() {}

	auto GetVerticies() const { return m_Verticies; }
	Block* FindPos(glm::vec3);

	void ReGenMesh() 
	{
		m_Verticies.clear();
		GenChunkMesh(); 
	}

	void DeletePos(glm::vec3 pos)
	{
		if (m_PosMap.erase(pos) == 1)
		{
			std::cout << "Deleted" << std::endl;
		}
	}

private:
	void GenChunk();
	void GenChunkMesh();

	Block* FindBlock(glm::vec3 pos);

private:
	glm::vec2 m_ChunkPositions;
	std::vector<Vertex> m_Verticies;
	std::unordered_map<glm::vec3, Block> m_PosMap; 
};

inline void ChunkMesh::GenChunk(float* HeightMap)
{
	int Index = 0;
	for (int x = 0; x < SizeOfChunk; x++)
	{
		for (int z = 0; z < SizeOfChunk; z++)
		{
			float height = abs(HeightMap[Index]);
			//height = (height + 1) / 2;
			//height = (height * 10) / 2;
			height += 2;

			//std::cout << "Height[" << Index << "] " << height << std::endl;

			for (int y = 0; y < int(height); y++)
			{
				//TODO: Add trees or smth smiliar to minecraft :)

				if (y > height - 4)
				{
					m_PosMap.insert(std::pair<glm::vec3, Block>(glm::vec3(x, y, z), { BlockType::Grass, glm::vec3(x, y, z) }));
				}
				else if(y < height - 4)
					m_PosMap.insert(std::pair<glm::vec3, Block>(glm::vec3(x, y, z), { BlockType::Stone, glm::vec3(x, y, z) }));

			}
			Index++;
		}
	}
	Time(GenChunkMesh());
}

inline Block* ChunkMesh::FindBlock(glm::vec3 pos)
{
	auto it = m_PosMap.find(pos);
	if (it != m_PosMap.end())
		return &it->second;
	return nullptr;
}

inline void ChunkMesh::GenChunkMesh()
{
	for(auto it = m_PosMap.begin(); it!= m_PosMap.end(); it++)
	{
		glm::vec3 positions = it->first;
		
		auto result = FindBlock(glm::vec3(positions.x, positions.y, positions.z - 1));

		if (result == nullptr)
		{
			auto FaceVerticies = result->CalculateModel(Side::eBack, positions, it->second.getType());
			for (uint32_t i = 0; i < FaceVerticies.size(); i++)
			{
				m_Verticies.push_back(FaceVerticies.at(i));
			}
		}

		result = FindBlock(glm::vec3(positions.x, positions.y, positions.z + 1));

		if (result == nullptr)
		{
			auto FaceVerticies = result->CalculateModel(Side::eFront, positions, it->second.getType());
			for (uint32_t i = 0; i < FaceVerticies.size(); i++)
			{
				m_Verticies.push_back(FaceVerticies.at(i));
			}
		}

		result = FindBlock(glm::vec3(positions.x, positions.y - 1, positions.z));

		if (result == nullptr)
		{
			auto FaceVerticies = result->CalculateModel(Side::eBottom, positions, it->second.getType());
			for (uint32_t i = 0; i < FaceVerticies.size(); i++)
			{
				m_Verticies.push_back(FaceVerticies.at(i));
			}
		}

		result = FindBlock(glm::vec3(positions.x, positions.y + 1, positions.z));

		if (result == nullptr)
		{
			auto FaceVerticies = result->CalculateModel(Side::eTop, positions, it->second.getType());
			for (uint32_t i = 0; i < FaceVerticies.size(); i++)
			{
				m_Verticies.push_back(FaceVerticies.at(i));
			}
		}

		result = FindBlock(glm::vec3(positions.x - 1, positions.y, positions.z));

		if (result == nullptr)
		{
			auto FaceVerticies = result->CalculateModel(Side::eLeft, positions, it->second.getType());
			for (uint32_t i = 0; i < FaceVerticies.size(); i++)
			{
				m_Verticies.push_back(FaceVerticies.at(i));
			}
		}

		result = FindBlock(glm::vec3(positions.x + 1, positions.y, positions.z));

		if (result == nullptr)
		{
			auto FaceVerticies = result->CalculateModel(Side::eRight, positions, it->second.getType());
			for (uint32_t i = 0; i < FaceVerticies.size(); i++)
			{
				m_Verticies.push_back(FaceVerticies.at(i));
			}
		}
	}
}

inline Block* ChunkMesh::FindPos(glm::vec3 pos)
{
	auto amount = (int)pos.x % SizeOfChunk;
	glm::vec3 Postion = glm::vec3((int)pos.x >= SizeOfChunk ? (int)pos.x - SizeOfChunk * (int)amount : (int)pos.x, (int)pos.y, (int)pos.z >= SizeOfChunk ? (int)pos.z - SizeOfChunk * (int)amount : (int)pos.z);
	//std::cout << "searching: " << Postion << std::endl;
	auto it = m_PosMap.find(Postion);
	if (it != m_PosMap.end())
		return &it->second;
	return nullptr;
}