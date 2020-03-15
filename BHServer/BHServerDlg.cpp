
// BHServerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "BHServer.h"
#include "BHServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* BH_ServerSocket 클래스에 있던 함수 AddWorkForAccept를 상속받아 재정의 함.(함수의 오버라이딩) 
   클라이언트가 서버에 접속될 때 호출되는 함수 AddWorkForAccept를 재정의 함. */
void MyServer::AddWorkForAccept(BH_UserData *ap_user)
{
	/* AddWorkForAccept 함수를 호출 하는 경우
	   클라이언트가 connect 함수를 호출하여 서버에 접속을 시도하면 
	   서버는 accept 함수를 호출해서 클라이언트의 접속을 허가해 주는데
	   서버가 클라이언트의 접속을 허가해준 직후에 클라이언트에 추가로 
	   하고 싶은 행위가 있을 때 사용함. */
	CString str;
	str.Format(L"%s에서 새로운 사용자가 접속했습니다!", ap_user->GetIP());
	mp_parent->AddEventStr(str);
}

/* 사용자가 접속을 해제했을 때 호출하는 함수 AddWorkForCloseUser를 상속 받아 재정의 함.(함수의 오버라이딩)
   클라이언트가 접속을 해제할 때 호출되는 함수 AddWorkForCloseUser를 재정의 함. */
void MyServer::AddWorkForCloseUser(BH_UserData *ap_user, int a_err_code)
{
	CString str;
	str.Format(L"%s에서 사용자가 접속을 해제했습니다!", ap_user->GetIP());
	
	// 구성된 문자열을 리스트 박스에 추가함.
	mp_parent->AddEventStr(str);
}

/* 클라이언트가 어떤 정보를 서버에 주었을 때 그 정보를 어떻게 처리할지
   정의하는 함수 ProcessRecvData를 상속 받아 재정의 함.(함수의 오버라이딩)*/
// 클라이언트에서 보낸 데이터가 서버에 수신될 때 호출되는 함수 ProcessRecvData의 정의
int MyServer::ProcessRecvData(SOCKET ah_socket, unsigned char a_msg_id, char *ap_recv_data, BSize a_body_size)
{
	// 대용량의 데이터가 전송 또는 수신될 때, 필요한 기본 코드를 수행하는 함수이다.

	BH_UserData *p_user = (BH_UserData *)FindUserData(ah_socket);
	/* ah_socket 핸들을 사용하는 클라이언트를 목록에서 찾아서 해당 클라이언트의 주소값을
	   p_user 포인터 변수에 저장함. */ 

	// 수신된 데이터가 채팅 데이터인 경우
	if (a_msg_id == NM_CHAT_DATA)
    // 클라이언트가 채팅 데이터를 서버에 보낸 경우 
	{
		/* 수신된 데이터에 채팅 정보가 유니코드 형태의 문자열로 되어 있기 때문에
		   유니코드 문자열 형태로 캐스팅하여 리스트 박스에 추가함. */

		CString str;

		/* ah_socket 핸들을 사용하는 해당 클라이언트의 주소를 채팅글 앞에 추가함.
		   예) "192.168.0.50 : 안녕하세요~"의 형태로 구성함. */

		str.Format(L"%s : %s", p_user->GetIP(), (wchar_t *)ap_recv_data);

		// 구성된 문자열을 리스트 박스에 추가함.
		mp_parent->AddEventStr(str);

		// 접속된 모든 사용자에게 채팅 정보를 전송하기 위해 for문을 실행함.
		for (int i = 0; i < m_max_user_cnt; i++)
		{
			// 현재 사용자가 접속 상태인지 확인함.
			if (mp_user_list[i]->GetHandle() != -1) // 0xFFFFFFFF
			/* -1은 접속 중이지 않은 사용자를 의미함.
				(INVALID_SOCKET, -1, 0xFFFFFFFF) 세 개다 모두 접속중이지
				않은 사용자를 의미함. */
			{
				/* SendFrameData 함수의 원형 
				   SendFrameData(SOCKET ah_socket, unsigned char a_msg_id, const char *ap_body_data, BSize a_body_size); */
				
				/* 함수 SendFrameData의 세 번째 매개변수 ap_body_data에는 
				   전송할 데이터의 형식이 (char *)이기 때문에 str이 유니코드 형식(wchar_t *)의 문자열
				   이여서 이 문자열을 (char *) 형식으로 형변환 해야함. 
				   
				   함수 SendFrameData의 네 번째 매개변수 a_body_size에는
				   전송할 데이터의 크기를 구해야 하는데, 이 문자열을 전송할 때는 NULL 문자를
				   포함해서 전송하기 때문에 문자열 길이에 1을 더하고 유니코드에서 문자 1개는
				   2바이트를 차지하기 때문에 문자열 길이에 2를 곱해야 함. */

				// 사용자가 접속한 상태라면 함수 SendFrameData를 호출하여 채팅 정보를 전송함.
				SendFrameData(mp_user_list[i]->GetHandle(), NM_CHAT_DATA, (const char *)(const wchar_t *)str, (str.GetLength() + 1) * 2);
			}
		}
	}
	
	return 1;
	/* ProcessRecvData가 0을 반환하면 실패했다는 것을 의미함.
	   쉽게 말해 내부적으로 네트웍 진행을 막아버리는
	   코드가 있기 때문에 처리를 못할 경우 0을 반환한다는 의미임. 
	   
	   1을 반환하면 정상적으로 처리했음을 의미함. */
}

// CBHServerDlg 대화 상자


