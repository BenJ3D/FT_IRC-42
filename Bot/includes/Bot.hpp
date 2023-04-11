/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 06:32:18 by amiguez           #+#    #+#             */
/*   Updated: 2023/04/10 08:33:45 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdlib.h>

# include "ANSI.hpp"

# define USAGE "./Bob [<ip_address> [port]]"

using namespace std;

class Bot{
	private :
		Bot(const Bot& src);
		Bot operator=(const Bot& rhs);

		int _server_fd;

	public :
		void init(string ip = "127.0.0.1", string port = "6667") throw(exception);
		Bot();
		~Bot();

};

#endif