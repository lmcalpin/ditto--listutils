// ListUtils.h : main header file for the ListUtils DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CListUtilsApp
// See ListUtils.cpp for the implementation of this class
//

class CListUtilsApp : public CWinApp
{
public:
	CListUtilsApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();

	DECLARE_MESSAGE_MAP()
};
