#pragma once

#ifdef LUNA_PLATFORM_WINDOWS
	#ifdef LUNA_BUILD_DLL
		#define LUNA_API __declspec(dllexport)
	#else
		#define LUNA_API __declspec(dllimport)
	#endif // LUNA_BUILD_DLL
#else
	#error Luna only supports Windows!
#endif // LUNA_PLATFORM_WINDOWS
