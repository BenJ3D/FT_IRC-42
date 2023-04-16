/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:40:42 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/16 23:50:54 by bducrocq         ###   ########.fr       */
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

		int					_id;
		std::string			_nick;
		std::string			_username;
		std::string			_realname;
		bool				_is_auth;
		bool				_pass_confirm;
		bool				_is_operator; //o
		bool				_is_away; //a
		std::string			_away_message;
		bool				_is_invisible; //i
		bool				_is_wallops; //w
		bool				_is_reciving_server_notices; //s


	public:
		Client();
		Client(int fd);
		Client( Client const & src );
		~Client();

		int				get_id() const;
		std::string		get_nick() const;
		std::string		get_username() const;
		std::string		get_realname() const;
		bool			get_is_auth() const;
		bool			get_pass_confirm() const;
		string 			get_away_message() const;
		int				find_user_by_nick(std::string nick, std::map<int, Client> & clients);

		void			set_nick(std::string nick);
		void			set_username(std::string nick);
		void			set_realname(std::string nick);

		void			now_auth();
		void			password_verified();

		bool			isOperatorInChannel(Channel & channel) const; // delete if getMod OK
		void			set_mode_a();
		void			set_mode_i();
		void			set_mode_o();
		// void			set_mode_O();
		void			set_mode_s();
		void			set_mode_w();
		void			unset_mode_a();
		void			unset_mode_i();
		void			unset_mode_o();
		// void			unset_mode_O();
		void			unset_mode_s();
		void			unset_mode_w();
		bool			get_mode_a() const;
		bool			get_mode_i() const;
		bool			get_mode_o() const;
		// bool			get_mode_O() const;
		bool			get_mode_s() const;
		bool			get_mode_w() const;
		
		




		Client &		operator=( Client const & rhs );
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */