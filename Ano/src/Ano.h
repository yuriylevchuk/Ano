#pragma once

#ifdef ANO_EXPORTS_DLL
	#define ANO_API __declspec(dllexport)
#else
	#define ANO_API __declspec(dllimport)
#endif


namespace Ano {

	ANO_API void Init();

}
