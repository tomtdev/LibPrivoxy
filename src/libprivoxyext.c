#include <time.h>
#include <stdio.h>
#ifdef WIN32
#include <process.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/time.h>
#endif
#include "libprivoxyext.h"

#ifdef WIN32
int	gettimeofday(struct timeval *tp, void *tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return (0);
}
#endif

void libprivoxy_exit(int _Code)
{
	_endthread();
}

unsigned long libprivoxy_resolve_hostname_to_ip(const char *host)
{
	unsigned long result = INADDR_NONE;
	unsigned char buf[65536], *qname, *reader;
	int i, j, stop;
	int timeout = 3000; //3 seconds
	int retry_times, retry_index;
	int recv_ok = 0;

	SOCKET s;
	struct sockaddr_in a;

	struct RES_RECORD answers[20], auth[20], addit[20]; //the replies from the DNS server
	struct sockaddr_in dest;

	struct DNS_HEADER *dns = NULL;
	struct QUESTION *qinfo = NULL;

	if ((host == NULL) || (*host == '\0'))
	{
		return INADDR_ANY;
	}

	result = inet_addr(host);
	if (result != INADDR_NONE)
	{
		return result;
	}

	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //UDP packet for DNS queries

	//Configure the sockaddress structure with information of DNS server
	dest.sin_family = AF_INET;
	dest.sin_port = htons(53);

	//Set the DNS structure to standard queries
	dns = (struct DNS_HEADER *)&buf;

	srand((unsigned)time(NULL));
	dns->id = (unsigned short)htons(rand());
	dns->qr = 0; //This is a query
	dns->opcode = 0; //This is a standard query
	dns->aa = 0; //Not Authoritative
	dns->tc = 0; //This message is not truncated
	dns->rd = 1; //Recursion Desired
	dns->ra = 0; //Recursion not available! hey we dont have it (lol)
	dns->z = 0;
	dns->ad = 0;
	dns->cd = 0;
	dns->rcode = 0;
	dns->q_count = htons(1); //we have only 1 question
	dns->ans_count = 0;
	dns->auth_count = 0;
	dns->add_count = 0;

	//point to the query portion
	qname = &buf[sizeof(struct DNS_HEADER)];

	libprivoxy_host_to_dns_format(qname, host);

	qinfo = (struct QUESTION*)&buf[sizeof(struct DNS_HEADER) + (strlen(qname) + 1)]; //fill it

	qinfo->qtype = htons(1); //we are requesting the ipv4 address
	qinfo->qclass = htons(1); //its internet (lol)

	retry_times = dns_servers_count == 1 ? 2 : dns_servers_count;

	for (retry_index = 0; retry_index < retry_times; ++retry_index) {
		if (dns_servers_count == 1) {
			dest.sin_addr.s_addr = inet_addr(dns_servers[0]);
		}
		else {
			dest.sin_addr.s_addr = inet_addr(dns_servers[retry_index]);
		}

		if (setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) == SOCKET_ERROR) {
			printf("%s setsockopt SO_SNDTIMEO error: %d\n", host, WSAGetLastError());
			continue;
		}

		if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == SOCKET_ERROR) {
			printf("%s setsockopt SO_RCVTIMEO error: %d\n", host, WSAGetLastError());
			continue;
		}

		if (sendto(s, buf, sizeof(struct DNS_HEADER) + (strlen(qname) + 1) + sizeof(struct QUESTION), 0, (struct sockaddr*)&dest, sizeof(dest)) == SOCKET_ERROR)
		{
			printf("%s sendto error: %d\n", host, WSAGetLastError());
			continue;
		}

		i = sizeof(dest);
		if (recvfrom(s, buf, 65536, 0, (struct sockaddr*)&dest, &i) == SOCKET_ERROR)
		{
			printf("%s recvfrom error: %d\n", host, WSAGetLastError());
			continue;
		}

		recv_ok = 1;
		break;
	}

	if (recv_ok == 0) {
		printf("%s retry failed.\n", host);
		return INADDR_NONE;
	}

	dns = (struct DNS_HEADER*)buf;

	//move ahead of the dns header and the query field
	reader = &buf[sizeof(struct DNS_HEADER) + (strlen(qname) + 1) + sizeof(struct QUESTION)];

	//reading answers
	stop = 0;
	for (i = 0; i < ntohs(dns->ans_count); i++)
	{
		answers[i].name = libprivoxy_read_dns_name(reader, buf, &stop);
		reader = reader + stop;

		answers[i].resource = (struct R_DATA*)(reader);
		reader = reader + sizeof(struct R_DATA);

		if (ntohs(answers[i].resource->type) == 1) //if its an ipv4 address
		{
			answers[i].rdata = (unsigned char*)malloc(ntohs(answers[i].resource->data_len));
			for (j = 0; j < ntohs(answers[i].resource->data_len); j++) {
				answers[i].rdata[j] = reader[j];
			}
			answers[i].rdata[ntohs(answers[i].resource->data_len)] = '\0';
			reader = reader + ntohs(answers[i].resource->data_len);
		}
		else
		{
			answers[i].rdata = libprivoxy_read_dns_name(reader, buf, &stop);
			reader = reader + stop;
		}
	}

	//read authorities
	for (i = 0; i < ntohs(dns->auth_count); i++)
	{
		auth[i].name = libprivoxy_read_dns_name(reader, buf, &stop);
		reader += stop;

		auth[i].resource = (struct R_DATA*)(reader);
		reader += sizeof(struct R_DATA);

		auth[i].rdata = libprivoxy_read_dns_name(reader, buf, &stop);
		reader += stop;
	}

	//read additional
	for (i = 0; i < ntohs(dns->add_count); i++)
	{
		addit[i].name = libprivoxy_read_dns_name(reader, buf, &stop);
		reader += stop;

		addit[i].resource = (struct R_DATA*)(reader);
		reader += sizeof(struct R_DATA);

		if (ntohs(addit[i].resource->type) == 1)
		{
			addit[i].rdata = (unsigned char*)malloc(ntohs(addit[i].resource->data_len));
			for (j = 0; j < ntohs(addit[i].resource->data_len); j++)
				addit[i].rdata[j] = reader[j];

			addit[i].rdata[ntohs(addit[i].resource->data_len)] = '\0';
			reader += ntohs(addit[i].resource->data_len);
		}
		else
		{
			addit[i].rdata = libprivoxy_read_dns_name(reader, buf, &stop);
			reader += stop;
		}
	}

	//print answers
	for (i = 0; i < ntohs(dns->ans_count); i++)
	{
		if (ntohs(answers[i].resource->type) == 1) //IPv4 address
		{
			long *p;
			p = (long*)answers[i].rdata;
			a.sin_addr.s_addr = (*p); //working without ntohl
			if (result == INADDR_NONE)
			{
				result = a.sin_addr.s_addr;
				printf("%s has IPv4 address: %s\n", host, inet_ntoa(a.sin_addr));
				break;
			}
		}
	}

	return result;
}

