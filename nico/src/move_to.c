/*
** move.c for PSU_2015_lemipc
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Mar 23 11:31:53 2016 Adrien WERY
** Last update	Thu Mar 24 23:14:00 2016 Nicolas Constanty
*/

#include "lemipc.h"

int	moveMsg(t_player *p)
{
  t_msg    msgR;
  t_msg    msgS;
  static unsigned int i = 0;

  if (msgrcv(p->msgID, &msgR, sizeof(t_msg), p->nteam, MSG_NOERROR |
  IPC_NOWAIT) == -1)
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
  if (checkChance(p, 3, 0, 0) > 0)
  {
    msgS.pos = avoid(p);
    if (p->caps == false && msgsnd(p->msgID, &msgS, sizeof(t_msg), 0) == -1)
    perror("msgsnd");
    return (msgS.pos);
  }
  else
  {
    if (p->caps == false && msgsnd(p->msgID,
      &msgS, sizeof(t_msg), 0) == -1)
      perror("msgsnd");
      return (attack(p, 3));
  }
}

void	moveToY(t_player *p, int y, int dice)
{
  if (getPos(p->y - 1, p->x) != -1 &&
  p->map[getPos(p->y - 1, p->x)] == 0 && p->y > y)
  --p->y;
  else if (getPos(p->y + 1, p->x) != -1 &&
  p->map[getPos(p->y + 1, p->x)] == 0 && p->y < y)
  ++p->y;
  else
    if (dice)
    {
      if (getPos(p->y, p->x - 1) != -1 &&
      p->map[getPos(p->y, p->x - 1)] == 0)
      --p->x;
      else if (getPos(p->y, p->x + 1) != -1 &&
      p->map[getPos(p->y, p->x + 1)] == 0)
      ++p->x;
    }
    else
    {
      if (getPos(p->y, p->x + 1) != -1 &&
      p->map[getPos(p->y, p->x + 1)] == 0)
      ++p->x;
      else if (getPos(p->y, p->x - 1) != -1 &&
      p->map[getPos(p->y, p->x - 1)] == 0)
      --p->x;
    }
}

void	moveToX(t_player *p, int x, int dice)
{
  if (getPos(p->y, p->x - 1) != -1 &&
  p->map[getPos(p->y, p->x - 1)] == 0 && p->x > x)
  --p->x;
  else if (getPos(p->y, p->x + 1) != -1 &&
  p->map[getPos(p->y, p->x + 1)] == 0 && p->x < x)
  ++p->x;
  else
    if (dice)
    {
      if (getPos(p->y + 1, p->x) != -1 &&
      p->map[getPos(p->y + 1, p->x)] == 0)
      ++p->y;
      else if (getPos(p->y - 1, p->x) != -1 &&
      p->map[getPos(p->y - 1, p->x)] == 0)
      --p->y;
    }
    else
    {
      if (getPos(p->y - 1, p->x) != -1 &&
      p->map[getPos(p->y - 1, p->x)] == 0)
      --p->y;
      else if (getPos(p->y + 1, p->x) != -1 &&
      p->map[getPos(p->y + 1, p->x)] == 0)
      ++p->y;
    }
}

void	movePos(t_player *p, int posTo)
{
  int y = posTo / HEIGHT;
  int x = posTo % WIDTH;
  int dice;

  dice = rand() % 2;
  p->map[getPos(p->y, p->x)] = 0;
  if (abs(p->y - y) > abs(p->x - x))
    moveToY(p, y, dice);
  else
    moveToX(p, x, dice);
  p->map[getPos(p->y, p->x)] = p->nteam;
}
