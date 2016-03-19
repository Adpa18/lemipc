/*
** map.c for PSU_2015_lemipc
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Sat Mar 19 19:32:39 2016 Adrien WERY
** Last update	Sat Mar 19 19:46:09 2016 Adrien WERY
*/

#include "lemipc.h"

void	my_putnbr(int nb)
{
    char    c;

    if (nb / 10 > 0)
        my_putnbr(nb / 10);
    c = nb % 10 + 48;
    write(1, &c, 1);
}

void    initMap(t_map *map)
{
    int i;

    i = -1;
    while (++i < WIDTH * HEIGHT)
    {
        map[i].x = i % WIDTH;
        map[i].y = i / HEIGHT;
        map[i].nteam = 0;
    }
}

t_map       *getMap(int semkey)
{
    int     memID;
    void    *ptr;

    if ((memID = shmget(semkey, sizeof(t_map) * WIDTH * HEIGHT, 0666 | IPC_CREAT)) < 0)
        return (NULL);
    if ((ptr = shmat(memID, NULL, 0)) == (void*) -1)
        return (NULL);
    return (ptr);
}

int         display(t_map *map)
{
    bool    go;
    int     i;
    int     j;

    i = -1;
    j = 0;
    go = false;
    write(1, "\n====MAP====\n", 13);
    while (++i < WIDTH * HEIGHT)
    {
        if (j != 0 && j != map[i].nteam)
            go = true;
        else
            j = map[i].nteam;
        my_putnbr(map[i].nteam);
        if (i % WIDTH == WIDTH - 1)
            write(1, "\n", 1);
    }
    if (go)
        return (0);
    return (j);
}
