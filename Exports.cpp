#include "StdAfx.h"
#include "Exports.h"
#include "ListUtils.h"
#include "pasteimageashtmlimage.h"
#include "TextProcessor.h"
#include <iostream>

bool Convert(const CDittoInfo &DittoInfo, IClip *pClip, ConversionType conversionType);

bool DittoAddin(const CDittoInfo &DittoInfo, CDittoAddinInfo &info)
{
	if(DittoInfo.ValidateSize() == false || info.ValidateSize() == false)
	{
		CString csError;
		csError.Format(_T("ListUtils Addin - Passed in structures are of different size, DittoInfo Passed: %d, Local: %d, DittoAddinInfo Passed: %d, Local: %d"), DittoInfo.m_nSizeOfThis, sizeof(CDittoInfo), info.m_nSizeOfThis, sizeof(CDittoAddinInfo));
		OutputDebugString(csError);
		return false;
	}

	info.m_Name = _T("ListUtils");
	info.m_AddinVersion = 1;

	return true;
}

bool SupportedFunctions(const CDittoInfo &DittoInfo, FunctionType type, std::vector<CFunction> &Functions)
{
	switch(type)
	{
	case eFuncType_PRE_PASTE:
		{
			CFunction func;
			func.m_csFunction = _T("ConvertListToCSV");
			func.m_csDisplayName = _T("Convert CRLF delimited List To CSV");
			func.m_csDetailDescription = _T("Takes a CRLF delimited list and converts it to a Comma Separated list");

			Functions.push_back(func);

			CFunction func2;
			func2.m_csFunction = _T("ConvertCSVToList");
			func2.m_csDisplayName = _T("Convert CSV To CRLF delimited List");
			func2.m_csDetailDescription = _T("Takes a Comma Separated list and converts it to a CRLF Separated list");

			Functions.push_back(func2);

			CFunction func3a;
			func3a.m_csFunction = _T("SingleQuote");
			func3a.m_csDisplayName = _T("Single quote each line");
			func3a.m_csDetailDescription = _T("Takes a CRLF delimited list and quotes each line");

			Functions.push_back(func3a);

			CFunction func3b;
			func3b.m_csFunction = _T("DoubleQuote");
			func3b.m_csDisplayName = _T("Double quote each line");
			func3b.m_csDetailDescription = _T("Takes a CRLF delimited list and quotes each line");

			Functions.push_back(func3b);

			CFunction func4;
			func4.m_csFunction = _T("TrimEachLine");
			func4.m_csDisplayName = _T("Trim each line");
			func4.m_csDetailDescription = _T("Takes a CRLF delimited list and trims the whitespace at the beginning of each line");

			Functions.push_back(func4);
		}
		break;
	}
	
	return true;
}

bool ConvertListToCSV(const CDittoInfo &DittoInfo, IClip *pClip)
{
	return Convert(DittoInfo, pClip, CONVERT_LIST_TO_CSV);
}

bool ConvertCSVToList(const CDittoInfo &DittoInfo, IClip *pClip)
{
	return Convert(DittoInfo, pClip, CONVERT_CSV_TO_LIST);
}

bool TrimEachLine(const CDittoInfo &DittoInfo, IClip *pClip)
{
	return Convert(DittoInfo, pClip, TRIM);
}

bool SingleQuote(const CDittoInfo &DittoInfo, IClip *pClip)
{
	return Convert(DittoInfo, pClip, SINGLE_QUOTE);
}

bool DoubleQuote(const CDittoInfo &DittoInfo, IClip *pClip)
{
	return Convert(DittoInfo, pClip, DOUBLE_QUOTE);
}

bool Convert(const CDittoInfo &DittoInfo, IClip *pClip, ConversionType conversionType)
{
	bool ret = false;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	IClipFormats *pFormats = pClip->Clips();

	CWnd* pWnd = CWnd::FromHandle(DittoInfo.m_hWndDitto);

	TextProcessor textProcessor(conversionType);
	IClipFormat *pText = pFormats->FindFormatEx(CF_TEXT);
	if(pText != NULL)
	{
		//We own the data, when we call DeleteAll tell it to not free the data
		pText->AutoDeleteData(false);
		HGLOBAL data = pText->Data();

		char *stringData = (char *)GlobalLock(data);

		int size = (int)GlobalSize(data);
		if(stringData != NULL)
		{
			std::string str(stringData);
			std::string ret = textProcessor.Process(str);

			size_t size = GlobalSize(pText->Data());
			if (ret.length() > size) {
				GlobalFree(data);
				size = ret.length() + 1;
				data = GlobalAlloc(GHND, size);
				stringData = (char *)GlobalLock(data);
			}
			strcpy_s(stringData, size, ret.c_str());
		}
		GlobalUnlock(data);

		//Remove all over formats and add the selected date back as CF_TEXT
		pFormats->DeleteAll();
		pFormats->AddNew(CF_TEXT, data);
		IClipFormat *pText = pFormats->FindFormatEx(CF_TEXT);
		if(pText != NULL)
		{
			pText->AutoDeleteData(true);
		}
		ret = true;
	}
	return ret;
}

