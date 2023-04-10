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

		Channel( int fd_client, string const & name, Client client);
		Channel( int fd_client, string const & name, Client client, string const & passwd );
		~Channel();

		vector<int>		const &		getList();
		string const &			getListStr();
		vector<int>				getOperator();
		void					addClient(int fd_client);
		void					removeClient(int fd_client);
		void					addOperator(int fd_client);
		void					removeOperator(int fd_client);
		void					setTopic(string const & topic);
		string					getTopic();
		string					getName();
		void					setPasswd(string const & passwd);
		string					getPasswd();
		void					setMode(string const & mode);

	private:
		Channel();
		
		bool					isOperator(int fd_client);
		

		string					_passwd;
		string					_topic;
		string		 			_name;
		vector<int>				_blackList; 
		vector<int>				_list; //OLD 
		vector<int>				_operator;
		string					_mode;
};

#endif /* ********************************************************* CHANNEL_H */