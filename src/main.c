/*
** main.c for PSU_2015_lemipc
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Sat Mar 19 12:42:51 2016 Adrien WERY
** Last update	Wed Mar 23 19:12:07 2016 Adrien WERY
*/

#include "lemipc.h"

void            run(t_player *p)
{
    struct sembuf   sops;
    t_msg   msg;

    sops.sem_num = 0;
    sops.sem_flg = 0;
    while (!is_dead(p) && !alone(p))
    {
        sops.sem_op = -1;
        semop(p->semID, &sops, 1);
        movePos(p, moveMsg(p));
        usleep(SLEEP);
        sops.sem_op = 1;
        semop(p->semID, &sops, 1);
        if (p->first && display(p->map) != -1)
            break;
    }
    msgrcv(p->msgID, &msg, sizeof(t_msg), p->nteam, MSG_NOERROR | IPC_NOWAIT);
    p->map[getPos(p->y, p->x)] = 0;
}

void    end(void *map)
{
    int     winner;

    while ((winner = display(map)) == -1)
        usleep(SLEEP);
    memset(map, 0, SIZE);
    write(1, "Winner is team ", 15);
    my_putnbr(winner);
    write(1, "\n", 1);
}


int     main(int ac, char **av)
{
    t_player    p;

    if (ac != 3)
    {
        write(1, USAGE, strlen(USAGE));
        return (0);
    }
    if (!init(av[1], &p, atoi(av[2])))
        return (1);
    run(&p);
    // if (p.first)
    //     end(p.map);
    shmctl(p.shmID, IPC_RMID, NULL);
    semctl(p.semID, 0, IPC_RMID);
    msgctl(p.msgID, IPC_RMID, NULL);
}
