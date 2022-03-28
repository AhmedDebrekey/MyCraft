#pragma once

#include <time.h> 

#include <noise/noise.h>

#include "noiseutils.h"

#include "glm/glm.hpp"

#include "BlockData.h"

namespace Debreky
{
	class ImageRenderer
	{
	public:
		ImageRenderer(utils::NoiseMap map, utils::NoiseMapBuilderPlane mapbuilder)
			:m_heightMap(map), m_heightMapBuilder(mapbuilder)
		{
			m_Renderer.SetSourceNoiseMap(m_heightMap);
			m_Renderer.SetDestImage(m_Image);
			m_Renderer.ClearGradient();
			m_Renderer.AddGradientPoint(-1.0000, utils::Color(0, 0, 128, 255)); // deeps
			m_Renderer.AddGradientPoint(-0.2500, utils::Color(0, 0, 255, 255)); // shallow
			m_Renderer.AddGradientPoint(0.0000, utils::Color(0, 128, 255, 255)); // shore
			m_Renderer.AddGradientPoint(0.0625, utils::Color(240, 240, 64, 255)); // sand
			m_Renderer.AddGradientPoint(0.1250, utils::Color(32, 160, 0, 255)); // grass
			m_Renderer.AddGradientPoint(0.3750, utils::Color(224, 224, 0, 255)); // dirt
			m_Renderer.AddGradientPoint(0.7500, utils::Color(128, 128, 128, 255)); // rock
			m_Renderer.AddGradientPoint(1.0000, utils::Color(255, 255, 255, 255)); // snow
			m_Renderer.EnableLight();
			m_Renderer.SetLightContrast(3.0); // Triple the contrast
			m_Renderer.SetLightBrightness(2.0); // Double the brightness
			m_Renderer.Render();
		}


		utils::Image Image() const { return m_Image; }

		void RenderImage(const char* Title)
		{
			auto image = Image();
			m_Writer.SetSourceImage(image);
			std::string fileType = ".bmp";
			m_Writer.SetDestFilename(Title + fileType);
			m_Writer.WriteDestFile();
		}

	private:
		utils::RendererImage m_Renderer;
		utils::Image m_Image;

		utils::NoiseMap m_heightMap;
		utils::NoiseMapBuilderPlane m_heightMapBuilder;

		utils::WriterBMP m_Writer;

	};

	class Perlin
	{
	public:


		void GenMap(uint8_t Size, glm::vec2 start, glm::vec2 end)
		{
			GenMap(Size, { start, end });
		}

		void GenMap(uint8_t Size, glm::vec4 coords)
		{
			module::RidgedMulti mountainTerrain;
			module::Billow baseFlatTerrain;
			baseFlatTerrain.SetFrequency(2.0);

			module::ScaleBias flatTerrain;
			flatTerrain.SetSourceModule(0, baseFlatTerrain);
			flatTerrain.SetScale(0.125);
			flatTerrain.SetBias(-0.75);

			m_PerlinModule.SetSeed(time(NULL));
			m_PerlinModule.SetFrequency(0.5);
			m_PerlinModule.SetPersistence(0.25);

			module::Select terrainSelector;
			terrainSelector.SetSourceModule(0, flatTerrain);
			terrainSelector.SetSourceModule(1, mountainTerrain);
			terrainSelector.SetControlModule(m_PerlinModule);
			terrainSelector.SetBounds(0.0, 1000.0);
			terrainSelector.SetEdgeFalloff(0.125);

			module::ScaleBias terrainScaler;
			terrainScaler.SetSourceModule(0, m_PerlinModule);
			terrainScaler.SetScale(10);
			terrainScaler.SetBias(10);

			module::Turbulence finalTerrain;
			finalTerrain.SetSourceModule(0, terrainScaler);
			finalTerrain.SetFrequency(4.0);
			finalTerrain.SetPower(0.125);

			m_HeightMapBuilder.SetSourceModule(finalTerrain);
			m_HeightMapBuilder.SetDestNoiseMap(m_HeightMap);
			m_HeightMapBuilder.SetDestSize(Size, Size);
			m_HeightMapBuilder.SetBounds(coords.x, coords.z, coords.y, coords.w);
			m_HeightMapBuilder.Build();
			m_Noise = m_HeightMap.GetSlabPtr();
		}

		utils::NoiseMap HeightMap() const { return m_HeightMap; }
		float* GetNoiseValues() const { return m_Noise; }
		utils::NoiseMapBuilderPlane MapBuilder() const { return m_HeightMapBuilder; }
	private:
		module::Perlin m_PerlinModule;
		utils::NoiseMap m_HeightMap;
		utils::NoiseMapBuilderPlane m_HeightMapBuilder;
		float* m_Noise;
	};

}

class HeightMap
{
public:
	HeightMap()
	{}

	void GenHeightMap(glm::vec4 Coordinates, const char* ImageTitle)
	{
		m_ImageTitle = ImageTitle; m_Coords = Coordinates;
		m_PerlinNoise.GenMap(SizeOfChunk, Coordinates);
		Debreky::ImageRenderer D_Renderer(m_PerlinNoise.HeightMap(), m_PerlinNoise.MapBuilder());
		utils::Image image = D_Renderer.Image();
		D_Renderer.RenderImage(ImageTitle);
	}

	float* GetSlabPtr() const { return m_PerlinNoise.GetNoiseValues(); }

private:
	const char* m_ImageTitle;
	glm::vec4 m_Coords;
	Debreky::Perlin m_PerlinNoise;
};