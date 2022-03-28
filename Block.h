#pragma once
#include <array>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "BlockData.h"
 
#include "Renderer.h"


using BlockFace = std::array<float, 42>;

/*
*How to change the Verticies layout
* 
* 1. Change the BlockFace size to the respective size of your layout.
* 2. In the CalculateModel() function change the loop iteration (ex: i+=n) to the width of your layout.
* 3. Make sure that the Vertex Attributes are done correctly.
*/

/*
*How to add a block to the game
* 
* 1. Make sure that the block is in the texture atlas / sprite sheet.
* 2. In the getTexPos(Type) function add the texture index of the block
* 3. Make sure to add it to the world generation code. GenChunk(heightmap) function is the one to look for.
*/

//		4VertexPos  2TexCoords	1LightLevel
static BlockFace Front = {
		0, 0, 1, 1,		0, 0,	1,
		1, 0, 1, 1,		1, 0,	1,
		1, 1, 1, 1,		1, 1,	1,
		1, 1, 1, 1,		1, 1,	1,
		0, 1, 1, 1,		0, 1,	1,
		0, 0, 1, 1,		0, 0,	1
};


//		4VertexPos  2TexCoords	1LightLevel
static BlockFace Back = {
		0, 0, 0, 1,		0, 0,	0.8,
		1, 0, 0, 1,		1, 0,	0.8,
		1, 1, 0, 1,		1, 1,	0.8,
		1, 1, 0, 1,		1, 1,	0.8,
		0, 1, 0, 1,		0, 1,	0.8,
		0, 0, 0, 1,		0, 0,	0.8
};

//		4VertexPos  2TexCoords	1LightLevel
static BlockFace Top = {
		0, 1, 0, 1,		0, 0,	1,
		1, 1, 0, 1,		1, 0,	1,
		1, 1, 1, 1,		1, 1,	1,
		1, 1, 1, 1,		1, 1,	1,
		0, 1, 1, 1,		0, 1,	1,
		0, 1, 0, 1,		0, 0,	1
};


//		4VertexPos  2TexCoords	1LightLevel
static BlockFace Bottom = {
		0, 0, 0, 1,		0, 0,	0.8,
		1, 0, 0, 1,		1, 0,	0.8,
		1, 0, 1, 1,		1, 1,	0.8,
		1, 0, 1, 1,		1, 1,	0.8,
		0, 0, 1, 1,		0, 1,	0.8,
		0, 0, 0, 1,		0, 0,	0.8
};


//		4VertexPos  2TexCoords	1LightLevel
static BlockFace Right = {
		1, 1, 1, 1,		0, 0,	1,
		1, 1, 0, 1,		1, 0,	1,
		1, 0, 0, 1,		1, 1,	1,
		1, 0, 0, 1,		1, 1,	1,
		1, 0, 1, 1,		0, 1,	1,
		1, 1, 1, 1,		0, 0,	1
};


//		4VertexPos  2TexCoords	1LightLevel
static BlockFace Left = {
		0, 1, 1, 1,		0, 0,	0.8,
		0, 1, 0, 1,		1, 0,	0.8,
		0, 0, 0, 1,		1, 1,	0.8,
		0, 0, 0, 1,		1, 1,	0.8,
		0, 0, 1, 1,		0, 1,	0.8,
		0, 1, 1, 1,		0, 0,	0.8
};

class Block
{
public:
	Block(BlockType type, glm::vec3 pos)
		:m_Model(glm::mat4(1.0f)),
		 m_Type(type),
		 m_Position(pos)
	{
	}

	std::vector<Vertex> CalculateModel(Side side, glm::vec3 ModelPosition, BlockType type)
	{
		std::vector<Vertex> ModelVertex;

		BlockFace Vertex;
		Vertex = addFace(side);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(ModelPosition));

		glm::vec2 TexPos = getTexPos(type);
		float x = TexPos.x, y = TexPos.y;
		constexpr float sheetWidth = 480, sheetHeight = 240;
		constexpr float spriteWidth = 80, spriteHeight = 80;
		/*
			0, 1, 2, 3
			6, 7, 8, 9
			12, 13, 14, 15
			18, 19, 20, 21
			24, 25, 26, 27
			30, 31, 32, 33
		*/
		for (size_t i = 0; i < Vertex.size(); i += 7)
		{
			ModelVertex.push_back(
				{
					model * glm::vec4(Vertex.at(0 + i), Vertex.at(1 + i), Vertex.at(2 + i), Vertex.at(3 + i)), 
					glm::vec2(((x + Vertex.at(4 + i)) * spriteWidth) / sheetWidth, ((y + Vertex.at(5 + i)) * spriteHeight) / sheetHeight),
					Vertex.at(6 + i)
				});
		}

		return ModelVertex;
	}


private:
	BlockFace addFace(Side side)
	{
		//Add which face to be added, and is gonna be used in the chunk class
		if (side == Side::eFront)
		{
			return Front;
		}

		if (side == Side::eBack)
		{
			return Back;
		}

		if (side == Side::eTop)
		{
			return Top;
		}

		if (side == Side::eBottom)
		{
			return Bottom;
		}

		if (side == Side::eRight)
		{
			return Right;
		}

		if (side == Side::eLeft)
		{
			return Left;
		}
	}

	glm::vec2 getTexPos(BlockType Type)
	{
		if (Type == BlockType::Grass)
		{
			return glm::vec2(0, 1);
		}

		if (Type == BlockType::Stone)
		{
			return glm::vec2(2, 2);
		}

		if (Type == BlockType::Sand)
		{
			return glm::vec2(4, 2);
		}

		return glm::vec2(1, 1);
	}
public:
	auto getPos() const { return m_Position; }
	auto getType() const { return m_Type; }
	void Type(BlockType type) { m_Type = type; }
private:
	glm::mat4 m_Model;
	
	BlockType m_Type;
	glm::vec3 m_Position;

};

