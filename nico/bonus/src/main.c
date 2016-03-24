/*
** main.c for PSU_2015_lemipc
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Sat Mar 19 11:53:43 2016 Adrien WERY
** Last update	Wed Mar 23 13:44:09 2016 Adrien WERY
*/

# include "lemipc-graphic.h"

SDL_Window      *win;
SDL_Renderer    *render;

bool    initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return (false);
    if (!(win = SDL_CreateWindow("LemIPC", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, WIDTH * STEP, HEIGHT * STEP, SDL_WINDOW_SHOWN)))
        return (false);
    if (!(render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED)))
        return (false);
        SDL_SetRenderDrawColor(render, 255, 0, 0, 0);
        SDL_RenderClear(render);
    return (true);
}

void    destroy()
{
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(render);
    SDL_Quit();
}


void        *getMap(char *path, int *shmID, int *semID, int *msgID)
{
    key_t   key;
    void    *ptr;

    if ((key = ftok(path, KEY)) == (key_t) -1)
        return (NULL);
    if ((*shmID = shmget(key, SIZE, IPC_CREAT | SHM_R | SHM_W)) < 0)
        return (NULL);
    if ((ptr = shmat(*shmID, NULL, 0)) == (void*) -1)
        return (NULL);
    if ((*semID = semget(key, 1, IPC_CREAT | SHM_R | SHM_W)) < 0)
        return (NULL);
    if ((*msgID = msgget(key, IPC_CREAT | SHM_R | SHM_W)) < 0)
        return (NULL);
    semctl(*semID, 0, SETVAL, 1);
    return (ptr);
}

void    run(SDL_Renderer *render, char *map)
{
    SDL_Event       event;
    bool            alive;
    int				i;
    SDL_Rect        rect;
    static int      colors[7] = {
        0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF, 0xFFFFFF
    };

    alive = true;
    rect.w = STEP;
    rect.h = STEP;
    while (alive)
    {
        SDL_PollEvent(&event);
        if (event.key.keysym.sym == SDLK_ESCAPE)
            break;
        SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
        SDL_RenderClear(render);
        i = -1;
        while (++i < HEIGHT * WIDTH)
        {
            if (map[i] != 0)
            {
                rect.y = i / HEIGHT * STEP;
                rect.x = i % WIDTH * STEP;
                SDL_SetRenderDrawColor(render, ((colors[map[i] - 1] >> 16) & 255),
                    ((colors[map[i] - 1] >> 8) & 255), ((colors[map[i] - 1]) & 255),
                    ((colors[map[i] - 1] >> 24) & 255));
                SDL_RenderFillRect(render, &rect);
            }
        }
        SDL_RenderPresent(render);
        usleep(1000);
    }
}

int     main(int ac, char **av)
{
    char    *map;
    int     shmID;
    int     semID;
    int     msgID;

    if (ac != 2)
    {
        write(1, "./lemipc-graphic PATH\n", 22);
        return (0);
    }
    if (!(initSDL()))
        return (1);
    if (!(map = getMap(av[1], &shmID, &semID, &msgID)))
        return (1);
    run(render, map);
    destroy();
    shmctl(shmID, IPC_RMID, NULL);
    semctl(semID, 0, IPC_RMID);
    msgctl(msgID, IPC_RMID, NULL);
    return (0);
}
