#include "Freenode.hpp"

bool    is_bot_command(std::string);

void privmsg(Server *serv, std::string buffer, int sd)
{
    int pos_following_command;
    std::string buf(buffer);

    // if (buf.compare(0, 6, "NOTICE") == 0)
    if (buf == "NOTICE")
        pos_following_command = 7;
    else
        pos_following_command = 8;

    size_t i;
    std::string msgtarget = "";

    if ((i = buf.find_first_not_of(SEP_CHARSET, pos_following_command)) != std::string::npos)
    {
        msgtarget = buf.substr(i, (buf.find_first_of(SEP_CHARSET, i) - i));
    }

    std::string msg = "";
    size_t j;

    j = buf.find_first_of(SEP_CHARSET, i);
    if ((j = buf.find_first_not_of(SEP_CHARSET, j)) != std::string::npos)
    {
        msg = buf.substr(j, (buf.find_first_of(BUFFER_ENDS, j) - j));
    }

    std::string channel_prefixes = "#&+";
    std::string user_answer;

    int user_to_send_socket_fd;


    user_answer = user_output(FIND_USER(sd));
    user_answer += "PRIVMSG " + msgtarget + " " + msg;

    std::cout << MAG "PRVMG 1 msgtarget: " RESET << msgtarget << nl;
    std::cout << MAG "PRVMG 1 usranswer: " RESET << user_answer << nl;

    if (!msgtarget.empty() && channel_prefixes.find(msgtarget[0]) != std::string::npos)
    {
        if (serv->get_channels().find(msgtarget) == serv->get_channels().end())
        {
            Broadcast(get_RPL_ERR(401, serv, FIND_USER(sd), msgtarget, ""), sd);
        }
        else if ((FIND_CHANNEL(msgtarget)->get_mode().find("n") != std::string::npos) && (FIND_CHANNEL(msgtarget)->search_user_by_nickname(FIND_USER(sd)->get_nickname()) == -1))
        {
            Broadcast(get_RPL_ERR(404, serv, FIND_USER(sd), msgtarget, ""), sd);
        }
        else if ((FIND_CHANNEL(msgtarget)->get_mode().find("m") != std::string::npos) && (!FIND_CHANNEL(msgtarget)->is_chanop(sd)) && (!FIND_CHANNEL(msgtarget)->is_voice(sd)))
        {
            Broadcast(get_RPL_ERR(404, serv, FIND_USER(sd), msgtarget, ""), sd);
        }
        else if (FIND_CHANNEL(msgtarget)->is_banned(FIND_USER(sd)->get_nickname()) == true)
        {
            Broadcast(get_RPL_ERR(404, serv, FIND_USER(sd), msgtarget, ""), sd);
        }
        /// Added following session for bot
        else if ( is_bot_command(buffer))
        {
            std::string command = buffer.substr(buffer.find('!') + 1, buffer.find('\r') != std::string::npos ? buffer.length() - 2 - (buffer.find('!') + 1) : buffer.length() - 1 - (buffer.find('!') + 1));
            serv->get_bot()->find_command(serv, FIND_CHANNEL(msgtarget), sd, command);
        }
        else if ((FIND_CHANNEL(msgtarget)->get_mode().find("a") != std::string::npos))
        {
            user_answer = anonymous_output() + "PRIVMSG " + msgtarget + " " + msg;
            send_everyone_in_channel_except_user(user_answer, FIND_CHANNEL(msgtarget), sd);
        }
        else
        {
            send_everyone_in_channel_except_user(user_answer, FIND_CHANNEL(msgtarget), sd);
        }
    }
    else
    {
        if (msgtarget == serv->get_bot()->get_name())
        {
            int k = 0;
            int j = -1;

            std::string     word;

            // for (j = 0; buffer[j] && k < 2; j++)

            while (buffer[++j] && k < 2)
            {
                if (buffer[j] == ' ' || buffer[j] == '\t')
                {
                    while (buffer[j] && (buffer[j] == ' ' || buffer[j] == '\t'))
                        j++;
                    k++;
                    j--;
                }
            }
            if (buffer[j] == ':')
            {
                word = buffer.substr(j + 1, buffer.find('\r') != std::string::npos ? buffer.length() - 2 - (j + 1) : buffer.length() - 1 - (j + 1));
            }
            else
            {
                word = buffer.substr(j, buffer.find('\r') != std::string::npos ? buffer.length() - 2 - j : buffer.length() - 1 - j);
            }
            std::cout << MAG "PRVMG 2 : " RESET << word << nl;
        }
        else if ((user_to_send_socket_fd = serv->search_user_by_nickname(msgtarget)) == -1)
        {
            Broadcast(get_RPL_ERR(401, serv, FIND_USER(sd), msgtarget, ""), sd);
        }
        else
        {
            Broadcast(user_answer, user_to_send_socket_fd);
        }
    }
}

bool is_bot_command(std::string buffer)
{
    int     i = 0, j = -1;

    // for (j = 0; buffer[j] && i < 2; j++)

    while (buffer[++j] && i < 2) // edit for bot
    {
        if (buffer[j] == ' ' || buffer[j] == '\t')
        {
            while (buffer[j] && (buffer[j] == ' ' || buffer[j] == '\t'))
            {
                j++;
            }
            i++;
            j--;
        }
    }
    if (buffer[j] == '!' || (buffer[j] == ':' && buffer[j + 1] == '!'))
    {
        return (true);
    }
    return (false);
}