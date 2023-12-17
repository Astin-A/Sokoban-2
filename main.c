#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0

char map[5][30][30]; // Variable that processes and stores the temp variable in load_map
char nowPlayMap[30][30] = {'\0', }; // Variable to store the map currently being played
int current_player_pos[2]; // Variable to store the player's location
int current_goals = 0; //Number of target points
int current_map_no; // Current map being played
_Bool check_error = 0; // Variable to check for errors
char name[10] = {'\0'}; //Variable that receives the user name
unsigned move_count=0; // Variables to be used in the leaderboard
int numberofmap = 0; // Variable that stores the total number of maps

_Bool is_box_moved = 0; // Variable that stores whether the box has moved

char cmd_history[5] = {'\0'}; //Stack variable that records the movement commands in reverse and stores 5
_Bool box_history[5]; // Variable to store box movement

void selectmap(int imap); // Declare in advance to use the function
void record_history(char move); // Same as above
void ranking(char imap); // Same as above

_Bool check_mapfile(int n,int m) // Check the number of boxes and goal points in the map file, and output an error if the numbers are different.
{
     if(n==m)
         return 1;
     else
         return 0;
}

int getch(void) // Function for using getch() in Linux
{
     int ch;

     struct termios buf;
     struct termios save;

     tcgetattr(0, &save);
     buf = save;

     buf.c_lflag &= ~(ICANON|ECHO);
     buf.c_cc[VMIN] = 1;
     buf.c_cc[VTIME] = 0;

     tcsetattr(0, TCSAFLUSH, &buf);

     ch = getchar();

     tcsetattr(0, TCSAFLUSH, &save);

     return ch;
}

void load_map(void) // Function that loads data from a map file, stores it in temp, processes it, and stores it in map.
{
     FILE *ifp;
     /*
      * temp: Variable that first reads the map file and stores the data
      * imap: map number
      * iy: Map row (Y-axis) number
      * ix: Column (X-axis) number of the map
      * will_load: Variable that sets whether to load characters
      * check_box: Variable that checks the number of boxes in the map file
      * check_goals: Variable that checks the location of goals in the map file
      * check_error: Variable that determines errors by checking the location of boxes and goals on the map
     */
     char temp[5][30][30] = {'\0', };
     int imap = 0, iy = 0, ix = 0, c = 0;
     int check_box=0,check_goals=0;
     _Bool will_load = false;
     ifp = fopen("map", "r");

     if (ifp == NULL)
     {
         check_error = 1;
         printf("Error : Map\n");
         return;
     }// If the file does not exist

     while ((c = getc(ifp)) != EOF)
     {
         switch (c)
         {
             case '1':
                 imap = 0;
                 iy = 0;
                 ix = 0;
                 will_load = false;
                 break;

             case '2':
                 imap = 1;
                 iy = 0;
                 ix = 0;
                 if(!(check_mapfile(check_box,check_goals)))
                     check_error=1;
                 check_box=0;
                 check_goals=0;
                 will_load = false;
                 break;

             case '3':
                 imap = 2;
                 iy = 0;
                 ix = 0;
                 if(!(check_mapfile(check_box,check_goals)))
                     check_error=1;
                 check_box=0;
                 check_goals=0;
                 will_load = false;
                 break;

             case '4':
                 imap = 3;
                 iy = 0;
                 ix = 0;
                 if(!(check_mapfile(check_box,check_goals)))
                     check_error=1;
                 check_box=0;
                 check_goals=0;
                 will_load = false;
                 break;

             case '5':
                 imap = 4;
                 iy = 0;
                 ix = 0;
                 if(!(check_mapfile(check_box,check_goals)))
                     check_error=1;
                 check_box=0;
                 check_goals=0;
                 will_load = false;
                 break;

             case '\n':
                 //The if clause below filters out 1, 2, 3, 4, 5, and n from the map file.
                 if (!will_load)
                     will_load = true;
                 else
                     iy++;
                 ix = 0;
                 break;

             case 'e':
                 if(!(check_mapfile(check_box,check_goals)))
                     check_error=1;
                 check_box=0;
                 check_goals=0;
                 numberofmap = imap;
                 goto load_map_end;

             default:
                 temp[imap][iy][ix] = c;
                 ix++;
                 if(c=='$')
                     check_box++;
                 else if(c=='O')
                     check_goals++;
                 break;
         }
     }

     load_map_end:

     fclose(ifp);
     if(check_error == 1) {
         printf("Error: Map");
         return;
     }

     for (int i1 = 0; i1 <= 4; i1++)
     {
         for (int i2 = 0; i2 <= 29; i2++)
         {
             for (int i3 = 0; i3 <= 29; i3++)
             {
                 if (temp[i1][i2][i3] == '\0')
                     map[i1][i2][i3] = 'C';
                 else
                     map[i1][i2][i3] = temp[i1][i2][i3];
             }
         }
     }
}

