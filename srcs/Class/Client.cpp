/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:40:42 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/26 18:38:27 by bducrocq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client() :
_id(-1),
_nick("*"),
_username(""),
_realname(""),
_away_message("Is away"),
_is_reciving_server_notices(false),
_is_invisible(false),
_is_operator(false),
_is_wallops(false),
_is_away(false),
_is_auth(false),
_is_pass(false){}


Client::Client(int fd) :
_id(fd),
_nick("*"),
_username(""),
_realname(""),
_away_message("Is away"),
_is_reciving_server_notices(false),
_is_invisible(false),
_is_operator(false),
_is_wallops(false),
_is_away(false),
_is_auth(false),
_is_pass(false){}

Client::Client( const Client & src ) {
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client() {}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Client &				Client::operator=( Client const & rhs ) {
	if ( this != &rhs )
	{
		this->_id = rhs._id;
		this->_nick = rhs._nick;
		this->_username = rhs._username;
		this->_realname = rhs._realname;
		this->_away_message = rhs._away_message;
		this->_is_reciving_server_notices = rhs._is_reciving_server_notices;
		this->_is_invisible = rhs._is_invisible;
		this->_is_operator = rhs._is_operator;
		this->_is_wallops = rhs._is_wallops;
		this->_is_away = rhs._is_away;
		this->_is_auth = rhs._is_auth;
		this->_is_pass = rhs._is_pass;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Client const & i ) {
	o	<<"FD = "	<< i.get_id() << " | "
		<<"NICK = "	<< i.get_nick() << " | "
		<<"FIST_CONNECT = "	<< i.get_is_auth() << " | "
		<<"PASS = "	<< i.get_pass_confirm() << std::endl;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

/**
 * @brief set the client as authentified (NICK and USER command)
 * @param connect 
 */
void Client::now_auth(Server &serv) {
	this->_is_auth = true;

		Rep().R001(_id, _nick);
		Rep().R002(_id, _nick, string(SERVER_NAME), string(SERVER_VERSION));
		Rep().R003(_id, _nick, string(SERVER_DATE));
		serv.parser("MOTD", _id);
}

/**
 * @brief set the client has validate password (PASS command)
 * @param connect 
 */
void Client::now_pass() {
	this->_is_pass = true;
}

bool Client::isOperatorInChannel(Channel &channel) const
{
	vector<int> chanOp = channel.getOperators();
	for (vector<int>::iterator it = chanOp.begin(); it != chanOp.end(); it++)
		if (*it == this->get_id())
			return true;
	return _is_operator;
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

set<char> Client::get_modes() const {
	return this->_modes;
}

string			Client::get_modes_str() const
{
	std::string modesStr;

	for (std::set<char>::const_iterator it = _modes.begin(); it != _modes.end(); ++it)
	{
		modesStr += *it;
	}

	return modesStr;
}

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
bool Client::get_pass_confirm() const {
	return this->_is_pass;
}

bool Client::get_is_auth() const {
	return this->_is_auth;
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

bool Client::get_mode_s() const {
	return this->_is_reciving_server_notices;
}

bool Client::get_mode_w() const {
	return this->_is_wallops;
}

string		Client::get_away_message() {
	return this->_away_message;
}


//######################### SETTER #########################

/**
 * @brief set the nick of the client
 * @param nick 
 */
void Client::set_nick(std::string nick) {
	this->_nick = nick;
}

void Client::set_is_pass(bool is_pass)
{
	this->_is_pass = is_pass;
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


void Client::set_mode_a() {
	_modes.insert('a');
	this->_is_away = true;
}

void Client::set_mode_i() {
	_modes.insert('i');
	this->_is_invisible = true;
}

void Client::set_mode_o() {
	_modes.insert('o');
	this->_is_operator = true;
	// cerr << ANSI::flash << "set mode o" << ANSI::reset << endl;
}

void Client::set_mode_s() {
	_modes.insert('s');
	this->_is_reciving_server_notices = true;
}

void Client::set_mode_w() {
	_modes.insert('w');
	this->_is_wallops = true;
}


//######################### UNSETTER #########################

void Client::unset_mode_a() {
	this->_is_away = false;
	_modes.erase('a');
}

void Client::unset_mode_i() {
	_modes.erase('i');
	this->_is_invisible = false;
}

void Client::unset_mode_o() {
	_modes.erase('o');
	this->_is_operator = false;
}

void Client::unset_mode_s() {
	_modes.erase('s');
	this->_is_reciving_server_notices = false;
}

void Client::unset_mode_w() {
	_modes.erase('w');
	this->_is_wallops = false;
}


/* ************************************************************************** */


/**
 * @brief 
 * 
                                     ---------------       ______
                                     --------------- ___//   \___\___
                                     ---------------|_            -    `|
                                     ---------------  `-(o)-------(o)-'
 * 
 */