/*
** player.c for PSU_2015_lemipc
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Mar 23 17:19:57 2016 Adrien WERY
** Last update	Wed Mar 23 17:34:16 2016 Adrien WERY
*/

#include "lemipc.h"

inline int   getPos(int y, int x)
{
    if (y < 0 || y >= HEIGHT || x < 0 || x >= WIDTH)
        return (-1);
    return (y * WIDTH + x);
}

inline bool isEnnemy(int ennemy, int team)
{
    return (ennemy != 0 && ennemy != team);
}

inline bool     alone(t_player *p)
{
    static bool start = false;
    int     i;

     i= -1;
     while (++i < WIDTH * HEIGHT)
     {
         if (isEnnemy(p->map[i], p->nteam))
         {
             start = true;
             return (false);
         }
     }
     return (start);
}

inline int  count(t_player *p)
{
    int     i;
    int     nb;

     i= -1;
     nb = 0;
     while (++i < WIDTH * HEIGHT)
     {
         if (p->map[i] == p->nteam)
            ++nb;
     }
     return (nb);
}
