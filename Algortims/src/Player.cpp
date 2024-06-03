#include "Player.h"
#include<string>
#include<d3dcompiler.h>
#include<sstream>


Player::Player(ID3D11Device* pDevice, LPCWSTR vertexDir, LPCWSTR pixelDir, float depthZ, float a)
{
	transform.transform = dx::XMMatrixScaling(3.0f / 4.0f, 1.f, 1.0f);

	vertices[0] = { -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f }; //FRONT Bottom Left   - [0]
	vertices[1] = { -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f }; //FRONT Top Left      - [1]
	vertices[2] = { 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f }; //FRONT Top Right     - [2]
	vertices[3] = { 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f }; //FRONT Bottom Right   - [3]
	vertices[4] = { -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f }; //BACK Bottom Left   - [4]
	vertices[5] = { -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f }; //BACK Top Left      - [5]
	vertices[6] = { 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f }; //BACK Top Right     - [6]
	vertices[7] = { 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f }; //BACK Bottom Right   - [7]

	DWORD indices[] =
	{
		0, 1, 2, //FRONT
		0, 2, 3, //FRONT
		4, 7, 6, //BACK 
		4, 6, 5, //BACK
		3, 2, 6, //RIGHT SIDE
		3, 6, 7, //RIGHT SIDE
		4, 5, 1, //LEFT SIDE
		4, 1, 0, //LEFT SIDE
		1, 5, 6, //TOP
		1, 6, 2, //TOP
		0, 3, 7, //BOTTOM
		0, 7, 4, //BOTTOM
	};


	size = (UINT)std::size(indices);

	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(vertices);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = sizeof(Vertex);
	
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;

	pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);

	D3D11_BUFFER_DESC ibd = {};
	ibd.ByteWidth = sizeof(indices);
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.StructureByteStride = sizeof(DWORD);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;

	pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer);


	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(pixelDir, &pBlob);

	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	D3DReadFileToBlob(vertexDir, &pBlob);

	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Pos",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,	DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"ALPHA",0,	DXGI_FORMAT_R32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);

	D3D11_SAMPLER_DESC spd = {};
	spd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	spd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	spd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	spd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	spd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	spd.MaxLOD = D3D11_FLOAT32_MAX;
	spd.MinLOD = 0u;

	pDevice->CreateSamplerState(&spd, pSampleState.GetAddressOf());

	D3D11_BUFFER_DESC Cbd = {};
	Cbd.ByteWidth = sizeof(transform);
	Cbd.Usage = D3D11_USAGE_DYNAMIC;
	Cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Cbd.MiscFlags = 0u;
	Cbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &transform.transform;

	pDevice->CreateBuffer(&Cbd, &csd, pConstBuffer.GetAddressOf());
}

Player::Player()
{
}

Player::~Player()
{
}

void Player::Bind(ID3D11DeviceContext* pContext,ID3D11Device* pDevice, const wchar_t* fileName,ID3D11DepthStencilState* state)
{
	HRESULT hr = dx::CreateWICTextureFromFile(pDevice, pContext, fileName, nullptr, pTexture.GetAddressOf());

	if (FAILED(hr))
		throw " sas";


	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	pContext->OMSetDepthStencilState(state, 0);
	pContext->PSSetSamplers(0u, 1u, pSampleState.GetAddressOf());
	pContext->IASetInputLayout(pInputLayout.Get());
}

void Player::Update()
{

}

void Player::Draw(ID3D11DeviceContext* pContext, ID3D11Device* pDevice)
{
	
	pContext->PSSetShaderResources(0u, 1u, pTexture.GetAddressOf());
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->DrawIndexed(size,0u, 0u);
}
