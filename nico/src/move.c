/*
** move.c for PSU_2015_lemipc
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Mar 23 11:31:53 2016 Adrien WERY
** Last update	Thu Mar 24 15:52:21 2016 Nicolas Constanty
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

int         checkChance(t_player *p, int len, int nb_e, int nb_a)
{
    int     i;
    int     pos;

    i = -1;
    while (++i < len)
    {
        pos = getPos(p->y - len / 2, p->x - len / 2 + i);
        if (pos != - 1)
        {
          if (isEnnemy(p->map[pos], p->nteam))
            ++nb_e;
          else
            ++nb_a;
        }
    }
    i = 0;
    while (++i < len - 1)
    {
        pos = getPos(p->y - len / 2 + i, p->x - len / 2);
        if (pos != - 1)
        {
          if (isEnnemy(p->map[pos], p->nteam))
            ++nb_e;
          else
            ++nb_a;
        }
    }
    i = 0;
    while (++i < len - 1)
    {
        pos = getPos(p->y - len / 2 + i, p->x + len / 2);
        if (pos != - 1)
        {
          if (isEnnemy(p->map[pos], p->nteam))
            ++nb_e;
          else
            ++nb_a;
        }
    }
    i = -1;
    while (++i < len)
    {
        pos = getPos(p->y + len / 2, p->x - len / 2 + i);
        if (pos != - 1)
        {
          if (isEnnemy(p->map[pos], p->nteam))
            ++nb_e;
          else
            ++nb_a;
        }
    }
  if (len < 5)
    return (checkChance(p, len + 2, nb_e, nb_a));
  return (nb_e - nb_a);
}

int			avoid(t_player *p)
{
  int 	pos;
  int		x;
  int		y;
  int		x1;
  int		y1;

  pos = attack(p, 3);
  x = pos % WIDTH;
  y = pos / HEIGHT;
  x1 = p->x;
  y1 = p->y;
  if (abs(y1 - y) > abs(x1 - x))
  {
    if (getPos(y1 - 1, x1) != -1 && p->map[getPos(y1 - 1, x1)] == 0 && y1 > y)
        ++y1;
    else if (getPos(y1 + 1, x1) != -1 && p->map[getPos(y1 + 1, x1)] == 0 && y1 < y)
        --y1;
  }
  else
  {
    if (getPos(y1, x1 - 1) != -1 && p->map[getPos(y1, x1 - 1)] == 0 && x1 > x)
        ++x1;
    else if (getPos(y1, x1 + 1) != -1 && p->map[getPos(y1, x1 + 1)] == 0 && x1 < x)
        --x1;
  }
  return (getPos(y1, x1));
}

int         moveMsg(t_player *p)
{
    t_msg    msgR;
    t_msg    msgS;
    static unsigned int i = 0;

    // msgS.mt = GROUP;

    if (msgrcv(p->msgID, &msgR, sizeof(t_msg), p->nteam, MSG_NOERROR | IPC_NOWAIT) == -1)
    {
        msgS.mtype = p->nteam;
        msgS.pos = getPos(p->y, p->x);
        if (msgsnd(p->msgID, &msgS, sizeof(t_msg), 0) == -1)
          perror("msgsnd");
        printf("%s\n", "coucou");
        p->caps = true;
        return (attack(p, 3));
    }
    ++i;
    if (i < 50 || rand() % 3 == 0)
    {
      printf("%d\n", p->caps);
      if (p->caps == false && msgsnd(p->msgID, &msgR, sizeof(t_msg), 0) == -1)
        perror("msgsnd");
      return (msgR.pos);
    }
    // Si plus ennemi appel les alliés
    if (checkChance(p, 3, 0, 0) > 0)
    {
      msgS.pos = avoid(p);
      if (p->caps == false && msgsnd(p->msgID, &msgS, sizeof(t_msg), 0) == -1)
        perror("msgsnd");
      return (msgS.pos);
    }
    else
    {
      if (p->caps == false && msgsnd(p->msgID, &msgS, sizeof(t_msg), 0) == -1)
      perror("msgsnd");
      return (attack(p, 3));
    }
}

int         _moveMsg(t_player *p)
{
    t_msg    msgR;
    t_msg    msgS;
    int      nb;
    static unsigned int i = 0;

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
    ++i;
    if (i < 50 || rand() % 3 == 0)
      return (msgR.pos);
    // Si plus ennemi appel les alliés
    if (checkChance(p, 3, 0, 0) > 0)
      return (avoid(p));
    else
      return (attack(p, 3));
}

void    movePos(t_player *p, int posTo)
{
    int y = posTo / HEIGHT;
    int x = posTo % WIDTH;
    int dice;

    p->map[getPos(p->y, p->x)] = 0;
    if (abs(p->y - y) > abs(p->x - x))
    {
        if (getPos(p->y - 1, p->x) != -1 && p->map[getPos(p->y - 1, p->x)] == 0 && p->y > y)
            --p->y;
        else if (getPos(p->y + 1, p->x) != -1 && p->map[getPos(p->y + 1, p->x)] == 0 && p->y < y)
            ++p->y;
        else
        {
          dice = rand() % 2;
          if (dice)
          {
            if (getPos(p->y, p->x - 1) != -1 && p->map[getPos(p->y, p->x - 1)] == 0)
              --p->x;
            else if (getPos(p->y, p->x + 1) != -1 && p->map[getPos(p->y, p->x + 1)] == 0)
              ++p->x;
          }
          else
          {
            if (getPos(p->y, p->x + 1) != -1 && p->map[getPos(p->y, p->x + 1)] == 0)
              ++p->x;
            else if (getPos(p->y, p->x - 1) != -1 && p->map[getPos(p->y, p->x - 1)] == 0)
             --p->x;
          }
        }
    }
    else
    {
        if (getPos(p->y, p->x - 1) != -1 && p->map[getPos(p->y, p->x - 1)] == 0 && p->x > x)
            --p->x;
        else if (getPos(p->y, p->x + 1) != -1 && p->map[getPos(p->y, p->x + 1)] == 0 && p->x < x)
            ++p->x;
        else
        {
          dice = rand() % 2;
          if (dice)
          {
            if (getPos(p->y + 1, p->x) != -1 && p->map[getPos(p->y + 1, p->x)] == 0)
              ++p->y;
            else if (getPos(p->y - 1, p->x) != -1 && p->map[getPos(p->y - 1, p->x)] == 0)
              --p->y;
          }
          else
          {
            if (getPos(p->y - 1, p->x) != -1 && p->map[getPos(p->y - 1, p->x)] == 0)
              --p->y;
            else if (getPos(p->y + 1, p->x) != -1 && p->map[getPos(p->y + 1, p->x)] == 0)
              ++p->y;
          }
        }
    }
    p->map[getPos(p->y, p->x)] = p->nteam;
}

void    move(t_player *p, struct sembuf *sops)
{
    sops->sem_op = -1;
    semop(p->semID, sops, 1);
    usleep(1000);
    movePos(p, _moveMsg(p));
    sops->sem_op = 1;
    semop(p->semID, sops, 1);
}
