#pragma once
#ifndef for_include_winsock2_h_
#define for_include_winsock2_h_
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
#endif

class Receiver
{
public:
	void receive_line(SOCKET & sock);
	void show();
	const char * get_text() const;

private:
	static const int buff_bytes = 2048;
	char buffer[buff_bytes];
};

