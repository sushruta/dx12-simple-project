#pragma once

#include "CrossWindow/CrossWindow.h"
#include "CrossWindow/Graphics.h"

#define GLM_FORCE_SSE42 1
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES 1
#define GLM_FORCE_LEFT_HANDED
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include <direct.h>

// Common Utils

inline std::vector<char> readFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    bool exists = (bool)file;

    if (!exists || !file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
};

// Renderer

class Renderer
{
  public:
    Renderer(xwin::Window& window);

    ~Renderer();

    // Render onto the render target
    void render();

    // Resize the window and internal data structures
    void resize(unsigned width, unsigned height);

  protected:
    // Initialize your Graphics API
    void initializeAPI(xwin::Window& window);

    // Destroy any Graphics API data structures used in this example
    void destroyAPI();

    // Initialize any resources such as VBOs, IBOs, used in this example
    void initializeResources();

    // Destroy any resources used in this example
    void destroyResources();

    // Create graphics API specific data structures to send commands to the GPU
    void createCommands();

    // Set up commands used when rendering frame by this app
    void setupCommands();

    // Destroy all commands
    void destroyCommands();

    // Set up the FrameBuffer
    void initFrameBuffer();

    void destroyFrameBuffer();

    // Set up the RenderPass
    void createRenderPass();

    void createSynchronization();

    // Set up the swapchain
    void setupSwapchain(unsigned width, unsigned height);

    struct Vertex
    {
        float position[3];
        float normal[3];
    };

    /*************************/
    /*** MY CODE FOR CUBES ***/
    /*************************/
    /*
    Vertex mVertexBufferData[24] = {
        // Front face
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        // Back face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
        // Left face
        {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
        // Right face
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        // Top face
        {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        // Bottom face
        {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}}
    };

    uint32_t mIndexBufferData[36] = {
        0,  1,  2,  0,  2,  3,  // Front face
        4,  5,  6,  4,  6,  7,  // Back face
        8,  9,  10, 8,  10, 11, // Left face
        12, 13, 14, 12, 14, 15, // Right face
        16, 17, 18, 16, 18, 19, // Top face
        20, 21, 22, 20, 22, 23  // Bottom face
    };
    */

    // Vertex* mVertexBufferData;
    // uint32_t* mIndexBufferData;

    Vertex mVertexBufferData[16384];
    uint32_t mIndexBufferData[16384];

    int mNumVertices;
    int mNumIndices;

    void initTeapot();
    void initCube();

    std::chrono::time_point<std::chrono::steady_clock> tStart, tEnd;
    float mElapsedTime = 0.0f;

    // Uniform data
    struct
    {
        glm::mat4 projectionMatrix;
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        glm::mat3 normalMatrix;
    } uboVS;

    static const UINT backbufferCount = 2;

    xwin::Window* mWindow;
    unsigned mWidth, mHeight;

    // Initialization
    IDXGIFactory4* mFactory;
    IDXGIAdapter1* mAdapter;
#if defined(_DEBUG)
    ID3D12Debug1* mDebugController;
    ID3D12DebugDevice* mDebugDevice;
#endif
    ID3D12Device* mDevice;
    ID3D12CommandQueue* mCommandQueue;
    ID3D12CommandAllocator* mCommandAllocator;
    ID3D12GraphicsCommandList* mCommandList;

    // Current Frame
    UINT mCurrentBuffer;
    ID3D12DescriptorHeap* mRtvHeap;
    ID3D12Resource* mRenderTargets[backbufferCount];
    IDXGISwapChain3* mSwapchain;

    // Resources
    D3D12_VIEWPORT mViewport;
    D3D12_RECT mSurfaceSize;

    ID3D12Resource* mVertexBuffer;
    ID3D12Resource* mIndexBuffer;

    ID3D12Resource* mUniformBuffer;
    ID3D12DescriptorHeap* mUniformBufferHeap;
    UINT8* mMappedUniformBuffer;

    D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
    D3D12_INDEX_BUFFER_VIEW mIndexBufferView;

    UINT mRtvDescriptorSize;
    ID3D12RootSignature* mRootSignature;
    ID3D12PipelineState* mPipelineState;

    // Sync
    UINT mFrameIndex;
    HANDLE mFenceEvent;
    ID3D12Fence* mFence;
    UINT64 mFenceValue;
};