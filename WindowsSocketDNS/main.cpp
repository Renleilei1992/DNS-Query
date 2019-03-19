// program 1
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <vector>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Dnsapi.lib")
#include <iostream>
#include "ThreadTask.h"

#include <WinDNS.h>

static const char* queryUrl = "www.baidu.com";

using namespace std;

void dnsQuery();

int main() {

	// ��ȡ������ID
	cout << "Main: GetCurrentProcessId: " << (USHORT)GetCurrentProcessId() << endl;

	ThreadTask task;
	task.GetHostByName(queryUrl, "", true);
	_sleep(2000);
	task.GetHostByName(queryUrl, "223.5.5.5", false);
	_sleep(2000);
	task.GetHostByName(queryUrl, "8.8.8.8", false);
	_sleep(2000);
	task.GetHostByName(queryUrl, "114.114.114.114", false);
	_sleep(2000);

	//_sleep(2000);
	//task.CancelThreadTask();

	// --- my add ---
	/*
	{
		//std::string hostName = "ct5.yeshj.com.hjdns.net";
		std::string hostName = "www.google11111.com";
		struct hostent *host1 = gethostbyname(hostName.c_str());
		if (!host1) {
			puts("Get IP address error!");
			system("pause");
			exit(0);
		}

		//����
		for (int i = 0; host1->h_aliases[i]; i++) {
			printf("Aliases %d: %s\n", i + 1, host1->h_aliases[i]);
		}

		//��ַ����
		printf("Address type: %s\n", (host1->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");

		//IP��ַ
		for (int i = 0; host1->h_addr_list[i]; i++) {
			printf("IP addr %d: %s\n", i + 1, inet_ntoa(*(struct in_addr*)host1->h_addr_list[i]));
		}

		// new add
		if (host1) {
			int j = 0;
			while (host1->h_addr_list[j] != 0) {
				std::string ipaddress = inet_ntoa(*(struct in_addr *)host1->h_addr_list[j++]);
				printf("gethostbyname, host(%s===>%s)\n", hostName.c_str(), ipaddress.c_str());
			}
		}
	}
	*/

	dnsQuery();

	system("pause");
	return 0;
}

void dnsQuery()
{
	PIP4_ARRAY pSrvList = NULL;      //pinter to IP4_ARRAY structure
	// Allocate memory for IP4_ARRAY structure
	pSrvList = (PIP4_ARRAY)LocalAlloc(LPTR, sizeof(IP4_ARRAY));
	if (!pSrvList) {
		printf("Memory allocation failed \n");
		exit(1);
	}

	pSrvList->AddrCount = 1;
	pSrvList->AddrArray[0] = inet_addr("223.5.5.5"); //DNS server IP address

	printf("------------------------------------------------dnsQuery()-------------------------------------------- \n");
	PDNS_RECORD pResult = NULL;
	DNS_STATUS dnsStatus = 0;
	//DNS_QUERY_STANDARD ���״ӻ����в�ѯ�����û�в鵽�ŷ���dns��ѯ��
	//dnsStatus = DnsQuery_A("www.baidu.com", DNS_TYPE_A, DNS_QUERY_BYPASS_CACHE/*DNS_QUERY_STANDARD*/, NULL, &pResult, NULL);
	dnsStatus = DnsQuery_A(queryUrl, DNS_TYPE_A, DNS_QUERY_BYPASS_CACHE/*DNS_QUERY_STANDARD*/, pSrvList, &pResult, NULL);
	PDNS_RECORD temp = pResult;
	while (temp)
	{
		IN_ADDR ipaddr;
		ipaddr.S_un.S_addr = temp->Data.A.IpAddress;
		char *pAddr = inet_ntoa(ipaddr);
		printf("Addr:%s \n", pAddr);
		temp = temp->pNext;
	}

	DnsRecordListFree(pResult, DnsFreeRecordListDeep);
}




