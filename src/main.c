/*
** main.c for PSU_2015_lemipc
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Sat Mar 19 12:42:51 2016 Adrien WERY
** Last update	Mon Mar 21 17:00:23 2016 Adrien WERY
*/

# include "lemipc.h"

bool    dead(t_map *map, int pos, int team)
{
    int     teams[8];
    int	    i;
    int     j;
    int     posW;
    int     posH;

    posW = pos % WIDTH;
    posH = pos / HEIGHT;
    teams[0] = (posW == 0) ? 0 : map[pos - 1].nteam;
    teams[1] = (posW == WIDTH - 1) ? 0 : map[pos + 1].nteam;
    teams[2] = (posH == 0) ? 0 : map[pos - HEIGHT].nteam;
    teams[3] = (posH == HEIGHT - 1) ? 0 : map[pos + HEIGHT].nteam;
    teams[4] = (posW == 0 || posH == 0) ? 0 : map[pos - 1 - HEIGHT].nteam;
    teams[5] = (posW == WIDTH - 1 || posH == 0) ? 0 : map[pos + 1 - HEIGHT].nteam;
    teams[6] = (posW == 0 || posH == HEIGHT - 1) ? 0 : map[pos - 1 + HEIGHT].nteam;
    teams[7] = (posW == WIDTH - 1 || posH == HEIGHT - 1) ? 0 : map[pos + 1 + HEIGHT].nteam;
    i = -1;
    while (++i < 8)
    {
        j = i;
        while (++j < 8)
        {
            if (team != teams[i] && teams[i] == teams[j])
                return (true);
        }
    }
    return (false);
}

int         run(t_map *map, int team, bool first)
{
    size_t  pos;
    int     winner;

    pos = rand() % (WIDTH * HEIGHT);
    while (map[pos].nteam != 0)
        pos = rand() % (WIDTH * HEIGHT);
    map[pos].nteam = team;
    while (42)
    {
        if (dead(map, pos, team))
            break;
        if (first && (winner = display(map)) != 0)
            return (winner);
        sleep(1);
    }
    map[pos].nteam = 0;
    return (winner);
}

t_map       *init(char *path, bool *first)
{
    key_t   semkey;
    t_map   *map;

    if ((semkey = ftok(path, KEY_C)) == (key_t) -1)
        return (NULL);
    if (!(map = getMap(semkey)))
        return (NULL);
    srand(time(NULL));
    *first = false;
    if (map[1].x != 1)
    {
        *first = true;
        initMap(map);
    }
    return (map);
}

void    end(t_map *map, int winner)
{
    int i;

    i = -1;
    while (++i < WIDTH * HEIGHT)
    {
        map[i].x = 0;
        map[i].y = 0;
        map[i].nteam = 0;
    }
    while (winner == 0)
    {
        winner = display(map);
        sleep(1);
    }
    write(1, "Winner is team ", 15);
    my_putnbr(winner);
    write(1, "\n", 1);
}

int     main(int ac, char **av)
{
    t_map   *map;
    bool    first;
    int     winner;

    if (ac != 3)
    {
        write(1, USAGE, strlen(USAGE));
        return (0);
    }
    if (!(map = init(av[1], &first)))
        return (1);
    if (atoi(av[2]) == 0) {
        end(map, (winner = 1));
    } else {
        winner = run(map, atoi(av[2]), first);
        if (first)
            end(map, winner);
    }
}
