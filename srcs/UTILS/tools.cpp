/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 16:45:36 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/13 16:51:19 by bducrocq         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "tools.hpp"

vector<string> split_sep(string const & str, char separator)
{
	vector<string> ret;
	stringstream ss(str);
	string tmp;

	while (getline(ss, tmp, separator))
		ret.push_back(tmp);
	return ret;
}