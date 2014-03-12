#include<string>
#include <map>
#include<vector>

namespace Injector
{
	#ifndef UNICODE  
		typedef std::string String; 
	#else
		typedef std::wstring String; 
	#endif

	__declspec(dllexport) bool InstallHook(std::vector<String>& ProcessNamesToInject, std::map<String, String>& DepsAndPathsForInjection);
	__declspec(dllexport) bool RemoveHook();
}
