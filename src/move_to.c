/*
** move.c for PSU_2015_lemipc
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Mar 23 11:31:53 2016 Adrien WERY
** Last update	Sat Mar 26 13:59:40 2016 Nicolas Constanty
*/

#include "lemipc.h"

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
