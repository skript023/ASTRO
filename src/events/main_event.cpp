#include "script.hpp"
#include "main_event.hpp"

#include "server/server_module.hpp"

namespace big
{
	void main_event::registration()
	{

	}
	void main_event::run()
	{
		while (true)
		{
			registration();

			script::get_current()->yield();
		}
	}
}