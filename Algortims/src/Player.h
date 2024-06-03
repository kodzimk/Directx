#pragma once
#include<d3d11.h>
#include<wrl.h>
#include<WICTextureLoader.h>

#include<DirectXMath.h>

namespace dx = DirectX;

class Player
{
public:
	Player(ID3D11Device* pDevice,LPCWSTR vertexDir,LPCWSTR pixelDir,float depthZ,float a);
	Player();

	~Player();
	void Bind(ID3D11DeviceContext* pContext,ID3D11Device* pDevice, const wchar_t* fileName, ID3D11DepthStencilState* state);
	void Update();
	void Draw(ID3D11DeviceContext* pContext, ID3D11Device* pDevice);
public:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampleState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture;

private:
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	UINT size = 0;

public:
	float angle = 0.0f;
	float speed = 0.2f;
private:
	struct Vertex {
		float x;
		float y;
		float z;
		float r;
		float g;
		float a;
	};	
	struct MATRIX {
		dx::XMMATRIX transform;
	};
	struct VECTOR {
		float x;
		float y;
		float z;
	};
public:
	VECTOR position = { 0.0f,0.0f,0.0f };
	MATRIX transform = {
	dx::XMMatrixTranspose(
		 dx::XMMatrixRotationZ(angle) *
			dx::XMMatrixScaling(3.0f / 4.0f,1.f,1.0f) *
			dx::XMMatrixTranslation(0.0f,0.0f,0.0f)
		)
	};

	 Vertex vertices[8] =  {
        Vertex(-0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,1.0f), //FRONT Bottom Left   - [0]
		Vertex(-0.5f,   0.5f, -0.5f, 0.0f, 0.0f,1.0f), //FRONT Top Left      - [1]
		Vertex(0.5f,   0.5f, -0.5f, 1.0f, 0.0f,1.0f), //FRONT Top Right     - [2]
		Vertex(0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,1.0f), //FRONT Bottom Right   - [3]
		Vertex(-0.5f,  -0.5f, 0.5f, 0.0f, 1.0f,1.0f), //BACK Bottom Left   - [4]
		Vertex(-0.5f,   0.5f, 0.5f, 0.0f, 0.0f,1.0f), //BACK Top Left      - [5]
		Vertex(0.5f,   0.5f, 0.5f, 1.0f, 0.0f,1.0f), //BACK Top Right     - [6]
		Vertex(0.5f,  -0.5f, 0.5f, 1.0f, 1.0f,1.0f), //BACK Bottom Right   - [7]
	};
};

