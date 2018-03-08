#ifndef _LIBPRIVOXYEXT_H_
#define _LIBPRIVOXYEXT_H_

//DNS header structure
struct DNS_HEADER
{
	unsigned short id; // identification number

	unsigned char rd : 1; // recursion desired
	unsigned char tc : 1; // truncated message
	unsigned char aa : 1; // authoritive answer
	unsigned char opcode : 4; // purpose of message
	unsigned char qr : 1; // query/response flag

	unsigned char rcode : 4; // response code
	unsigned char cd : 1; // checking disabled
	unsigned char ad : 1; // authenticated data
	unsigned char z : 1; // its z! reserved
	unsigned char ra : 1; // recursion available

	unsigned short q_count; // number of question entries
	unsigned short ans_count; // number of answer entries
	unsigned short auth_count; // number of authority entries
	unsigned short add_count; // number of resource entries
};

//Constant sized fields of query structure
struct QUESTION
{
	unsigned short qtype;
	unsigned short qclass;
};

//Constant sized fields of the resource record structure
#pragma pack(push, 1)
struct R_DATA
{
	unsigned short type;
	unsigned short _class;
	unsigned int ttl;
	unsigned short data_len;
};
#pragma pack(pop)

//Pointers to resource record contents
struct RES_RECORD
{
	unsigned char *name;
	struct R_DATA *resource;
	unsigned char *rdata;
};

//Structure of a Query
typedef struct
{
	unsigned char *name;
	struct QUESTION *ques;
} QUERY;

//List of DNS Servers registered on the system
const char **dns_servers;
int dns_servers_count;

#ifdef WIN32
	int	gettimeofday(struct timeval *tp, void *tzp);
#endif

	void libprivoxy_exit(int _Code);

	/* this will convert www.google.com to 3www6google3com ;got it :) */
	void libprivoxy_host_to_dns_format(char* dns, const char* host);

	unsigned char* libprivoxy_read_dns_name(unsigned char* reader, unsigned char* buffer, int* count);

	unsigned long libprivoxy_resolve_hostname_to_ip(const char *host);
#endif