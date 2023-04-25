/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ANSI.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 01:04:31 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/17 19:43:03 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STYLE_HPP
# define STYLE_HPP

# include <string>

namespace ANSI
{
	static const std::string	ansi(		"\033[");

	static const std::string	reset(		"\033[0m");
	static const std::string	&r(reset);

	static const std::string	clear(		"\033[1;1H\033[2J");

	static const std::string	bold(		"\033[1m");
	static const std::string	dim(		"\033[2m");
	static const std::string	italic(		"\033[3m");
	static const std::string	underline(	"\033[4m");
	static const std::string	flash(		"\033[5m");
	static const std::string	inverse(	"\033[7m");
	static const std::string	cross(		"\033[9m");

	static const std::string	black(		"\033[30m");
	static const std::string	red(		"\033[31m");
	static const std::string	green(		"\033[32m");
	static const std::string	yellow(		"\033[33m");
	static const std::string	blue(		"\033[34m");
	static const std::string	purple(		"\033[35m");
	static const std::string	cyan(		"\033[36m");
	static const std::string	white(		"\033[37m");
	static const std::string	gray(		"\033[90m");
	static const std::string	pink(		"\033[91m");

	static const std::string	back_black(	"\033[40m");
	static const std::string	back_red(	"\033[41m");
	static const std::string	back_green(	"\033[42m");
	static const std::string	back_yellow("\033[43m");
	static const std::string	back_blue(	"\033[44m");
	static const std::string	back_purple("\033[45m");
	static const std::string	back_cyan(	"\033[46m");
	static const std::string	back_white(	"\033[47m");

	static const std::string	time(		"\033[38;2;255;0;120m");
	static const std::string	src(		"\033[38;2;255;229;196m");
	static const std::string	trailing(	"\033[1;38;2;0;90;190m");
	static const std::string	cmd(		"\033[1;38;2;255;200;200m");
	static const std::string	arg(		"\033[1;38;2;120;230;180m");
	static const std::string	reply(		"\033[1;38;2;169;230;190m");
}

#endif /* *********************************************************** STYLE_H */