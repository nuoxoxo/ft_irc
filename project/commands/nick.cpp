#include "Freenode.hpp"

//  /nick nickname
//  Changes your nickname to a new nickname

bool nickname_is_validated(std::string nick)
{
    std::string letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string special = "[]\\`_^{|}";
    std::string digit = "0123456789";
    std::string minus = "-";
    std::string firstChar = letter + special;
    std::string all = letter + special + digit + minus;
    if (nick.size() > 9)
        return (false);
    if (firstChar.find(nick[0]) == std::string::npos)
        return (false);

    // for (int i = 1; nick[i] && i < 9; i++)

    int i = 0;
    while (nick[++i] && i < 9)
    {
        if (all.find(nick[i]) == std::string::npos)
            return (false);
    }
    return (true);
}

bool nickname_is_in_use(Server *serv, std::string nick)
{
    // for (std::map<int, User *>::iterator it = serv->get_users().begin(); it != serv->get_users().end(); it++)

    std::map<int, User *>::iterator     it;

    it = serv->get_users().begin();
    while (it != serv->get_users().end())
    {
        // if (nick.compare(it->second->get_nickname()) == 0)
        if (it->second->get_nickname() == nick)
            return (true);
        ++it;
    }
    return (false);
}

void nick(Server *serv, std::string buffer, int sd)
{
    size_t i = 0;
    std::string newNickname = "";
    std::string buf(buffer);
    if ((i = buf.find_first_not_of(SEP_CHARSET, 5)) != std::string::npos)
        newNickname = buf.substr(i, (buf.find_first_of(SEP_CHARSET, i) - i));
    if (newNickname.empty())
    {
        ___Broadcast___(___Broadcast_RPL_ERR___(431, serv, FIND_USER(sd), "", ""), sd);
        return ;
    }
    if (FIND_USER(sd)->get_mode().find('r') != std::string::npos)
    {
        ___Broadcast___(___Broadcast_RPL_ERR___(484, serv, FIND_USER(sd), "", ""), sd);
        return ;
    }
    if (!nickname_is_validated(newNickname))
    {
        ___Broadcast___(___Broadcast_RPL_ERR___(432, serv, FIND_USER(sd), newNickname, ""), sd);
        return ;
    }
    if (nickname_is_in_use(serv, newNickname))
    {
        ___Broadcast___(___Broadcast_RPL_ERR___(433, serv, FIND_USER(sd), newNickname, ""), sd);
        return ;
    }
    std::string user_answer = user_output(FIND_USER(sd));
    user_answer += "NICK " + newNickname;
    ___Broadcast___(user_answer, sd);
    FIND_USER(sd)->set_nick(newNickname);
}