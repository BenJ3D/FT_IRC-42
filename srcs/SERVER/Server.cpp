/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:12:30 by bducrocq          #+#    #+#             */
/*   Updated: 2023/03/29 00:45:44 by bducrocq         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server(int port)
{
}

Server::Server( const Server & src )
{
	(void)src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::Server(string port, string address)
{
	(void)address;
	init(atoi(port.c_str())); //fonction TEST pour le moment
}

Server::~Server()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Server &				Server::operator=( Server const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	(void)rhs;
	return *this;
}

void Server::init(int port)
{
	if (start_server(port))
		/*...*/
}

ostream &			operator<<( ostream & o, Server const & i )
{
	(void)i;
	//o << "Value = " << i.getValue();
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

int Server::openSocket()
{
	

	// accepter une connexion entrante
	sockaddr_in client_address;
	socklen_t client_address_size = sizeof(client_address);
	//while CONNECTION CLIENT
	while (true)
	{
		int client_fd = accept(server_fd, (sockaddr *)&client_address, &client_address_size);

		cout << "test" << endl;
		if (client_fd == -1)
		{
			cerr << "Erreur lors de l'acceptation de la connexion entrante" << endl;
			return 1;
		}
		cerr << "////////////////////\nClient fd : " << client_fd << "\n////////////////////\n" << endl;
		_clients[client_fd] = Client();

		char buffer[1024] = {0};
		int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
		cout << "---####DEBUG####---\n";
		cout << buffer;
		cout << "---#############---\n" << endl;
		
		//cout << map<int, Client>::const_iterator(_clients.find(client_fd)) << endl;

		if (bytes_received == -1)
		{
			cerr << "Erreur lors de la réception des données" << endl;
			break;
		}
		else if (bytes_received == 0)
		{
			cerr << "Connexion fermée par le client" << endl;
			continue;
		}
		else
		{
			string msg(":127.0.0.1 001 username\r\n:Welcome to my IRC server!\r\n");
			if (send_message(client_fd, msg.c_str()) == -1)
			{
				cerr << "Erreur lors de l'envoi des données au serveur distant" << endl;
				break;
			}
		}
	}
	return (0);
}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */