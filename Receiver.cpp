#include "Receiver.h"
#include <stdio.h>

void Receiver::receive_line(SOCKET & sock)
{
	int total_bytes = 0;
	while (true)
	{
		total_bytes += recv(sock, buffer + total_bytes, buff_bytes - total_bytes - 1, 0);
		if (total_bytes < 2) continue;
		if (total_bytes >= buff_bytes - 1)
		{
			break;
		}
		if (buffer[total_bytes - 2] != '\r') continue;
		if (buffer[total_bytes - 1] != '\n') continue;
		break;
	}

	buffer[total_bytes] = 0;
}

void Receiver::show()
{
	puts(buffer);
}

const char * Receiver::get_text() const
{
	return buffer;
}
