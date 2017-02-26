//////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
// Function: Handles geometry and passes it to GPU
//////////////////////////////////////////////////////////////////////////
#include "modelclass.h"



ModelClass::ModelClass() : 
    m_vertexBuffer(NULL), 
    m_indexBuffer(NULL)
{
    ; // Empty
}

ModelClass::ModelClass(const ModelClass& other)
{
    ; // Empty
}

ModelClass::~ModelClass()
{
    ; // Empty
}

bool ModelClass::Initialize(ID3D11Device *device)
{
    bool result;

    // Initialize vertex and index buffers. 
    result = InitializeBuffers(device);
    if (!result)
        return false;

    return true;
}

void ModelClass::Shutdown()
{
    // Shutdown buffers
    ShutdownBuffers();
}

void ModelClass::Render(ID3D11DeviceContext *deviceContext)
{
    // Place thge vetex and index buffers on graphics pipeline. 
    RenderBuffers(deviceContext);
}

int ModelClass::GetIndexCount()
{
    return m_indexCount;
}

//////////////////////////////////////////////////////////////////////////
// INTILIALIZE BUFFER
// - Usually vetex information is read from file
//////////////////////////////////////////////////////////////////////////
bool ModelClass::InitializeBuffers(ID3D11Device *device)
{
    VertexType *vertices;
    unsigned long *indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;

    // Set number of vertices in vetex array
    m_vertexCount = 3;

    // Set number of indices in index array;
    m_indexCount = 3;

    // Create vertex array. 
    vertices = new VertexType[m_vertexCount];
    if (!vertices)
        return false;

    // Create index array
    indices = new unsigned long[m_indexCount];
    if (!indices)
        return false;


    // Load the vertex array with data. (triangle)
    // NOTE: Order of vertices is very important. 
    vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f); // Bottom Left
    vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f); // Top center
    vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f); // Bottom right
    vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    // Load the index array with data
    indices[0] = 0; // Bottom left
    indices[1] = 1; // Top Center
    indices[2] = 2; // Bottom right

    // Set up the description of the static vertex buffer
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a point to the vertex data.
    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // Create vertex buffer
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if (FAILED(result))
        return false;


    // Setup static index buffer description
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // Give subresources structure a pointer to index data.
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // Create index buffer
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result))
        return false;


    // Release resources (data copied into buffers)
    delete[] vertices;
    vertices = NULL;

    delete[] indices;
    indices = NULL;


    return true;
}

void ModelClass::ShutdownBuffers()
{
    // Release the buffer
    if (m_indexBuffer) {
        m_indexBuffer->Release();
        m_indexBuffer = NULL;
    }
    else {
        m_vertexBuffer->Release();
        m_vertexBuffer = NULL;
    }
}

void ModelClass::RenderBuffers(ID3D11DeviceContext *deviceContext)
{
    unsigned int stride = sizeof(VertexType);
    unsigned int offset = 0;

    // Set vertex buffer as active in input assembler for rendering
    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set index buffer to acctive in input assembler for rendering
    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set type of primitive that should be rendered form vertex buffer (triangle)
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
