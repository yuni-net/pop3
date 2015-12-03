#ifndef yuni_pop3_receiver_h_
#define yuni_pop3_receiver_h_

extern "C" {
	__declspec(dllexport) int * connect_server(const char * mail_address, const char * password);
	__declspec(dllexport) int get_subject_quantity(int * handle);
	__declspec(dllexport) void get_subject(int * handle, char * subject, int index);
	__declspec(dllexport) void get_content(int * handle, char * content, int index);
	__declspec(dllexport) void finish(int * handle);
}

#endif