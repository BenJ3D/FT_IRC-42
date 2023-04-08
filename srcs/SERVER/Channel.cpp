#include "Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel()
{
}

Channel::Channel(int fd_client, string const & name, Client client) : _name(name)
{
	if(find(_list.begin(), _list.end(), fd_client) == _list.end())
	{
		_list.push_back(fd_client);// suprression si listClient ok
		_listClient.push_back(client);//WIP
	}
}

Channel::Channel(int fd_client, string const & name, Client client, string const & passwd ) : _name(name), _passwd(passwd)
{
	if(find(_list.begin(), _list.end(), fd_client) == _list.end())
	{
		_list.push_back(fd_client);// suprression si listClient ok
		_listClient.push_back(client);//WIP
	}
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel()
{
}

vector<int>				Channel::getList()
{
	return _list;
}

vector<int>				Channel::getOperator()
{
	return _operator;
}

void					Channel::addOperator(int fd_client)
{
	_operator.push_back(fd_client);
}

void 					Channel::addClient(int fd_client)
{
	if (find(_list.begin(), _list.end(), fd_client) == _list.end())
		_list.push_back(fd_client);
}

void					Channel::removeClient(int fd_client)
{
	for (vector<int>::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if ((*it) == fd_client)
		{
			_list.erase(it);
			break;
		}
	}
}

void					Channel::removeOperator(int fd_client)
{
	for (vector<int>::iterator it = _operator.begin(); it != _operator.end(); it++)
	{
		if ((*it) == fd_client)
		{
			_operator.erase(it);
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

void 					Channel::setPasswd(string const & passwd)
{
	_passwd = passwd;
}

void 					Channel::setMode(string const & mode)
{
	_mode = mode;
}

string const &			Channel::getListStr()
{
	stringstream ss;
	if (_listClient.size() == 0)
		return "";
	for (vector<Client>::iterator it = _listClient.begin(); it != _listClient.end(); it++)
	{
		ss << it->get_nick() << " ";
	}
	return ss.str();
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