/*
#include "BH_Socket.h"

#pragma comment(lib, "WS2_32.lib")

#ifdef _DEBUG
    #pragma comment(lib, "D_BH_Socket.lib")
#else
    #pragma comment(lib, "R_BH_Socket.lib")
#endif
*/

#ifndef _BH_SOCKET_H_
#define _BH_SOCKET_H_

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>

#define LM_SEND_COMPLETED      29001
#define LM_RECV_COMPLETED      29002

typedef unsigned short int BSize;
#define HEAD_SIZE          2+sizeof(BSize)

// ���۰� ���ſ��� ���������� �۾��� ������ ������ Ŭ����
class BH_EchManager
{
protected:
    // ���� �Ǵ� ������ ���� �Ҵ�� �޸��� ��ü ũ��� ���� �۾����� ũ��
    int m_tot_size, m_cur_size;

    // ���� �Ǵ� ������ ���ؼ� �Ҵ�� �޸��� ���� �ּ�
    char* mp_data;

public:
    BH_EchManager();   // ��ü ������
    ~BH_EchManager();  // ��ü �Ҹ���

    /* ���� �Ǵ� ���ſ� ����� �޸𸮸� �Ҵ��Ѵ�.
       a_data_size�� �Ҵ��� ũ�⸦ ����ϸ� �� �Լ��� ��ȯ ������
       �Ҵ�� �޸��� �ּҸ� ��ȯ�Ѵ�.(��ȯ�� �ּҴ� �� Ŭ������ �����ϰ� �ִ�.) */
    char* MemoryAlloc(int a_data_size);

    // ���� �Ǵ� ���ſ��� ���Ǵ� �޸𸮸� �����Ѵ�.
    void DelData();

    // �Ҵ�� �޸��� ũ�⸦ ��ȯ�ϴ� GetTotSize �Լ�
    inline int GetTotSize() { return m_tot_size; }

    // ���� �۾� ���� �޸��� ��ġ�� ��ȯ�ϴ� GetCurSize �Լ�
    inline int GetCurSize() { return m_cur_size; }
};

class BH_SendManager : public BH_EchManager
{
public:
    // ���� ������ ��ġ�� ũ�⸦ ����ϴ� GetPos �Լ�
    int GetPos(char** ap_data, int a_data_size = 2048);

    // ������ �Ϸ�Ǿ����� üũ�ϴ� IsProcessing �Լ�
    inline char IsProcessing() { return m_tot_size != m_cur_size; }
};

class BH_RecvManager : public BH_EchManager
{
public:
    // ���ŵ� �����͸� ������ ���� �Ǿ� �ִ� �����Ϳ� �߰��Ѵ�.
    int AddData(char* ap_data, int a_size);

    // ���ŵ� �����͸� �ϳ��� ��ģ �޸��� ���� �ּҸ� ��� GetData �Լ�
    inline char* GetData() { return mp_data; }
};

class BH_Socket
{
protected:
    // ���������� ��ȿ���� üũ�ϱ� ���� ���� ���� ������ ����
    unsigned char m_val_key;

    // ���۰� ���ſ� ����� ����
    char* mp_send_data, * mp_recv_data;

    // �����Ͱ� ���ŵǰų� ������� ������ �������� �� �޽����� ������ ������ �ڵ� ��
    HWND mh_notify_wnd;

    // �����Ͱ� ���ŵǰų� ������� ������ �������� �� ����� �޽��� ID
    int m_data_notify_id;

public:
    /* ��ü �����ÿ� �������� ���� ���� ������ ���� �� ���� ������ �����
       �޽��� ID�� �����ؾ� ��. */
       // BH_Socket Ŭ������ ��ü ������ BH_Socket
    BH_Socket(unsigned char a_val_key, int a_data_notify_id);

    // BH_Socket Ŭ������ ��ü �Ҹ��� ~BH_Socket
    virtual ~BH_Socket();

    /* ����� ��󿡰� ��Ʈ��ũ�� �����͸� ������ �� ����� �Լ� SendFrameData
       �� �Լ��� ���޵� ������ ������ �ϳ��� �޸𸮿� ��ӵ� Head ������ �����ؼ� ������. */
    int SendFrameData(SOCKET ah_socket, unsigned char a_msg_id, const char* ap_body_data, BSize a_body_size);

