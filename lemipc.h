#ifndef LEMIPC_H__
# define LEMIPC_H__

# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <time.h>
# include <stdio.h>

# define KEY_C  'A'
# define WIDTH  10
# define HEIGHT 10
# define USAGE  "./lemipc PATH TEAM_NUMBER\n"

typedef enum { false, true } bool;

enum Colors {
    RED = 1,
    GREEN,
    BLUE,
    YELLOW,
    MAGENTA,
    CYAN,
    WHITE
};
typedef struct s_case t_map;

struct  s_case
{
  int           x;
  int           y;
  int           nteam;
};

void	my_putnbr(int nb);
void    initMap(t_map *map);
t_map   *getMap(int semkey);
int     display(t_map *map);

#endif
