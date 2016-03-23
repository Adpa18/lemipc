/*
** main.c for PSU_2015_lemipc
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Sat Mar 19 12:42:51 2016 Adrien WERY
** Last update	Wed Mar 23 18:10:44 2016 Nicolas Constanty
*/

#include "lemipc.h"

bool    is_dead(t_player *p)
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

int         run(t_player *p)
{
    int         winner;
    struct sembuf   sops;

    winner = 0;
    sops.sem_num = 0;
    sops.sem_flg = 0;
    while (42)
    {
        if (is_dead(p) || alone(p))
            break;
        move(p, &sops);
        if (p->first && (winner = display(p->map)) != 0)
            return (winner);
    }
    p->map[getPos(p->y, p->x)] = 0;
    return (winner);
}

void    end(void *map, int winner)
{
    memset(map, 0, SIZE);
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
    t_player    p;
    int         winner;

    if (ac != 3)
    {
        write(1, USAGE, strlen(USAGE));
        return (0);
    }
    if (!init(av[1], &p, atoi(av[2])))
        return (1);
    winner = run(&p);
    if (p.first)
        end(p.map, winner);
    shmdt(p.map);
    shmctl(p.shmID, IPC_RMID, NULL);
    semctl(p.semID, 0, IPC_RMID);
    msgctl(p.msgID, IPC_RMID, NULL);
    return (0);
}
