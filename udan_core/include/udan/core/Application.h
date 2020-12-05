#pragma once

#include <memory>

#include "Clock.h"
#include "Config.h"

namespace udan::core
{
	class Application
	{
	public:
		Application(int argv, char* argc[]);
		void Run();
		void SetFramerate(uint16_t framerate);

	private:
		std::unique_ptr<IClock> m_clock;
		Config m_config;

	};
}
