#pragma once
#include<Windows.h>
#include<optional>
#include"Keyboard.h"
#include"Graphics.h"
#include"Mouse.h"

class Window
{
public:
	Window(int width,int height,HINSTANCE hInstance);
	~Window();
	static std::optional<int> ProcessMessages();
	HINSTANCE GetINSTANCE() noexcept;
	void KeyEvents();
	void DoFrame();
public:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT CALLBACK HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	MouseClass mouse;
	KeyboardClass keyboard;
	std::unique_ptr<Graphics> gfx;
	HWND hwnd;
private:
	HINSTANCE hInstance;
	int width;
	int height;
};

