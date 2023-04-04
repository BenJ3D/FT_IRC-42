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
using namespace std;

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

Server::Server(string port, string address)
{
	(void)address;
	openSocket(atoi(port.c_str())); //fonction TEST pour le moment
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

ostream &			operator<<( ostream & o, Server const & i )
{
	(void)i;
	//o << "Value = " << i.getValue();
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

int Server::send_message(int client_fd, const char *message) const
{
	int msg_len = strlen(message);
	if (msg_len >=1)
	{
		if (send(client_fd, message, msg_len, 0) == -1)
			return -1;
		return 0;
	}
	cerr << "Buffer is empty, can't send message" << endl;
	return 0;
}

int Server::openSocket(int port)
{
	// création du socket en ipv4 -> AF_IFNET
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (server_fd == -1)
	{
		cerr << "Erreur lors de la création du socket" << endl;
		return 1;
	}

	// configuration de l'adresse et du port
	sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));

	server_address.sin_addr.s_addr = inet_addr("0.0.0.0"); //defini l'ip du serveur
	server_address.sin_family = AF_INET; //defini le protocole d'ip (IPV4)
	server_address.sin_port = htons(port); //defini le port du serveur

	// association du socket à l'adresse et au port
	if (bind(server_fd, (sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
		cerr << "Erreur lors de la mise en écoute des connexions entrantes1" << endl;
		return 1;
	}

	// mise en écoute des connexions entrantes
	if (listen(server_fd, SOMAXCONN) == -1)
	{
		cerr << "Erreur lors de la mise en écoute des connexions entrantes" << endl;
		return 1;
	}

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
			// if (map<int, Client>::const_iterator(_clients.find(client_fd)) == _clients.end())
			// {
			// 	cerr << "Send Welcome To New User" << endl;
			// 	send(client_fd, ":Welcome to my IRC server.", bytes_received, 0);
			// }
			//send(client_fd, ":127.0.0.1 001 root\r\n:Welcome to my IRC server\r\n", bytes_received, 0);
			// string user_string(buffer);
			// size_t user_pos = user_string.find("NICK ");
			// if (user_pos != string::npos)
			// {
			// 	cout << "User pos for NICKNAME :" << user_pos << endl;
			// 	user_pos += 5; // Skip "USER "
			// 	size_t end_pos = user_string.find(" ", user_pos);
			// 	if (end_pos != string::npos)
			// 	{
			// 		string username = user_string.substr(user_pos, end_pos - user_pos);
			// 		_clients[client_fd].Change_Nick(username);
			// 		string out = ":IRC 001 " + username + "\r\n:Welcome to my IRC server, " + username + "!\r\n";
			// 		if (send(client_fd, out.c_str(), strlen(out.c_str()), 0) == -1)
			// 		{
			// 			cerr << "Erreur lors de l'envoi des données au serveur distant" << endl;
			// 			break;
			// 		}
			// 	}
			// }
			// else
			// {
			// 	char ret[] = ":127.0.0.1 001 root\r\n:Welcome to my IRC server.";
			// 	send(client_fd, ret, strlen(ret), 0);
			// }
		}
	}

	return (0);
}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */