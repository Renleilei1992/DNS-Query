#include "ThreadTask.h"
#include <iostream>
#include "Request_DNS.h"
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

static string static_hostUrl = "";
static string static_DNSaddr = "";
static bool static_bRun = false;

ThreadTask::ThreadTask()
{
}

ThreadTask::~ThreadTask()
{
	cout << "析构开始 =================> handle close!" << endl;
	static_bRun = false;
	CloseHandle(m_hThread01);
	CloseHandle(m_hThread02);
}

int ThreadTask::GetHostByName(const char * url, const char* dnsAddr, bool isNormalGet /* = false*/)
{
	if (url == NULL)
	{
		return -1;
	}

	if (std::string(url).empty())
	{
		printf("%s url is empty!\n", __FUNCTION__);
		return -1;
	}

	static_bRun = false;
	/*if (m_hThread01)
	{
		printf("%s already run!\n", __FUNCTION__);
		CancelThreadTask();
	}*/

	{
		static_hostUrl = url;
		static_DNSaddr = dnsAddr;
		m_ipTable.clear();
		
		if (!static_bRun)
		{
			printf("%s isRun 01!\n", __FUNCTION__);
			static_bRun = true;
			m_hThread01 = CreateThread(NULL, 0, isNormalGet ? ThreadFun_normalGet : ThreadFun_newGet, this, 0, &m_threadID);
		}
		else
		{
			printf("%s isRun 02!\n", __FUNCTION__);
			m_hThread02 = CreateThread(NULL, 0, isNormalGet ? ThreadFun_normalGet : ThreadFun_newGet, this, 0, 0);
		}
	}

	return 0;
}

bool ThreadTask::CancelThreadTask()
{
	cout << "取消正在进行的任务 =================> handle close!" << endl;
	static_bRun = false;
	CloseHandle(m_hThread01);
	CloseHandle(m_hThread02);
	return false;
}

DWORD WINAPI ThreadTask::ThreadFun_normalGet(LPVOID p)
{
	printf("%s ----> thread run!\n", __FUNCTION__);
	cout << "ThreadFun_normalGet: GetCurrentThreadId: " << (USHORT)GetCurrentThreadId() << endl;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	struct hostent *host = gethostbyname(static_hostUrl.c_str());
	if (!host) {
		puts("Get IP address error!");
		system("pause");
		exit(0);
	}

	//别名
	for (int i = 0; host->h_aliases[i]; i++) {
		printf("Aliases %d: %s\n", i + 1, host->h_aliases[i]);
	}

	//地址类型
	printf("Address type: %s\n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");

	//IP地址
	for (int i = 0; host->h_addr_list[i]; i++) {
		printf("IP addr %d: %s\n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
	}

	// 更新线程运行结束标志
	if (!static_bRun)
	{
		static_bRun = true;
	}

	return 0;
}

DWORD WINAPI ThreadTask::ThreadFun_newGet(LPVOID p)
{
	printf("%s ----> thread run!\n", __FUNCTION__);
	cout << "ThreadFun_newGet: GetCurrentThreadId: " << (USHORT)GetCurrentThreadId() << endl;

	// send DNS request
	//char szDomainName[] = "www.cctalk.com";
	char *szDomainName = const_cast<char*>(static_hostUrl.c_str());
	std::vector<ULONG> veculIPList;
	std::vector<std::string> vecstrIPList;
	std::vector<std::string> vecCNameList;
	ULONG ulTimeSpent = 0;
	RequestDNS dnsLookup;

	veculIPList.clear();
	vecstrIPList.clear();
	vecCNameList.clear();

	if (static_DNSaddr.empty())
	{
		printf("%s ----> DNS addr is empty! \n", __FUNCTION__);
		return -1;
	}
	else
	{
		printf("%s ----> DNS addr is not empty! dns: [%s], url: [%s]\n", __FUNCTION__, static_DNSaddr.c_str(), static_hostUrl.c_str());
	}

	BOOL bRetVal = dnsLookup.DNSLookup(inet_addr(static_DNSaddr.c_str()), szDomainName, &vecstrIPList, &veculIPList, &vecCNameList, 1000, &ulTimeSpent);

	if (bRetVal)
	{
		for (auto iVal_IPul : veculIPList)
		{
			std::cout << "IP(ULONG) = " << iVal_IPul << "  " << std::endl;
		}
		for (auto iVal_IPstr : vecstrIPList)
		{
			std::cout << "IP(string) = " << iVal_IPstr << "  " << std::endl;
		}
		for (auto iVal_CNAME : vecCNameList)
		{
			std::cout << "CNAME = " << iVal_CNAME << "  " << std::endl;
		}
		std::cout << "time Spent: " << ulTimeSpent << std::endl;

		static_hostUrl.clear();
		static_DNSaddr.clear();
		veculIPList.clear();
		vecstrIPList.clear();
		vecCNameList.clear();
	}
	else
	{
		cout << "dns look up failed!" << endl;
	}
	

	// 更新线程运行结束标志
	if (!static_bRun)
	{
		static_bRun = true;
	}

	return 0;
}
