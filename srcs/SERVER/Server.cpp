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
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::Server(std::string port, std::string address)
{

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
	return *this;
}

void Server::init(void)
{
}

std::ostream &			operator<<( std::ostream & o, Server const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

int Server::openSocket( int port )
{
	// création du socket
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (server_fd == -1) {
		 std::cerr << "Erreur lors de la création du socket" << std::endl;
		 return 1;
	}

	// configuration de l'adresse et du port
	sockaddr_in server_address;
	std::memset(&server_address, 0, sizeof(server_address));

	server_address.sin_addr.s_addr = inet_addr("0.0.0.0");
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);

	// association du socket à l'adresse et au port
	if (bind(server_fd, (sockaddr*)&server_address, sizeof(server_address)) == -1)
	{
		std::cerr << "Erreur lors de la mise en écoute des connexions entrantes1" << std::endl;
		return 1;
	}

 	 // mise en écoute des connexions entrantes
    if (listen(server_fd, SOMAXCONN) == -1) {
        std::cerr << "Erreur lors de la mise en écoute des connexions entrantes2" << std::endl;
        return 1;
    }
	
	// accepter une connexion entrante
    sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);

    int client_fd = accept(server_fd, (sockaddr*)&client_address, &client_address_size);

    if (client_fd == -1) {
        std::cerr << "Erreur lors de l'acceptation de la connexion entrante" << std::endl;
        return 1;
    }


while (true) {
    char buffer[1024]= {0};
    int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
	std::cout << buffer << std::endl;
    if (bytes_received == -1) {
        std::cerr << "Erreur lors de la réception des données" << std::endl;
        break;
    } else if (bytes_received == 0) {
        std::cerr << "Connexion fermée par le client" << std::endl;
        break;
    } else {
       // envoyer les données reçues vers le socket distant
		if (send(client_fd, ":127.0.0.1 001 bducrocq :Welcome to my IRC server, bducrocq!\r\n", 64, 0) == -1) {
            std::cerr << "Erreur lors de l'envoi des données au serveur distant" << std::endl;
            break;
        }
    }
}

}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */