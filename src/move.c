/*
** move.c for PSU_2015_lemipc
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Mar 23 11:31:53 2016 Adrien WERY
** Last update	Sun Mar 27 10:47:52 2016 Adrien WERY
*/

#include "lemipc.h"

int	attack(t_player *p, int len)
{
  int     i, pos;

  if (len > WIDTH)
    return (rand() % (WIDTH * HEIGHT));
  i = -1;
  while (++i < len)
    if ((pos = getPos(p->y - len / 2, p->x - len / 2 + i)) != - 1 &&
	isEnnemy(p->map[pos], p->nteam))
      return (pos);
  i = 0;
  while (++i < len - 1)
    if ((pos = getPos(p->y - len / 2 + i, p->x - len / 2)) != - 1 &&
	isEnnemy(p->map[pos], p->nteam))
      return (pos);
  i = 0;
  while (++i < len - 1)
    if ((pos = getPos(p->y - len / 2 + i, p->x + len / 2)) != - 1 &&
	isEnnemy(p->map[pos], p->nteam))
      return (pos);
  i = -1;
  while (++i < len)
    if ((pos = getPos(p->y + len / 2, p->x - len / 2 + i)) != - 1 &&
        isEnnemy(p->map[pos], p->nteam))
      return (pos);
  return (attack(p, len + 2));
}

int	checkChance(t_player *p, int len, int nb_e, int nb_a)
{
  int     i;
  int     pos;

  i = -1;
  while (++i < len)
      if ((pos = getPos(p->y - len / 2, p->x - len / 2 + i)) != - 1)
          (isEnnemy(p->map[pos], p->nteam)) ? ++nb_e : ++nb_a;
  i = 0;
  while (++i < len - 1)
      if ((pos = getPos(p->y - len / 2 + i, p->x - len / 2)) != - 1)
        (isEnnemy(p->map[pos], p->nteam)) ? ++nb_e : ++nb_a;
  i = 0;
  while (++i < len - 1)
      if ((pos = getPos(p->y - len / 2 + i, p->x + len / 2)) != - 1)
        (isEnnemy(p->map[pos], p->nteam)) ? ++nb_e : ++nb_a;
  i = -1;
  while (++i < len)
      if ((pos = getPos(p->y + len / 2, p->x - len / 2 + i)) != - 1)
        (isEnnemy(p->map[pos], p->nteam)) ? ++nb_e : ++nb_a;
  if (len < 5)
    return (checkChance(p, len + 2, nb_e, nb_a));
  return (nb_e - nb_a);
}

int	avoid(t_player *p)
{
  int 	pos;
  int	x, y, x1, y1;

  pos = attack(p, 3);
  x = pos % WIDTH;
  y = pos / HEIGHT;
  x1 = p->x;
  y1 = p->y;
  if (abs(y1 - y) > abs(x1 - x))
    {
      if (getPos(y1 - 1, x1) != -1 && p->map[getPos(y1 - 1, x1)] == 0 && y1 > y)
	++y1;
      else if (getPos(y1 + 1, x1) != -1 &&
	       p->map[getPos(y1 + 1, x1)] == 0 && y1 < y)
	--y1;
    }
  else
    {
      if (getPos(y1, x1 - 1) != -1 && p->map[getPos(y1, x1 - 1)] == 0 && x1 > x)
        ++x1;
      else if (getPos(y1, x1 + 1) != -1 &&
	       p->map[getPos(y1, x1 + 1)] == 0 && x1 < x)
        --x1;
    }
  return (getPos(y1, x1));
}

int	_moveMsg(t_player *p)
{
  t_msg    msgR;
  t_msg    msgS;
  int      nb;
  static unsigned int i = 0;

  msgS.mtype = p->nteam;
  msgS.pos = getPos(p->y, p->x);
  if (msgrcv(p->msgID, &msgR, sizeof(t_msg), p->nteam, MSG_NOERROR
        | IPC_NOWAIT) == -1)
    {
      nb = count(p);
        while (--nb)
            if (msgsnd(p->msgID, &msgS, sizeof(t_msg), 0) == -1)
	      perror("msgsnd");
        return (attack(p, 3));
    }
  ++i;
  if (i < 50 || rand() % 3 == 0)
    return (msgR.pos);
  if (checkChance(p, 3, 0, 0) > 0)
    return (avoid(p));
  else
    return (attack(p, 3));
}

void	move(t_player *p, struct sembuf *sops)
{
    sops->sem_op = -1;
    semop(p->semID, sops, 1);
    usleep(1000);
    movePos(p, _moveMsg(p));
    sops->sem_op = 1;
    semop(p->semID, sops, 1);
}
