#include "./Server.hpp"

int Server::start_server(int port) {
	// création du socket en ipv4 -> AF_IFNET
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (server_fd == -1)
	{
		cerr << "Erreur lors de la création du socket" << endl;
		return EXIT_FAILURE;
	}

	// configuration de l'adresse et du port
	sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));

	server_address.sin_addr.s_addr = INADDR_ANY; //defini l'ip du serveur
	server_address.sin_family = AF_INET; //defini le protocole d'ip (IPV4)
	server_address.sin_port = htons(port); //defini le port du serveur

	// association du socket à l'adresse et au port
	if (bind(server_fd, (sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
		cerr << "Erreur lors de la mise en écoute des connexions entrantes1" << endl;
		return EXIT_FAILURE;
	}

	// mise en écoute des connexions entrantes
	if (listen(server_fd, SOMAXCONN) == -1)
	{
		cerr << "Erreur lors de la mise en écoute des connexions entrantes" << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}