int checkXsize(void) // Function to find the X size of the array
{
     int size = 0, iy = 0;

     for (int ix = 0; ix <= 29; ix++)
     {
         if (map[current_map_no][iy][ix] == 'C')
         {
             size = ix;
             break;
         }
     }

     return size;
}

int checkYsize(void) // Function to find the Y size of the array
{
     int size = 0;

     for (int iy = 0; iy <= 29; iy++)
     {
         if (map[current_map_no][iy][0] == 'C')
         {size = iy;
             break;
         }
     }

     return size;
}

void get_player_pos(void) // Function to find the player's position
{
     for (int iy = 0; iy < checkYsize(); iy++)
     {
         for (int ix = 0; ix < checkXsize(); ix++)
         {
             switch(nowPlayMap[iy][ix])
             {
                 case '@':
                     current_player_pos[0] = ix;
                     current_player_pos[1] = iy;
             }
         }
     }
}

void check_goals(void) // Function to check whether the box is in the final position
{
     int goals_achieved = 0;
     for (int iy = 0; iy < checkYsize(); iy++)
     {
         for (int ix = 0; ix < checkXsize(); ix++)
         {
             if ((nowPlayMap[iy][ix] == '$') && (map[current_map_no][iy][ix] == 'O'))
                 goals_achieved++;
         }
     }

     if (goals_achieved == current_goals)
     {
         ranking('S');
         current_map_no++;
         selectmap(current_map_no);
     }
}

void move_player(char move) // Function to move the player
{
     if(map[current_map_no][current_player_pos[1]][current_player_pos[0]]=='O') // If the player's position was originally O (capital o)
     {
         nowPlayMap[current_player_pos[1]][current_player_pos[0]] = 'O'; // Change to O
     }
     else
     {
         nowPlayMap[current_player_pos[1]][current_player_pos[0]] = ' '; // Otherwise change to blank
     }

     switch (move)
     {
         case 'h':// left
             current_player_pos[0]-=1;
             record_history('l');
             break;
         case 'j':// ha
             current_player_pos[1]+=1;
             record_history('k');
             break;
         case 'k':// award
             current_player_pos[1]-=1;
             record_history('j');
             break;
         case 'l' :// right
             current_player_pos[0]+=1;
             record_history('h');
             break;
     }
  
     // Take a new picture of the golbaengi location
     nowPlayMap[current_player_pos[1]][current_player_pos[0]] = '@';

     check_goals(); //Check whether a goal is scored each time the player moves
}

void move_box(char c) // Check if a box exists in front of the player's movement direction. If there is a wall or another box in front of the box, it will not move.
{
     int move_x = 0, move_y = 0;

     switch (c)
     {
         case 'h':// left
             move_x = -2;
             break;
         case 'j':// ha
             move_y = 2;
             break;
         case 'k':// award
             move_y = -2;
             break;
         case 'l' :// right
             move_x = 2;
             break;
     }

     is_box_moved = nowPlayMap[current_player_pos[1]+move_y][current_player_pos[0]+move_x] != '#' && nowPlayMap[current_player_pos[1]+move_y][current_player_pos[0]+move_x] != '$';
     if (is_box_moved)
     {
         nowPlayMap[current_player_pos[1]+move_y][current_player_pos[0]+move_x] = '$';
         move_player(c);
     }
}

