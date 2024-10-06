#pragma once
#include "common.hpp"

namespace big
{
	using dx_callback = std::function<void()>;
	using wndproc_callback = std::function<void(HWND, UINT, WPARAM, LPARAM)>;

	class gui
	{
		friend class renderer;

		std::map<uint32_t, dx_callback> m_dx_callbacks;
		std::vector<wndproc_callback> m_wndproc_callbacks;
	public:
		void init();
		void dx_init();

		/**
		 * @brief Add a callback function to draw your ImGui content in
		 *
		 * @param callback Function
		 * @param priority The higher the priority the value the later it gets drawn on top
		 * @return true
		 * @return false
		 */
		bool add_dx_callback(dx_callback callback, uint32_t priority);
		/**
		 * @brief Add a callback function on wndproc
		 *
		 * @param callback Function
		 */
		void add_wndproc_callback(wndproc_callback callback);
		/**
		 * @brief Execute a callback function in dx thread
		 *
		 * @param callback Function
		 */
		void execute(std::function<void()> callback);

		void dx_on_tick();
		void dx_on_opened();

		void script_init();
		void script_on_tick();

		void wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static void script_func();
	public:
		bool m_opened{};
		bool m_open_key_pressed{};
		bool m_back_key_pressed{};
		bool m_enter_key_pressed{};
		bool m_up_key_pressed{};
		bool m_down_key_pressed{};
		bool m_left_key_pressed{};
		bool m_right_key_pressed{};
	private:
		std::function<void()> m_callback;
	};

	inline gui g_gui;
}