    // ��õ� ������ �����ϴ� �Լ� ReceiveData
    int ReceiveData(SOCKET ah_socket, BSize a_body_size);

    // �����Ͱ� ���ŵǾ��� �� ���ŵ� �����͸� ó���ϴ� �Լ� ProcessRecvEvent
    void ProcessRecvEvent(SOCKET ah_socket);

    // ���ӵ� ����� ���� �� ����ϴ� �Լ� - �ڽ� Ŭ�������� �� �������ؼ� ����ؾ� ��.
    virtual void DisconnSocket(SOCKET ah_socket, int a_err_code);

    // ���ŵ� �����͸� ó���ϴ� �Լ� - �ڽ� Ŭ�������� �� �������ؼ� ����ؾ� ��.
    virtual int ProcessRecvData(SOCKET ah_socket, unsigned char a_msg_id, char* ap_recv_data, BSize a_body_size);

    // ASCII ������ ���ڿ��� �����ڵ�� ��ȯ��.
    static void AscToUnic(wchar_t* ap_dest_ip, char* ap_src_ip);

    // �����ڵ� ������ ���ڿ��� ASCII�� ��ȯ��.
    static void UnicToAsc(char* ap_dest_ip, wchar_t* ap_src_ip);
};

class BH_UserData
{
protected:
    // Ŭ���̾�Ʈ�� ����ϱ� ���� ����� ���� �ڵ�
    SOCKET mh_socket;

    // Ŭ���̾�Ʈ���� ū �����͸� �����ϱ� ���� ����� ��ü
    BH_SendManager* mp_send_man;

    // Ŭ���̾�Ʈ���� ū �����͸� �����ϱ� ���� ����� ��ü
    BH_RecvManager* mp_recv_man;

    // Ŭ���̾�Ʈ�� IP �ּ�
    wchar_t m_ip_addr[16];

public:
    BH_UserData();            // BH_UserData Ŭ������ ��ü ������ BH_UserData
    virtual ~BH_UserData();   // BH_UserData Ŭ������ ��ü �Ҹ��� ~BH_UserData

    // ���� �ڵ� ���� ��� �Լ� GetHandle
    inline SOCKET GetHandle() { return mh_socket; }

    // ���� �ڵ� ���� �����ϴ� �Լ� SetHandle
    inline void SetHandle(SOCKET ah_socket) { mh_socket = ah_socket; }

    // ���ۿ� ��ü�� �ּҸ� ��� �Լ� GetSendMan
    inline BH_SendManager* GetSendMan() { return mp_send_man; }

    // ���ſ� ��ü�� �ּҸ� ��� �Լ� GetRecvMan
    inline BH_RecvManager* GetRecvMan() { return mp_recv_man; }

    // Ŭ���̾�Ʈ�� IP �ּҸ� ��� �Լ� GetIP
    wchar_t* GetIP();

    // Ŭ���̾�Ʈ�� IP �ּҸ� �����ϴ� �Լ� SetIP
    void SetIP(const wchar_t* ap_ip_addr);

    /* ���� ������ �Ҵ�Ǿ� �ִٸ� ������ �ݰ� �ʱ�ȭ�Ѵ�.
       a_linger_flag�� 1�� ����ϸ� ������ �����͸� ���� ���̴���
       ��� ������ �����Ѵ�. */
    void CloseSocket(char a_linger_flag = 0);

    // ������ ���� ��, ������ Ŭ������ Ȯ���� �� ����ϴ� �Լ� CreateObj
    virtual BH_UserData* CreateObj() { return new BH_UserData; }

};

class BH_ServerSocket : public BH_Socket
{
protected:
    // listen �۾��� ����� ���� �ڵ�
    SOCKET mh_listen_socket;

    // ���ο� Ŭ���̾�Ʈ�� �������� �� �߻��� �޽��� ID ��
    int m_accept_notify_id;

    // ������ ������ �ִ� ����� ��
    unsigned short int m_max_user_cnt;

    // �ִ� ����ڸ� �����ϱ� ���ؼ� ����� ��ü��
    BH_UserData** mp_user_list;

public:
    // BH_ServerSocket Ŭ������ ��ü ������ BH_ServerSocket
    BH_ServerSocket(unsigned char a_val_key, unsigned short int a_max_user_cnt, BH_UserData* ap_user_data,
        int a_accept_notify_id = 25001, int a_data_notify_id = 25002);

