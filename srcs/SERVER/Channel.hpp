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
		Channel( int fd_client, string const & name);
		// Channel( int fd_client, string const & name, string const & passwd );
		~Channel();

		void										addClient(int fd_client, char mode);
		void										removeClient(int fd_client);

		void										addOperator(int fd_client);
		void										removeOperator(int fd_client);

		void										addBlackList(int fd_client);
		void										removeBlackList(int fd_client);


		void										setInviteOnly(bool const & inviteOnly);
		void										setModerated(bool const & moderated);
		void										setLimit(int const & limit);
		void										setTopic(string const & topic);
		void										setPasswd(string const & passwd);
		void										setVisibilityMode(char const & mode);  // = for public | * for private | @ for secret
		bool										isInviteOnly( void );
		int											getNbClient( void );
		void										setOwner(int fd_client);


		vector<int>									getBlackList();
		map<int, pair<char, vector<string> > >	&	getList();
		vector<int>									getOperators();
		string										getPasswd();
		string										getTopic();
		string										getName();
		set<char>									getModes();
		string										getModesStr();
		char										getVisibilityMode();
		// char										getClientMode(int fd_client);
		string										ListNick(map<int, Client>& clients, int fd_client);
		int											getOwner();

		// string const &								getMode();


	private:
		
		bool										isOperator(int fd_client);
		bool										_requiredPass;

		bool 										_isInviteOnly;
		bool 										_isModerated;
		string		 								_name;
		string										_passwd;
		string										_topic;
		int											_topicClientSetter; //save fd du client qui a set le topic
		vector<int>									_blackList; // fd_client
		vector<int>									_inviteList; // fd_client -- permet dinviter le client sans besoin de passwd (si mode +k), valable pour une connection
		map<int, pair<char, vector<string> > >		_list; // fd_client, mode, flags
		int 										_limit;
		char										_visibilityMode; // = for public | * for private | @ for secret
		set<char>									_modes;
		int											_owner;
};

#endif /* ********************************************************* CHANNEL_H */