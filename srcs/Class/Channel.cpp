#include "Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel()
{
}

#include "ANSI.hpp"

Channel::Channel(int fd_client, string const & name, Server &refServer) :
_requiredPass(false),
_name(name),
_visibilityMode('='),
_isInviteOnly(false),
_isModerated(false),
_limit(10),
_topic(""),
_refServ(&refServer)
{
	_list[fd_client] = make_pair('@', vector<string>());
	setOwner(fd_client);
	// setPasswd("42");
	_list[fd_client] = make_pair('@', vector<string>());
	setOwner(fd_client);
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel(){}

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

bool Channel::isOperator(int fd_client)
{
	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
		if ((*it).first == fd_client && (*it).second.first == '@')
			return true;
	return false;
}

map<int, pair<char, vector<string> > > & Channel::getList()
{
	return _list;
}

string Channel::list_all_nick(map<int, Client> & _client)
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
char Channel::getClientMode(int fd_client)
{
	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
		if ((*it).first == fd_client)
			return (*it).second.first;
	return '0';
}

vector<int> Channel::getOperators()
{
	vector<int>		operator_list;

	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
		if ((*it).second.first == '@')
			operator_list.push_back((*it).first);
	return operator_list;
}

void Channel::addClient(int fd_client, char mode)
{
	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
		if ((*it).first == fd_client)
			return;
	_list[fd_client] = make_pair(mode, vector<string>());
}

void Channel::removeClient(int fd_client)
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

/**
 * @brief Confirm to everyone that fd_client have leaved the chan
 * 
 * @param isQuit true if cmd == QUIT 
 * @param isQuit false if cmd == PART
 */
void	Channel::ClientLeave(int fd_client, Server &serv, string const & msg, bool isQuit)
{
	string ret ;
	// = ":" + serv.get_client()[fd_client].get_nick() + "!" + serv.get_client()[fd_client].get_username() + "@" + string(SERVER_NAME);
	if (isQuit)
		ret = " QUIT " + msg + "\r\n";
	else
		ret = " PART " + _name + " " + msg + "\r\n";
	// for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
	// {
	// 	if (send((*it).first, ret.c_str(), ret.length(), 0) == -1)
	// 		serv.send_error(fd_client);
	// 	cout << ANSI::gray << "{send} => " << ANSI::purple << ret << endl;
	// }
	confirm_to_all_channel_client(fd_client, ret, serv, *this);
	this->_list.erase(fd_client);
}

void Channel::addOperator(int fd_client, Server &serv, string target_nick) //TODO : check if cli
{
	//if client deja dans le channel ? sinon return une erreur IRC:
	cerr << "addOperator HELLLOOO" << endl;
	if (_list.find(serv.findClientFdWithNick(target_nick)) == _list.end())
		return;
	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if ((*it).first == serv.findClientFdWithNick(target_nick))
		{
			(*it).second.first = '@';
			confirm_to_all_channel_client_mode_o(fd_client, " MODE " + _name + " +o " + target_nick + "\r\n", serv, *this);
			break;
		}
	}
}

void Channel::removeOperator(int fd_client, Server &serv, string target_nick)
{
	if (_list.find(serv.findClientFdWithNick(target_nick)) == _list.end())
		return;
	for (map<int, pair<char, vector<string> > >::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if ((*it).first == serv.findClientFdWithNick(target_nick))
		{
			(*it).second.first = ' ';
			confirm_to_all_channel_client_mode_o(fd_client, " MODE " + _name + " -o " + target_nick + "\r\n", serv, *this);
			break;
		}
	}
}

void Channel::addBlackList(int target ,int fd_client)
{
	_blackList.push_back(target);
	if (_list.find(target) != _list.end())
	{
		// removeClient(fd_client); //TODO: chopper kick de server
		_refServ->parser("KICK "+ _name + " " + _refServ->get_client()[target].get_nick() + " :User got blacklisted :P", fd_client);
	}
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

void Channel::setTopicClientSetter(string const &msg)
{
	_topicClientSetter = msg;
}

string Channel::getTopicClientSetter()
{
	return _topicClientSetter;
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
	return this->_isInviteOnly;
}

vector<int>	Channel::getBlackList()
{
	return this->_blackList;
}

vector<int> Channel::getInviteList()
{
	return this->_inviteList;
}



void	Channel::setPasswd(string const & passwd)
{
	_passwd = passwd;
}

void Channel::setVisibilityMode(char const &mode)
{
	_visibilityMode = mode;
}

int					Channel::getNumberClientInChannel()
{
	return _list.size();
}

void Channel::setOwner(int fd_client)
{
	_owner = fd_client;
}

bool Channel::isClientInInviteList(int fd_client)
{
	cerr << ANSI::red << "isClientInInviteList start" << ANSI::reset << endl;
	for (vector<int>::iterator it = _inviteList.begin(); it != _inviteList.end(); it++)
		if ((*it) == fd_client)
			return true;
	return false;
}

bool Channel::isClientInBlackList(int fd_client)
{
	for (vector<int>::iterator it = _blackList.begin(); it != _blackList.end(); it++)
		if ((*it) == fd_client)
			return true;
	return false;
}

bool Channel::isClientInChannel(int fd_client)
{
	if (_list.find(fd_client) != _list.end())
		return true;
	return false;
}

bool Channel::isOperatorInChannel(int fd_client)
{
	vector<int>::const_iterator it = getOperators().begin();
	for (; it != getOperators().end(); it++)
	{
		if ((*it) == fd_client)
			return true;
	}
	if (_refServ->get_client().find(fd_client)->second.get_mode_o() == true)
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