    // BH_ServerSocket Ŭ������ ��ü �Ҹ��� ~BH_ServerSocket
    virtual ~BH_ServerSocket();

    /* ���� ���񽺸� �����ϴ� �Լ� StartServer
       ��ȯ���� 1�̸� ���� ���� ����, -1�̸� ���� ���� ����,
       -2�̸� ������ ������ ���ε� �ϴµ� ���и� �ǹ���. */
    int StartServer(const wchar_t* ap_ip_addr, int a_port, HWND ah_notify_wnd);

    // Accept �ÿ� �߰������� �ؾ��� �۾��� �ִٸ� �� �Լ��� �������̵��ؼ� ó���ؾ���.
    virtual void AddWorkForAccept(BH_UserData* ap_user) { }

    // �ִ� ����� �� �ʰ� �ÿ� �߰������� �ؾ��� �۾��� �ִٸ� �� �Լ��� �������̵��ؼ� ó���ؾ���.
    virtual void ShowLimitErr(const wchar_t* ap_ip_addr) { }

    /* ���ο� Ŭ���̾�Ʈ�� ������ �� ����� �Լ� ProcessToAccpet
       - FD_ACCEPT�� ����� �޽����� �߻����� �� �����.
       �Լ��� ��ȯ���� 1�̸� accept ����, -1�̸� accept ����,
       -2�̸� ���� ������ �ִ� ����� �� �ʰ��� �ǹ���. */
    int ProcessToAccept(WPARAM wParam, LPARAM lParam);

    /* Ŭ���̾�Ʈ�� ���� ���� �ÿ� �߰������� �ؾ��� �۾��� �ִٸ�
       �� �Լ��� �������̵��ؼ� ó���ؾ���.
       a_err_code : 0�̸� ���� ����, -1�̸� Ű���� ��ȿ���� �ʾƼ� ����,
                    -2�̸� �ٵ� ���� ���� �߿� ������ �߻����� �ǹ���. */
    virtual void AddWorkForCloseUser(BH_UserData* ap_user, int a_err_code);

    /* ���ο� �����Ͱ� ����(FD_READ)�ǰų� Ŭ���̾�Ʈ�� ������ ����(FD_CLOSE)�� ��
       �߻��ϴ� �޽������� ���� ���ϰ� ���õ� �۾��� ó�����ִ� �Լ� ProcessClientEvent */
    void ProcessClientEvent(WPARAM wParam, LPARAM lParam);

    /* ���� �ڵ��� ����Ͽ� � ��������� ã�´�.
       ã���� ������� ��ġ�� ��ȯ�ϴ� �Լ� FindUserIndex */
    inline int FindUserIndex(SOCKET ah_socket)
    {
        for (int i = 0; i < m_max_user_cnt; i++)
        {
            if (mp_user_list[i]->GetHandle() == ah_socket)
                return i;
        }
        return -1;
    }

    /* ���� �ڵ��� ����Ͽ� � ��������� ã�´�.
       ã���� ����� ������ �����ϴ� ��ü�� �ּҸ� ��ȯ�ϴ� �Լ� FindUserData */
    inline BH_UserData* FindUserData(SOCKET ah_socket)
    {
        for (int i = 0; i < m_max_user_cnt; i++)
        {
            if (mp_user_list[i]->GetHandle() == ah_socket)
                return mp_user_list[i];
        }
        return NULL;
    }

    // ��ü ����ڿ� ���� ������ ���� �� ����ϴ� �Լ� GetUserList
    inline BH_UserData** GetUserList() { return mp_user_list; }

    // ����(a_index)�� ����Ͽ� Ư�� ����ڿ� ���� ������ ���� �� ����ϴ� �Լ� GetUserData
    inline BH_UserData* GetUserData(int a_index) { return mp_user_list[a_index]; }

    // �ִ� ����� ���� ���� �� ����ϴ� �Լ� GetMaxUserCnt
    unsigned short int GetMaxUserCnt() { return m_max_user_cnt; }

    // ah_socket �ڵ� ���� ����ϴ� ���� ����ڸ� ������ ���� ��Ű�� �Լ� DisconnSocket
    virtual void DisconnSocket(SOCKET ah_socket, int a_err_code);

