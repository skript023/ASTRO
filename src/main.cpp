#include "common.hpp"
#include "gui.hpp"
#include "logger.hpp"
#include "hooking.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "settings.hpp"
#include "benchmark.hpp"
#include "script_mgr.hpp"
#include "fiber_pool.hpp"

#include "ui/ui_manager.hpp"
#include "events/main_event.hpp"
#include "server/server_module.hpp"
#include "services/notification/notification_service.hpp"

DWORD APIENTRY main_thread(LPVOID)
{
	using namespace big;

	while (!FindWindow(WINDOW_CLASS, WINDOW_NAME))
		std::this_thread::sleep_for(1s);

	benchmark initialization_benchmark("Initialization");

	std::filesystem::path base_dir = std::getenv("appdata");
	base_dir /= FOLDER_NAME;

	g_file_manager.init(base_dir);

	g_settings.load();
	g_logger.initialize(WINDOW_NAME, g_file_manager.get_project_file(std::format("./{}.log", FOLDER_NAME)), g_settings.debug.external_console);

	try
	{
		LOG(INFO) << R"kek(
 __          __    _   _               _              __          __             
\ \        / /   | | | |             (_)             \ \        / /             
 \ \  /\  / /   _| |_| |__   ___ _ __ _ _ __   __ _   \ \  /\  / /_ ___   _____ 
  \ \/  \/ / | | | __| '_ \ / _ \ '__| | '_ \ / _` |   \ \/  \/ / _` \ \ / / _ \
   \  /\  /| |_| | |_| | | |  __/ |  | | | | | (_| |    \  /\  / (_| |\ V /  __/
    \/  \/  \__,_|\__|_| |_|\___|_|  |_|_| |_|\__, |     \/  \/ \__,_| \_/ \___|
                                               __/ |                            
                                              |___/                                                                                            
)kek";
		LOG(INFO) << "Settings initialized.";
		
		auto pointers_instance = std::make_unique<pointers>();
		LOG(INFO) << "Pointers initialized.";

		auto renderer_instance = std::make_unique<renderer>();
		auto ui_instance = std::make_unique<ui_manager>();
		LOG(INFO) << "Renderer initialized.";

		auto fiber_pool_instance = std::make_unique<fiber_pool>(10);
		LOG(INFO) << "Fiber pool initialized.";

		auto thread_pool_instance = std::make_unique<thread_pool>();
		LOG(INFO) << "Thread Pool initialized.";

		auto hooking_instance = std::make_unique<hooking>();
		LOG(INFO) << "Hooking initialized.";
;
		auto notification_instance = std::make_unique<notification_service>();
		LOG(INFO) << "Service registered.";

		//auto server_instance = std::make_unique<server_module>();
		LOG(INFO) << "Server initialized.";

		g_script_mgr.add_script(std::make_unique<script>(&gui::script_func));
		g_script_mgr.add_script(std::make_unique<script>(&main_event::run));
		LOG(INFO) << "Scripts registered.";

		g_hooking->enable();
		LOG(INFO) << "Hooking enabled.";

		initialization_benchmark.get_runtime();
		initialization_benchmark.reset();

		while (g_running)
		{
			g_settings.attempt_save();
			std::this_thread::sleep_for(2s);
		}

		g_hooking->disable();
		LOG(INFO) << "Hooking disabled.";

		std::this_thread::sleep_for(1000ms);

		g_script_mgr.remove_all_scripts();
		LOG(INFO) << "Scripts unregistered.";

		LOG(INFO) << "Server unregistered.";

		notification_instance.reset();
		LOG(INFO) << "Service unregistered.";

		hooking_instance.reset();
		LOG(INFO) << "Hooking uninitialized.";

		fiber_pool_instance.reset();
		LOG(INFO) << "Fiber pool uninitialized.";

		g_thread_pool->destroy();
		LOG(INFO) << "Destroyed thread pool.";

		thread_pool_instance.reset();
		LOG(INFO) << "Thread Pool uninitialized.";

		renderer_instance.reset();
		LOG(INFO) << "Renderer uninitialized.";

		pointers_instance.reset();
		LOG(INFO) << "Pointers uninitialized.";

		g_settings.attempt_save();
		LOG(INFO) << "Settings saved and uninitialized.";
	}
	catch (std::exception const& ex)
	{
		LOG(WARNING) << ex.what();
		MessageBoxA(nullptr, ex.what(), nullptr, MB_OK | MB_ICONEXCLAMATION);
	}

	LOG(INFO) << "Farewell!";
	g_logger.destroy();

	CloseHandle(g_main_thread);
	FreeLibraryAndExitThread(g_hmodule, 0);

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hmod, DWORD reason, PVOID)
{
	using namespace big;

	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hmod);

		g_hmodule = hmod;
		g_main_thread = CreateThread(nullptr, 0, &main_thread, nullptr, 0, &g_main_thread_id);
		break;
	case DLL_PROCESS_DETACH:
		g_running = false;
		break;
	}

	return true;
}
