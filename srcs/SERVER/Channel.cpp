#include "Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel()
{
}

#include "../UTILS/ANSI.hpp"

Channel::Channel(int fd_client, string const & name) :  requiredPass(false), _name(name), _mode('=')
{
	// setPasswd("42");
	_list[fd_client] = make_pair('@', vector<string>());
}

//En faite a priori on ne peux pas creer de channel tout en definissant un passwd
// Channel::Channel(int fd_client, string const & name, string const & passwd ) : requiredPass(false), _name(name), _mode('=')//TODO: DBG
// {
// 	_list[fd_client] = make_pair('@', vector<string>());
// }


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel()
{
	
}

string Channel::ListNick(Server &serv, int fd_client)
{
	string list;
	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
	{
		string msg = ":" + serv.get_client()[fd_client].get_nick() + "!" + serv.get_client()[fd_client].get_username() + "@" + string(SERVER_NAME) + " JOIN :" + _name + "\r\n";
		if (send((*it).first, msg.c_str(), msg.length(), 0) == -1)
			serv.send_error(fd_client);
		string prefix = ((*it).second.first == '@') ? "@" : ((*it).second.first == '+') ? "+" : "";
		list += prefix + serv.get_client()[(*it).first].get_nick() + " ";

		cout << ANSI::gray << "{send} => " << ANSI::purple << msg << endl;
	}
	return list;
}


map<int, pair<char, vector<string> > >	&	Channel::getList()
{
	return _list;
}

string	Channel::list_all_nick(map<int, Client> & _client)
{
	string list;
	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
	{
		string prefix = ((*it).second.first == '@') ? "@" : ((*it).second.first == '+') ? "+" : "";
		list += prefix + _client[(*it).first].get_nick() + " ";
	}
	return list;
}

/**
 * @brief get the mode of a client in the channel, if the client is not in the channel, return '0'
 * @param fd_client 
 * @return 
 */
char	Channel::getClientMode(int fd_client)
{
	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
		if ((*it).first == fd_client)
			return (*it).second.first;
	return '0';
}

vector<int>			Channel::getOperators()
{
	vector<int>		operator_list;

	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
		if ((*it).second.first == '@')
			operator_list.push_back((*it).first);
	return operator_list;
}

void	Channel::addClient(int fd_client, char mode)
{
	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
		if ((*it).first == fd_client)
			return;
	_list[fd_client] = make_pair(mode, vector<string>());
}

void					Channel::removeClient(int fd_client)
{
	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if ((*it).first == fd_client)
		{
			_list.erase((*it).first);
			break;
		}
	}
}

void	Channel::ClientLeave(int fd_client, Server &serv, string const & msg, bool isQuit)
{
	string ret = ":" + serv.get_client()[fd_client].get_nick() + "!" + serv.get_client()[fd_client].get_username() + "@" + string(SERVER_NAME);
	if (isQuit)
		ret = " QUIT " + _name + " " + msg + "\r\n";
	else
		ret = " PART " + _name + " " + msg + "\r\n";
	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if (send((*it).first, ret.c_str(), ret.length(), 0) == -1)
			serv.send_error(fd_client);
		cout << ANSI::gray << "{send} => " << ANSI::purple << ret << endl;
	}
	this->_list.erase(fd_client);
}

void Channel::removeOperator(int fd_client)
{
	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if ((*it).first == fd_client)
		{
			(*it).second.first = ' ';
			break;
		}
	}
}

void					Channel::setTopic(string const & topic)
{
	_topic = topic;
}

string					Channel::getTopic()
{
	return _topic;
}
string					Channel::getName()
{
	return _name;
}

string					Channel::getPasswd()
{
	return _passwd;
}

char 					Channel::getMode()
{
	return _mode;
}

bool 					Channel::isInviteOnly()
{
	return (_mode == 'i');
}

vector<int>	Channel::getBlackList()
{
	return this->_blackList;
}

void 					Channel::setPasswd(string const & passwd)
{
	_passwd = passwd;
}

void 					Channel::setMode(char const & mode)
{
	_mode = mode;
}

int					Channel::getNbClient()
{
	return _list.size();
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */