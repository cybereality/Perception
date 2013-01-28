#define HIJACKDLL_EXPORTS
#ifdef HIJACKDLL_EXPORTS
#define HIJACKDLL_API __declspec(dllexport)
#else
#define HIJACKDLL_API __declspec(dllimport)
#endif

HIJACKDLL_API LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam);

HIJACKDLL_API void InstallHook();
HIJACKDLL_API void RemoveHook();
