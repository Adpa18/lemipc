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
# define SLEEP  10000
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

typedef enum {
    ATTACK,
    GROUP
} MoveType;

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

typedef         struct s_msg
{
    long        team;
    MoveType    mt;
    // char    pos;
    // int     nb;
}               t_msg;

void    my_putnbr(int nb);
int     display(char *map);
bool    init(char *path, t_player *p, int team);
void    movePos(t_player *p, int posTo);
int     moveMsg(t_player *p);

bool    isEnnemy(int ennemy, int team);
bool    isFriend(int friend, int team);
int     getPos(int y, int x);
int     countTotal(char *map);
int     count(t_player *p);
bool    is_dead(t_player *p);
bool    alone(t_player *p);


#endif
