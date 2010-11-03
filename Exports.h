#pragma once

#include "..\..\Shared\DittoDefines.h"
#include "..\..\Shared\IClip.h"
#include <vector>

extern "C" 
{
	bool __declspec(dllexport) DittoAddin(const CDittoInfo &DittoInfo, CDittoAddinInfo &info);
	bool __declspec(dllexport) SupportedFunctions(const CDittoInfo &DittoInfo, FunctionType type, std::vector<CFunction> &Functions);
	bool __declspec(dllexport) ConvertListToCSV(const CDittoInfo &DittoInfo, IClip *pClip);
	bool __declspec(dllexport) ConvertCSVToList(const CDittoInfo &DittoInfo, IClip *pClip);
	bool __declspec(dllexport) TrimEachLine(const CDittoInfo &DittoInfo, IClip *pClip);
	bool __declspec(dllexport) SingleQuote(const CDittoInfo &DittoInfo, IClip *pClip);
	bool __declspec(dllexport) DoubleQuote(const CDittoInfo &DittoInfo, IClip *pClip);
}