#include<vector>
#include<string>
#include <map>

namespace Injector
{
	#ifndef UNICODE  
		typedef std::string String; 
	#else
		typedef std::wstring String; 
	#endif

	std::vector<String> ProcessNamesToInject;
	std::map<String, String> DepsAndPathsForInjection;

	__declspec(dllexport) bool InstallHook();
	__declspec(dllexport) bool RemoveHook();
}
