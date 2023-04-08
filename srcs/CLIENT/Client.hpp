/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:40:42 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/08 07:49:57 by amiguez          ###   ########.fr       */
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
		bool is_pass() const;
		bool get_fisrt_connection() const;

		void comfirm_password();
		void set_nick(std::string nick);
		void set_realnick(std::string nick);
		void set_first_connection(bool connect);

		Client &		operator=( Client const & rhs );
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */