#include <Windows.h>
#include<tchar.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<vector>
#ifdef _DEBUG
#include <iostream>
#endif

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

//using namespace std;

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
ID3D12CommandAllocator* _cmdAllocator = nullptr;
ID3D12GraphicsCommandList* _cmdsList = nullptr;
ID3D12CommandQueue* _cmdsQueue = nullptr;
IDXGISwapChain1* _swapchain = nullptr;






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

		// get proper adapter
		auto result = CreateDXGIFactory1(IID_PPV_ARGS(&_dxgiFactory));
		
		// define container for all adapters
		std::vector <IDXGIAdapter*> adapters;
		IDXGIAdapter* tmpAdapter = nullptr;

		// get all adapters
		for (int i = 0; _dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
			adapters.push_back(tmpAdapter);
		}

		// get proper adapter name
		for (auto adpt : adapters) {

			DXGI_ADAPTER_DESC adesc = {};
			adpt->GetDesc(&adesc);
			std::wstring strDesc = adesc.Description;

			if (strDesc.find(L"NVIDIA") != std::string::npos) {
					
				tmpAdapter = adpt;
				break;

			}

		}


		D3D_FEATURE_LEVEL featureLevel;

		// Test proper feature level
		for (auto lv : levels) {
			if (D3D12CreateDevice(nullptr, lv, IID_PPV_ARGS(&_dev)) == S_OK) {
				featureLevel = lv;
				break;
			}
		}
		
		// prepare creating command queue
		result = _dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAllocator));
		result = _dev->CreateCommandList(0,D3D12_COMMAND_LIST_TYPE_DIRECT,_cmdAllocator,nullptr,IID_PPV_ARGS(&_cmdsList));

		D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};

		// no timeout
		cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

		// when use only one adapter, 0 should be fine
		cmdQueueDesc.NodeMask = 0;

		// no priority
		cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

		cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		//create queue

		result = _dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&_cmdsQueue));




		// create window
		ShowWindow(hwnd, SW_SHOW);



		// roop
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

