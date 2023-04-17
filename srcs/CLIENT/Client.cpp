/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:40:42 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/16 23:51:35 by bducrocq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client() : _pass_confirm(false) {
}

Client::Client(int fd) : _id(fd), _nick("*"), _is_auth(false), _pass_confirm(false), _is_away(false), _away_message("is away")
{
}

Client::Client( const Client & src ){
	this->_id = src._id;
	this->_nick = src._nick;
	this->_pass_confirm = src._pass_confirm;
	this->_is_auth = src._is_auth;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client() {
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

bool Client::isOperatorInChannel(Channel &channel) const
{
	vector<int> chanOp = channel.getOperators();
	for (vector<int>::iterator it = chanOp.begin(); it != chanOp.end(); it++)
		if (*it == this->get_id())
			return true;
	return false;
}

/**
 * @brief return the client id if the nick is found in the clients map, else return -1
 * @param nick 
 * @param clients 
 * @return 
 */
int Client::find_user_by_nick(std::string nick, std::map<int, Client> &clients)
{
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if ((*it).second.get_nick() == nick)
			return (*it).first;
	}
	return -1;
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

/**
 * @brief set the client password verified (PASS command)
 * @param connect 
 */
bool Client::get_pass() const {
	return this->_pass_confirm;
}

bool Client::get_is_auth() const {
	return this->_is_auth;
}

string Client::get_away_message() const {
	return this->_away_message;
}

void Client::unset_mode_a() {
	this->_is_away = false;
}

void Client::unset_mode_i() {
	this->_is_invisible = false;
}

void Client::unset_mode_o() {
	this->_is_operator = false;
}

// void Client::unset_mode_O() {
// 	this->_is_reciving_server_notices = false;
// }

void Client::unset_mode_s() {
	this->_is_reciving_server_notices = false;
}

void Client::unset_mode_w() {
	this->_is_wallops = false;
}

bool Client::get_mode_a() const {
	return this->_is_away;
}

bool Client::get_mode_i() const {
	return this->_is_invisible;
}

bool Client::get_mode_o() const {
	return this->_is_operator;
}

// bool Client::get_mode_O() const
// {
// 	return false;
// }

bool Client::get_mode_s() const {
	return this->_is_reciving_server_notices;
}

bool Client::get_mode_w() const {
	return this->_is_wallops;
}





//######################### SETTER #########################

void Client::set_mode_a() {
	this->_is_away = true;
}

void Client::set_mode_i() {
	this->_is_invisible = true;
}

void Client::set_mode_o() {
	this->_is_operator = true;
}

// void Client::set_mode_O() {
// 	this->_is_reciving_server_notices = true;
// }

void Client::set_mode_s() {
	this->_is_reciving_server_notices = true;
}

void Client::set_mode_w() {
	this->_is_wallops = true;
}

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