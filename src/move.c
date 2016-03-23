/*
** move.c for PSU_2015_lemipc
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Mar 23 11:31:53 2016 Adrien WERY
** Last update	Wed Mar 23 17:30:53 2016 Adrien WERY
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

int         moveMsg(t_player *p)
{
    t_msg    msgR;
    t_msg    msgS;
    int     nb;

    msgS.mtype = p->nteam;
    msgS.pos = getPos(p->y, p->x);
    // msgS.mt = GROUP;

    if (msgrcv(p->msgID, &msgR, sizeof(t_msg), p->nteam, MSG_NOERROR | IPC_NOWAIT) == -1)
    {
        nb = count(p);
        while (--nb)
        {
            if (msgsnd(p->msgID, &msgS, sizeof(t_msg), 0) == -1)
                perror("msgsnd");
        }
        return (attack(p, 3));
    }
    return (msgR.pos);
}

void    movePos(t_player *p, int posTo)
{
    int y = posTo / HEIGHT;
    int x = posTo % WIDTH;

    p->map[getPos(p->y, p->x)] = 0;
    if (abs(p->y - y) > abs(p->x - x))
    {
        if (p->map[getPos(p->y - 1, p->x)] == 0 && p->y > y)
            --p->y;
        else if (p->map[getPos(p->y + 1, p->x)] == 0 && p->y < y)
            ++p->y;
    }
    else
    {
        if (p->map[getPos(p->y, p->x - 1)] == 0 && p->x > x)
            --p->x;
        else if (p->map[getPos(p->y, p->x + 1)] == 0 && p->x < x)
            ++p->x;
    }
    p->map[getPos(p->y, p->x)] = p->nteam;
}

void    move(t_player *p, struct sembuf *sops)
{
    sops->sem_op = -1;
    semop(p->semID, sops, 1);
    usleep(10000);
    movePos(p, moveMsg(p));
    sops->sem_op = 1;
    semop(p->semID, sops, 1);
}
