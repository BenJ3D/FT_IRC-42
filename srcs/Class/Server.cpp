/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:12:30 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/26 22:29:59 by bducrocq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ANSI.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{}

Server::Server(const Server &src) {
	// this->_channel = src._channel;
	this->_client = src._client;
	this->_max_fd = src._max_fd;
	this->_read_fds = src._read_fds;
	this->_server_name = src._server_name;
	this->_delete_client = false;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::Server(string port, string password) : _pass_word(password)
{

		//DBG SEGFAUT *******/

		// vector<string> test;
		// test.push_back("JOIN");
		// test.push_back("#1,#2,#");
		// test.push_back("44,42,");
		// join(test, 4);
		// join(test, 5);
		// cerr << ANSI::red << "FINNNNNNNN DEBUG" << endl << ANSI::reset;
	
		//DBG SEGFAUT *******/
		
	init_parsing_map();
	config();
	openSocket(atoi(port.c_str())); // fonction TEST pour le moment
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server()
{
	for (map<int, Client>::iterator it = _client.begin(); it != _client.end(); it++)
	{
		close((*it).first);
		_client.erase(it);
	}
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Server &Server::operator=(Server const &rhs)
{
	(void)rhs;
	return *this;
}

ostream &operator<<(ostream &o, Server const &i)
{
	(void)i;
	// o << "Value = " << i.getValue(); // FIXME: 
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	Server::send_error(int fd_client)
{
	this->_delete_client = true;
	vector<string> res;
	res.push_back("QUIT :");
	if (_client.find(fd_client) != _client.end())
		(this->*commands["QUIT"].second)(res, fd_client);
	else
		cerr << ANSI::red << "ERROR: Undefined Client" << endl;
}

int Server::openSocket(int port)
{
	// création du socket
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (server_fd == -1)
	{
		cerr << ANSI::red << "Erreur lors de la création du socket" << endl;
		return 1;
	}

	// configuration de l'adresse et du port
	sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));

	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);

	// association du socket à l'adresse et au port
	if (bind(server_fd, (sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
		cerr << ANSI::red << "Erreur lors de l'association du socket à l'adresse et au port" << endl;
		return 1;
	}

	// mise en écoute des connexions entrantes
	if (listen(server_fd, SOMAXCONN) == -1)
	{
		cerr << ANSI::red << "Erreur lors de la mise en écoute des connexions entrantes" << endl;
		return 1;
	}

	fd_set _read_fds;
	int _max_fd;
	int new_client_fd;
	sockaddr_in client_address;
	socklen_t client_address_size = sizeof(client_address);
	int bytes_received;

	while (true)
	{
		FD_ZERO(&_read_fds);
		FD_SET(server_fd, &_read_fds);
		_max_fd = server_fd;

		for (map<int, Client>::iterator it = _client.begin(); it != _client.end(); it++)
		{
			FD_SET((*it).first, &_read_fds);
			_max_fd = max(_max_fd, (*it).first);
		}

		// utilisation de la fonction select pour attendre des connexions entrantes ou des données reçues des clients existants
		if (select(_max_fd + 1, &_read_fds, NULL, NULL, NULL) == -1)
		{
			cerr << ANSI::red << "Erreur lors de l'utilisation de la fonction select" << endl;
			continue;
		}

		// vérification des connexions entrantes
		if (FD_ISSET(server_fd, &_read_fds))
		{
			new_client_fd = accept(server_fd, (sockaddr *)&client_address, &client_address_size);

			if (new_client_fd == -1)
			{
				cerr << ANSI::red << "Erreur lors de l'acceptation de la connexion entrante" << endl;
				continue;
			}
			_client[new_client_fd] = Client(new_client_fd);
			cout << ANSI::green << ANSI::bold << "Nouvelle connexion entrante sur le socket " << new_client_fd << ANSI::r << endl;
			if (send(new_client_fd, "", 0, MSG_CONFIRM) == -1)
			{
				cerr << ANSI::red << "Erreur lors de l'envoie du 1er msg" << endl;
				send_error(new_client_fd);
				continue;
			}
		}
		_delete_client = false;
		// vérification des données reçues des clients existants
		for (map<int, Client>::iterator it = _client.begin(); it != _client.end(); it++)
		{
			if (FD_ISSET((*it).first, &_read_fds))
			{
				char buffer[1024] = {0};
				bytes_received = recv((*it).first, buffer, 1024, 0);

				if (bytes_received == -1)
				{
					cerr << ANSI::red << "Erreur lors de la réception des données" << endl;
					close((*it).first);
					_client.erase(it);
					break;
				}
				else if (!bytes_received)
				{
					cout << ANSI::red << ANSI::bold << "Connexion fermée par le client n°" << it->first << ANSI::r << endl;
					send_error((*it).first);
					break;
				}
				else
				{
					string str_buff(buffer);
					cout << ANSI::back_green << " " << it->first << " " << ANSI::r << "=>" << ANSI::green << str_buff << ANSI::r ;
					if (str_buff == " " || str_buff == "\r\n" || str_buff == "\n" || str_buff.empty())
						continue;

					try {
						this->parser(str_buff, (*it).first);
					}
					catch (std::exception &ex) {
						send_error((*it).first);
						break;
					}
				}
			}
			if (this->_delete_client)
				break;
		} 
	}
}

/**
 * @brief recherche avec un nickname si le client existe et return son fd, sinon return 0
 * 
 * @param nick 
 * @return int 
 */
int	Server::findClientFdWithNick(string const &nick)
{
	map<int,Client>::iterator it = _client.begin();

	for(; it != _client.end(); ++it)
	{
		if (it->second.get_nick().compare(nick) == 0)
			return it->first;
	}
	return 0;
}


bool Server::isExistChannelName(string const &channelName)
{
	if (_channel.find(channelName) != _channel.end())
		return true;
	return false;
}

void	Server::config(){
	std::ifstream conf(".config", ios_base::in);
	string line;
	int pwd = 0;
	int motd = 0;
	int usr = 0;
	_motd = string();
	_oper_passw = string();

	while(getline(conf,line)){
		if (line.find("PASSWORD_OPER:") == 0){
			if (pwd == 0){
				_oper_passw = line.substr(14);
				pwd = 1;
			}
			else if (pwd == 1){
				pwd = 2;
				cout << ANSI::back_red << ANSI::black << "Error:" << ANSI::r << ANSI::red << " Multiple definition of PASSWORD_OPER (set password to empty)" << endl;
				_oper_passw = string();
			}
		}
		else if (line.find("USERNAME_OPER:") == 0){
			if (usr == 0){
				_oper_user = line.substr(14);
				usr = 1;
			}
			else if (usr == 1){
				usr = 2;
				cout << ANSI::back_red << ANSI::black << "Error:" << ANSI::r << ANSI::red << " Multiple definition of MOTD (set motd to empty)" << endl;
				_oper_user = string();
			}
		}
		else if (line.find("MOTD:") == 0){
			if (motd == 0){
				_motd = line.substr(5);
				motd = 1;
			}
			else if (motd == 1){
				motd = 2;
				cout << ANSI::back_red << ANSI::black << "Error:" << ANSI::r << ANSI::red << " Multiple definition of MOTD (set motd to empty)" << endl;
				_motd = string();
			}
		}
	}
	string passw_set("abcdefghijklmnopqrstuvwxyzABCDEFGHIJklMNOPQRSTUVWXYZ0123456789");
	for (size_t i = 0; i < _oper_passw.size(); i++){
		if (passw_set.find(_oper_passw[i]) == string::npos){
			cout << ANSI::back_red << ANSI::black << "Error:" << ANSI::r << ANSI::red << " Invalid char of PASSWORD_OPER (set motd to empty)" << endl;
			_oper_passw = string();
		}
	}
	for (size_t i = 0; i < _oper_user.size(); i++){
		if (passw_set.find(_oper_user[i]) == string::npos){
			cout << ANSI::back_red << ANSI::black << "Error:" << ANSI::r << ANSI::red << " Invalid char of PASSWORD_OPER (set motd to empty)" << endl;
			_oper_user = string();
		}
	}
	// cout << ANSI::green << "_operPassWord - = '" << _oper_passw << "'" << endl;
	// cout << ANSI::green << "_operUserName - = '" << _oper_user << "'" << endl;
	// cout << ANSI::green << "_motd - - - - - = '" << _motd << "'" << endl; 
		// FIXME: LOGS ?

}

//GETTER
map<int, Client> &Server::get_client()
{
	return _client;
}
map<string, Channel> &Server::get_channel()
{
	return _channel;
}

/* ************************************************************************** */
