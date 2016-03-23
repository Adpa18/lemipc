/*
** init.c for PSU_2015_lemipc
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Tue Mar 22 17:31:26 2016 Adrien WERY
** Last update	Wed Mar 23 18:31:18 2016 Adrien WERY
*/

#include "lemipc.h"

bool        init_pos(t_player *p, int team)
{
    int     i;

    i = -1;
    while (++i < WIDTH * HEIGHT)
        if (p->map[i] == 0)
            break;
    if (i == WIDTH * HEIGHT)
        return (false);
    p->y = rand() % HEIGHT;
    p->x = rand() % WIDTH;
    if ((p->nteam = team) == 0)
        return (false);
    while (p->map[getPos(p->y, p->x)] != 0)
    {
        p->y = rand() % HEIGHT;
        p->x = rand() % WIDTH;
    }
    p->map[getPos(p->y, p->x)] = team;
    return (true);
}

bool        init_first(key_t key, t_player *p)
{
    p->first = true;
    if (((p->shmID = shmget(key, SIZE, IPC_CREAT | SHM_R | SHM_W)) < 0))
        return (false);
    if ((p->map = shmat(p->shmID, NULL, 0)) == (void*) -1)
        return (false);
    if ((p->semID = semget(key, 1, IPC_CREAT | SHM_R | SHM_W)) < 0)
        return (false);
    if ((p->msgID = msgget(key, IPC_CREAT | SHM_R | SHM_W)) < 0)
        return (false);
    memset(p->map, 0, SIZE);
    semctl(p->semID, 0, SETVAL, 1);
    return (true);
}

bool        init(char *path, t_player *p, int team)
{
    key_t   key;

    if ((key = ftok(path, KEY)) == (key_t) -1)
        return (false);
    if ((p->shmID = shmget(key, SIZE, SHM_R | SHM_W)) < 0)
    {
        if (!init_first(key, p))
            return (false);
    }
    else
    {
        p->first = false;
        if ((p->map = shmat(p->shmID, NULL, 0)) == (void*) -1)
            return (false);
        if ((p->semID = semget(key, 1, SHM_R | SHM_W)) < 0)
            return (false);
        if ((p->msgID = msgget(key, SHM_R | SHM_W)) < 0)
            return (false);
    }
    if (!init_pos(p, team))
        return (false);
    srand(time(NULL));
    return (true);
}
