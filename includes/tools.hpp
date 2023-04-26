#ifndef TOOLS_HPP
# define TOOLS_HPP
# include "Server.hpp"
	using namespace std;

	void confirm_to_client(const int &fd, string msg, Server &serv);
	void confirm_to_all_channel_client(int const &fd, string msg, Server &serv, Channel chan);
	void confirm_to_all_channel_client_mode_o(int const &fd, string msg, Server &serv, Channel chan);

	vector<string> split_sep(string const & str, char separator);
	vector<string> split_cmd(const string command, char separator);

	int send_to_user(string msg, int cl_fd);

#endif