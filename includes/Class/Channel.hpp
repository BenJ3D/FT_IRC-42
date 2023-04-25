#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <algorithm>
# include "Server.hpp"
# include "Client.hpp"

class Client;
class Server;

using namespace std;
class Channel
{
	public:

		Channel();
		Channel( int fd_client, string const & name, Server &refServer);
		~Channel();

		void										addClient(int fd_client, char mode);
		void										removeClient(int fd_client);
		void 										ClientLeave(int fd_client, Server &serv, string const & msg, bool isQuit);

		void										addOperator(int fd_client, Server &serv, string target_nick);
		void										removeOperator(int fd_client, Server &serv, string target_nick);

		void										addBlackList(int target ,int fd_client);
		void										removeBlackList(int fd_client);

		void										addInviteList(int fd_client);
		void										removeInviteList(int fd_client);

		void										setInviteOnly(bool const & inviteOnly);
		void										setModerated(bool const & moderated);
		void										setLimit(int const & limit);
		void										setTopic(string const & topic);
		void										setPasswd(string const & passwd);
		void										setVisibilityMode(char const & mode);  //== channel mode ==>> for public | * for private | @ for secret
		bool										isInviteOnly( void );
		void										setOwner(int fd_client);

		bool										isClientInInviteList(int fd_client);
		bool										isClientInBlackList(int fd_client);
		bool										isClientInChannel(int fd_client);
		bool										isOperatorInChannel(int fd_client);

		int											getNumberClientInChannel( void );
		vector<int>									getBlackList();
		vector<int>									getInviteList();
		map<int, pair<char, vector<string> > >	&	getList();
		vector<int>									getOperators();
		string										getPasswd();
		string										getTopic();
		string										getName();
		set<char>									getModes();
		string										getModesStr();
		char										getVisibilityMode(); //== channel mode
		char										getClientMode(int fd_client);
		string										ListNick(Server &serv, int fd_client);
		string										list_all_nick(map<int, Client>& clients);
		int											getOwner();
		string										getTopicClientSetter();
		void 										setTopicClientSetter(string const &msg);

		// string const &								getMode();


	private:
		string										_topicClientSetter; //save fd du client qui a set le topic
		bool										isOperator(int fd_client);
		bool										_requiredPass;
		string		 								_name;
		string										_passwd;
		// int											_topicClientSetter; //save fd du client qui a set le topic
		vector<int>									_blackList; // fd_client
		vector<int>									_inviteList; // fd_client -- permet dinviter le client sans besoin de passwd (si mode +k), valable pour une connection
		map<int, pair<char, vector<string> > >		_list; // fd_client, mode, flags
		char										_visibilityMode; // = for public | * for private | @ for secret
		bool 										_isInviteOnly;
		bool 										_isModerated;
		int 										_limit;
		set<char>									_modes;
		int											_owner;
		string										_topic;
		Server										*_refServ;
};

#endif /* ********************************************************* CHANNEL_H */