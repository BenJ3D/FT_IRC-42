/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabucia <aabucia@student.42lyonlyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 03:25:20 by abucia            #+#    #+#             */
/*   Updated: 2023/04/23 05:23:33 by aabuia           ###   ########lyonlyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

void Server::invite(vector<string> args, int fd_client)
{
	if (args.size() != 3)
		return Rep().E461(fd_client, _client[fd_client].get_nick(), args[0]);
	if (findClientFdWithNick(args[1]) == 0 || _channel.find(args[2]) == _channel.end())
		return Rep().E401(fd_client, _client[fd_client].get_nick(), args[1]);
	if (_channel[args[2]].getList().find(fd_client) == _channel[args[2]].getList().end())
		return Rep().E442(fd_client, _client[fd_client].get_nick(), args[1]);
	if (_channel[args[2]].getList().find(findClientFdWithNick(args[1])) != _channel[args[2]].getList().end())
		return Rep().E443(fd_client, _client[fd_client].get_nick(), args[2], args[1]);
	_channel[args[2]].addInviteList(findClientFdWithNick(args[1]));
	string msg = ":" + _client[fd_client].get_nick() + "!" + _client[fd_client].get_username() + "@" + string(SERVER_NAME) + " INVITE " + args[1] + " :" + args[2] + "\n\r";
	if (send(findClientFdWithNick(args[1]), msg.c_str(), msg.size(), 0) == -1)
		send_error(findClientFdWithNick(args[1]));
	cout << ANSI::gray << "{send} =>" << ANSI::purple << msg << endl;
}

/* easter car

		   PUTPUT               *        ______
					**   *          ___//   \___\___
					   VROUM       |_            -    `|
						VROUM   *    `-(o)-------(o)-'
									VIOUuuumMmmm

  											 _..----.._
  											]_.--._____[
  										  ___|'--'__..|--._
  					  __               """    ;            :
  					()_ """"---...__.'""!":  /    ___       :
  					   """---...__\]..__] | /    [ :)]      :
  								  """!--./ /      """        :
  						   __  ...._____;""'.__________..--..:_
  						  /  !"''''''!''''''''''|''''/' ' ' ' \"--..__  __..
  						 /  /.--.    |          |  .'          \' ' '.""--.{'.
  	 _...__            >=7 //.-.:    |          |.'             \ ._.__  ' '""'.
    .-' /    """"----..../ "">==7-.....:______    |                \| |  "";.;-"> \
    """";           __.."   .--"/"""""----...."""""----.....H_______\_!....'----""""]
  _..---|._ __..--""       _!.-=_.            """""""""""""""                   ;"""
 /   .-";-.'--...___     ." .-""; ';""-""-...^..__...-v.^___,  ,__v.__..--^"--""-v.^v,
;   ;   |'.         """-/ ./;  ;   ;\P.        ;   ;        """"____;  ;.--""""// '""<,;
;   | 1            ;  ;  '.: .'  ;<   ___.-'._.'------""""""____'..'.--""";;'  o ';
'.   \__:/__           ;  ;--""()_   ;'  /___ .-" ____---""""""" __.._ __._   '>.,  ,/;
  \   \    /"""<--...__;  '_.-'/; ""; ;.'.'  "-..'    "-.      /"/    `__. '.   "---";
   '.  'v ; ;     ;;    \  \ .'  \ ; ////    _.-" "-._   ;    : ;   .-'__ '. ;   .^".'
     '.  '; '.   .'/     '. `-.__.' /;;;   .o__.---.__o. ;    : ;   '"";;""' ;v^" .^
  	 '-. '-.___.'<__v.^,v'.  '-.-' ;|:   '    :      ` ;v^v^'.'.    .;'.__/_..-'
  		'-...__.___...---""'-.   '-'.;\     'WW\     .'_____..>."^"-""""""""    fact
  							  '--..__ '"._..'  '"-;;"""
  									 """---'""""""

<P>
     _o >
    //\|
   _/\
     /
<P>
     _o    >
    //\/
   _/\
     /
<P>
     _o         >
    //\_
    /\
   / /
<P>
     _o           _,
    //\
    |\ `
   / /
<P>
     o
    /|\          `-
    |\
   / /
<P>
     o
    /|\       <
    |\
    //
<P>
    o _  <
    /\
     |
    //
<P>
  'o <_
  ` /\
     |
    //
<P>
<     _
  , /\
 o   />
    //
<F>

  ,   \
 ,   _/|
o   //

Matthew Thomas


            _      ()              ()      _
           / \     ||______________||     / \
          /___\    |                |    /___\
            |      |      ~@@~      |      |
           (_)     |_______  _______|     (_)
        ___/_\___  {_______}{_______}  ___/_\___
         |__~__|   %%%%%%%%%%%%%%%%%%   |__~__|
      ___|_____|__%%%%%%%%%%%%%%%%%%%%__|_____|___
         |     | %%%%%%%%%%%%%%%%%%%%%% |     |
          `=====%%%%%%%%%%%%%%%%%%%%%%%%=====`
         `=====%%%%%%%%%%%%%%%%%%%%%%%%%%=====`
        `=====%%%%%%%%%%%%%%%%%%%%%%%%%%%%=====`
       `=====/||||||||||||||||||||||||||||\=====`
      `======||||||||||||||||||||||||||||||======`
     `=======|||||||||||||||||||||||||||lc|=======`
    `==============================================`
   `================================================`
  `==================================================`
 `====================================================`




 X | X | O
---+---+---
 O | O | O
---+---+---
 X | O | X
		Ahaha i win !! -O

 */
