/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 00:24:59 by amiguez           #+#    #+#             */
/*   Updated: 2023/04/26 23:18:41 by bducrocq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
RPL_YOUREOPER		R381
// ERR_NEEDMOREPARAMS	E461
// ERR_NOOPERHOST		E491
// ERR_PASSWDMISMATCH	E464
*/
void Server::oper(vector<string> args, int cl){
	if (args.size() < 3)
		return Rep().E461(cl, _client[cl].get_nick(), "OPER");
	if (_oper_passw.empty())
		return Rep().E491(cl, _client[cl].get_nick());
	if (_oper_user.empty())
		return Rep().E491(cl, _client[cl].get_nick());
	if (args[2] != _oper_passw)
		return Rep().E464(cl, _client[cl].get_nick());
	if (args[1] != _oper_user)
		return Rep().E464(cl, _client[cl].get_nick());
	_client[cl].set_mode_o();
	Rep().R381(cl, _client[cl].get_nick());
}