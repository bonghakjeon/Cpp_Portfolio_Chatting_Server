
// BHServerDlg.h: 헤더 파일
//

#pragma once


#define MAX_USER_CNT   50    // 최대 사용자 수 50명
// 채팅 Message ID (NM_CHAT_DATA)
#define NM_CHAT_DATA   1     // 여기서 1은 채팅 데이터가 들어왔다는 것을 의미함. 

class CBHServerDlg;   
// MyServer 클래스에서 CBHServerDlg 대화 상자 클래스를 사용할 수 있도록 선언함.
/* MyServer 클래스와 CBHServerDlg 클래스가 서로의 클래스에
   멤버 변수로 상호참조 되어 있기 때문에 CBHServerDlg 클래스 선언문을
   위에 써줘야 오류가 나지 않는다. */

// 자신이 서버용 소켓으로 사용할 MyServer 클래스를 정의함.
class MyServer : public BH_ServerSocket
{
private:
	// 대화 상자 CBHServerDlg의 주소를 저장할 변수
	CBHServerDlg *mp_parent;

public:
	/* MyServer 클래스가 대화 상자 CBHServerDlg 안에 있는 리스트 박스 m_event_list를
	   쓰기 위해서 이 대화 상자의 주소를 MyServer 클래스가 알도록 해주기 위해서 
	   MyServer 클래스 객체가 생성될 때 이 대화 상자의 주소를 MyServer 클래스
	   객체 생성자의 매개변수 ap_parent에 인자 값으로 넘겨줄 것이다. */
	
	/* 0x27의 의미?
	   MyServer 객체 생성자의 부모 생성자 BH_ServerSocket을 호출할 때
	   패킷의 제일 앞에다가 개발자가 사용하는 프로토콜을 구별 짓기 위해서 0x27이라는 값을
	   쓰겠다는 뜻이다. (프로토콜 구별자로써 사용)
	   0x27 16진수 27을 의미하기도 한다. */

	/* 사용자를 관리하게 하기 위한 클래스는 BH_UserData 이다.
	   쉽게 말해 BH_UserData 클래스를 사용하면 여러 가지 정보들(기본적으로 데이터를 주고 받고
	   ip 기록하고 등등)을 BH_UserData 객체로 작업을 하겠다는 의미이다. */

	/* 프로토콜 헤더의 시작 바이트를 0x27값으로 하고 최대 사용자의 수를 50명으로 정함.
	   그리고 사용자 관리용 클래스는 BH_UserData를 사용함. */
	MyServer(CBHServerDlg *ap_parent) : BH_ServerSocket(0x27, MAX_USER_CNT, new BH_UserData)
	{
		mp_parent = ap_parent;
		/* 대화 상자의 포인터(ap_parent)의 값을 
		   mp_parent에 저장하여 기억하고 있다가 나중에
		   출력하고 싶은 것이 있으면 포인터(mp_parent)를 
		   이용해서 리스트 박스에 접근할 수 있다. */
	}

	// BH_ServerSocket 클래스에 있던 함수 AddWorkForAccept를 상속 받아 재정의 함.(함수의 오버라이딩) 
	// 클라이언트가 서버에 접속될 때 호출되는 함수 AddWorkForAccept를 재정의 함.
	virtual void AddWorkForAccept(BH_UserData *ap_user);

	/* 사용자가 접속을 해제했을 때 호출하는 함수 AddWorkForCloseUser를 상속 받아 재정의 함.
	   (함수의 오버라이딩)
	   
	   ap_user : 접속을 해제한 사용자의 정보가 들어있음.
	   a_err_code : 어떤 이유로 접속을 해제하는 지를 표현한 에러 코드가 들어있음. */
	// 클라이언트가 접속을 해제할 때 호출되는 함수를 재정의 함.
	virtual void AddWorkForCloseUser(BH_UserData *ap_user, int a_err_code);

	/* 클라이언트가 어떤 정보를 서버에 주었을 때 그 정보를 어떻게 처리할지
	   정의하는 함수 ProcessRecvData를 상속 받아 재정의 함.(함수의 오버라이딩) 
	   
	   ah_socket : 서버로 데이터를 전송한 클라이언트 소켓 핸들이 저장된 매개변수
	   a_msg_id : 클라이언트가 보낸 메시지의 종류가 무엇인지 알려주는 id가 저장됨. 
	   ap_recv_data : 클라이언트가 보낸 데이터가 저장됨.
	   a_body_size : 클라이언트가 보낸 데이터의 크기(헤드를 제외한 순수 바디 크기)가 저장됨. */
	// 클라이언트에서 보낸 데이터가 수신될 때 호출되는 함수 ProcessRecvData
	virtual int ProcessRecvData(SOCKET ah_socket, unsigned char a_msg_id, char *ap_recv_data, BSize a_body_size);
};


// CBHServerDlg 대화 상자
class CBHServerDlg : public CDialogEx
{
private:
	MyServer m_server;

// 생성입니다.
public:
	CBHServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	/* CBHServerDlg 클래스 밖에서 이 리스트 박스(m_event_list)의
	   항목을 추가할 때 사용하는 인터페이스 함수 AddEventStr */
	// 리스트 박스에 문자열을 추가하는 함수 AddEventStr
	void AddEventStr(const wchar_t *ap_str);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BHSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 대화 상자 CBHServerDlg 리스트 박스 m_event_list
	CListBox m_event_list;

protected:
	// 사용자가 접속을 시도할 때 호출되는 함수 OnAcceptUser
	afx_msg LRESULT OnAcceptUser(WPARAM wParam, LPARAM lParam);

	// 사용자가 데이터를 보내거나 접속을 해제할 때 호출되는 함수 OnReadAndClose
	afx_msg LRESULT OnReadAndClose(WPARAM wParam, LPARAM lParam);
};
