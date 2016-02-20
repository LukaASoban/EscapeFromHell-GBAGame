/* General Settings */
#define NUM_RECS 24
#define RECWIDTH 10
#define RECHEIGHT 20

/* Game States */
#define	TITLE 0
#define	NORMAL 1
#define GAMEOVER 2

/* Structure representing the red wall
	where thee rectangles are 10 x 10 */
typedef struct {

	u16 color;
	int xPos;
	int yPos;
	int width;
	int height;
} WALL;

/* Structure representing Gary the Ghost */
typedef struct {

	int xPos;
	int yPos;
	int width;
	int height;
	const u16* image;

} GARY;

/* External Variables */
extern int state;
extern int playedOnce;
extern float upCount;
extern int goingUp;
extern int speed;
extern int acceleration;


extern WALL nextWall;
extern WALL currentWall;
extern WALL walls[NUM_RECS];
extern WALL topWall[NUM_RECS];
extern WALL bottomWall[NUM_RECS];
extern WALL middleWall[3];
extern WALL oldMidWall[3];
extern WALL oldTopWall[NUM_RECS];
extern WALL oldBottomWall[NUM_RECS];
extern GARY gary;
extern GARY oldGary;

void startGame();
void endGame();
void initiate();
int randomMidHeight();
void runGame();
void updateBarriers();
void lastRec();
void moveDown();
void moveUp();
void animator(int destination_row, int destination_col);
void updateGary();
int isHit();
int isInMidRange(int n);
void jump();
void goScreen();