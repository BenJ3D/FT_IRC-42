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

Server::Server()
{
}

Server::Server( const Server & src )
{
	(void)src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::Server(std::string port, std::string address)
{
	(void)address;
	openSocket(std::atoi(port.c_str())); //fonction TEST pour le moment
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

void Server::init(void)
{
}

std::ostream &			operator<<( std::ostream & o, Server const & i )
{
	(void)i;
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

int Server::openSocket(int port)
{
	// création du socket en ipv4 -> AF_IFNET
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (server_fd == -1)
	{
		std::cerr << "Erreur lors de la création du socket" << std::endl;
		return 1;
	}

	// configuration de l'adresse et du port
	sockaddr_in server_address;
	std::memset(&server_address, 0, sizeof(server_address));

	server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); //defini l'ip du serveur
	server_address.sin_family = AF_INET; //defini le protocole d'ip (IPV4)
	server_address.sin_port = htons(port); //defini le port du serveur

	// association du socket à l'adresse et au port
	if (bind(server_fd, (sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
		std::cerr << "Erreur lors de la mise en écoute des connexions entrantes1" << std::endl;
		return 1;
	}

	// mise en écoute des connexions entrantes
	if (listen(server_fd, SOMAXCONN) == -1)
	{
		std::cerr << "Erreur lors de la mise en écoute des connexions entrantes" << std::endl;
		return 1;
	}

	// accepter une connexion entrante
	sockaddr_in client_address;
	socklen_t client_address_size = sizeof(client_address);
	//while CONNECTION CLIENT
	while (true)
	{
		int client_fd = accept(server_fd, (sockaddr *)&client_address, &client_address_size);

		std::cout << "test" << std::endl;
		if (client_fd == -1)
		{
			std::cerr << "Erreur lors de l'acceptation de la connexion entrante" << std::endl;
			return 1;
		}

		_clients[client_fd] = Client();

		char buffer[512] = {0};
		int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
		std::cout << "DBG " << buffer << std::endl;
		if (bytes_received == -1)
		{
			std::cerr << "Erreur lors de la réception des données" << std::endl;
			break;
		}
		else if (bytes_received == 0)
		{
			std::cerr << "Connexion fermée par le client" << std::endl;
			break;
		}
		else
		{
			std::string user_string(buffer);
			size_t user_pos = user_string.find("USER ");
			if (user_pos != std::string::npos)
			{
				user_pos += 5; // Skip "USER "
				size_t end_pos = user_string.find(" ", user_pos);
				if (end_pos != std::string::npos)
				{
					std::string username = user_string.substr(user_pos, end_pos - user_pos);
					_clients[client_fd].Change_Nick(username);
					std::string out = ":servername 001 username\r\n:Welcome to my IRC server, " + username +"!\r\n";
					if (send(client_fd, out.c_str(), bytes_received, 0) == -1)
					{
						std::cerr << "Erreur lors de l'envoi des données au serveur distant" << std::endl;
						break;
					}
				}
			}
		}
	}
	
	return (0);
}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */