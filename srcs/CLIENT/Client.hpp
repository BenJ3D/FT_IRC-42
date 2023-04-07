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

	public:
		Client();
		Client(int fd);
		Client( Client const & src );
		~Client();

		const int get_id() const;
		const std::string get_nick() const;

		void set_nick(std::string nick);

		Client &		operator=( Client const & rhs );
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */