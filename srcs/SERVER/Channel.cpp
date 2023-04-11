#include "Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel()
{
}

#include "../UTILS/ANSI.hpp"

Channel::Channel(int fd_client, string const & name, Client client) : _name(name), _mode('=')
{
	_list.insert(pair<int, char>(fd_client, '@'));
	cout << ANSI::green << "Channel " << name << " created by " << client.get_nick() << endl;
}

Channel::Channel(int fd_client, string const & name, Client client, string const & passwd ) : _passwd(passwd), _name(name),  _mode('=')
{
	_list.insert(pair<int, char>(fd_client, '@'));
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel()
{
}

string Channel::ListNick(map<int, Client> & clients)
{
	string list;
	for (map<int, char>::iterator it = _list.begin(); it != _list.end(); it++)
	{
		confirm_to_client(clients[(*it).first].get_id(), "JOIN :" + _name, clients);
		if (clients[(*it).first].getClientModeInChannel(*this) == '@')
			list += "@" + clients[(*it).first].get_nick() + " ";
		else if (clients[(*it).first].getClientModeInChannel(*this) == '+')
			list += "+" + clients[(*it).first].get_nick() + " ";
		else
			list += clients[(*it).first].get_nick() + " ";
		// if (clients[(*it).first].isOperatorInChannel(*this)) //TODO add prefix @ or + if operator or bot
		// 	list += "@" + clients[(*it).first].get_nick() + " ";
		// else
		// 	list += clients[(*it).first].get_nick() + " ";
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

void 					Channel::addClient(int fd_client, char mode)
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

char Channel::getClientMode(int fd_client)
{
	for (map<int, char>::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if (it->first == fd_client)
			return it->second;
	}
	return '\0';
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