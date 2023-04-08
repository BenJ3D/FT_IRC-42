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

Client::Client(int fd) : _id(fd), _first_connection(true), _pass_confirm(false)
{
	
}

Client::Client( const Client & src )
{
	this->_id = src._id;
	this->_nick = src._nick;
	this->_first_connection = src._first_connection;
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
		this->_first_connection = rhs._first_connection;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Client const & i )
{
	o << "FD = " << i.get_id() << 
	" | NICK = " << i.get_nick() << 
	" | FIST_CONNECT = " << i.get_fisrt_connection() << 
	" | PASS_CONFIRMED = " << i.get_pass_confirm() << std::endl;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
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

bool Client::get_fisrt_connection() const {
	return this->_first_connection;
}

bool Client::get_pass_confirm() const {
	return this->_pass_confirm;
}



void Client::set_nick(std::string nick) {
	this->_nick = nick;
}

void Client::set_realnick(std::string nick) {
	this->_realnick = nick;
}

void Client::set_first_connection(bool connect) {
	this->_first_connection = connect;
}

void Client::set_pass_confirm(bool pass) {
	this->_pass_confirm = pass;
}

/* ************************************************************************** */