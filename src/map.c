/*
** map.c for PSU_2015_lemipc
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Sat Mar 19 19:32:39 2016 Adrien WERY
** Last update	Wed Mar 23 18:36:49 2016 Adrien WERY
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

int         display(char *map)
{
    bool    go;
    int     i;
    int     k;

    go = false;
    write(1, "\n====MAP====\n", 13);
    i = -1;
    k = 0;
    while (++i < HEIGHT * WIDTH)
    {
        if (map[i] != 0)
        {
            if (k != 0 && k != map[i])
                go = true;
            k = map[i];
        }
        my_putnbr(map[i]);
        if (i % WIDTH == WIDTH - 1)
            write(1, "\n", 1);
    }
    if (go)
        return (-1);
    return (k);
}
