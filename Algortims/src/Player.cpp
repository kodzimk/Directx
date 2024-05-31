#include "Player.h"
#include<string>
#include<d3dcompiler.h>
#include<sstream>


Player::Player(ID3D11Device* pDevice, LPCWSTR vertexDir, LPCWSTR pixelDir, float depthZ)
{

	vertices[0] = { -0.5f,-0.5f,depthZ,0.0f,1.0f };
	vertices[1] = { -0.5f,0.0f,depthZ,0.0f,0.0f };
	vertices[2] = { 0.5f,0.0f,depthZ,1.0f,0.0f };
	vertices[3] = { 0.5f,0.0f,depthZ,1.0f,0.0f };
	vertices[4] = { 0.5f,-0.5f,depthZ,1.0f,1.0f };
	vertices[5] = { -0.5f,-0.5f,depthZ,0.0f,1.0f };

	size = (UINT)std::size(vertices);

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


	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(pixelDir, &pBlob);

	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	D3DReadFileToBlob(vertexDir, &pBlob);

	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Pos",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,	DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
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
	pContext->Draw(size, 0u);
}
