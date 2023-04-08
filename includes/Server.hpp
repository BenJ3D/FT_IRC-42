/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:12:30 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/08 03:17:32 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <stdlib.h>
# include <string>
# include <cstring>
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <poll.h>

# include "ANSI.hpp"
# include "Client.hpp"

# include <map>

class Server
{

	public:

		Server( std::string port, std::string address);
		~Server();

		Server &		operator=( Server const & rhs );

		void	init( void );


	private:

		Server( Server const & src );
		Server( void );
		
		int		openSocket( int port );

		std::map<sockaddr_in, int>		_clients;
		struct pollfd					_pollfd;


};

std::ostream &			operator<<( std::ostream & o, Server const & i );



# define COLOR_RED		"\033[0;31m"
# define COLOR_GREEN	"\033[0;32m"
# define COLOR_YELLOW	"\033[0;33m"
# define COLOR_BLUE		"\033[0;34m"
# define COLOR_PURPLE	"\033[0;35m"
# define COLOR_CYAN		"\033[0;36m"
# define COLOR_NONE		"\033[0;37m"
#endif /* ********************************************************* SERVIRC_H */