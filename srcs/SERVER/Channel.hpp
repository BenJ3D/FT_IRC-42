#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include "Server.hpp"
# include "../CLIENT/Client.hpp"

class Client;

class Channel
{

	public:

		Channel( Channel const & name );
		~Channel();

		vector<int>				getList();
		vector<int>				getOperator();
		void					addOperator(int fd_client);
		void					addClient(int fd_client);

	private:
		Channel();
		Channel &				operator=( Channel const & rhs );
		
		bool					isOperator(int fd_client);

		string const &			_topic;
		string const &			_name;
		vector<int>				_list;
		vector<int>				_operator;
};

std::ostream &			operator<<( std::ostream & o, Channel const & i );

#endif /* ********************************************************* CHANNEL_H */