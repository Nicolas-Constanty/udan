#pragma once

//#include "fmt/core.h"

#include "Option.h"
#include "udan/debug/verbosity_level.h"

namespace udan::core
{
	struct ApplicationConfig
	{
		uint16_t framerate;
		debug::VerbosityLevel verbosity;
		bool silent;
		Option<double> game_time;
		Option<std::string> log_dir;
		
	};

	struct RendererConfig
	{
		uint32_t width;
		uint32_t height;
		bool fullscreen;
	};

	struct Config
	{
		ApplicationConfig app {};
		RendererConfig renderer {};
	};

	void createConfig(int argc, char *argv[], Config *config);
	void setupFromConfigFile(const std::string& path, Config *config);
}
