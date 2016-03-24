/*
** player.c for PSU_2015_lemipc
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Mar 23 17:19:57 2016 Adrien WERY
** Last update	Wed Mar 23 23:02:16 2016 Adrien WERY
*/

#include "lemipc.h"

inline int   getPos(int y, int x)
{
    if (y < 0 || y >= HEIGHT || x < 0 || x >= WIDTH)
        return (-1);
    return (y * WIDTH + x);
}

inline bool isFriend(int friend, int team)
{
    return (friend != 0 && friend != team);
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

inline bool    is_dead(t_player *p)
{
    int     teams[8];
    int	    i;
    int     j;

    teams[0] = (p->x == 0) ? 0 : p->map[getPos(p->y, p->x - 1)];
    teams[1] = (p->x == WIDTH - 1) ? 0 : p->map[getPos(p->y, p->x + 1)];
    teams[2] = (p->y == 0) ? 0 : p->map[getPos(p->y - 1, p->x)];
    teams[3] = (p->y == HEIGHT - 1) ? 0 : p->map[getPos(p->y + 1, p->x)];

    teams[4] = (p->y == 0 || p->x == 0) ? 0 : p->map[getPos(p->y - 1, p->x - 1)];
    teams[5] = (p->y == 0 || p->x == WIDTH - 1) ? 0 : p->map[getPos(p->y - 1, p->x + 1)];
    teams[6] = (p->y == HEIGHT - 1 || p->x == 0) ? 0 : p->map[getPos(p->y + 1, p->x - 1)];
    teams[7] = (p->y == HEIGHT - 1 || p->x == WIDTH - 1) ? 0 : p->map[getPos(p->y + 1, p->x + 1)];
    i = -1;
    while (++i < 8)
    {
        j = i;
        while (++j < 8)
        {
            if (teams[i] != 0 && teams[j] != 0 &&
                p->nteam != teams[i] && teams[i] == teams[j])
                return (true);
        }
    }
    return (false);
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

inline int  countTotal(char *map)
{
    int     i;
    int     nb;

     i= -1;
     nb = 0;
     while (++i < WIDTH * HEIGHT)
     {
         if (map[i] != 0)
            ++nb;
     }
     return (nb);
}