void decide_move(char c) //Function to check the object in front and decide whether to move
{
     switch (c)
     {
         case 'h':// left
             if (nowPlayMap[current_player_pos[1]][current_player_pos[0]-1] != '#')
             {
                 if(nowPlayMap[current_player_pos[1]][current_player_pos[0]-1] == '$')
                 {
                     move_box(c);
                 }
                 else
                 {
                     move_player(c);
                 }
             }
             break;
         case 'j':// ha
             if (nowPlayMap[current_player_pos[1]+1][current_player_pos[0]] != '#')
             {
                 if (nowPlayMap[current_player_pos[1]+1][current_player_pos[0]] == '$')
                 {
                     move_box(c);
                 }
                 else
                 {
                     move_player(c);
                 }
             }
             break;
         case 'k':// award
             if (nowPlayMap[current_player_pos[1]-1][current_player_pos[0]] != '#')
             {
                 if (nowPlayMap[current_player_pos[1]-1][current_player_pos[0]] == '$')
                 {
                     move_box(c);
                 }
                 else
                 {
                     move_player(c);
                 }
             }
             break;
         case 'l' :// right
             if (nowPlayMap[current_player_pos[1]][current_player_pos[0]+1] != '#')
             {
                 if (nowPlayMap[current_player_pos[1]][current_player_pos[0]+1] == '$')
                 {
                     move_box(c);
                 }
                 else
                 {
                     move_player(c);
                 }
             }
             break;
     }
}

void printmap(int imap) // Prints the map currently being played
{
     system("clear");
     printf("\nHello %s\n\n",name);
     for (int iy = 0; iy < checkYsize(); iy++)
     {
         for (int ix = 0; ix < checkXsize(); ix++)
         {
             printf("%c", nowPlayMap[iy][ix]);
         }
         printf("\n");
     }
}

void selectmap(int imap) // select map to play
{
     for (int i = 0; i <= 4; ++i)
         cmd_history[i] = box_history[i] = '\0';

     for (int i1 = 0; i1 <= 29; i1++)
     {
         for (int i2 = 0; i2 <= 29; i2++)
             nowPlayMap[i1][i2] = '\0';
     }


     current_goals = 0;

     for (int iy = 0; iy < checkYsize(); iy++)
     {
         for (int ix = 0; ix < checkXsize(); ix++)
         {
             nowPlayMap[iy][ix] = map[imap][iy][ix];
             if(map[imap][iy][ix]=='.')
                 nowPlayMap[iy][ix]=' '; // fill with spaces
             //Counting target point
             if (nowPlayMap[iy][ix] == 'O')
                 current_goals++;
         }
     }

     get_player_pos();
}

void newgame(int imap) // Function to restart the game
{
     selectmap(imap);
     printmap(imap);
}

void record_history(char move) //Function to record player's movements
{
     //When a general command is input
     for (int i = 0; i <= 3; ++i)
     {
         cmd_history[i] = cmd_history[i+1];
         box_history[i] = box_history[i+1];
     }

     cmd_history[4] = move;
     box_history[4] = is_box_moved;
}

void undo(void) // Function that performs the undo function
{
     //user location undo
     if(map[current_map_no][current_player_pos[1]][current_player_pos[0]]=='O') // If the player's position was originally O (capital o)
     {
         nowPlayMap[current_player_pos[1]][current_player_pos[0]] = 'O'; // Change to O
     }
     else
     {
         nowPlayMap[current_player_pos[1]][current_player_pos[0]] = ' '; // Otherwise change to blank
     }

     switch (cmd_history[4])
     {
         case 'h':// left
             current_player_pos[0]-=1;
             break;
         case 'j':// ha
             current_player_pos[1]+=1;
             break;
         case 'k':// award
             current_player_pos[1]-=1;
             break;
         case 'l' :// right
             current_player_pos[0]+=1;
             break;
     }
  
     // Take a new picture of the golbaengi location
     nowPlayMap[current_player_pos[1]][current_player_pos[0]] = '@';

     //box location undo
     if (box_history[4])
     {
         switch (cmd_history[4])
         {
             case 'h':// left
                 if(map[current_map_no][current_player_pos[1]][current_player_pos[0]+2] == 'O')
                     nowPlayMap[current_player_pos[1]][current_player_pos[0]+2] = 'O';
                 else
                     nowPlayMap[current_player_pos[1]][current_player_pos[0]+2] = ' ';
                 nowPlayMap[current_player_pos[1]][current_player_pos[0]+1] = '$';
                 break;
             case 'j':// ha
                 if(map[current_map_no][current_player_pos[1]-2][current_player_pos[0]] == 'O')
                     nowPlayMap[current_player_pos[1]-2][current_player_pos[0]] = 'O';
                 else
                     nowPlayMap[current_player_pos[1]-2][current_player_pos[0]] = ' ';
                 nowPlayMap[current_player_pos[1]-1][current_player_pos[0]] = '$';
                 break;
             case 'k':// award
                 if(map[current_map_no][current_player_pos[1]+2][current_player_pos[0]] == 'O')
                     nowPlayMap[current_player_pos[1]+2][current_player_pos[0]] = 'O';
                 else
                     nowPlayMap[current_player_pos[1]+2][current_player_pos[0]] = ' ';
                 nowPlayMap[current_player_pos[1]+1][current_player_pos[0]] = '$';
                 break;
             case 'l' :// right
                 if(map[current_map_no][current_player_pos[1]][current_player_pos[0]-2] == 'O')
                     nowPlayMap[current_player_pos[1]][current_player_pos[0]-2] = 'O';
                 else
                     nowPlayMap[current_player_pos[1]][current_player_pos[0]-2] = ' ';
                 nowPlayMap[current_player_pos[1]][current_player_pos[0]-1] = '$';
                 break;
         }
     }

     printmap(current_map_no);

     for (int i = 3; i >= 0; --i)
     {
         cmd_history[i+1] = cmd_history[i];
         box_history[i+1] = box_history[i];
     }

     cmd_history[0] = '\0';
     box_history[0] = '\0';
}

