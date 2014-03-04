#include<vector>
#include<string>

namespace Injector
{
	std::vector<std::string> ProcessNamesToInject;
	std::vector<std::string> ModulesToInject;

	__declspec(dllexport) bool InstallHook();
	__declspec(dllexport) bool RemoveHook();
}
