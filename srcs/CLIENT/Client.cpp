/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:40:42 by bducrocq          #+#    #+#             */
/*   Updated: 2023/03/29 00:40:42 by bducrocq         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client()
{
}

Client::Client(int fd) : _id(fd)
{
}

Client::Client( const Client & src )
{
	this->_id = src._id;
	this->_nick = src._nick;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Client &				Client::operator=( Client const & rhs )
{
	if ( this != &rhs )
	{
		this->_id = rhs._id;
		this->_nick = rhs._nick;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Client const & i )
{
	o << "FD = " << i.get_id() << " | NICK = " << i.get_nick() << std::endl;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

int Client::get_id() const {
	return this->_id;
}

std::string Client::get_nick() const {
	return this->_nick;
}

std::string Client::get_realnick() const {
	return this->_realnick;
}

void Client::set_nick(std::string nick) {
	this->_nick = nick;
}

void Client::set_realnick(std::string nick) {
	this->_realnick = nick;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */