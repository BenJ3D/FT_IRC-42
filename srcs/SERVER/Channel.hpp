#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <algorithm>
# include "Server.hpp"
# include "../CLIENT/Client.hpp"

class Client;

using namespace std;
class Channel
{

	public:

		Channel();
		Channel( int fd_client, string const & name, Client client);
		Channel( int fd_client, string const & name, Client client, string const & passwd );
		~Channel();

		map<int, char> 								getList();
		vector<int>									getOperators();
		void										addClient(int fd_client, char mode);
		void										removeClient(int fd_client);
		void										removeOperator(int fd_client);
		void										setTopic(string const & topic);
		string										getTopic();
		string										getName();
		void										setPasswd(string const & passwd);
		string										getPasswd();
		void										setMode(char const & mode);  // = for public | * for private | @ for secret
		char										getMode();

	private:
		
		bool					isOperator(int fd_client);

		string				_passwd;
		string				_topic;
		string		 		_name;
		vector<int>			_blackList; 
		map<int, char>		_list; //OLD 
		char				_mode;
};

#endif /* ********************************************************* CHANNEL_H */