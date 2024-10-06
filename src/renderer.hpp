#pragma once
#include "common.hpp"
#include <imgui.h>

namespace big
{
	struct ImageDimensions
	{
		int x, y;
		ImageDimensions() { x = y = 0; }
		ImageDimensions(int _x, int _y) { x = _x; y = _y; }
	};

	class renderer
	{
	public:
		explicit renderer();
		~renderer();

		bool init(IDXGISwapChain* swapchain);
		void imgui_init();

		void new_frame();
		void end_frame();
		void on_present();

		void pre_reset();
		void post_reset(IDXGISwapChain* this_);
		void merge_icon_with_latest_font(float font_size, bool FontDataOwnedByAtlas = false);

		void wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		bool load_from_file(const char* filename, ID3D11Device* d3dDevice, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
		bool load_from_memory(const unsigned char* filename, int buffer_size, ID3D11Device* d3dDevice, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
	public:
		bool m_init = false;
		ID3D11ShaderResourceView* m_header{};
		ImageDimensions m_header_size = { 0, 0 };

		ID3D11ShaderResourceView* m_toggle{};
		ImageDimensions m_toggle_size = { 0, 0 };
		ImFont* m_font = nullptr;
		ImFont* m_ui_manager_font = nullptr;
		ImFont* m_monospace_font = nullptr;

		HWND m_window = NULL;
	private:
		ID3D11Device* m_d3d_device = nullptr;
		ID3D11DeviceContext* m_d3d_context = nullptr;
		ID3D11RenderTargetView* m_d3d_render_target = nullptr;
	};

	inline renderer *g_renderer{};
}
