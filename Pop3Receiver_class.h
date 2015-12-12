#pragma once
#include <string>
#include <fstream>

#ifndef for_include_winsock2_h_
#define for_include_winsock2_h_
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
#endif

class Pop3Receiver
{
public:
	void connect(const char * mail_address, const char * password);
	int get_subject_quantity();
	void get_subject(char * subject, int index);
	void get_content(char * content, int index);

	~Pop3Receiver();
private:
	SOCKET sock;
	int mail_quantity;

	bool get_status();
	void download_mail(int index, std::string & buffer);
};