/* this will convert www.google.com to 3www6google3com ;got it :) */
void libprivoxy_host_to_dns_format(char* dns, const char* host)
{
	int lock = 0, i;
	char host_dot[256];
	strcpy_s(host_dot, 256, host);
	strcat_s(host_dot, 256, ".");

	for (i = 0; i < (int)strlen(host_dot); i++)
	{
		if (host_dot[i] == '.')
		{
			*dns++ = i - lock;
			for (; lock < i; lock++)
			{
				*dns++ = host_dot[lock];
			}
			lock++; //or lock=i+1;
		}
	}

	*dns++ = '\0';
}

unsigned char* libprivoxy_read_dns_name(unsigned char* reader, unsigned char* buffer, int* count)
{
	unsigned char *name;
	unsigned int p = 0, jumped = 0, offset;
	int i, j;

	*count = 1;
	name = (unsigned char*)malloc(256);

	name[0] = '\0';

	//read the names in 3www6google3com format
	while (*reader != 0)
	{
		if (*reader >= 192)
		{
			offset = (*reader) * 256 + *(reader + 1) - 49152; //49152 = 11000000 00000000 ;)
			reader = buffer + offset - 1;
			jumped = 1; //we have jumped to another location so counting wont go up!
		}
		else
		{
			name[p++] = *reader;
		}

		reader = reader + 1;

		if (jumped == 0) *count = *count + 1; //if we havent jumped to another location then we can count up
	}

	name[p] = '\0'; //string complete
	if (jumped == 1)
	{
		*count = *count + 1; //number of steps we actually moved forward in the packet
	}

	//now convert 3www6google3com0 to www.google.com
	for (i = 0; i < (int)strlen(name); i++)
	{
		p = name[i];
		for (j = 0; j < (int)p; j++)
		{
			name[i] = name[i + 1];
			i = i + 1;
		}
		name[i] = '.';
	}

	name[i - 1] = '\0'; //remove the last dot

	return name;
}