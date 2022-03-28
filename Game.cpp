#include <iostream>

#include <stb_image/stb_image.h>

#include "Game.h"

#include "VertexBuffer.h"
#include "VertrexBufferLayout.h"

#include "Renderer.h"

template<typename T>
size_t sizeof_vec(std::vector<T> const& v)
{
	return v.size() * sizeof(T);
}

Game::Game(const char* name)
	:window(name),
	m_Window(window.GetWindow()),
	camera(m_Window),
	texture(0)
{
}


void Game::Inits()
{
	Biome.Setup();

	SquareShader.Create("SquareVertex.glsl", "SquareFragment.glsl");
	SquareShader.Bind();

	// load and create a texture 
	// -------------------------
	stbi_set_flip_vertically_on_load(1);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load("textures/Spritesheet.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
#if 0
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
	SquareShader.Unbind();

}

void Game::Run()
{
	Inits();

	double previousTime = glfwGetTime();
	int frameCount = 0;

	while (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		!glfwWindowShouldClose(m_Window))
	{
		double currentTime = glfwGetTime();
		frameCount++;
		if (currentTime - previousTime >= 1.0)
		{
			std::cout << frameCount  << " fps" << std::endl;
			std::cout << "Camera Position : " << (int)camera.Position().x << " , " << (int)camera.Position().y << " , " << (int)camera.Position().z << std::endl;
			auto CameraDirection = camera.Front();
			std::cout << "Camera Direction: " << CameraDirection.x << " , " << CameraDirection.y << " , " << CameraDirection.z << std::endl;

			frameCount = 0;
			previousTime = currentTime;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor((1.0f / 255) * 86, (1.0f / 255) * 204, (1.0f / 255) * 219, 1.0);

		SquareShader.Bind();
		camera.Compute();
		SquareShader.SetMat4f("view", camera.ViewMatrix());
		SquareShader.SetMat4f("projection", camera.ProjectionMatrix());
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, texture);

		Biome.RenderBiome(SquareShader, camera.Position(), m_Window);

		// Swap front and back buffers

		glfwSwapBuffers(m_Window);

		// Poll for and process events
		glfwPollEvents();
	}
}