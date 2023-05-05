#pragma once
#ifndef __CHANNEL_HPP__
# define __CHANNEL_HPP__

# include "Freenode.hpp"

class	User;

class	Channel
{

private:

	std::map<int, User*>	m_users;
	std::map<int, User*>	m_chanops;
	std::map<int, User*>	m_voices;
	std::string		m_channelname;
	std::string		m_topic;
	std::string		m_mode;
	std::string		m_key;

	std::map<std::string, std::string>	m_banlist;

	int		m_maximum_users;
	time_t		m_last_joke_told;


public:
	Channel(std::string);
	~Channel();

	std::map<std::string, std::string>
		& get_banlist();

	std::map<int, User*>	& get_users();
	std::map<int, User*>	& get_chanops();
	std::map<int, User*>	& get_voices();

	std::string	get_topic() const;
	std::string	get_channelname() const;
	std::string	get_mode() const;
	std::string	get_key() const;

	std::string get_list_of_users_in_channel();
	std::string get_list_of_users_banned();

	int	get_user_number() const;
	int	get_maximum_users() const;

	time_t	get_last_joke_told() const;

	bool	is_chanop(int sd);
	bool	is_voice(int sd);
	bool	is_banned(std::string nick);

	void	set_topic(std::string top);
	void	set_mode(std::string mod);
	void	set_key(std::string key);
	void	set_maximum_users(int max);
	void	set_last_joke_told();

	void	add_user(int sd, User *u);
	void	add_chanop(int sd, User *u);
	void	add_voice(int sd, User *u);

	void	left_user_of_what_use(int sd);
	int	search_user_by_nickname(std::string nick);

};


#endif


