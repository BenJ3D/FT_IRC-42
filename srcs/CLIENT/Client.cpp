/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:40:42 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/11 16:31:44 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client() {
}

Client::Client(int fd) : _id(fd), _nick("*"), _is_auth(false), _pass_confirm(false){ 
}

Client::Client( const Client & src ){
	this->_id = src._id;
	this->_nick = src._nick;
	this->_is_auth = src._is_auth;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client() {
	this->_pass_confirm = false;
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Client &				Client::operator=( Client const & rhs ) {
	if ( this != &rhs )
	{
		this->_id = rhs._id;
		this->_nick = rhs._nick;
		this->_is_auth = rhs._is_auth;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Client const & i ) {
	o << "FD = " << i.get_id() << 
	" | NICK = " << i.get_nick() << 
	" | FIST_CONNECT = " << i.get_is_auth() << 
	" | PASS = " << i.get_pass() << std::endl;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

/**
 * @brief set the client as authentified (NICK and USER command)
 * @param connect 
 */
void Client::now_auth() {
	this->_is_auth = true;
}

/**
 * @brief set the client password verified (PASS command)
 * @param connect 
 */
void Client::password_verified() {
	this->_pass_confirm = true;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

//######################### GETTER #########################

int Client::get_id() const {
	return this->_id;
}

std::string Client::get_nick() const {
	return this->_nick;
}

std::string Client::get_username() const {
	return this->_username;
}

std::string Client::get_realname() const {
	return this->_realname;
}


bool Client::get_pass() const {
	return (this->_pass_confirm);
}

bool Client::get_is_auth() const {
	return this->_is_auth;
}

//######################### SETTER #########################

/**
 * @brief set the nick of the client
 * @param nick 
 */
void Client::set_nick(std::string nick) {
	this->_nick = nick;
}

void Client::comfirm_password() {
	this->_pass_confirm = true;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/**
 * @brief set the username of the client
 * @param username 
 */
void Client::set_username(std::string username) {
	this->_username = username;
}

/**
 * @brief set the realname of the client
 * @param realname 
 */
void Client::set_realname(std::string realname) {
	this->_realname = realname;
}

/* ************************************************************************** */