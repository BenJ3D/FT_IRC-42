#include "Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel()
{
}

#include "../UTILS/ANSI.hpp"

Channel::Channel(int fd_client, string const & name) : _name(name), _mode('=')
{
	_list.insert(pair<int, char>(fd_client, '@'));
}

Channel::Channel(int fd_client, string const & name, string const & passwd ) : _name(name), _passwd(passwd), _mode('=')
{
	_list.insert(pair<int, char>(fd_client, '@'));
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel()
{
}

string Channel::ListNick(map<int, Client> & clients, int fd_client)
{
	string list;
	for (map<int, char>::iterator it = _list.begin(); it != _list.end(); it++)
	{
		string msg = ":" + clients[fd_client].get_nick() + "!" + clients[fd_client].get_username() + "@" + string(SERVER_NAME) + " JOIN" + _name + "\r\n";
		if (send(fd_client, msg.c_str(), msg.length(), 0) == -1)
			cerr << ANSI::red << "Erreur lors de l'envoi des données au client" << endl;
		string prefix = "";
		if ((*it).second == '@')
			prefix = "@";
		else if ((*it).second == '+')
			prefix = "+";
		list += prefix + clients[(*it).first].get_nick() + " ";
	}
	return list;
}

map<int, char>		Channel::getList()
{
	return _list;
}

vector<int>			Channel::getOperators()
{
	vector<int>		operator_list;

	for (map<int, char>::iterator it = _list.begin(); it != _list.end(); it++)
		if ((*it).second == '@')
			operator_list.push_back((*it).first);
	return operator_list;
}

void	Channel::addClient(int fd_client, char mode)
{
	for (map<int, char>::iterator it = _list.begin(); it != _list.end(); it++)
		if ((*it).first == fd_client)
			return;
	_list.insert(pair<int, char>(fd_client, mode));
}

void					Channel::removeClient(int fd_client)
{
	for (map<int, char>::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if ((*it).first == fd_client)
		{
			_list.erase((*it).first);
			break;
		}
	}
}

void					Channel::removeOperator(int fd_client)
{
	for (map<int, char>::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if ((*it).first == fd_client)
		{
			(*it).second = ' ';
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