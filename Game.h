#pragma once
#include "Window.h"
#include "PerspectiveCamera.h"
#include "Shader.h"
#include "Chunk.h"
#include "VertexArray.h"
#include "BiomeManager.h"

class Game
{
public:
	Game(const char* name);

	void Inits();
	void Run();
private:
	Window window;
	GLFWwindow* m_Window;
	Camera camera;

private:
	Shader SquareShader;
	uint32_t texture;
	size_t m_ChunkSize1, m_ChunkSize2;
	uint32_t SecondChunkVAO;
	VertexArray Cube_VAO;
	BiomeManager Biome;
	std::vector<BiomeManager> myBiomes;
};

