#include "Graphics.h"
#include<wrl.h>
#include<d3dcompiler.h>
#include<memory>
#include<string>
#include<math.h>
#include<DirectXMath.h>
#include"Imgui/imgui.h"
#include"Imgui/imgui_impl_win32.h"
#include"Imgui/imgui_impl_dx11.h"

namespace dx = DirectX;



Graphics::Graphics(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferCount = 1;
	scd.BufferDesc.Width = 800;
	scd.BufferDesc.Height = 600;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.Windowed = TRUE;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.OutputWindow = hwnd;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;



	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&scd,
		pSwap.GetAddressOf(),
		pDevice.GetAddressOf(),
		NULL,
		pContext.GetAddressOf()
	);

	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;

	pSwap->GetBuffer(0,__uuidof(ID3D11Resource),reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	pDevice->CreateRenderTargetView(backBuffer.Get(), NULL, pTarget.GetAddressOf());

	D3D11_TEXTURE2D_DESC dtd = {};
	dtd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dtd.ArraySize = 1;
	dtd.MipLevels = 1;
	dtd.CPUAccessFlags = 0;
	dtd.MiscFlags = 0;
	dtd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dtd.Usage = D3D11_USAGE_DEFAULT;
	dtd.SampleDesc.Count = 1;
	dtd.SampleDesc.Quality = 0;
	dtd.Height = 600;
	dtd.Width = 800;


	pDevice->CreateTexture2D(&dtd,nullptr,pDepthBuffer.GetAddressOf());
	pDevice->CreateDepthStencilView(pDepthBuffer.Get(), nullptr, pDepthView.GetAddressOf());

	pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), pDepthView.Get());

	D3D11_DEPTH_STENCIL_DESC dt = {};
	dt.DepthFunc =D3D11_COMPARISON_LESS;
	dt.DepthEnable = true;
	dt.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		pDevice->CreateDepthStencilState(&dt,depthState.GetAddressOf());


	player = std::make_unique<Player>(pDevice.Get(), L"VertexShader.cso", L"PixelShader.cso",0.3f,0.1f);

	pSpriteBatch = std::make_unique <DirectX::SpriteBatch>(this->pContext.Get());
	pSpriteFont = std::make_unique<DirectX::SpriteFont>(this->pDevice.Get(), L"res\\comic_sans_ms_16.spritefont");

	backBuffer->Release();

	D3D11_VIEWPORT vt;
	vt.TopLeftX = 0;
	vt.TopLeftY = 0;
	vt.Width = 800;
	vt.Height = 600;
	vt.MinDepth = 0;
	vt.MaxDepth = 1;

	pContext->RSSetViewports(1u, &vt);

	camera.SetPosition(0.0f, 0.0f, -2.0f);
	camera.SetProjection(90.f, static_cast<float>(800) / static_cast<float>(600), 0.1f, 1000.f);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
	ImGui::StyleColorsDark();

	D3D11_BLEND_DESC bd = {};
	D3D11_RENDER_TARGET_BLEND_DESC rtbd = {};
	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	bd.RenderTarget[0] = rtbd;

	pDevice->CreateBlendState(&bd, blendState.GetAddressOf());


	D3D11_RASTERIZER_DESC rd = {};
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;

	pDevice->CreateRasterizerState(&rd, rasterizeState.GetAddressOf());
	

	D3D11_RASTERIZER_DESC crd = {};
	crd.FillMode = D3D11_FILL_SOLID;
	crd.CullMode = D3D11_CULL_NONE;

	pDevice->CreateRasterizerState(&crd, rasterizeState_CullFront.GetAddressOf());
}

Graphics::~Graphics()
{
	
}

void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u);
}

void Graphics::DrawSomething()
{
	XMMATRIX worldMatrix = XMMatrixIdentity();

	matrix = worldMatrix * camera.GetViewMatrix() * camera.GetProjectionMatrix();

	//D3D11_BUFFER_DESC cbd = {};
	//cbd.ByteWidth = sizeof(matrix);
	//cbd.Usage = D3D11_USAGE_DYNAMIC;
	//cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//cbd.MiscFlags = 0u;
	//cbd.StructureByteStride = 0u;

	//D3D11_SUBRESOURCE_DATA csd = {};
	//csd.pSysMem = &matrix;

	//pDevice->CreateBuffer(&cbd, &csd, &pConstBuffer);
	/*pContext->VSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());*/


	pContext->OMSetBlendState(blendState.Get(), NULL, 0xFFFFFFFF);
	pContext->OMSetDepthStencilState(depthState.Get(), 0);

	player->SetPosition(2.0f, 0.0f, 0.0f);
	player->Bind(pContext.Get(), pDevice.Get(), L"res\\pinksquare.jpg",matrix);
	pContext->RSSetState(rasterizeState_CullFront.Get());
	player->Draw(pContext.Get(), pDevice.Get());
	pContext->RSSetState(rasterizeState.Get());



	pSpriteBatch->Begin();
	pSpriteFont->DrawString(pSpriteBatch.get(), L"Hello world",
		DirectX::XMFLOAT2(0, 0), DirectX::Colors::Black,0.0f,DirectX::XMFLOAT2(0,0),
		DirectX::XMFLOAT2(1, 1));
	pSpriteBatch->End();


	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Test");
	ImGui::Text("Text to test");
	ImGui::Button("Nigaa");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

void Graphics::DoFrame()
{
	DrawSomething();
}

