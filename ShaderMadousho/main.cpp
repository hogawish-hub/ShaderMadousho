#include <Windows.h>
#include<tchar.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#ifdef _DEBUG
#include <iostream>
#endif

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace std;

void DebugOutputFormatString(const char* format, ...) {
#ifdef _DEBUG
	va_list valist;
	va_start(valist, format);
	printf(format, valist);
	va_end(valist);
#endif
}


// WindowProcedure
LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

const unsigned int window_width = 1280;
const unsigned int window_height = 720;



ID3D12Device* _dev = nullptr;
IDXGIFactory6* _dxgiFactory = nullptr;
IDXGISwapChain4* _swapchain = nullptr;

//D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1,IID_PPV_ARGS(&_dev));



#ifdef _DEBUG

int main() {
#else

int WINAPI WinMain(HINSTANCE,HINSTANCE, LPSTR, int){

#endif

		DebugOutputFormatString("Show window test.");
	
		// create window calss and register it
		WNDCLASSEX w = {};
		w.cbSize = sizeof(WNDCLASSEX);
		w.lpfnWndProc = (WNDPROC)WindowProcedure; //callback function
		w.lpszClassName = _T("HogaTest"); //Application class name
		w.hInstance = GetModuleHandle(nullptr); //Get Hundle

		RegisterClassEx(&w);//Application Class

		RECT wrc = { 0,0,window_width,window_height }; //define window size

		//correct window size
		AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

		//Create window object

		HWND hwnd = CreateWindow(w.lpszClassName,_T("HogaTest"),WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, wrc.right - wrc.left,wrc.bottom - wrc.top,
			nullptr, nullptr, w.hInstance, nullptr);

		D3D_FEATURE_LEVEL levels[] = {
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,

		};

		D3D_FEATURE_LEVEL featureLevel;

		for (auto lv : levels) {
			if (D3D12CreateDevice(nullptr, lv, IID_PPV_ARGS(&_dev)) == S_OK) {
				featureLevel = lv;
				break;
			}
		}

		ShowWindow(hwnd, SW_SHOW);

		MSG msg = {};

		while (true) {

			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (msg.message == WM_QUIT) {

				break;
			}
			UnregisterClass(w.lpszClassName, w.hInstance);
		}
		// so far ignore below
		//getchar();
		return 0;


}

