#pragma once

#include <fstream>
#include <iostream>
#include <string>



namespace logger
{
	enum class Level { DEBUG, INFO, WARNING, ERROR, CRITICAL };

	class Logger
	{
	public:

		static void set_output(std::ostream* out) { log_output = out; }

		static void debug(const std::string& message)
		{
			std::cout << "\nDEBUG: " << message;
		}

		static void info(const std::string& message)
		{
			std::cout << "\nINFO: " << message;
		}

		static void warning(const std::string& message)
		{
			std::cout << "\nWARNING: " << message;
		}

		static void error(const std::string& message)
		{
			std::cout << "\nERROR: " << message;
		}

		static void critical(const std::string& message)
		{
			std::cout << "\nCRITICAL: " << message;
		}

		template<typename T, typename... Targs>
		static void debug(const char* format, T value, Targs... fargs)
		{
			std::cout << "\nDEBUG: ";
			tprint(format, value, fargs...);
		}

		template<typename T, typename... Targs>
		static void info(const char* format, T value, Targs... fargs)
		{
			std::cout << "\nINFO: ";
			tprint(format, value, fargs...);
		}

		template<typename T, typename... Targs>
		static void warning(const char* format, T value, Targs... fargs)
		{
			std::cout << "\nWARNING: ";
			tprint(format, value, fargs...);
		}

		template<typename T, typename... Targs>
		static void error(const char* format, T value, Targs... fargs)
		{
			std::cout << "\nERROR: ";
			tprint(format, value, fargs...);
		}

		template<typename T, typename... Targs>
		static void critical(const char* format, T value, Targs... fargs)
		{
			std::cout << "\nCRITICAL: ";
			tprint(format, value, fargs...);
		}

		static std::ostream* log_output;
		static Level log_level;

	private:
		Logger() {}

		static void tprint(const char* format)
		{
			std::cout << format;
		}

		template<typename T, typename... Targs>
		static void tprint(const char* format, T value, Targs... fargs)
		{
			for (; *format != '\0'; format++)
			{
				if (*format == '%')
				{
					std::cout << value;
					tprint(format + 1, fargs...);
					return;
				}
				std::cout << *format;
			}
		}
	};

}

inline std::ostream* logger::Logger::log_output = &std::cout;
inline logger::Level logger::Logger::log_level = logger::Level::INFO;
