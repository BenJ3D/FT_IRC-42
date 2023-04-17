/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 02:46:29 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/17 05:49:46 by bducrocq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::mode_client(vector<string> args, int fd_client)
{
	if (args[2][0] != '+' && args[2][0] != '-')
	{
		Rep().E501(fd_client, _client[fd_client].get_nick());
		return ;
	}
	bool mod = false;
	if(args[2][0] == '+')
		mod = true;
	set<char> newMode = _client[fd_client].get_modes();
	for(string::iterator it = args[2].begin(); it != args[2].end(); it++)
	{
		
		// cerr << ANSI::red << "mod =" + mod + *it << endl << ANSI::reset;
		switch (*it)
		{
		case '+': // away
			mod = true;
			break;
		case '-': // away
			mod = false;
			break;
		case 'a': // away
			if (mod)
			{
				newMode.insert('a');
				_client[fd_client].set_mode_a();
			}
			else
			{
				newMode.erase('a');
				_client[fd_client].unset_mode_a();
			}
			break;
		case 'i': // invisible
			if (mod)
			{
				newMode.insert('i');
				_client[fd_client].set_mode_i();
			}
			else
			{
				newMode.erase('i');
				_client[fd_client].unset_mode_i();
			}
			break;
		case 'o': // operator
			if (mod)
			{

			}
			else
			{
				
				newMode.erase('o');
				_client[fd_client].unset_mode_o();
			}
			break;
		case 's': // server notices
			if (mod)
			{
				newMode.insert('s');
				_client[fd_client].set_mode_s();
			}
			else
			{
				newMode.erase('s');
				_client[fd_client].unset_mode_s();
			}
			break;
		case 'w': // wallops
			if (mod)
			{
				newMode.insert('w');
				_client[fd_client].set_mode_w();
			}
			else
			{
				newMode.erase('w');
				_client[fd_client].unset_mode_w();
			}
			break;
		default: // error
			Rep().E472(fd_client, _client[fd_client].get_nick(), *it);
			break;
		}
		
	}
	Rep().R221(fd_client, _client[fd_client].get_nick(), _client[fd_client].get_modes_str());
}

void Server::mode_channel(vector<string> args, int fd_client)
{
	(void)args;
	(void)fd_client;
}

void Server::mode(vector<string> args, int fd_client) 
{
	cout << ANSI::cyan << fd_client << " --> " << args[0] << endl;
	if (args.size() == 2)
	{
		if (args[1][0] == '#')
		{
			if (!isExistChannelName(args[1]))
				return Rep().E403(fd_client, _client[fd_client].get_nick(), args[1]);
			// Rep().R324(fd_client, _client[fd_client].get_nick(), args[1], _channel[args[1]].get_modes_str(), ""); //TODO:
		}
	}

	if (args.size() < 3)
		return Rep().E461(fd_client, _client[fd_client].get_nick(), args[0]);
	if (args[1][0] == '#')
	{
		if (!isExistChannelName(args[1]))
			return Rep().E403(fd_client, _client[fd_client].get_nick(), args[1]);
		mode_channel(args, fd_client);
	}
	else
	{
		if(_client[fd_client].get_nick().compare(args[1]) != 0)
			return Rep().E502(fd_client, _client[fd_client].get_nick());
		mode_client(args, fd_client);
	}
}

// void Server::mode(vector<string> args, int fd_client) {
// 	cout << ANSI::cyan << fd_client << " --> " << args[0] << endl;

// 	if (args.size() < 3)
// 		return Rep().E461(fd_client, _client[fd_client].get_nick(), args[0]);
	
// 	if (args[1][0] != '#' || _channel.find(args[1]) == _channel.end())
// 		return Rep().E403(fd_client, _client[fd_client].get_nick(), args[1]);
	
// 	string check = "ov";
// 	if (string("+-").find(args[2][0]) == string::npos && args[2].length() < 2)
// 		return Rep().E472(fd_client, _client[fd_client].get_nick(), args[2][0]);
// 	size_t i = 1;
// 	for (; i < args[2].length(); i++)
// 		if (check.find(args[2][i]) == string::npos)
// 			return Rep().E472(fd_client, _client[fd_client].get_nick(), args[2][i]);
// 	if (i != args.size() - 2)
// 		return Rep().E461(fd_client, _client[fd_client].get_nick(), args[0]); // OU 401 ?

