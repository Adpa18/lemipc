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
  bool          caps;
  char          *map;
};

typedef     struct s_msg
{
    long    mtype;
    int     pos;
    // int     nb;
    // MoveType    mt;

}           t_msg;


void    my_putnbr(int nb);
int     display(char *map);
bool    init(char *path, t_player *p, int team);
void    move(t_player *p, struct sembuf *sops);

void		movePos(t_player *p, int posTo);
int			avoid(t_player *p);
int			attack(t_player *p, int len);
int			checkChance(t_player *p, int len, int nb_e, int nb_a);
bool    isEnnemy(int ennemy, int team);
int     getPos(int y, int x);
bool    alone(t_player *p);
int     count(t_player *p);


#endif