void ranking(char imap) // Function that performs the ranking function
{
     FILE *ifp;
     ifp = fopen("ranking", "r");

     char c = 0;
     char Read[5][6][100] = {'\0',};
     int ReadRank[5][5] = {'\0',};
     char ReadName[5][5][20] = {'\0',};
     int rankint = 0, rankstart = 0, rankend = 0;
     int first = 0, i1 = 0, i2 = 0, i3 = 0;
     // first: First check variable, i1: Map designation variable, i2: Ranking variable for that map, i3: Variable for player of that rank and number of moves

     if (ifp == NULL) // If the file does not exist,,
     {
         fclose(ifp);
         ifp = fopen("ranking", "w+");
         fprintf(ifp, "%%map1\n"
                      " :0^\n"
                      " :0^\n"
                      " :0^\n"
                      " :0^\n"
                      " :0^\n"
                      "%%map2\n"
                      " :0^\n"
                      " :0^\n"
                      " :0^\n"
                      " :0^\n"
                      " :0^\n"
                      "%%map3\n"
                      " :0^\n"
                      " :0^\n"
                      " :0^\n"
                      " :0^\n"
                      " :0^\n"
                      "%%map4\n"
                      " :0^\n"
                      " :0^\n"
                      " :0^\n"
                      " :0^\n"
                      " :0^\n"
                      "%%map5\n"
                      " :0^\n"
                      " :0^\n"
                      " :0^\n"
                      " :0^\n"
                      " :0^");
         fclose(ifp);
         ifp = fopen("ranking", "r");
     }


     while ((c = getc(ifp)) != EOF)
     {
         switch (c)
         {
             case '\n':
                 if (i2 == 5)
                 {
                     i2 = 0;
                     i3 = 0;
                 }
                 else
                 {
                     i2++;
                     i3 = 0;
                 }

                 break;

             case '%':
                 if (first == 0)
                     first = 1;
                 else
                 {
                     i1++;
                     i2 = 0;
                     i3 = 0;
                 }
                 break;

             case ':':
                 rankstart = i3;
                 Read[i1][i2][i3] = c;
                 i3++;
                 break;

             case '^':
                 rankend = i3;
                 // 0: 48
                 // 1: 49
                 // 2: 50
                 // 3: 51
                 // 4: 52
                 // 5: 53
                 //6:54
                 //7:55
                 //8:56
                 //9:57

                 int ten = rankend - rankstart - 2;
                 int tempten = 1;
                 for (int i = 1; i <= ten; i++)
                     tempten *= 10;
                 ten = tempten;

                 for (int i = rankstart + 1; i <= rankend - 1; i++) {
                     int temp = Read[i1][i2][i] - 48; // Char to int
                     rankint += temp * ten;
                     ten /= 10;
                 }

                 ReadRank[i1][i2 - 1] = rankint;
                 rankint = 0;
                 break;

             default:
                 Read[i1][i2][i3] = c;
                 i3++;
                 break;
         }
     }
     fclose(ifp);

     for (i1 = 0; i1 <= 4; i1++)
     {
         for (i2 = 1; i2 <= 5; i2++)
         {
             for (i3 = 0; i3 <= 100; i3++)
             {
                 char temp = Read[i1][i2][i3];
                 if (temp == ' ')
                 {
                     ReadName[i1][i2-1][i3] = ' ';
                     break;
                 }
                 else if (temp == ':')
                     break;
                 else
                     ReadName[i1][i2-1][i3] = Read[i1][i2][i3];
             }
         }
     }




     if (imap == '\n') // enter only t
     {
         system("clear");
         for (i1 = 0; i1 <= 4; i1++)
         {
             printf("map%d\n", i1+1);
             for (i2 = 0; i2 <= 4; i2++)
             {
                 if (ReadName[i1][i2][0] != ' ')
                     printf("%s : %d\n", ReadName[i1][i2], ReadRank[i1][i2]);
             }
             printf("\n");
         }
     }
     else if (imap == '1') // input t1
     {
         getch();
         system("clear");
         i1 = 0;
         printf("map%d\n", i1+1);
         for (i2 = 0; i2 <= 4; i2++)
         {
             if (ReadName[i1][i2][0] != ' ')
                 printf("%s : %d\n", ReadName[i1][i2], ReadRank[i1][i2]);
         }
         printf("\n");
     }
     else if (imap == '2') // t2 input
     {
         getch();
         system("clear");
         i1 = 1;
         printf("map%d\n", i1+1);
         for (i2 = 0; i2 <= 4; i2++)
         {
             if (ReadName[i1][i2][0] != ' ')
                 printf("%s : %d\n", ReadName[i1][i2], ReadRank[i1][i2]);
         }
         printf("\n");
     }
     else if (imap == '3') // t3 input
     {
         getch();
         system("clear");
         i1 = 2;
         printf("map%d\n", i1+1);
         for (i2 = 0; i2 <= 4; i2++)
         {
             if (ReadName[i1][i2][0] != ' ')
                 printf("%s : %d\n", ReadName[i1][i2], ReadRank[i1][i2]);
         }
         printf("\n");
     }
     else if (imap == '4') // t4 input
     {
         getch();
         system("clear");
         i1 = 3;
         printf("map%d\n", i1+1);
         for (i2 = 0; i2 <= 4; i2++)
         {
             if (ReadName[i1][i2][0] != ' ')
                 printf("%s : %d\n", ReadName[i1][i2], ReadRank[i1][i2]);
         }
         printf("\n");
     }
     else if (imap == '5') // t5 input
     {
         getch();
         system("clear");
         i1 = 4;
         printf("map%d\n", i1+1);
         for (i2 = 0; i2 <= 4; i2++)
         {
             if (ReadName[i1][i2][0] != ' ')
                 printf("%s : %d\n", ReadName[i1][i2], ReadRank[i1][i2]);
         }
         printf("\n");
     }
     else if (imap == 'S') // Save
     {
         //name , //move_count
         //current_map_no


         for (i2 = 0; i2 <= 4; i2++)
         {
             if ((ReadRank[current_map_no][i2] > move_count) || (ReadRank[current_map_no][i2] == 0))
             {
                 if (i2 == 4)
                 {
                     ReadRank[current_map_no][i2] = move_count;
                     for (int i = 0; i <= 19; i++)
                     {
                         if (i >= 10)
                             ReadName[current_map_no][i2][i] = '\0';
                         else
                             ReadName[current_map_no][i2][i] = name[i];
                     }
                 }
                 else
                 {for (int i = 0; i <= 19; i++) // initialize the contents of the last rank
                         ReadName[current_map_no][4][i] = '\0';
                     ReadRank[current_map_no][4] = 0;

                     for (int i = 3; i >= i2; i--) // Decrease the rankings one by one -> Up to the newly updated ranking
                     {
                         for (int a = 0; a <= 19; a++)
                             ReadName[current_map_no][i + 1][a] = ReadName[current_map_no][i][a];
                         ReadRank[current_map_no][i+1] = ReadRank[current_map_no][i];
                     }

                     ReadRank[current_map_no][i2] = move_count; // Insert new ranking
                     for (int i = 0; i <= 19; i++)
                     {
                         if (i >= 10)
                             ReadName[current_map_no][i2][i] = '\0';
                         else
                             ReadName[current_map_no][i2][i] = name[i];
                     }
                 }
                 break;
             }
         }


         FILE *ofp;
         ofp = fopen("ranking", "w+");

         for (i1 = 0; i1 <= 4; i1++)
         {
             fprintf(ofp, "%%map%d\n", i1+1);
             for (i2 = 0; i2 <= 4; i2++)
             {
                 fprintf(ofp, "%s:%d^\n", ReadName[i1][i2], ReadRank[i1][i2]);
             }
         }
         fclose(ofp);

         move_count = 0;
     }
}