// 	for (size_t i = 1; i < args[2].length(); i++)
// 	{
// 		if (args[2][i] == 'o')
// 		{
// 			if (args[2][0] == '+')
// 				_channel[args[1]].addClient(fd_client, '@');
// 			else if (args[2][0] == '-')
// 				_channel[args[1]].addClient(fd_client, ' ');
// 		}
// 		else if (args[2][i] == 'v')
// 		{
// 			if (args[2][0] == '+')
// 				_channel[args[1]].addClient(fd_client, '+');
// 			else if (args[2][0] == '-')
// 				_channel[args[1]].addClient(fd_client, ' ');
// 		}
// 	}
// }
/*
:bducrocq!bducrocq@host JOIN #test
:bducrocq!bducrocq@host MODE #test +o bducrocq
:irc.server.com 353 bducrocq = #test :nickname1 nickname2 nickname3
:irc.server.com 366 bducrocq  #test :End of /NAMES list.
*/

/***
 * 3.1.5 User mode message

      Command: MODE
   Parameters: <nickname>
               *( ( "+" / "-" ) *( "i" / "w" / "o" / "O" / "r" ) )

   The user MODE's are typically changes which affect either how the
   client is seen by others or what 'extra' messages the client is sent.

   A user MODE command MUST only be accepted if both the sender of the
   message and the nickname given as a parameter are both the same.  If
   no other parameter is given, then the server will return the current
   settings for the nick.

      The available modes are as follows:

           a - user is flagged as away;    aioOs
           i - marks a users as invisible;
           w - user receives wallops;
           r - restricted user connection;
           o - operator flag;
           O - local operator flag;
           s - marks a user for receipt of server notices.

   Additional modes may be available later on.
 * 
 * Voici une explication détaillée de chaque mode utilisateur :

a - Away (Absent) :
L'utilisateur est marqué comme étant absent. Cela signifie qu'ils sont temporairement 
indisponibles ou inactifs sur le serveur. Les autres utilisateurs seront informés de 
leur absence lorsqu'ils tenteront de contacter l'utilisateur marqué comme absent.

i - Invisible :
L'utilisateur est marqué comme invisible. Cela signifie qu'ils sont cachés des listes 
d'utilisateurs et des requêtes publiques (telles que la commande /who), rendant leur 
présence sur le serveur moins visible pour les autres.

w - Wallops :
L'utilisateur reçoit des messages "wallops". Les wallops sont des messages d'information 
générale envoyés par les opérateurs du serveur à tous les utilisateurs ayant ce mode activé. 
Ces messages peuvent contenir des annonces importantes, des mises à jour du serveur 
ou des informations sur les problèmes en cours.

r - Restricted user connection (Utilisateur restreint) :
L'utilisateur est connecté avec des restrictions. Cela signifie qu'ils ont des fonctionnalités
 ou des commandes limitées sur le serveur, généralement en raison d'un abus présumé du protocole 
 IRC ou d'une violation des règles du serveur.

o - Operator flag (Opérateur) :
L'utilisateur a le statut d'opérateur. Les opérateurs ont des privilèges supplémentaires 
pour gérer et modérer le serveur et les canaux IRC. Ils peuvent, par exemple, kicker ou bannir
 des utilisateurs, modifier les modes de canal et effectuer des actions administratives sur le serveur.

O - Local operator flag (Opérateur local) :
L'utilisateur a le statut d'opérateur local. Les opérateurs locaux ont des privilèges 
similaires aux opérateurs globaux, mais leurs permissions sont limitées à un serveur 
spécifique au lieu de l'ensemble du réseau IRC.

s - Server notices (Avis du serveur) :
L'utilisateur est marqué pour recevoir des avis de serveur. Les avis de serveur sont des
 messages d'information envoyés par le serveur concernant son état, les erreurs ou les problèmes. 
 Les utilisateurs avec ce mode activé recevront ces messages pour les aider à comprendre les 
 événements qui se produisent sur le serveur.

Ces modes utilisateur peuvent être activés ou désactivés en utilisant la commande /mode suivie 
du nom d'utilisateur, du signe + ou - pour ajouter ou supprimer le mode, et du mode correspondant 
(par exemple, /mode JohnDoe +i pour marquer l'utilisateur JohnDoe comme invisible).
 * 
 * 
 * Voici les numéros de réponses numériques IRC correspondant aux messages suivants :

ERR_NEEDMOREPARAMS : 461
ERR_USERSDONTMATCH : 502
ERR_UMODEUNKNOWNFLAG : 501
RPL_UMODEIS : 221
Ces numéros de réponse sont également définis dans la spécification IRC RFC 1459. Voici une brève 
explication de ces messages :

ERR_NEEDMOREPARAMS (461) : ce message est renvoyé par le serveur IRC lorsqu'une commande est 
reçue avec un nombre insuffisant de paramètres.
ERR_USERSDONTMATCH (502) : ce message est renvoyé par le serveur IRC lorsqu'une tentative de 
changement de mode utilisateur échoue car l'utilisateur spécifié n'est pas le même que celui 
qui a émis la commande.
ERR_UMODEUNKNOWNFLAG (501) : ce message est renvoyé par le serveur IRC lorsqu'une commande pour
 modifier le mode utilisateur est reçue avec un drapeau inconnu.
RPL_UMODEIS (221) : ce message est renvoyé par le serveur IRC en réponse à la commande MODE lorsque 
l'utilisateur demande ses propres modes.
 * 
 * 
 * 
 * 
 * 
 * 
 * 3.2.3 Channel mode message

      Command: MODE
   Parameters: <channel> *( ( "-" / "+" ) *<modes> *<modeparams> )

   The MODE command is provided so that users may query and change the
   characteristics of a channel.  For more details on available modes
   and their uses, see "Internet Relay Chat: Channel Management" [IRC-
   CHAN].  Note that there is a maximum limit of three (3) changes per
   command for modes that take a parameter.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_KEYSET
           ERR_NOCHANMODES                 ERR_CHANOPRIVSNEEDED
           ERR_USERNOTINCHANNEL            ERR_UNKNOWNMODE
           RPL_CHANNELMODEIS
           RPL_BANLIST                     RPL_ENDOFBANLIST
           RPL_EXCEPTLIST                  RPL_ENDOFEXCEPTLIST
           RPL_INVITELIST                  RPL_ENDOFINVITELIST
           RPL_UNIQOPIS

ERR_NEEDMOREPARAMS : 461
ERR_KEYSET : 467
ERR_NOCHANMODES : 477
ERR_CHANOPRIVSNEEDED : 482
ERR_USERNOTINCHANNEL : 441
ERR_UNKNOWNMODE : 472
RPL_CHANNELMODEIS : 324
RPL_BANLIST : 367
RPL_ENDOFBANLIST : 368
RPL_EXCEPTLIST : 348
RPL_ENDOFEXCEPTLIST : 349
RPL_INVITELIST : 346
RPL_ENDOFINVITELIST : 347
RPL_UNIQOPIS : 325

   The following examples are given to help understanding the syntax of
   the MODE command, but refer to modes defined in "Internet Relay Chat:
   Channel Management" [IRC-CHAN].

   Examples:

   MODE #Finnish +imI *!*@*.fi     ; Command to make #Finnish channel
                                   moderated and 'invite-only' with user
                                   with a hostname matching *.fi
                                   automatically invited.

   MODE #Finnish +o Kilroy         ; Command to give 'chanop' privileges
                                   to Kilroy on channel #Finnish.

   MODE #Finnish +v Wiz            ; Command to allow WiZ to speak on
                                   #Finnish.

   MODE #Fins -s                   ; Command to remove 'secret' flag
                                   from channel #Fins.

   MODE #42 +k oulu                ; Command to set the channel key to
                                   "oulu".

   MODE #42 -k oulu                ; Command to remove the "oulu"
                                   channel key on channel "#42".




Kalt                         Informational                     [Page 18]

RFC 2812          Internet Relay Chat: Client Protocol        April 2000


   MODE #eu-opers +l 10            ; Command to set the limit for the
                                   number of users on channel
                                   "#eu-opers" to 10.

   :WiZ!jto@tolsun.oulu.fi MODE #eu-opers -l
                                   ; User "WiZ" removing the limit for
                                   the number of users on channel "#eu-
                                   opers".

   MODE &oulu +b                   ; Command to list ban masks set for
                                   the channel "&oulu".

   MODE &oulu +b *!*@*             ; Command to prevent all users from
                                   joining.

   MODE &oulu +b *!*@*.edu +e *!*@*.bu.edu
                                   ; Command to prevent any user from a
                                   hostname matching *.edu from joining,
                                   except if matching *.bu.edu

   MODE #bu +be *!*@*.edu *!*@*.bu.edu
                                   ; Comment to prevent any user from a
                                   hostname matching *.edu from joining,
                                   except if matching *.bu.edu

   MODE #meditation e              ; Command to list exception masks set
                                   for the channel "#meditation".

   MODE #meditation I              ; Command to list invitations masks
                                   set for the channel "#meditation".

   MODE !12345ircd O               ; Command to ask who the channel
                                   creator for "!12345ircd" is

 * 
*/