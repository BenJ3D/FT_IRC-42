/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 14:46:35 by amiguez           #+#    #+#             */
/*   Updated: 2023/04/17 14:46:35 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

void	Server::pass(vector<string> args, int cl_fd) {
	cout << ANSI::cyan << cl_fd << " --> " << args[0] << endl;
	if (_client[cl_fd].get_pass())
		return Rep().E462(cl_fd, _client[cl_fd].get_nick());
	if (args.size() == 1)
		return Rep().E461(cl_fd, _client[cl_fd].get_nick(), "PASS");
	if (args[1] == this->_pass_word)
		_client[cl_fd].comfirm_password();
}
