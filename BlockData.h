#pragma once
#include <iostream>

using u32 = unsigned int;
static constexpr u32 SizeOfChunk = 16; //has to match with the size of the Map generated by the noise


struct Vertex
{
	glm::vec4 Positions;
	glm::vec2 TexCoords;
	float LightLevel;
};

enum Side
{
	eFront = 0, eBack, eTop, eBottom, eLeft, eRight
};

enum BlockType
{
	Air = 0, Grass, Stone, Sand
};

inline std::ostream& operator<<(std::ostream& os, const glm::vec3& data)
{
	return os << data.x << " , " << data.y << " , " << data.z;
}