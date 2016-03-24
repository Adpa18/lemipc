/*
** move.c for PSU_2015_lemipc
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Mar 23 11:31:53 2016 Adrien WERY
** Last update	Wed Mar 23 22:53:33 2016 Adrien WERY
*/

#include "lemipc.h"

int         attack(t_player *p, int len)
{
    int     i;
    int     pos;

    if (len > WIDTH)
        return (rand() % (WIDTH * HEIGHT));
    i = -1;
    while (++i < len)
    {
        pos = getPos(p->y - len / 2, p->x - len / 2 + i);
        if (pos != - 1 && isEnnemy(p->map[pos], p->nteam))
            return (pos);
    }
    i = 0;
    while (++i < len - 1)
    {
        pos = getPos(p->y - len / 2 + i, p->x - len / 2);
        if (pos != - 1 && isEnnemy(p->map[pos], p->nteam))
            return (pos);
    }
    i = 0;
    while (++i < len - 1)
    {
        pos = getPos(p->y - len / 2 + i, p->x + len / 2);
        if (pos != - 1 && isEnnemy(p->map[pos], p->nteam))
            return (pos);
    }
    i = -1;
    while (++i < len)
    {
        pos = getPos(p->y + len / 2, p->x - len / 2 + i);
        if (pos != - 1 && isEnnemy(p->map[pos], p->nteam))
            return (pos);
    }
    return (attack(p, len + 2));
}

int         group(t_player *p, int len)
{
    int     i;
    int     pos;

    if (len > WIDTH)
        return (rand() % (WIDTH * HEIGHT));
    i = -1;
    while (++i < len)
    {
        pos = getPos(p->y - len / 2, p->x - len / 2 + i);
        if (pos != - 1 && isFriend(p->map[pos], p->nteam))
            return (pos);
    }
    i = 0;
    while (++i < len - 1)
    {
        pos = getPos(p->y - len / 2 + i, p->x - len / 2);
        if (pos != - 1 && isFriend(p->map[pos], p->nteam))
            return (pos);
    }
    i = 0;
    while (++i < len - 1)
    {
        pos = getPos(p->y - len / 2 + i, p->x + len / 2);
        if (pos != - 1 && isFriend(p->map[pos], p->nteam))
            return (pos);
    }
    i = -1;
    while (++i < len)
    {
        pos = getPos(p->y + len / 2, p->x - len / 2 + i);
        if (pos != - 1 && isFriend(p->map[pos], p->nteam))
            return (pos);
    }
    return (group(p, len + 2));
}

int         moveMsg(t_player *p)
{
    // t_msg   msg;
    //
    // if (msgrcv(p->msgID, &msg, sizeof(t_msg), p->nteam, MSG_NOERROR | IPC_NOWAIT) == -1)
    // {
    //     msg.team = p->nteam;
    //     msgsnd(p->msgID, &msg, sizeof(t_msg), 0);
    //     return (attack(p, 3));
    // }
    // msgsnd(p->msgID, &msg, sizeof(t_msg), 0);
    if (rand() % 3)
        return (group(p, 3));
    return (attack(p, 3));
}

void    movePos(t_player *p, int posTo)
{
    int y = posTo / HEIGHT;
    int x = posTo % WIDTH;
    bool    moved;

    moved = false;
    p->map[getPos(p->y, p->x)] = 0;
    if (abs(p->y - y) > abs(p->x - x))
    {
        if (p->map[getPos(p->y - 1, p->x)] == 0 && p->y > y)
        {
            --p->y;
            moved = true;
        }
        else if (p->map[getPos(p->y + 1, p->x)] == 0 && p->y < y)
        {
            ++p->y;
            moved = true;
        }
    }
    if (!moved)
    {
        if (p->map[getPos(p->y, p->x - 1)] == 0 && p->x > x)
            --p->x;
        else if (p->map[getPos(p->y, p->x + 1)] == 0 && p->x < x)
            ++p->x;
    }
    p->map[getPos(p->y, p->x)] = p->nteam;
}
