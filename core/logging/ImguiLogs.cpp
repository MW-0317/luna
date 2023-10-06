#include "ImguiLogs.h"

DefaultLog Log::console = DefaultLog();
Overlay Log::overlay = Overlay();

void Log::log(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	Log::console.log("[Log] ");
	Log::console.log(fmt, args);
	Log::console.log("\n");
	va_end(args);
}

void Log::warn(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	Log::console.log("[Warn] ");
	Log::console.log(fmt, args);
	Log::console.log("\n");
	va_end(args);
}

void Log::error(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	Log::console.log("[Error] ");
	Log::console.log(fmt, args);
	Log::console.log("\n");
	va_end(args);
}

void Log::updateOverlay(std::string text, float value)
{
	Log::overlay.update(text, value);
}

void Log::draw(bool* p_open)
{
	Log::console.draw("Console", p_open);
	Log::overlay.draw("Overlay", p_open);
}