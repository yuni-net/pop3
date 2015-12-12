#include "Pop3Receiver_class.h"
#include "Receiver.h"
#include <stdio.h>
#include "Base64Coder.h"

#pragma warning(push)
#pragma warning(disable:4996)

void Pop3Receiver::connect(const char * mail_address, const char * password)
{
	Receiver receiver;

	SOCKADDR_IN ServAddr;
	u_short ServPort = 110;
	char servIP[256] = "192.168.120.11";
	int totalBytesRcvd = 0;	// 受信バイト数

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// TCPソケット作成
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		printf("ソケット生成失敗\n");
	}

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(servIP);
	ServAddr.sin_port = htons(ServPort);

	// 接続
	if (::connect(sock, (PSOCKADDR)&ServAddr,
		sizeof(ServAddr)) == SOCKET_ERROR) {
		printf("接続エラー\n");
		getchar();
		return;
	}

	receiver.receive_line(sock);
	receiver.show();

	get_status();


}

int Pop3Receiver::get_subject_quantity()
{
	return mail_quantity;
}

void Pop3Receiver::get_subject(char * subject, int index)
{

	std::string buffer;
	download_mail(index, buffer);

	std::string subject_beg = strstr(buffer.c_str(), "Subject: ") + strlen("Subject: ");
	size_t subject_end = reinterpret_cast<size_t>(strstr(subject_beg.c_str(), "\r\n"));
	size_t subject_group_len = subject_end - reinterpret_cast<size_t>(subject_beg.c_str());
	std::string subject_group = subject_beg.substr(0, subject_group_len);
	if (subject_group.substr(0, 2) == "=?")
	{
		// this is base64 encoded
		size_t first_quest = subject_group.find_first_of("?", 2);
		std::string subject_with_quest2 = strstr(subject_group.c_str() + 2, "?");
		std::string subject_only = strstr(subject_with_quest2.c_str() + 1, "?") + 1;
		Base64Coder decoder;
		decoder.Decode(subject_only.c_str());
		strcpy(subject, decoder.DecodedMessage());
	}
	else
	{
		/*
		std::ofstream ofs;
		ofs.open("log.txt", std::ios::trunc);
		ofs << "received:\r\n" << buffer<<"\r\n\r\n";
		ofs << "subject_beg:\r\n"<<subject_beg << "\r\n\r\n";
		ofs << "subject_group:\r\n" << subject_group << "\r\n\r\n";
		ofs.close();
		//*/
		strcpy(subject, subject_group.c_str());
	}

}

void Pop3Receiver::get_content(char * content, int index)
{
	std::string buffer;
	download_mail(index, buffer);

	std::ofstream ofs;
	ofs.open("log.txt", std::ios::trunc);
	ofs << buffer;

	/*
	const char * near_html_p = buffer.c_str();
	while (true)
	{
		const char * backup = near_html_p;
		near_html_p = strstr(near_html_p, "text/html;") + strlen("text/html;");
		if (near_html_p == NULL)
		{
			near_html_p = backup;
			break;
		}
	}
	std::string near_html = near_html_p;

	bool is_64 = strstr(near_html.c_str(), "Content-Transfer-Encoding: base64") != NULL;
	std::string html_with_trash = strstr(near_html.c_str(), "\r\n\r\n") + strlen("\r\n\r\n");
	const char * trash = strstr(html_with_trash.c_str(), "\r\n--");
	int html_len = trash - html_with_trash.c_str();
	std::string html = html_with_trash.substr(0, html_len);
	if (is_64 == true)
	{
		// this is base64 encoded
		Base64Coder decoder;
		decoder.Decode(html.c_str());
		strcpy(content, decoder.DecodedMessage());
	}
	else
	{
		strcpy(content, html.c_str());
	}
	//*/

	strcpy(content, buffer.c_str());
}


Pop3Receiver::~Pop3Receiver()
{
	shutdown(sock, SD_BOTH);
	closesocket(sock);

	WSACleanup();
}




bool Pop3Receiver::get_status()
{
	Receiver receiver;

	char * user = "USER nhs30070";
	printf("%s\n", user);
	char command_user[1024];
	sprintf(command_user, "%s%s", user, "\r\n");
	send(sock, command_user, strlen(command_user), 0);

	receiver.receive_line(sock);
	receiver.show();

	char * pass = "PASS d19941005";
	printf("%s\n", pass);
	char command_pass[1024];
	sprintf(command_pass, "%s%s", pass, "\r\n");
	send(sock, command_pass, strlen(command_pass), 0);

	receiver.receive_line(sock);
	receiver.show();

	puts("user name and password is ok.\n");

	char * stat = "STAT";
	printf("%s\n", stat);
	char command_stat[1024];
	sprintf(command_stat, "%s%s", stat, "\r\n");
	send(sock, command_stat, strlen(command_stat), 0);

	receiver.receive_line(sock);
	receiver.show();

	int damy;
	sscanf(receiver.get_text(), "+OK %d %d", &mail_quantity, &damy);

	return true;
}

void Pop3Receiver::download_mail(int index, std::string & buffer)
{
	char request[1024];
	sprintf(request, "RETR %u\r\n", index + 1);
	send(sock, request, strlen(request), 0);

	Receiver receiver;
	receiver.receive_line(sock);
	receiver.show();
	const char * reply = receiver.get_text();
	if (memcmp(reply, "+OK", 3) != 0)
	{
		puts("メッセージの受信に失敗しました。\n");
		return;
	}

	while (true)
	{
		receiver.receive_line(sock);
		buffer.append(receiver.get_text());
		const char * text = receiver.get_text();
		const int length = strlen(text);
		const char * end3 = text + length - 3;
		if (memcmp(end3, ".\r\n", 3) == 0)
		{
			return;
		}
	}
}

#pragma warning(pop)