// rain.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
// CrainApp:
// �йش����ʵ�֣������ rain.cpp
//

class CrainApp : public CWinApp
{
public:
	CrainApp();
// ��д
public:
	virtual BOOL InitInstance();
// ʵ��
	DECLARE_MESSAGE_MAP()
};
extern CrainApp theApp;