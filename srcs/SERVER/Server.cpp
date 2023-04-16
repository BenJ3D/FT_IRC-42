/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:12:30 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/14 01:13:46 by bducrocq         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "../UTILS/ANSI.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{
}

Server::Server(const Server &src)
{
	// this->_channel = src._channel;
	this->_client = src._client;
	this->_max_fd = src._max_fd;
	this->_pass_word = src._pass_word;
	this->_read_fds = src._read_fds;
	this->_server_name = src._server_name;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::Server(string port, string address)
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
		
	(void)address;
	init_parsing_map();
	openSocket(atoi(port.c_str())); // fonction TEST pour le moment
}

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
	// if ( this != &rhs )
	//{
	// this->_value = rhs.getValue();
	//}
	return *this;
}

ostream &operator<<(ostream &o, Server const &i)
{
	(void)i;
	// o << "Value = " << i.getValue();
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

int Server::openSocket(int port)
{
	// création du socket
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (server_fd == -1)
	{
		cerr << "Erreur lors de la création du socket" << endl;
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
		cerr << "Erreur lors de l'association du socket à l'adresse et au port" << endl;
		return 1;
	}

	// mise en écoute des connexions entrantes
	if (listen(server_fd, SOMAXCONN) == -1)
	{
		cerr << "Erreur lors de la mise en écoute des connexions entrantes" << endl;
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
			cerr << "Erreur lors de l'utilisation de la fonction select" << endl;
			return 1;
		}

		// vérification des connexions entrantes
		if (FD_ISSET(server_fd, &_read_fds))
		{
			new_client_fd = accept(server_fd, (sockaddr *)&client_address, &client_address_size);

			if (new_client_fd == -1)
			{
				cerr << ANSI::red << "Erreur lors de l'acceptation de la connexion entrante" << endl;
				return 1;
			}
			_client[new_client_fd] = Client(new_client_fd);
			cout << ANSI::green << ANSI::bold << "Nouvelle connexion entrante sur le socket " << new_client_fd << endl;
			if (send(new_client_fd, "", 0, MSG_CONFIRM) == -1)
			{
				cerr << ANSI::red << "Erreur lors de l'envoi des données au client" << endl;
				return 1;
			}
		}
		// vérification des données reçues des clients existants
		for (map<int, Client>::iterator it = _client.begin(); it != _client.end(); it++)
		{
			if (FD_ISSET((*it).first, &_read_fds))
			{
				char buffer[1024] = {0};
				bytes_received = recv((*it).first, buffer, 1024, 0);

				if (bytes_received == -1)
				{
					cerr << "Erreur lors de la réception des données" << endl;
					close((*it).first);
					_client.erase(it);
					break;
				}
				else if (!bytes_received)
				{
					cout << ANSI::red << "Connexion fermée par le client n°" << (*it).first << endl;
					close((*it).first);
					_client.erase(it);
					break;
				}
				else
				{
					string str_buff(buffer);
					cout << ANSI::purple << "\n### Recv client " << (*it).first << " ###\n"
						 << ANSI::italic << str_buff.c_str() << ANSI::purple << "#####################\n"
						 << endl;
					if (str_buff == " " || str_buff == "\r\n" || str_buff == "\n" || str_buff.empty())
						continue;

					// parsing...
					this->parser(str_buff, (*it).first);
				}
			}
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

/* ************************************************************************** */