void save(void) // Function that performs the save function
{
     FILE *ofp;

     ofp = fopen("sokoban","w+");
     fprintf(ofp, "%s\t%d\t%d\t%d\t%d\t%d\n", name, move_count, current_map_no, current_goals, current_player_pos[0], current_player_pos[1]);

     for (int i = 0; i <= checkYsize(); i++)
         fprintf(ofp, "%s\n", nowPlayMap[i]);

     fclose(ofp);
     printmap(current_map_no);
     printf("Save Complete!");
}

void load(void) // Function that performs the load function
{
     FILE *ifp;

     ifp = fopen("sokoban","r");

     if (ifp == NULL)
     {
         printmap(current_map_no);
         printf("Load Fail : No Save");
         fclose(ifp);
         return;
     }

     for (int i = 0; i <= 9; i++)
         name[i] = '\0';

     move_count = 0, current_map_no = 0, current_goals = 0, current_player_pos[0] = 0, current_player_pos[1] = 0;

     for (int i = 0; i <= 4; ++i)
         cmd_history[i] = box_history[i] = '\0';

     for (int i1 = 0; i1 <= 29; i1++)
     {
         for (int i2 = 0; i2 <= 29; i2++)
             nowPlayMap[i1][i2] = '\0';
     }


     fscanf(ifp, "%s\t%d\t%d\t%d\t%d\t%d", name, &move_count, &current_map_no, &current_goals, &current_player_pos[0], &current_player_pos[1]);

     //selectmap(current_map_no);


     char c;
     int i1 = 0, i2 = 0, first = 0;
     while ((c = getc(ifp)) != EOF)
     {
         switch (c)
         {
             case '\n':
                 if (first == 0)
                     first = 1;
                 else
                 {
                     i1++;
                     i2 = 0;
                 }
                 break;

             default:
                 nowPlayMap[i1][i2] = c;
                 i2++;
                 break;

         }
     }

     fclose(ifp);
     printmap(current_map_no);
     printf("Load Complete!");
}

