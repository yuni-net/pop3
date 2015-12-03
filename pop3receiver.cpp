#include "pop3receiver.h"
#include "Pop3Receiver_class.h"

int * connect_server(const char * mail_address, const char * password)
{
	auto * receiver = new Pop3Receiver();
	receiver->connect(mail_address, password);
	return reinterpret_cast<int *>(receiver);
}

int get_subject_quantity(int * handle)
{
	Pop3Receiver & receiver = *reinterpret_cast<Pop3Receiver *>(handle);
	return receiver.get_subject_quantity();
}

void get_subject(int * handle, char * subject, int index)
{
	Pop3Receiver & receiver = *reinterpret_cast<Pop3Receiver *>(handle);
	receiver.get_subject(subject, index);
}

void get_content(int * handle, char * content, int index)
{
	Pop3Receiver & receiver = *reinterpret_cast<Pop3Receiver *>(handle);
	receiver.get_content(content, index);
}

void finish(int * handle)
{
	Pop3Receiver & receiver = *reinterpret_cast<Pop3Receiver *>(handle);
	delete &receiver;
}
