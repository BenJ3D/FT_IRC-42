/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:40:42 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/11 04:30:07 by bducrocq         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include "../SERVER/Server.hpp"

class Channel;
class Client
{
	private:

		int _id;
		std::string _nick;
		std::string _username;
		std::string _realname;
		bool _is_auth;
		bool _pass_confirm;

	public:
		Client();
		Client(int fd);
		Client( Client const & src );
		~Client();

		int get_id() const;
		std::string get_nick() const;
		std::string get_username() const;
		std::string get_realname() const;
		bool get_is_auth() const;
		bool get_pass_confirm() const;
		int	find_user_by_nick(std::string nick, std::map<int, Client> & clients);

		void set_nick(std::string nick);
		void set_username(std::string nick);
		void set_realname(std::string nick);

		void now_auth();
		void password_verified();

		bool isOperatorInChannel(Channel & channel) const; // delete if getMod OK

		Client &		operator=( Client const & rhs );
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */