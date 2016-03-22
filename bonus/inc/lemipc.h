#ifndef LEMIPC_H__
# define LEMIPC_H__

# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/sem.h>
# include <sys/msg.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <time.h>
# include <stdio.h>

# define KEY    42
# define WIDTH  100
# define HEIGHT 100
# define SIZE   sizeof(char) * WIDTH * HEIGHT
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
typedef struct s_case t_player;

struct  s_case
{
  int           x;
  int           y;
  int           nteam;
  int           shmID;
  int           semID;
  int           msgID;
  bool          first;
  char          *map;
};

typedef     struct s_msg
{
    long    mtype;
    int     pos;

}           t_msg;

void    my_putnbr(int nb);
int     getPos(int y, int x);
int     display(char *map);
bool    init(char *path, t_player *p, int team);


#endif
