#include "Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel()
{
}

#include "../UTILS/ANSI.hpp"

Channel::Channel(int fd_client, string const & name) :  _requiredPass(false), _name(name), _visibilityMode('=')
{
	// setPasswd("42");
	_list[fd_client] = make_pair('@', vector<string>());
	setOwner(fd_client);
}

//En faite a priori on ne peux pas creer de channel tout en definissant un passwd
// Channel::Channel(int fd_client, string const & name, string const & passwd ) : requiredPass(false), _name(name), _modes('=')//TODO: DBG
// {
// 	_list[fd_client] = make_pair('@', vector<string>());
// }


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel()
{
}

string Channel::ListNick(map<int, Client> & clients, int fd_client)
{
	string list;
	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
	{
		string msg = ":" + clients[fd_client].get_nick() + "!" + clients[fd_client].get_username() + "@" + string(SERVER_NAME) + " JOIN :" + _name + "\r\n";
		if (send((*it).first, msg.c_str(), msg.length(), 0) == -1)
			cerr << ANSI::red << "Erreur lors de l'envoi des données au client" << endl;
		string prefix = ((*it).second.first == '@') ? "@" : ((*it).second.first == '+') ? "+" : "";
		list += prefix + clients[(*it).first].get_nick() + " ";

		cout << ANSI::gray << "{send} => " << ANSI::purple << msg << endl;
	}
	return list;
}

int Channel::getOwner() //TODO: check if working
{
	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
		if ((*it).second.first == '@')
			return (*it).first;
	return 0;
}

bool Channel::isOperator(int fd_client)
{
	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
		if ((*it).first == fd_client && (*it).second.first == '@')
			return true;
	return false;
}

map<int, pair<char, vector<string> > >	&	Channel::getList()
{
	return _list;
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

void Channel::addOperator(int fd_client) //TODO : check if cli
{
	//if client deja dans le channel ? sinon return une erreur IRC:
	if (_list.find(fd_client) == _list.end())
		return;
	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if ((*it).first == fd_client)
		{
			(*it).second.first = '@';
			//TODO: send to all client channel -->> ben!~ben@ipserver.net MODE #chan +o :nickname
			break;
		}
	}
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

void Channel::addBlackList(int fd_client)
{
	_blackList.push_back(fd_client);
	//TODO: si client deja dans le channel, le kick avec PART
}

void Channel::removeBlackList(int fd_client)
{
	for (vector<int>::iterator it = _blackList.begin(); it != _blackList.end(); it++)
	{
		if ((*it) == fd_client)
		{
			_blackList.erase(it);
			break;
		}
	}
}

void Channel::addInviteList(int fd_client)
{
	_inviteList.push_back(fd_client);
}

void Channel::removeInviteList(int fd_client)
{
	for (vector<int>::iterator it = _inviteList.begin(); it != _inviteList.end(); it++)
	{
		if ((*it) == fd_client)
		{
			_inviteList.erase(it);
			break;
		}
	}
}

void Channel::setInviteOnly(bool const &inviteOnly)
{
	_isInviteOnly = inviteOnly;
}

void Channel::setModerated(bool const &moderated)
{
	_isModerated = moderated;
}

void Channel::setLimit(int const &limit)
{
	_limit = limit;
}

void Channel::setTopic(string const &topic)
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

set<char> Channel::getModes()
{
	return set<char>();
}

string Channel::getModesStr()
{
	string modes;
	for (set<char>::iterator it = _modes.begin(); it != _modes.end(); it++)
		modes += (*it);
	return modes;
}

string					Channel::getPasswd()
{
	return _passwd;
}

char 					Channel::getVisibilityMode()
{
	return _visibilityMode;
}

bool 					Channel::isInviteOnly()
{
	set<char>::iterator it = _modes.find('i');
	if (it == _modes.end())
		return false;
	return true;
}

vector<int>	Channel::getBlackList()
{
	return this->_blackList;
}

vector<int> Channel::getInviteList()
{
	return this->_inviteList;
}



void 					Channel::setPasswd(string const & passwd)
{
	_passwd = passwd;
}

void Channel::setVisibilityMode(char const &mode)
{
	_visibilityMode = mode;
}

// void 					Channel::setMode(char const & mode)
// {
// 	_modes = mode;
// }

int					Channel::getNbClient()
{
	return _list.size();
}

void Channel::setOwner(int fd_client)
{
	_owner = fd_client;
}

bool Channel::isClientInInviteList(int fd_client)
{
	for (vector<int>::iterator it = _inviteList.begin(); it != _inviteList.end(); it++)
		if ((*it) == fd_client)
			return true;
	return false;
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