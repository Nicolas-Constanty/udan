#include "corepch.h"

#include <iostream>
#include <string>
#include <codecvt>
#include <locale>
#include <fstream>

#include <clipp.h>
#include <toml++/toml.h>

#include "Config.h"


#include "udan/debug/Logger.h"

namespace udan::core
{
	std::wstring utf8ToUtf16(const std::string& utf8Str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.from_bytes(utf8Str);
	}
	
	void createConfig(int argc, char* argv[], Config* config)
	{
		if (config == nullptr)
		{
			LOG_ERR("Config object is null!");
			return;
		}
		
		{
			std::string configPath;
			const auto cliPath = (
				clipp::option("-c", "--config") & clipp::value("path of the config file", configPath)
				);
			if (!clipp::parse(argc, argv, cliPath))
				std::cout << clipp::make_man_page(cliPath, argv[0]);
			
			setupFromConfigFile(configPath, config);
		}
		
		int verbosity;
		double gametime;
        const auto cli = (
			clipp::option("-v", "--verbosity") & clipp::integer("verbosity level(0 to 6)", verbosity),
			clipp::option("-f", "--framerate") & clipp::number("target framerate", config->app.framerate),
			clipp::option("-t", "--gametime") & clipp::number("delay before stop", gametime),
			clipp::option("-s", "--silent").set(config->app.silent).doc("Disable logging.")
            );

        if (!clipp::parse(argc, argv, cli))
        {
			std::cout << clipp::make_man_page(cli, argv[0]);
			LOG_FATAL("Invalid main argument");
			return;
        }
		config->app.verbosity = static_cast<debug::VerbosityLevel>(verbosity);
		if (gametime > 0)
			config->app.game_time = gametime;
	}

	std::string getErrorString()
	{
		DWORD errCode = GetLastError();
		char* err;
		if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			nullptr,
			errCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
			reinterpret_cast<LPTSTR>(&err),
			0,
			nullptr))
		{
			LOG_FATAL("Format message failed.");
		}
		return std::string(err);
	}

	BOOL FileExists(LPCTSTR szPath)
	{
		DWORD dwAttrib = GetFileAttributes(szPath);

		return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
			!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	BOOL DirectoryExists(LPCTSTR szPath)
	{
		DWORD dwAttrib = GetFileAttributes(szPath);

		return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
			(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	std::wstring getCurrentDirectory()
	{
		wchar_t workingDirectory[MAX_PATH];
		if (GetCurrentDirectoryW(MAX_PATH, workingDirectory) == 0)
		{
			LOG_FATAL(getErrorString());
			return nullptr;
		}
		return std::wstring(workingDirectory);
	}

	void setupFromConfigFile(const std::string& path, Config * config)
	{
		std::wstring filePath;
		if (path.length() > 0)
		{
			std::wstring wpath = utf8ToUtf16(path);
			if (!FileExists(wpath.c_str()))
			{
				LOG_TRACE("The file {} doesn't exist. Fallback to default.toml", path);
			}
			else
			{
				filePath = wpath;
			}
		}
		if (filePath.length() == 0)
		{
			const auto workingDirectory = getCurrentDirectory();
			filePath = std::wstring(workingDirectory) + std::wstring(L"\\default.toml");
		}

		try
		{
			const auto configFile = toml::parse_file(filePath);
			config->app.framerate = configFile["application"]["framerate"].value_or<uint16_t>(60);
			config->app.verbosity = static_cast<debug::VerbosityLevel>(configFile["application"]["verbosity"].value_or(static_cast<int>(debug::VerbosityLevel::WAR)));
			config->app.silent = configFile["application"]["silent"].value_or(false);
			config->app.game_time = configFile["application"]["game_time"].value_or(0);
			config->app.log_dir = configFile["application"]["log_dir"].value_or("");

			config->renderer.width = configFile["renderer"]["width"].value_or(1280);
			config->renderer.height = configFile["renderer"]["height"].value_or(720);
			config->renderer.fullscreen = configFile["renderer"]["height"].value_or(false);
		}
		catch (const toml::parse_error& err)
		{
			LOG_FATAL(static_cast<std::string>(err.description()));
		}
		LOG_INFO("Engine config successfully loaded.");
	}
}

