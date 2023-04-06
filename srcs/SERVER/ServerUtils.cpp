#include "./Server.hpp"

int Server::send_message(int client_fd, const char *message) const
{
	int msg_len = strlen(message);
	if (msg_len >=1)
	{
		if (send(client_fd, message, msg_len, 0) == -1)
			return -1;
		return 0;
	}
	cerr << "Buffer is empty, can't send message" << endl;
	return 0;
}