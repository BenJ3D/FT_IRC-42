/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 01:27:56 by bducrocq          #+#    #+#             */
/*   Updated: 2023/03/22 01:29:25 by bducrocq         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"



int openSocket( int port )
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

	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);

	// association du socket à l'adresse et au port
	if (bind(server_fd, (sockaddr*)&server_address, sizeof(server_address)) == -1)
	{
		std::cerr << "Erreur lors de la mise en écoute des connexions entrantes" << std::endl;
		return 1;
	}

 	 // mise en écoute des connexions entrantes
    if (listen(server_fd, SOMAXCONN) == -1) {
        std::cerr << "Erreur lors de la mise en écoute des connexions entrantes" << std::endl;
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

//  // connexion à un serveur distant
//     int remote_fd = socket(AF_INET, SOCK_STREAM, 0);

//     if (remote_fd == -1) {
//         std::cerr << "Erreur lors de la création du socket" << std::endl;
//         return 1;
//     }

//     sockaddr_in remote_address;
//     std::memset(&remote_address, 0, sizeof(remote_address));

//     remote_address.sin_family = AF_INET;
//     remote_address.sin_addr.s_addr = inet_addr("irc.example.com"); // adresse du serveur IRC distant
//     remote_address.sin_port = htons(6667); // port IRC par défaut

//     if (connect(remote_fd, (sockaddr*)&remote_address, sizeof(remote_address)) == -1) {
//         std::cerr << "Erreur lors de la connexion au serveur distant" << std::endl;
//         return 1;
//     }

while (true) {
    char buffer[1024]= {0};
    int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
	std::cout << "DBG " << buffer << std::endl;
    if (bytes_received == -1) {
        std::cerr << "Erreur lors de la réception des données" << std::endl;
        break;
    } else if (bytes_received == 0) {
        std::cerr << "Connexion fermée par le client" << std::endl;
        break;
    } else {
        // envoyer les données reçues vers le socket distant
        if (send(client_fd, buffer, bytes_received, 0) == -1) {
            std::cerr << "Erreur lors de l'envoi des données au serveur distant" << std::endl;
            break;
        }
    }
}

}


int	main( int ac, char **av)
{

	std::cout << "Hello World!" << std::endl;

	if (av[1])
		openSocket(atoi(av[1]));
	else 
		openSocket(6667);
	return 0;
}