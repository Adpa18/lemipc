/*
** main.c for PSU_2015_lemipc
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Sat Mar 19 11:53:43 2016 Adrien WERY
** Last update	Mon Mar 21 17:08:48 2016 Adrien WERY
*/

# include "lemipc-graphic.h"

SDL_Window      *win;
SDL_Renderer    *render;

bool    initSDL()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
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


t_map       *getMap(int semkey)
{
    int     memID;
    void    *ptr;

    if ((memID = shmget(semkey, sizeof(t_map) * WIDTH * HEIGHT, 0444)) < 0)
        return (NULL);
    if ((ptr = shmat(memID, NULL, 0)) == (void*) -1)
        return (NULL);
    shmctl(memID, IPC_RMID, NULL);
    return (ptr);
}

void    run(SDL_Renderer *render, t_map *map)
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
        while (++i < WIDTH * HEIGHT)
        {
            if (map[i].nteam != 0)
            {
                rect.x = map[i].x * STEP;
                rect.y = map[i].y * STEP;
                SDL_SetRenderDrawColor(render, ((colors[map[i].nteam - 1] >> 16) & 255),
                ((colors[map[i].nteam - 1] >> 8) & 255), ((colors[map[i].nteam - 1]) & 255),
                ((colors[map[i].nteam - 1] >> 24) & 255));
                SDL_RenderFillRect(render, &rect);
            }
        }
        SDL_RenderPresent(render);
        usleep(1000);
    }
}

int     main(int ac, char **av)
{
    t_map   *map;
    key_t   semkey;

    if (ac != 2)
    {
        write(1, "./lemipc-graphic PATH\n", 22);
        return (0);
    }
    if ((semkey = ftok(av[1], KEY_C)) == (key_t) -1)
        return (1);
    if (!(initSDL()))
        return (1);
    if (!(map = getMap(semkey)))
        return (1);
    run(render, map);
    destroy();
}
