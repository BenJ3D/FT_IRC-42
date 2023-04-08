/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NumericReplies.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 02:58:43 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/04 03:01:44 by bducrocq         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "./NumericReplies.hpp"
#include "./ANSI.hpp"

Rep::Rep() {}

void Server::confirm_to_client(int const &fd, string &msg) {
	msg = ":" + _client[fd].get_nick() + "!~" + _client[fd].get_realnick() + "@" + string(SERVER_NAME) + " " + msg + "\r\n";
	cout << ANSI::gray << "{send} => " << ANSI::purple << msg << endl;
	if (send(fd, msg.c_str(), msg.length(), 0) == -1)
		cerr << ANSI::red << "Erreur lors de l'envoi des données au client" << endl;
}

void send_to_client(string &msg, int const &fd) {
	msg = ":" + string(SERVER_NAME) + " " + msg +"\r\n";
	cout << ANSI::gray << "{send} => " << ANSI::purple << msg << endl;
	if (send(fd, msg.c_str(), msg.length(), 0) == -1)
		cerr << ANSI::red << "Erreur lors de l'envoi des données au client" << endl;
}

void Rep::R001(int const &fd, const string &cNick)
{
	output << "001 " << cNick << " :Welcome " << cNick << " to the Internet Chat Relay!";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R002(int const &fd, const string &cNick, const string& servName, const string &servVersion)
{
	output << "002 " << cNick << " :Your host is " << servName << ", running version " << servVersion;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R003(int const &fd, const string &cNick, const string& date)
{
	output << "003 " << cNick << " :This server was created " << date;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R004(int const &fd, const string &cNick)
{
	/* User modes https://www.rfc-editor.org/rfc/rfc2812#section-3.1.5 */
	/* Channel modes https://www.rfc-editor.org/rfc/rfc2811#section-4 */
	output << "004 " << cNick << " 42ircserv 1.0 " << CHANNELMODE_CHARLIST << " " << USERMODE_CHARLIST;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R211(int const &fd, const string &cNick, const string& infostr)
{
	output << "211 " << cNick << " " << infostr;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R212(int const &fd, const string &cNick, const string& infostr)
{
	output << "212 " << cNick << " " << infostr << " 0 0";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R219(int const &fd, const string &cNick, const string& letters)
{
	output << "219 " << cNick << " " << letters << " :End of STATS report";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void	Rep::R221(int const &fd, const string &cNick, const string& userModes)
{
	output << "221 " << cNick << " " << userModes;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R242(int const &fd, const string &cNick, const string& infostr)
{
	output << "242 " << cNick << " :" << infostr;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}
void Rep::R243(int const &fd, const string &cNick, const string& infostr)
{
	output << "243 " << cNick << " " << infostr;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R251(int const &fd, const string &cNick, const string& infostr)
{
	output << "251 " << cNick << " " << infostr;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R252(int const &fd, const string &cNick, int ops)
{
	output << "252 " << cNick << " " << ops << " :operator(s) online";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R253(int const &fd, const string &cNick, int unknownConnnections)
{
	output << "253 " << cNick << " " << unknownConnnections << " :unknown connection(s)";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R254(int const &fd, const string &cNick, int channels)
{
	output << "254 " << cNick << " " << channels << " :channel(s) currently open";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R255(int const &fd, const string &cNick, const string& infostr)
{
	output << "255 " << cNick << " " << infostr;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R265(int const &fd, const string &cNick, int users)
{
	output << "265 " << cNick << " " << users << " " << MAX_CONNECTIONS <<" :Current local users " << users << ", max " << MAX_CONNECTIONS;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R266(int const &fd, const string &cNick, int users)
{
	output << "266 " << cNick << " " << users << " " << MAX_CONNECTIONS <<" :Current global users " << users << ", max " << MAX_CONNECTIONS;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R311(int const &fd, const string &cNick, const string& queryNick, const string& queryName, const string& queryRealName)
{
	output << "311 " << cNick << " " << queryNick << " " << queryName << " * * :" << queryRealName;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R313(int const &fd, const string &cNick, const string& queryNick)
{
	output << "313 " << cNick << " " << queryNick << " :Is an IRC operator";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R315(int const &fd, const string &cNick, const string& name)
{
	output << "315 " << cNick << " " << name << " :End of Who query";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R318(int const &fd, const string &cNick, const string& queryList)
{
	output << "318 " << cNick << " " << queryList << " :End of /WHOIS list";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R319(int const &fd, const string &cNick, const string& queryNick, char prefix, const string& chanName)
{
	output << "319 " << cNick << " " << queryNick << " :";
	if (prefix != 'u')
	 	output << prefix;
	output << chanName;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void	Rep::R322(int const &fd, const string &cNick, int nuser, const string& topic, const string& chanName)
{
	output << "322 " << cNick << " " << chanName << " " << nuser << " :" << topic;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void	Rep::R323(int const &fd, const string &cNick)
{
	output << "323 " << cNick << " :End of LIST";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void	Rep::R324(int const &fd, const string &cNick, const string& chanName, const string& chanModeStr, const string& chanModeArgs)
{
	output << "324 " << cNick << " " << chanName << " " << chanModeStr << " " << chanModeArgs;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R331(int const &fd, const string &cNick, const string& chanName)
{
	output << "331 " << cNick << " " << chanName << " :No topic set";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R332(int const &fd, const string &cNick, const string& chanName, const string& topic)
{
	output << "332 " << cNick << " " << chanName << " :" << topic;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R333(int const &fd, const string &cNick, const string& chanName, const string& setterNick, time_t timestamp)
{
	output << "333 " << cNick << " " << chanName << " " << setterNick << " " << timestamp;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R341(int const &fd, const string &cNick, const string& otherNick, const string& chanName)
{
	output << "341 " << cNick << " " << otherNick << " " << chanName;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R352(int const &fd, const string &cNick, const string& message)
{
	output << "352 " << cNick << " " << message;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R353(int const &fd, const string &cNick, const string& chanName, const string& nick, char chanPrefix, char userPrefix)
{
	if (userPrefix == 'u')
		output << "353 " << cNick << " " << chanPrefix << " " << chanName << " :" << nick;
	else
		output << "353 " << cNick << " " << chanPrefix << " " << chanName << " :" << userPrefix << nick;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R366(int const &fd, const string &cNick, const string& chanName)
{
	output << "366 " << cNick << " " << chanName << " :End of NAMES list";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R367(int const &fd, const string &cNick, const string& chanName, const string& bannedUser)
{
	output << "367 " << cNick << " " << chanName << " " << bannedUser;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R368(int const &fd, const string &cNick, const string& chanName)
{
	output << "368 " << cNick << " " << chanName << " :End of channel ban list";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void	Rep::R372(int const &fd, const string &cNick, const string& Motd)
{
	output << "372 " << cNick << " :" << Motd;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void	Rep::R375(int const &fd, const string &cNick, const string& serverName)
{
	output << "375 " << cNick << " :- " << serverName << " Message of the day - ";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void	Rep::R376(int const &fd, const string &cNick)
{
	output << "376 " << cNick << " :End of /MOTD command.";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void	Rep::R381(int const &fd, const string &cNick)
{
	output << "381 " << cNick << " " << " :You are now an IRC operator";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::R391(int const &fd, const string &cNick, const string& servName)
{
	char date_string[128];
	time_t curr_time;
	tm *curr_tm;
	time(&curr_time);
	curr_tm = localtime(&curr_time);

	strftime(date_string, 50, "%c", curr_tm);

	output << "391 " << cNick << " " << servName << " :" << date_string;
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

/* Errors */
void Rep::E401(int const &fd, const string &cNick, const string& inputNick)
{
	output << "401 " << cNick << " " << inputNick << " :No such nick/channel";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E403(int const &fd, const string &cNick, const string& chanName)
{
	output << "403 " << cNick << " " << chanName << " :No such channel";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E404(int const &fd, const string &cNick, const string& chanName)
{
	output << "404 " << cNick << " " << chanName << " :Cannot send to channel";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E405(int const &fd, const string &cNick, const string& chanName)
{
	output << "405 " << cNick << " " << chanName <<  " :You have joined too many channels";	
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E411(int const &fd, const string &cNick, const string& cmd)
{
	output << "411 " << cNick << " :No recipient given (" << cmd << ")";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E412(int const &fd, const string &cNick)
{
	output << "412 " << cNick << " :No text to send";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E421(int const &fd, const string &cNick, const string& cmd)
{
	output << "421 " << cNick << " " << cmd << " :Unknown command";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E422(int const &fd, const string &cNick)
{
	output << "422 " << cNick << " :No MOTD in config File";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E431(int const &fd, const string &cNick)
{
	output << "431 " << cNick << " :No nickname given";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E432(int const &fd, const string &cNick, const string& badNick)
{
	output << "432 " << cNick << " " << badNick << " :Erronous nickname";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E433(int const &fd, const string &cNick, const string& badNick)
{
	output << "433 " << cNick << " " << badNick << " :Nickname is already in user";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E441(int const &fd, const string &cNick, const string& chanName, const string& inputNick)
{
	output << "441 " << cNick << " " << inputNick << " " << chanName << " :They aren't on that channel";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E442(int const &fd, const string &cNick, const string& chanName)
{
	output << "442 " << cNick << " " << chanName << " :You're not on that channel";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E443(int const &fd, const string &cNick, const string& chanName, const string& inputNick)
{
	output << "443 " << cNick << " " << inputNick << " " << chanName << " :Is already on channel";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E451(int const &fd, const string &cNick)
{
	output << "451 " << cNick << " :You have not registered";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E461(int const &fd, const string &cNick, const string& cmd)
{
	output << "461 " << cNick << " " << cmd << " :Not enough parameters";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E462(int const &fd, const string &cNick)
{
	output << "462 " << cNick << " :Unauthorized command (already registered)";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E464(int const &fd, const string &cNick)
{
	output << "464 " << cNick << " :Password incorrect";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E465(int const &fd, const string &cNick)
{
	output << "465 " << cNick << " :You are banned from this server";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E471(int const &fd, const string &cNick, const string& chanName)
{
	output << "471 " << cNick << " " << chanName << " :Cannot join channel (<<l)";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E472(int const &fd, const string &cNick, const char& modeChar)
{
	output << "472 " << cNick << " " << modeChar << " :is unknown mode char to me";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E473(int const &fd, const string &cNick, const string& chanName)
{
	output << "473 " << cNick << " " << chanName << " :Cannot join channel (<<i)";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E474(int const &fd, const string &cNick, const string& chanName)
{
	output << "474 " << cNick << " " << chanName << " :Cannot join channel (<<b)";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E475(int const &fd, const string &cNick, const string& chanName)
{
	output << "475 " << cNick << " " << chanName << " :Cannot join channel (<<k)";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E476(int const &fd, const string &cNick)
{
	output << "476 " << cNick << " :Bad Channel Mask";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E481(int const &fd, const string &cNick)
{
	output << "481 " << cNick << " :Permission Denied- You're not an IRC operator";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E482(int const &fd, const string &cNick, const string& chanName)
{
	output << "482 " << cNick << " " << chanName << " :You're not channel operator";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E501(int const &fd, const string &cNick)
{
	output << "501 " << cNick << " :Unknown MODE flag";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void Rep::E502(int const &fd, const string &cNick)
{
	output << "502 " << cNick << " :Can't view or change mode for other users";
	string str = output.str();
	send_to_client(str, fd);
	clearBuffer();
}

void	Rep::clearBuffer()
{
	output.str("");
}