#include "../inc/Logger.hpp"
#include "../inc/ConsoleExtension.hpp"

#include <iostream>

Logger::Logger(LoggerPriority priority) : m_priority(priority) {}

Logger::~Logger() {}

void Logger::Log(std::string const &message)
{
    ConsoleExtension::ConsoleColor color;
    std::string header;

	switch (m_priority)
	{
	case LoggerPriority::Fatal_Error:
		color = ConsoleExtension::ConsoleColor::Red;
		header = "[FATAL ERROR] : ";
		break;
	case LoggerPriority::Error:
		color = ConsoleExtension::ConsoleColor::LightRed;
		header = "[Error] : ";
		break;
	case LoggerPriority::Warn:
		color = ConsoleExtension::ConsoleColor::Yellow;
		header = "[WARN] : ";
		break;
	case LoggerPriority::Info:
		color = ConsoleExtension::ConsoleColor::Blue;
		header = "[INFO] : ";
		break;
	case LoggerPriority::Debug:
		color = ConsoleExtension::ConsoleColor::Green;
		header = "[DEBUG] : ";
		break;
	}
    ConsoleExtension::SetConsoleTextColor(color);

    std::cout << header << message;

    ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Default_Color);
}