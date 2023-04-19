/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 16:55:36 by amiguez           #+#    #+#             */
/*   Updated: 2023/04/18 16:56:33 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Bot.hpp"

vector<string> split_cmd(const string command, char separator)
{
	stringstream stream(command);
	vector<string> args;
	string buffer;
	while (getline(stream, buffer, separator)){
		if (buffer.length() != 0)
			args.push_back(buffer);
	}
	return args;
}
