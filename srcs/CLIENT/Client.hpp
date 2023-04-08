/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:40:42 by bducrocq          #+#    #+#             */
/*   Updated: 2023/03/29 00:40:43 by bducrocq         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>

class Client
{
	private:

		int _id;
		std::string _nick;
		std::string _realnick;
		bool _first_connection;
		bool _pass_confirm;

	public:
		Client();
		Client(int fd);
		Client( Client const & src );
		~Client();

		int get_id() const;
		std::string get_nick() const;
		std::string get_realnick() const;
		bool get_fisrt_connection() const;
		bool get_pass_confirm() const;

		void set_nick(std::string nick);
		void set_realnick(std::string nick);
		void set_first_connection(bool connect);
		void set_pass_confirm(bool pass);

		Client &		operator=( Client const & rhs );
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */