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

		void				addClient(int fd_client, char mode);
		void				removeClient(int fd_client);

		void				addOperator(int fd_client);
		void				removeOperator(int fd_client);

		void				addBlackList(int fd_client);
		void				removeBlackList(int fd_client);

		bool				isInviteOnly();

		void				setInviteOnly(bool const & inviteOnly);
		void				setModerated(bool const & moderated);
		void				setLimit(int const & limit);
		void				setTopic(string const & topic);
		void				setPasswd(string const & passwd);
		void				setMode(char const & mode);  // = for public | * for private | @ for secret

		vector<int>			getBlackList();
		map<int, char>		getList();
		vector<int>			getOperators();
		string				getPasswd();
		string				getTopic();
		string				getName();
		char				getMode();

		string ListNick(map<int, Client> & clients);

	private:
		
		bool					isOperator(int fd_client);

		string				_passwd;
		string				_topic;
		string		 		_name;
		vector<int>			_blackList; // fd_client
		map<int, char>		_list; // fd_client, mode 
		char				_mode;
		bool 				_inviteOnly;
		bool 				_moderated;
		int 				_limit;
};

#endif /* ********************************************************* CHANNEL_H */