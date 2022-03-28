#pragma once

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "BlockChunkMesh.h"
#include "BlockData.h" 

#include "DebNoise.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertrexBufferLayout.h"

#include "Shader.h"

struct ChunkDetails
{
    size_t Size;
    VertexArray* VAO;
};

//The level above this class is supposed to create multiple "Biomes" they are not really biomes but whatever hehe :) LOVELY ;D MWAH, you're beautiful and I know it { Hopefully you too know it (: }
class BiomeManager
{
public:
    BiomeManager()
    {
    }

    void Setup();

    ~BiomeManager()
    {
        for (int i = 0; i < Width; i++)
        {
            delete[] maps[i]; delete[] chunks[i];
        }
        delete[] maps; delete[] chunks;
    }

    void RenderBiome(Shader shader, glm::vec3 pos, GLFWwindow* Window);

private:

    template<typename T>
    size_t sizeof_vec(std::vector<T> const& v)
    {
        return v.size() * sizeof(T);
    }

    void GenBiome();

    void SetRenderer();

    

private:
    std::vector<float> m_HeightMap;
#define SIZE 3
    u32 Width = SIZE, Length = Width;

    ChunkMesh** chunks;
    HeightMap** maps;

    u32 VAOS[SIZE][SIZE];
    u32 VBOS[SIZE][SIZE];

    std::vector<u32> m_ChunkSizes;
    std::vector<VertexArray> Vaos;
};


inline void BiomeManager::GenBiome()
{
    //Loop thru the chunks and give them the HeightValues

    maps = new HeightMap * [Width];
    for (int i = 0; i < Width; i++)
        maps[i] = new HeightMap[Length];

    chunks = new ChunkMesh * [Width];
    for (int i = 0; i < Width; i++)
        chunks[i] = new ChunkMesh[Length];

    int Index = 0;
    for (int x = 0; x < Width; x++)
    {
        for (int y = 0; y < Length; y++)
        {
            //Gen height map
            //Must find a way to change the coordinates acoordingly.
            glm::vec4 Coords = { glm::vec2(0 + (Index * 5),0 + (Index * 5)) , glm::vec2(5 + (Index * 5) ,5 + (Index * 5)) }; Index++;
            maps[x][y].GenHeightMap(Coords, "LastMap");
            //Gen chunks using the height map
            chunks[x][y].GenChunk(maps[x][y].GetSlabPtr());
        }
    }
}

inline void BiomeManager::Setup()
{
    GenBiome();
    SetRenderer();
}

inline void BiomeManager::SetRenderer()
{
    for (int x = 0; x < Width; x++)
    {
        for (int z = 0; z < Length; z++)
        {
            glGenVertexArrays(1, &VAOS[x][z]);
            glGenBuffers(1, &VBOS[x][z]);

            glBindBuffer(GL_ARRAY_BUFFER, VBOS[x][z]);
            size_t Size = sizeof_vec(chunks[x][z].GetVerticies());
            glBufferData(GL_ARRAY_BUFFER, Size, &chunks[x][z].GetVerticies()[0], GL_DYNAMIC_DRAW);

            glBindVertexArray(VAOS[x][z]);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            m_ChunkSizes.push_back(Size); 
        }
    }
}



inline void BiomeManager::RenderBiome(Shader shader, glm::vec3 pos, GLFWwindow* Window)
{
    int Index = 0;
    bool update = false;
    glm::vec3 newPos = { (int)pos.x, (int)pos.y, (int)pos.z };
    for (int x = 0; x < Width; x++)
    {
        for (int z = 0; z < Length; z++)
        {
            update = false;
            glBindVertexArray(VAOS[x][z]);
            glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(x * SizeOfChunk, 0, z * SizeOfChunk));

            if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            {
                Block* found = chunks[x][z].FindPos(newPos);
                if (found != nullptr)
                {
                    std::cout << "found: " << found->getPos() << " chunk: " << x << " , " << z << std::endl;
                    std::cout << "Type: " << found->getType() << std::endl;
                    chunks[x][z].DeletePos(found->getPos()); //to delete
                    update = true;
                }
            }
            if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                Block* found = chunks[x][z].FindPos(newPos);
                if (found != nullptr)
                {
                    std::cout << "found: " << found->getPos() << " chunk: " << x << " , " << z << std::endl;
                    std::cout << "Type: " << found->getType() << std::endl;
                    found->Type(BlockType::Sand);            // to add  
                    std::cout << "Type: " << found->getType() << std::endl;
                    update = true;
                }
            }

            if (update)
            {
                chunks[x][z].ReGenMesh();
                size_t Size = sizeof_vec(chunks[x][z].GetVerticies());
                glBindBuffer(GL_ARRAY_BUFFER, VBOS[x][z]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, Size, &chunks[x][z].GetVerticies()[0]);
            }
            shader.SetMat4f("model", Model);
            glDrawArrays(GL_TRIANGLES, 0, m_ChunkSizes.at(Index));
            Index++;
        }
    }
}

