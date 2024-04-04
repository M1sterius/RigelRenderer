#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

namespace lgr
{	
	typedef unsigned short cout_color;

	const cout_color DEFAULT_LOG_COLOR = 7;
	const cout_color DEFAULT_WARNING_COLOR = 6;
	const cout_color DEFAULT_ERROR_COLOR = 4;

	void SetConsoleColor(const cout_color& color)
	{
		#ifdef _WIN32
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
		#endif
	}

	cout_color GetConsoleColor()
	{
		#ifdef _WIN32
		CONSOLE_SCREEN_BUFFER_INFO info;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hConsole, &info);
		return info.wAttributes;
		#endif
	}

	std::string GetFormattedTime()
	{
		const auto now = std::chrono::system_clock::now();
		const std::time_t time = std::chrono::system_clock::to_time_t(now);
		const std::tm timeStruct = *std::localtime(&time);

		std::stringstream ss;
		ss << std::setfill('0') << std::setw(2) << timeStruct.tm_hour << ":"
			<< std::setfill('0') << std::setw(2) << timeStruct.tm_min << ":"
			<< std::setfill('0') << std::setw(2) << timeStruct.tm_sec;

		return ss.str();
	}

	template<typename T> void Log(const T& log)
	{	
		const std::string time_now_f = "[" + GetFormattedTime() + "]";
		const std::string log_full = time_now_f + " - " + log;

		#ifdef _WIN32
		const cout_color old_console_color = GetConsoleColor();
		SetConsoleColor(DEFAULT_LOG_COLOR);
		std::cout << log_full << '\n';
		SetConsoleColor(old_console_color);
		#else
		std::cout << log_full << '\n';
		#endif
	}

	template<typename T> void Warning(const T& warning)
	{
		const std::string time_now_f = "[" + GetFormattedTime() + "]";
		const std::string warning_full = time_now_f + " - " + warning;

		#ifdef _WIN32
		const cout_color old_console_color = GetConsoleColor();
		SetConsoleColor(DEFAULT_WARNING_COLOR);
		std::cout << warning_full << '\n';
		SetConsoleColor(old_console_color);
		#else
		std::cout << warning_full << '\n';
		#endif
	}

	template<typename T> void Error(const T& error)
	{
		const std::string time_now_f = "[" + GetFormattedTime() + "]";
		const std::string error_full = time_now_f + " - " + error;

		#ifdef _WIN32
		const cout_color old_console_color = GetConsoleColor();
		SetConsoleColor(DEFAULT_ERROR_COLOR);
		std::cout << error_full << '\n';
		SetConsoleColor(old_console_color);
		#else
		std::cout << error_full << '\n';
		#endif
	}
}
