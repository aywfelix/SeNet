#pragma once

#include "SocketDefine.h"
#include "common/CommonDefines.h"

class Socket
{
public:

	void SetSocket(socket_t sock, struct sockaddr_in& addr);
	void CreateFd();
	void CloseSocket();

	// server
	bool Listen(uint32 port);
	bool Accept(socket_t& connfd, struct sockaddr_in& addr);
	// client
	bool Connect(const char* ip, uint32 port);

	int Send(const char* buf, int len);
	int Recv(char* buf, int len);

	// udp
	void CreateUDPFd();
	int RecvFrom(char* buf, int len);
	int SendTo(const char* buf, int len, const struct sockaddr& addr);
	void SetBroadCast();

	socket_t GetFd() { return m_fd; }
	void SetSocketOptions();
private:
	void BindAddr(struct sockaddr_in& addr, uint32 port, const char* ip = "127.0.0.1");
	void SetNonBlock();
	// set socket options
	void SetReUseAddr();
	void SetNodelay();
	void SetKeepAlive(uint32 interval = 15);  // 15 seconds
	void SetBufferSize(uint32 send_size, uint32 recv_size);
private:
	const char* m_ip;
	uint32 m_port;
	socket_t m_fd;
	struct sockaddr_in m_sockAddr;
};