    /* FD_READ �̺�Ʈ�� �߻����� �� ���� �����͸� ó���ϴ� �Լ� ProcessRecvData
       (���� ������ ��� ���� Ŭ�������� �ݵ�� ������ �Ͽ� �ڽ��� ������ �޽����� ó���ؾ� �Ѵ�.) */
    virtual int ProcessRecvData(SOCKET ah_socket, unsigned char a_msg_id, char* ap_recv_data,
        BSize a_body_size);
};

class BH_ClientSocket : public BH_Socket
{
protected:
    // ������ ����ϱ� ���� ����� ���� �ڵ�
    SOCKET mh_socket;

    // 0 : ���� �ȵ�, 1 : ���� �õ� ��, 2 : ���� ��
    char m_conn_flag;

    // ������ ������ �õ��� ����� �˷��� ������ �޽����� ������.
    int m_conn_notify_id;

    // ������ ū �����͸� �����ϱ� ���� ����� ��ü
    BH_SendManager m_send_man;

    // ������ ū �����͸� �����ϱ� ���� ����� ��ü
    BH_RecvManager m_recv_man;

public:
    // BH_ClientSocket Ŭ������ ��ü ������ BH_ClientSocket
    BH_ClientSocket(unsigned char a_val_key, int a_conn_notify_id = 26001,
        int a_data_notify_id = 26002);

    // BH_ClientSocket Ŭ������ ��ü �Ҹ��� ~BH_ClientSocket
    virtual ~BH_ClientSocket();

    // ���ۿ� ��ü�� �ּҸ� ��´�.
    inline BH_SendManager* GetSendMan() { return &m_send_man; }

    // ���ſ� ��ü�� �ּҸ� ��´�.
    inline BH_RecvManager* GetRecvMan() { return &m_recv_man; }

    // ������ ������ �õ��ϴ� �Լ� ConnToServer
    int ConnToServer(const wchar_t* ap_ip_addr, int a_port_num, HWND ah_notify_wnd);

    /* ���� ���ӿ� ���� ����� �޽����� ���޵Ǿ��� �� �� �Լ��� ����ϸ� �ȴ�.
       ��ȯ���� 1�̸� ������ ������ ������, 0�̸� ������ ������ ������. */
    int ResultOfConn(LPARAM lParam);

    /* ������ �����͸� �����ϰų� ������ �������� �� �߻��ϴ� ������ �޽�������
       ����ϴ� �Լ� ProcessServerEvent
       ��ȯ���� 0�̸� ������ ������ ����, 1�̸� �������� �����͸� ������. */
    int ProcessServerEvent(WPARAM wParam, LPARAM lParam);

    // ������ ������ ������ ������ �� ����ϴ� �Լ� DisconnSocket
    virtual void DisconnSocket(SOCKET ah_socket, int a_err_code);

    /* FD_READ �̺�Ʈ�� �߻����� �� ���� �����͸� ó���ϴ� �Լ� ProcessRecvData
       (���� ������ ��� ���� Ŭ�������� �ݵ�� �������Ͽ� �ڽ��� ������ �޽����� ó���ؾ� �Ѵ�.) */
    virtual int ProcessRecvData(SOCKET ah_socket, unsigned char a_msg_id, char* ap_recv_data,
        BSize a_body_size);

    /* ������ �����͸� ������ �� ����ϴ� �Լ� SendFrameData
       �޽��� ID�� ������ �������� �ּ�, �������� ũ�⸦ �Ѱ��ָ�
       ��ӵ� �������� �������� �����ؼ� �����մϴ�. */
    int SendFrameData(unsigned char a_msg_id, const char* ap_body_data, BSize a_body_size);

    /* �������� ���� ���¸� �˰� ���� �� ����ϴ� �Լ� IsConnected
       0 : ���� ����, 1 : ���� ���¸� �ǹ���.
       ���������δ� ���¸� �� ������ ���������� �ܺο� �˷��� ����
       �� ���� ���·� �˷��ش�. '���� �õ� ��' ���´� ������ �����Ѵ�. */
    inline int IsConnected() { return m_conn_flag == 2; }

    // ������ ����ϱ� ���� ������ ������ �ڵ� ���� �˰� ���� �� ����Ѵ�. 
    inline SOCKET GetHandle() { return mh_socket; }
};

#endif

