#include <main/luna.h>

namespace luna
{
	__declspec(dllimport) int run();
}

int main()
{
	return luna::run();
}