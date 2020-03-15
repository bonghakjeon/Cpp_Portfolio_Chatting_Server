
// BHServer.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "BHServer.h"
#include "BHServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBHServerApp

BEGIN_MESSAGE_MAP(CBHServerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CBHServerApp 생성

CBHServerApp::CBHServerApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CBHServerApp 개체입니다.

CBHServerApp theApp;


// CBHServerApp 초기화

BOOL CBHServerApp::InitInstance()
{

	CWinApp::InitInstance();

	CBHServerDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	return FALSE;
}

