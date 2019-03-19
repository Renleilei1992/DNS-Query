#ifndef REQUEST_DNS_H
#define REQUEST_DNS_H

#pragma once
#include <string>
#include <vector>
#include <WinSock2.h>

// 配置packet的大小
#define DNS_PORT			53
#define MAX_DOMAINNAME_LEN  255
#define DNS_TYPE_SIZE       2
#define DNS_CLASS_SIZE      2
#define DNS_TTL_SIZE        4
#define DNS_DATALEN_SIZE    2
#define DNS_TYPE_A          0x0001 //1 a host address
#define DNS_TYPE_CNAME      0x0005 //5 the canonical name for an alias
#define DNS_PACKET_MAX_SIZE (sizeof(DNSHeader) + MAX_DOMAINNAME_LEN + DNS_TYPE_SIZE + DNS_CLASS_SIZE)


struct DNSHeader
{
	USHORT usTransID; //标识符
	USHORT usFlags; //各种标志位
	USHORT usQuestionCount; //Question字段个数 
	USHORT usAnswerCount; //Answer字段个数
	USHORT usAuthorityCount; //Authority字段个数
	USHORT usAdditionalCount; //Additional字段个数
};

//DNS header structure
struct DNS_HEADER {
	unsigned short identifier;				// identification number
	unsigned char recursion_desired : 1;	// recursion desired
	unsigned char truncated_message : 1;	// truncated message
	unsigned char authority_answer : 1;		// authoritive answer
	unsigned char op_code : 4;				// purpose of message
	unsigned char query_or_response : 1;	// query/response flag
	unsigned char response_code : 4;		// response code
	unsigned char checking_disabled : 1;	// checking disabled
	unsigned char authenticated_data : 1;	// authenticated data
	unsigned char z : 1;					// its z! reserved
	unsigned char recursion_available : 1;	// recursion available
	unsigned short question_count;			// number of question entries
	unsigned short answer_count;			// number of answer entries
	unsigned short authority_count;			// number of authority entries
	unsigned short resource_count;			// number of resource entries
};

class RequestDNS
{
public:
	RequestDNS();
	~RequestDNS();

	BOOL DNSLookup(ULONG ulDNSServerIP, char *szDomainName, std::vector<std::string> *pvecstrIPList, std::vector<ULONG> *pveculIPList, std::vector<std::string> *pvecstrCNameList, ULONG ulTimeout, ULONG *pulTimeSpent);

private:
	BOOL Init();
	BOOL UnInit();
	BOOL SendDNSRequest(sockaddr_in sockAddrDNSServer, char *szDomainName);
	BOOL RecvDNSResponse(sockaddr_in sockAddrDNSServer, ULONG ulTimeout, std::vector<ULONG> *pveculIPList, std::vector<std::string> *pvecstrCNameList, ULONG *pulTimeSpent);
	BOOL EncodeDotStr(char *szDotStr, char *szEncodedStr, USHORT nEncodedStrSize);
	BOOL DecodeDotStr(char *szEncodedStr, USHORT *pusEncodedStrLen, char *szDotStr, USHORT nDotStrSize, char *szPacketStartPos);
	ULONG GetTickCountCalibrate();

private:
	BOOL m_bIsInitOK;
	SOCKET m_socket;
	WSAEVENT m_event;
	USHORT m_usCurrentProcID;
	char *m_szDNSPacket;
};
#endif // REQUEST_DNS_H