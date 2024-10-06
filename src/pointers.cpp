#include "common.hpp"
#include "logger.hpp"
#include "pointers.hpp"
#include "memory/all.hpp"

namespace big
{
	pointers::pointers(): m_resolution(new iVector2(1920, 1080))
	{
		memory::pattern_batch main_batch;

		if (!this->get_swapchain())
			LOG(WARNING) << "Failed get swapchain";
		
		/*main_batch.add("Engine", "48 89 05 ? ? ? ? 48 85 C9 74 05 E8 ? ? ? ? 48 8D 4C 24 48 E8 ? ? ? ? 0F 28 D6 0F 28 CF", [this](memory::handle ptr)
		{
			m_engine = ptr.add(3).rip().as<decltype(m_engine)>();
		});*/
		m_engine = SDK::UGameEngine::GetEngine();
		
		main_batch.add("UWorld", "48 8B 0D ? ? ? ? 48 8D 54 24 48 48 89 5C 24 40 33 DB 48 89 6C 24 50 48 89 74 24 58 4C 89 74 24 28 45 33 F6", [this](memory::handle ptr)
		{
			m_world = ptr.add(3).rip().sub(8).as<decltype(m_world)>();
		});

		main_batch.add("Process Event", "E8 ? ? ? ? 80 7C 24 40 00 74 0A 48 8D 4C 24 30 E8", [this](memory::handle ptr)
		{
			m_process_event = ptr.add(1).rip().as<decltype(m_process_event)>();
		});

		main_batch.add("GUObjectArray", "40 38 3D ? ? ? ? 0F 84 C3", [this](memory::handle ptr)
		{
			m_object_array = ptr.add(3).rip().add(4).as<decltype(m_object_array)>();
		});

		main_batch.add("FName Pool", "8B 05 ? ? ? ? FF C0 C1 E9 10 3B C8 0F 92 C0 C3", [this](memory::handle ptr)
		{
			m_name = ptr.add(2).rip().sub(8).as<decltype(m_name)>();
		});

		//main_batch.add("Screen Resolution", "F7 3D ? ? ? ? 66 0F 6E C0 0F 5B C0 0F 2F", [this](memory::handle ptr)
		//{
		//	m_resolution = ptr.add(2).rip().as<decltype(m_resolution)>();
		//});
		//
		main_batch.add("FPS Unlocker", "48 89 5C 24 10 57 48 83 EC 60 0F 29 74 24 50 48 8B D9 44 0F 29 4C 24 30 45 0F 57 C9", [this](memory::handle ptr)
		{
			m_fps_unlock = ptr.as<void*>(); //F3 0F 10 04 9F EB
		});
		
		main_batch.add("Return Address", "FF 23", [this](memory::handle ptr)
		{
			m_return_address = ptr.as<void*>();
		});

		main_batch.run(memory::module(nullptr));

		this->m_hwnd = FindWindow(WINDOW_CLASS, WINDOW_NAME);
		if (!this->m_hwnd)
			throw std::runtime_error("Failed to find the game's window.");

		g_pointers = this;
	}

	pointers::~pointers()
	{
		g_pointers = nullptr;
	}

	bool pointers::get_swapchain()
	{
		WNDCLASSEX windowClass;
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = DefWindowProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = GetModuleHandle(NULL);
		windowClass.hIcon = NULL;
		windowClass.hCursor = NULL;
		windowClass.hbrBackground = NULL;
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = "Kiero";
		windowClass.hIconSm = NULL;

		::RegisterClassEx(&windowClass);

		this->m_window = ::CreateWindow(windowClass.lpszClassName, "Kiero DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL);

		if (this->m_window == NULL)
		{
			return false;
		}

		HMODULE d3d11 = ::GetModuleHandle("d3d11.dll");
		if (d3d11 == NULL)
		{
			::DestroyWindow(this->m_window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return false;
		}

		void* create_device_and_swapchain = ::GetProcAddress(d3d11, "D3D11CreateDeviceAndSwapChain");
		if (create_device_and_swapchain == NULL)
		{
			::DestroyWindow(this->m_window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return false;
		}

		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1, };

		DXGI_RATIONAL refreshRate;
		refreshRate.Numerator = 60;
		refreshRate.Denominator = 1;

		DXGI_MODE_DESC bufferDesc;
		bufferDesc.Width = 100;
		bufferDesc.Height = 100;
		bufferDesc.RefreshRate = refreshRate;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC sampleDesc;
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		swapChainDesc.BufferDesc = bufferDesc;
		swapChainDesc.SampleDesc = sampleDesc;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = this->m_window;
		swapChainDesc.Windowed = 1;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr = ((functions::create_d3d11_device_and_swapchain_t)(create_device_and_swapchain))(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 2, D3D11_SDK_VERSION, &swapChainDesc, &this->m_swapchain, &this->m_d3d_device, &featureLevel, &this->m_d3d_context);
		
		if (FAILED(hr))
		{
			::DestroyWindow(this->m_window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return false;
		}

		::memcpy(this->m_swapchain_methods, *(void***)this->m_swapchain, sizeof(m_swapchain_methods));

		this->m_swapchain->Release();
		this->m_swapchain = nullptr;

		this->m_d3d_device->Release();
		this->m_d3d_device = nullptr;

		this->m_d3d_context->Release();
		this->m_d3d_context = nullptr;

		::DestroyWindow(this->m_window);
		::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

		return true;
	}
}