/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:40:42 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/26 08:17:32 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <set>
# include "Server.hpp"

using namespace std;
class Channel;
class Server;
class Client
{
	private:
		int					_id;
		string				_nick;
		string				_username;
		string				_realname;
		bool				_is_auth;
		bool				_is_pass;
		bool				_is_operator; //o
		bool				_is_away; //a
		string				_away_message;
		bool				_is_invisible; //i
		bool				_is_wallops; //w
		bool				_is_reciving_server_notices; //s
		set<char>			_modes;

	public:
		Client();
		Client(int fd);
		Client( Client const & src );
		~Client();


		int					get_id() const;
		string				get_nick() const;
		string				get_username() const;
		string				get_realname() const;
		bool				get_is_auth() const;
		bool				get_pass_confirm() const;
		int					find_user_by_nick(string nick, map<int, Client> & clients);

		void				set_nick(string nick);
		void				set_username(string nick);
		void				set_realname(string nick);

		bool				isOperatorInChannel(Channel & channel) const; // delete if getMod OK

		void				set_modes_str(string const &mode); // FIXME: delete ?
		void				set_modes(set<char> newMode);
		void				set_mode_a();
		void				set_mode_i();
		void				set_mode_o();
		void				set_mode_s();
		void				set_mode_w();

		// void				unset_mode(string const &mode);
		void				unset_mode_a();
		void				unset_mode_i();
		void				unset_mode_o();
		void				unset_mode_s();
		void				unset_mode_w();

		set<char>			get_modes() const;
		string				get_modes_str() const;
		bool				get_mode_a() const;
		bool				get_mode_i() const;
		bool				get_mode_o() const;
		bool				get_mode_s() const;
		bool				get_mode_w() const;
		
		void				now_auth(Server &serv);
		void				now_pass();

		string				get_away_message();
		void				set_is_pass(bool is_pass);
		


		Client &		operator=( Client const & rhs );
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */