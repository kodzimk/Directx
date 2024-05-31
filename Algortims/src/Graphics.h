#pragma once
#include"Player.h"
#include<memory>
#include<SpriteBatch.h>
#include<SpriteFont.h>

#include <d3d11.h>
#include <directxmath.h>
#include <iostream>
#include"Camera.h"

using namespace DirectX;

class Graphics
{
public:
	Graphics(HWND hwnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept
	{
		const float color[] = { red,green,blue,1.0f };
		pContext->ClearRenderTargetView(pTarget.Get(), color);
		pContext->ClearDepthStencilView(pDepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	void DrawSomething();
	void DoFrame();
public:
	std::unique_ptr<Player> player;
	std::unique_ptr<Player> player2;
	XMFLOAT3 position = { 0.0f,0.0f,-2.0f };
	Camera camera;
private:
	dx::XMMATRIX matrix;
	Microsoft::WRL::ComPtr<ID3D11Buffer>pConstBuffer;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr <IDXGISwapChain> pSwap = nullptr;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthState;

	std::unique_ptr<DirectX::SpriteBatch> pSpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> pSpriteFont;
};

