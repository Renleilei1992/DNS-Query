#pragma once
#ifndef THREAD_TASK_H
#define THREAD_TASK_H

#include <WinSock2.h>
#include <windows.h>
#include <string>
#include <vector>

typedef std::vector<std::string> IPtable;

class ThreadTask
{
public:
	ThreadTask();
	~ThreadTask();

	int		GetHostByName(const char* url, const char* dnsAddr, bool isNormalGet = false);
	bool	CancelThreadTask();

	static DWORD WINAPI ThreadFun_normalGet(LPVOID p);
	static DWORD WINAPI ThreadFun_newGet(LPVOID p);

private:
	HANDLE					m_hThread01;
	HANDLE					m_hThread02;
	DWORD					m_threadID;

	std::string				m_host;
	IPtable					m_ipTable;
};
#endif // THREAD_TASK_H