// program 2
//#include <stdio.h>
//#include <stdlib.h>
//#include <WinSock2.h>
//#include <string>
//#include <time.h>
//#pragma comment(lib, "ws2_32.lib")
//#pragma pack(push, 1)
//struct DNSHeader
//{
//	/* 1. �Ự��ʶ��2�ֽڣ�*/
//	unsigned short usTransID;        // Transaction ID
//
//									 /* 2. ��־����2�ֽڣ�*/
//	unsigned char RD : 1;            // ��ʾ�����ݹ飬1bit
//	unsigned char TC : 1;            // ��ʾ�ɽضϵģ�1bit
//	unsigned char AA : 1;            // ��ʾ��Ȩ�ش�1bit
//	unsigned char opcode : 4;        // 0��ʾ��׼��ѯ��1��ʾ�����ѯ��2��ʾ������״̬����4bit
//	unsigned char QR : 1;            // ��ѯ/��Ӧ��־λ��0Ϊ��ѯ��1Ϊ��Ӧ��1bit
//
//	unsigned char rcode : 4;         // ��ʾ�����룬4bit
//	unsigned char zero : 3;          // ����Ϊ0��3bit
//	unsigned char RA : 1;            // ��ʾ���õݹ飬1bit
//
//									 /* 3. �����ֶΣ���8�ֽڣ� */
//	unsigned short Questions;        // ������
//	unsigned short AnswerRRs;        // �ش���Դ��¼��
//	unsigned short AuthorityRRs;     // ��Ȩ��Դ��¼��
//	unsigned short AdditionalRRs;    // ������Դ��¼��
//};
//#pragma pack(pop)
//
//// ������������
//bool SendDnsPack(IN unsigned short usID,
//	IN SOCKET *pSocket,
//	IN const char *szDnsServer,
//	IN const char *szDomainName);
//void RecvDnsPack(IN unsigned short usId,
//	IN SOCKET *pSocket);
//
//int main(int argc, char* argv[])
//{
//	WSADATA wsaData = {};
//	if (0 != ::WSAStartup(MAKEWORD(2, 2), &wsaData))
//	{
//		printf("WSAStartup fail \n");
//		return -1;
//	}
//
//	SOCKET socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//	if (INVALID_SOCKET == socket)
//	{
//		printf("socket fail \n");
//		return -1;
//	}
//
//	int nNetTimeout = 2000;
//
//	// ���÷���ʱ��
//	::setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
//	// ���ý���ʱ��
//	::setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
//
//	// �������һ��ID
//	srand((unsigned int)time(NULL));
//	unsigned short usId = (unsigned short)rand();
//
//	// �Զ�����Ҫ��ѯ������
//	char szDomainName[256] = {};
//	printf("����Ҫ��ѯ��������");
//	scanf("%s", szDomainName);
//
//	// ����DNS���ģ���Ϊ���ԣ�����ͼ�ָ��8.8.8.8��Ϊ��ѯ������
//	if (!SendDnsPack(usId, &socket, "233.5.5.5", szDomainName))
//	{
//		return -1;
//	}
//
//	// ������Ӧ���ģ�����ʾ��õ�IP��ַ
//	RecvDnsPack(usId, &socket);
//
//	closesocket(socket);
//
//	WSACleanup();
//	return 0;
//}
//
//
//// @Brief : ����DNS��ѯ����
//// @Param: usID: ����ID���
////                pSocket: ��Ҫ���͵�socket
////                szDnsServer: DNS��������ַ
////                szDomainName: ��Ҫ��ѯ������
//// @Retrun: true��ʾ���ͳɹ���false��ʾ����ʧ��
//bool SendDnsPack(IN unsigned short usID,
//	IN SOCKET *pSocket,
//	IN const char *szDnsServer,
//	IN const char *szDomainName)
//{
//	bool bRet = false;
//
//	if (*pSocket == INVALID_SOCKET
//		|| szDomainName == NULL
//		|| szDnsServer == NULL
//		|| strlen(szDomainName) == 0
//		|| strlen(szDnsServer) == 0)
//	{
//		return bRet;
//	}
//
//	unsigned int uiDnLen = strlen(szDomainName);
//
//	// �ж������Ϸ��ԣ�����������ĸ�����ǵ�ţ�������
//	// ����������������ĵ�� 
//	if ('.' == szDomainName[0] || ('.' == szDomainName[uiDnLen - 1]
//		&& '.' == szDomainName[uiDnLen - 2])
//		)
//	{
//		return bRet;
//	}
//
//	/* 1. ������ת��Ϊ���ϲ�ѯ���ĵĸ�ʽ */
//	// ��ѯ���ĵĸ�ʽ�����������ģ�
//	//      6 j o c e n t 2 m e 0
//	unsigned int uiQueryNameLen = 0;
//	BYTE *pbQueryDomainName = (BYTE *)malloc(uiDnLen + 1 + 1);
//	if (pbQueryDomainName == NULL)
//	{
//		return bRet;
//	}
//	// ת����Ĳ�ѯ�ֶγ���Ϊ�������� +2
//	memset(pbQueryDomainName, 0, uiDnLen + 1 + 1);
//
//	// �����ѭ���������£�
//	// �������Ϊ  jocent.me ����ת������ 6 j o c e n t  ������һ����û�и���
//	// �������Ϊ  jocent.me.����ת������ 6 j o c e n t 2 m e
//	unsigned int uiPos = 0;
//	unsigned int i = 0;
//	for (i = 0; i < uiDnLen; ++i)
//	{
//		if (szDomainName[i] == '.')
//		{
//			pbQueryDomainName[uiPos] = i - uiPos;
//			if (pbQueryDomainName[uiPos] > 0)
//			{
//				memcpy(pbQueryDomainName + uiPos + 1, szDomainName + uiPos, i - uiPos);
//			}
//			uiPos = i + 1;
//		}
//	}
//
//	// �������������ǵ�ţ���ô�����ѭ��ֻת����һ����
//	// ����Ĵ������ת��ʣ��Ĳ��֣� ���� 2 m e
//	if (szDomainName[i - 1] != '.')
//	{
//		pbQueryDomainName[uiPos] = i - uiPos;
//		memcpy(pbQueryDomainName + uiPos + 1, szDomainName + uiPos, i - uiPos);
//		uiQueryNameLen = uiDnLen + 1 + 1;
//	}
//	else
//	{
//		uiQueryNameLen = uiDnLen + 1;
//	}
//	// �������  ͷ�� + name + type + class
//	DNSHeader *PDNSPackage = (DNSHeader*)malloc(sizeof(DNSHeader) + uiQueryNameLen + 4);
//	if (PDNSPackage == NULL)
//	{
//		goto exit;
//	}
//	memset(PDNSPackage, 0, sizeof(DNSHeader) + uiQueryNameLen + 4);
//
//	// ���ͷ������
//	PDNSPackage->usTransID = htons(usID);  // ID
//	PDNSPackage->RD = 0x1;   // ��ʾ�����ݹ�
//	PDNSPackage->Questions = htons(0x1);  // ���ĵ�һ����ʾ��������htons����ת��
//
//										  // �����������  name + type + class
//	BYTE* PText = (BYTE*)PDNSPackage + sizeof(DNSHeader);
//	memcpy(PText, pbQueryDomainName, uiQueryNameLen);
//
//	unsigned short *usQueryType = (unsigned short *)(PText + uiQueryNameLen);
//	*usQueryType = htons(0x1);        // TYPE: A
//
//	++usQueryType;
//	*usQueryType = htons(0x1);        // CLASS: IN    
//
//									  // ��Ҫ���͵���DNS�������ĵ�ַ
//	sockaddr_in dnsServAddr = {};
//	dnsServAddr.sin_family = AF_INET;
//	dnsServAddr.sin_port = ::htons(53);  // DNS����˵Ķ˿ں�Ϊ53
//	dnsServAddr.sin_addr.S_un.S_addr = ::inet_addr(szDnsServer);
//
//	// ����ѯ���ķ��ͳ�ȥ
//	int nRet = ::sendto(*pSocket,
//		(char*)PDNSPackage,
//		sizeof(DNSHeader) + uiQueryNameLen + 4,
//		0,
//		(sockaddr*)&dnsServAddr,
//		sizeof(dnsServAddr));
//	if (SOCKET_ERROR == nRet)
//	{
//		printf("DNSPackage Send Fail! \n");
//		goto exit;
//	}
//
//	printf("DNSPackage Send Success! \n");
//	bRet = true;
//
//	// ͳһ����Դ����       
//exit:
//	if (PDNSPackage)
//	{
//		free(PDNSPackage);
//		PDNSPackage = NULL;
//	}
//
//	if (pbQueryDomainName)
//	{
//		free(pbQueryDomainName);
//		pbQueryDomainName = NULL;
//	}
//
//	return bRet;
//}
//
//void RecvDnsPack(IN unsigned short usId,
//	IN SOCKET *pSocket)
//{
//	if (*pSocket == INVALID_SOCKET)
//	{
//		return;
//	}
//
//	char szBuffer[256] = {};        // ������յ�������
//	sockaddr_in servAddr = {};
//	int iFromLen = sizeof(sockaddr_in);
//
//	int iRet = ::recvfrom(*pSocket,
//		szBuffer,
//		256,
//		0,
//		(sockaddr*)&servAddr,
//		&iFromLen);
//	if (SOCKET_ERROR == iRet || 0 == iRet)
//	{
//		printf("recv fail \n");
//		return;
//	}
//
//	/* �����յ������� */
//	DNSHeader *PDNSPackageRecv = (DNSHeader *)szBuffer;
//	unsigned int uiTotal = iRet;        // ���ֽ���
//	unsigned int uiSurplus = iRet;  // ���ܵ����ܵ��ֽ���
//
//									// ȷ���յ���szBuffer�ĳ��ȴ���sizeof(DNSHeader)
//	if (uiTotal <= sizeof(DNSHeader))
//	{
//		printf("���յ������ݳ��Ȳ��Ϸ�\n");
//		return;
//	}
//
//	// ȷ��PDNSPackageRecv�е�ID�Ƿ��뷢�ͱ����е���һ�µ�
//	if (htons(usId) != PDNSPackageRecv->usTransID)
//	{
//		printf("���յ��ı���ID���ѯ���Ĳ����\n");
//		return;
//	}
//
//	// ȷ��PDNSPackageRecv�е�FlagsȷʵΪDNS����Ӧ����
//	if (0x01 != PDNSPackageRecv->QR)
//	{
//		printf("���յ��ı��Ĳ�����Ӧ����\n");
//		return;
//	}
//
//	// ��ȡQueries�е�type��class�ֶ�
//	unsigned char *pChQueries = (unsigned char *)PDNSPackageRecv + sizeof(DNSHeader);
//	uiSurplus -= sizeof(DNSHeader);
//
//	for (; *pChQueries && uiSurplus > 0; ++pChQueries, --uiSurplus) { ; } // ����Queries�е�name�ֶ�
//
//	++pChQueries;
//	--uiSurplus;
//
//	if (uiSurplus < 4)
//	{
//		printf("���յ������ݳ��Ȳ��Ϸ�\n");
//		return;
//	}
//
//	unsigned short usQueryType = ntohs(*((unsigned short*)pChQueries));
//	pChQueries += 2;
//	uiSurplus -= 2;
//
//	unsigned short usQueryClass = ntohs(*((unsigned short*)pChQueries));
//	pChQueries += 2;
//	uiSurplus -= 2;
//
//	// ����Answers�ֶ�
//	unsigned char *pChAnswers = pChQueries;
//	while (0 < uiSurplus && uiSurplus <= uiTotal)
//	{
//		// ����name�ֶΣ����ã�
//		if (*pChAnswers == 0xC0)  // ��ŵ���ָ��
//		{
//			if (uiSurplus < 2)
//			{
//				printf("���յ������ݳ��Ȳ��Ϸ�\n");
//				return;
//			}
//			pChAnswers += 2;       // ����ָ���ֶ�
//			uiSurplus -= 2;
//		}
//		else        // ��ŵ�������
//		{
//			// ������������Ϊ�Ѿ�У����ID�������Ͳ�����
//			for (; *pChAnswers && uiSurplus > 0; ++pChAnswers, --uiSurplus) { ; }
//			pChAnswers++;
//			uiSurplus--;
//		}
//
//		if (uiSurplus < 4)
//		{
//			printf("���յ������ݳ��Ȳ��Ϸ�\n");
//			return;
//		}
//
//		unsigned short usAnswerType = ntohs(*((unsigned short*)pChAnswers));
//		pChAnswers += 2;
//		uiSurplus -= 2;
//
//		unsigned short usAnswerClass = ntohs(*((unsigned short*)pChAnswers));
//		pChAnswers += 2;
//		uiSurplus -= 2;
//
//		if (usAnswerType != usQueryType || usAnswerClass != usQueryClass)
//		{
//			printf("���յ�������Type��Class�뷢�ͱ��Ĳ�һ��\n");
//			return;
//		}
//
//		pChAnswers += 4;    // ����Time to live�ֶΣ�����DNS Client��˵������ֶ�����
//		uiSurplus -= 4;
//
//		if (htons(0x04) != *(unsigned short*)pChAnswers)
//		{
//			uiSurplus -= 2;     // ����data length�ֶ�
//			uiSurplus -= ntohs(*(unsigned short*)pChAnswers); // ����������length
//
//			pChAnswers += 2;
//			pChAnswers += ntohs(*(unsigned short*)pChAnswers);
//		}
//		else
//		{
//			if (uiSurplus < 6)
//			{
//				printf("���յ������ݳ��Ȳ��Ϸ�\n");
//				return;
//			}
//
//			uiSurplus -= 6;
//			// TypeΪA, ClassΪIN
//			if (usAnswerType == 1 && usAnswerClass == 1)
//			{
//				pChAnswers += 2;
//
//				unsigned int uiIP = *(unsigned int*)pChAnswers;
//				in_addr in = {};
//				in.S_un.S_addr = uiIP;
//				printf("IP: %s\n", inet_ntoa(in));
//
//				pChAnswers += 4;
//			}
//			else
//			{
//				pChAnswers += 6;
//			}
//		}
//	}
//}


