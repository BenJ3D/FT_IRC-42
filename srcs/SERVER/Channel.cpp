#include "Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel()
{
}

Channel::Channel(int fd_client, string const & name, Client client) : _name(name), _mode('=')
{
	_list[fd_client] = '@';
}

Channel::Channel(int fd_client, string const & name, Client client, string const & passwd ) : _name(name), _passwd(passwd), _mode('=')
{
	_list[fd_client] = '@';
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel()
{
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
	_list[fd_client] = mode;
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