void display_help(void) // Function that performs the display help function
{
     system("clear");
     printf("h(left), j(bottom), k(top), l(right)\n"
            "u(undo)\n"
            "r(replay)\n"
            "n(new)\n"
            "e(exit)\n"
            "s(save)\n"
            "f(file load)\n"
            "d(display help)\n"
            "t(top)");
}

int main(void) // main function of the program
{
     char command;

     load_map();

     if (check_error == 1)
         return 0;

     printf("Please enter your name: ");
     scanf("%10s", name);

     current_map_no = 0;
     selectmap(current_map_no);
     printmap(current_map_no);
     printf("\nMove_count:%d\n", move_count);

     while(1)
     {
         // Assuming map file number 1, map selection function will be added in the future
         int noinfor = 0;
         command = getch();
         switch(command)
         {
             case 'h':
             case 'j':
             case 'k':
             case 'l':
                 get_player_pos();
                 move_count++;
                 decide_move(command);
                 printmap(current_map_no);
                 break;

             case 'u':
                 get_player_pos();
                 undo();
                 move_count++;
                 break;

             case 'r':
                 newgame(current_map_no);
                 break;

             case 'n':
                 current_map_no = 0;
                 newgame(0);
                 move_count=0;
                 break;

             case 's':
                 save();
                 break;

             case 'f':
                 load();
                 break;

             case 'd':
                 display_help();
                 noinfor = 1;
                 break;

             case 't':
                 command = getch();
                 ranking(command);
                 noinfor = 1;
                 break;

             case 'e':
                 save();
                 goto end;
                 break;

             case '\n':
                 printmap(current_map_no);break;

             default:
                 noinfor = 1;
                 break;
         }

         if (noinfor == 1)
             continue;

         if (current_map_no > numberofmap)
         {
             system("clear");
             printf("You Win!!\n");
             printf("See You %s\n", name);
             return 0;
         }

         printf("\nMove_count:%d\n", move_count);
     }

     end:
     system("clear");
     printf("\nSee You %s\n", name);
     return 0;
}