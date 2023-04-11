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
	_list[fd_client] = make_pair('@', vector<string>());
}

Channel::Channel(int fd_client, string const & name, string const & passwd ) : _name(name), _passwd(passwd), _mode('=')
{
	_list[fd_client] = make_pair('@', vector<string>());
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

map<int, pair<char, vector<string> > >		Channel::getList()
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

void					Channel::removeOperator(int fd_client)
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