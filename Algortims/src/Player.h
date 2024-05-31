#pragma once
#include<d3d11.h>
#include<wrl.h>
#include<WICTextureLoader.h>

#include<DirectXMath.h>

namespace dx = DirectX;

class Player
{
public:
	Player(ID3D11Device* pDevice,LPCWSTR vertexDir,LPCWSTR pixelDir,float depthZ);
	Player();

	~Player();
	void Bind(ID3D11DeviceContext* pContext,ID3D11Device* pDevice, const wchar_t* fileName, ID3D11DepthStencilState* state);
	void Update();
	void Draw(ID3D11DeviceContext* pContext, ID3D11Device* pDevice);
public:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstBuffer;
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

	 Vertex vertices[6] =  {
   {-0.5f,-0.5f,1.0f,0.0f,1.0f},
   {-0.5f,0.0f,1.0f,0.0f,0.0f},
   {0.5f,0.0f,1.0f,1.0f,0.0f},
   {0.5f,0.0f,1.0f,1.0f,0.0f},
   {0.5f,-0.5f,1.0f,1.0f,1.0f},
   {-0.5f,-0.5f,1.0f,0.0f,1.0f}
	};
};