/* MyServer 클래스 객체 m_server로 대화 상자 CBHServerDlg의 주소가 
   매개변수 this에 인자 값으로 넘겨준다. 
   그리고 이 넘겨준 주소가 MyServer 클래스의 객체 생성자에 있는 
   매개변수 ap_parent로 들어오게 된다.
   그리고 이 들어온 주소를 MyServer 클래스가 알게 된다. */
CBHServerDlg::CBHServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BHSERVER_DIALOG, pParent), m_server(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBHServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EVENT_LIST, m_event_list);
}

BEGIN_MESSAGE_MAP(CBHServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(25001, &CBHServerDlg::OnAcceptUser)
	ON_MESSAGE(25002, &CBHServerDlg::OnReadAndClose)
END_MESSAGE_MAP()


// CBHServerDlg 메시지 처리기

BOOL CBHServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	/* 서버 서비스를 시작하는 IP 주소를 StartServer 함수의 인자로 적음.  
	   m_hWnd는 메시지를 전달 받는 윈도우 핸들이며,
	   현재 대화 상자 CBHServerDlg의 핸들을 의미함.
	   27100은 포트 번호를 의미함. (포트 번호는 임의로 지정해도 됨.)
	   하지만 포트 번호를 100번 이하의 숫자로 적을 경우, 
	   시스템이 사용하는 포트 번호와 충돌하기 때문에 가능하면
	   100 이상의 번호를 사용하는 것이 좋음. */
	    

	/* localhost 주소인 '127.0.0.1' 주소를 사용하면
	   자신의 컴퓨터에서 실행된 클라이언트만 접속이 가능하기 때문에
	   자신의 컴퓨터가 아닌 외부에서 접속이 가능하게 하려면 cmd 명령 프롬프트 창에서 
	   ipconfig 명령으로 자신의 IP 주소를 확인해서 그 주소를 아래의 StartServer 함수의
	   첫 번째 인자 자리에 L"127.0.0.1" 을 바꿔서 적으면 됨. */


	/* localhost 용어 정리 
	   localhost는 컴퓨터 네트워크에서 사용하는 루프백 호스트명으로, 자신의 컴퓨터를 의미함.
	   IPv4에서의 IP 주소는 '127.0.0.1' 이다. 
	   
	   그리고 localhost는 로컬 컴퓨터를 원격 컴퓨터인것 처럼 통신할 수 있어 테스트 목적으로
	   주로 사용된다. */

	// StartServer 함수를 호출하여 서버 서비스를 '127.0.0.1'에서 27100 포트로 시작함.
	m_server.StartServer(L"127.0.0.1", 27100, m_hWnd);
	AddEventStr(L"서버 서비스를 시작합니다!");
	AddEventStr(L"서버의 IP 주소는 127.0.0.1 입니다.");

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
// 아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CBHServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
// 이 함수를 호출합니다.
HCURSOR CBHServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/* CBHServerDlg 클래스 밖에서 이 리스트 박스(m_event_list)의
   항목을 추가할 때 사용하는 인터페이스 함수 AddEventStr의 정의 */
// 리스트 박스에 문자열을 추가하는 함수 AddEventStr의 정의
void CBHServerDlg::AddEventStr(const wchar_t *ap_str)
{
	/* 사용자가 AddEventStr을 호출해서 문자열(ap_str)을 넘겨주면 
	   리스트박스(m_event_list)에 등록함. 
	   InsertString 함수가 반환하는 값이 현재 리스트박스에 추가된 
	   항목의 위치이다. 
	   그래서 그 위치 값을 index에 저장함. 
	   그리고 InsertString의 첫 번째 인자가 문자열을 삽입시킬 위치를
	   정하는 것인데, 이 때 -1이 의미하는 것은 맨 뒤부터 넣겠다는 뜻이다.
	   쉽게 말해 채팅 프로그램에서 먼저 삽입된 값이 결국 제일 위에 출력되고
	   제일 마지막에 삽입된 값은 제일 아래쪽에 출력이 된다는 의미이다. */

	// 리스트 박스의 가장 아래쪽에 문자열을 추가함.
	int index = m_event_list.InsertString(-1, ap_str); 
	// 문자열(ap_str)을 리스트 박스(m_event_list)에 보냄

	/* SetCurSel 함수를 호출하여 인덱스 값을 얻고 해당 인덱스를 커서로
	   설정하여 문자열을 추가할 때마다 해당 문자열을 표시하게 해줌. */
	// 문자열이 추가된 위치에 선택을 설정하여 바로 확인이 가능하도록 함.
	m_event_list.SetCurSel(index);
	/* 문자열(ap_str)의 인덱스를 인자로 받아 커서 위치 설정함.
	   이렇게 하면 입력한 곳에 자동으로 커서 위치가 설정이 되기 때문에
	   화면이 세팅 했을 때 가장 마지막으로 입력한 부분으로 커서가 가게 된다. */
}

// 사용자가 접속을 시도할 때 호출되는 함수 OnAcceptUser의 정의 
afx_msg LRESULT CBHServerDlg::OnAcceptUser(WPARAM wParam, LPARAM lParam)
{
	// 새로운 클라이언트가 접속했을 때 발생하는 메시지를 처리함. (Windows 메시지 : 25001)
	m_server.ProcessToAccept(wParam, lParam);   // FD_ACCEPT

	return 0;
}

// 사용자가 데이터를 보내거나 접속을 해제할 때 호출되는 함수 OnReadAndClose의 정의
afx_msg LRESULT CBHServerDlg::OnReadAndClose(WPARAM wParam, LPARAM lParam)
{
	/* 접속한 클라이언트가 데이터를 전송하거나 접속을 해제할 때 발생하는 메시지를 처리함.
	   (Windows 메시지 : 25002) */
	m_server.ProcessClientEvent(wParam, lParam);   // FD_READ, FD_CLOSE

	return 0;
}
