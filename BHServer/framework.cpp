#include "pch.h"
#include "framework.h"

#pragma comment(lib, "WS2_32.lib")
// Windows ���� ���̺귯��(WS2_32.lib)�� �����.

#ifdef _DEBUG 
// Debug�� ������ �� ������ �� �� D_BH_Socket.lib ���̺귯�� ������ ����. 
	#pragma comment(lib, "D_BH_Socket.lib")
#else
// Release�� ������ �� ������ �� �� R_BH_Socket.lib ���̺귯�� ������ ����.
	#pragma comment(lib, "R_BH_Socket.lib")
#endif
