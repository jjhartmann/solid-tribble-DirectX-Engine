//////////////////////////////////////////////////////////////////////////
// Filename: d3dclass.cpp
// Author: Jeremy J. Hartmann
//////////////////////////////////////////////////////////////////////////
#include "d3dclass.h"



D3DClass::D3DClass() :
    m_swapChain(0),
    m_device(0),
    m_deviceContext(0),
    m_renderTargetView(0),
    m_depthStencilBuffer(0),
    m_depthStencilState(0),
    m_depthStencilView(0), 
    m_rasterState(0)
{
}

D3DClass::D3DClass(const D3DClass& other)
{}

D3DClass::~D3DClass()
{
}


bool D3DClass::Initialize(int screenWdith, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
    HRESULT result;
    IDXGIFactory *factory;
    IDXGIAdapter *adapter;
    IDXGIOutput *adapterOutput;
    unsigned int numModes, i, numerator, denominator;
    unsigned long long stringLength;
    DXGI_MODE_DESC *displayModeList;
    DXGI_ADAPTER_DESC adapterDesc;
    int error;
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    D3D_FEATURE_LEVEL featureLevel;
    ID3D11Texture2D *backBufferPtr;
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    D3D11_RASTERIZER_DESC rasterDesc;
    D3D11_VIEWPORT viewport;
    float fieldOfView, screenAspect;

    //Store the vsync setting
    m_vsync_enabled = vsync;

    // create a DirectX graphics interface factory
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    if (FAILED(result)) {
        return false;
    }
    

    // Use factory to create an adapter for primary graphics interface (video card)
    result = factory->EnumAdapters(0, &adapter);
    if (FAILED(result)) {
        return false;
    }

    // Enumerate the primary adapter output (monitor)
    result = adapter->EnumOutputs(0, &adapterOutput);
    if (FAILED(result)) {
        return false;
    }

    // Get the number of modes that fit the DXGI_FORMATE_R8G8B8A8_UNORM display format for the adapter output
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
    if (FAILED(result)) {
        return false;
    }

    // Create a list of hold all the possible display modes fort this monitor video card combination. 
    displayModeList = new DXGI_MODE_DESC[numModes];
    if (!displayModeList) {
        return false;
    }

    // Now fill the display mode list structures. 
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
    if (FAILED(result)) {
        return false;
    }

    // Iterate through the display mode list and find a display that matches the monitor height. Store it. 
    for (unsigned int i = 0; i < numModes; ++i) {
        if (displayModeList[i].Width == (unsigned int)screenWdith && displayModeList[i].Height == (unsigned int)screenHeight) {
            numerator = displayModeList[i].RefreshRate.Numerator;
            denominator = displayModeList[i].RefreshRate.Denominator;
        }
    }

    // Get the adepter (video card) description
    result = adapter->GetDesc(&adapterDesc);
    if (FAILED(result)) {
        return false;
    }

    // Retrieve the video card memory (mb)
    m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

    // Convert name of video card to char array
    error = wcstombs_s(&stringLength, m_videoChardDescription, 128, adapterDesc.Description, 128);
    if (error != 0) {
        return false;
    }

    // Release display mode list
    delete[] displayModeList;
    displayModeList = NULL;

    // release adapter output
    adapterOutput->Release();
    adapterOutput = NULL;

    // Release the adapter
    adapter->Release();
    adapter = NULL;

    // Release factory
    factory->Release();
    factory = NULL;


    // Initialize the swap chain description (double buffer)
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

    // Set to a single back buffer
    swapChainDesc.BufferCount = 1;

    //Set the width and height of back buffer
    swapChainDesc.BufferDesc.Width = screenWdith;
    swapChainDesc.BufferDesc.Height = screenHeight;

    // Set back buffer to 32 bit surface
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    // Set Refresh rate of the back buffer
    if (m_vsync_enabled) {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
    }
    else {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    }

    // Set the usage of the back buffer
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    // Set the handle for the window to render to
    swapChainDesc.OutputWindow = hwnd;

    // Toggle multi sampling
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    // Fullscreen mode toggle
    if (fullscreen) {
        swapChainDesc.Windowed = false;
    }
    else {
        swapChainDesc.Windowed = true;
    }

    // Set scan line ordering and scaling to unspecified
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // Discard back buffer content after presentation
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // Advanced Flag
    swapChainDesc.Flags = 0;


    // Set feature level
    featureLevel = D3D_FEATURE_LEVEL_11_0;

    /////////////////////
    // Setting up Devices
    /////////////////////

    // Create the swap chain, Driect3D and Direct3D device context
    // (Can replace the D3D_DRIVER_TYPE from HARDWARE to REFERENCE for CPU rendering)
    result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
        D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
    if (FAILED(result)) {
        return false;
    }

    // Get pointer to the back buffer
    result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
    if (FAILED(result)) {
        return false;
    }

    // Create the render target view with the back buffer pointer
    result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
    if (FAILED(result)) {
        return false;
    }

    // Release pointer to the back buffer (no longer needed)
    backBufferPtr->Release();
    backBufferPtr = NULL;


    ///////////////////////
    // Set up depth buffer
    ///////////////////////

    // Initialize the description of the depth buffer
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    // Set up description of depth buffer
    depthBufferDesc.Width = screenWdith;
    depthBufferDesc.Height = screenHeight;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;


    // Create the texture for the dpeth buffer using desc
    result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
    if (FAILED(result)) {
        return false;
    }

    ///////////////////////
    // Set up Depth Stencil
    ///////////////////////

    // Initialize description of stencil state
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    // Setup stencil
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Stencil Operations if pixels front-facing
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil Operationif pixel back-facing
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create depth stencil
    result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
    if (FAILED(result)) {
        return false;
    }


    // Set the depth stencil in Device context
    m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);



    ////////////////////////////
    // Set up Depth Stencil View
    ////////////////////////////

    // Initialize the depth stencil view
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    // Setup depth stencil view description
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view
    result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
    if (FAILED(result)) {
        return false;
    }

    // **BINDING PIPLINE** Bind The render target view and depth stencil buffer tot he output render pipeline
    m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);



    /////////////////////////////
    // Config Rasterizer setting
    /////////////////////////////

    // Setup the raster description to determine how polygons are drawn
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    //Create rasterizer state from description
    result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
    if (FAILED(result)) {
        return false;
    }

    // Set the rasterizer state
    m_deviceContext->RSSetState(m_rasterState);


    ////////////////////////////
    // Configure View port
    ////////////////////////////

    // Setup viewport
    viewport.Width = (float)screenWdith;
    viewport.Height = (float)screenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    // Create Viewport 
    m_deviceContext->RSSetViewports(1, &viewport);

    // Setup projection matrix
    fieldOfView = 3.141592654f / 4.0f; // 45 degrees
    screenAspect = (float)screenWdith / (float)screenHeight;
    m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

    // Setup world matrix (translate object vertices to vertices in the 3D scene)
    m_worldMatrix = XMMatrixIdentity();

    // Orthographic projection matrix  for 2D rendering
    m_orthoMatrix = XMMatrixOrthographicLH((float)screenWdith, (float)screenHeight, screenNear, screenDepth);



    return true;
}



