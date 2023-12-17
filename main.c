#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0



char map[5][30][30]; //* Variable that processes and stores the temp variable in load_map
char nowPlayMap[30][30] = {NULL, }; //* Variable to store the map currently being played
int current_player_pos[2]; //* Variable to store the player's location
int current_goals = 0; //* Number of target points
int current_map_no;
_Bool check_error = 0, is_undoing;
char name[10] = {'\0'}; //* Variable that receives the user name
int move_count=0; //* Variables to be used in the leaderboard

_Bool check_mapfile(int n,int m) //* Check the number of boxes and goal points in the map file, and output an error if the numbers are different.
{
    if(n==m)
        return 1;
    else
        return 0;
}

void selectmap(int imap);
void record_history(char move);

int getch(void) //* Function for using getch() in Linux
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



void load_map(void){ //* Function to load data from a map file, store it in temp, process it, and save it in map

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
    char temp[5][30][30] = {NULL, };
    int imap = 0, iy = 0, ix = 0, c = 0;
    int check_box=0,check_goals=0;
    _Bool will_load = false;
    ifp = fopen("map", "r");

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
                //* The if clause below filters out 1, 2, 3, 4, 5, and n from the map file.
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
    if(check_error==1) {
        printf("Error");
        return;
    }


    for (int i1 = 0; i1 <= 4; i1++)
    {
        for (int i2 = 0; i2 <= 29; i2++)
        {
            for (int i3 = 0; i3 <= 29; i3++)
            {
                if (temp[i1][i2][i3] == NULL)
                    map[i1][i2][i3] = 'C';
                else
                    map[i1][i2][i3] = temp[i1][i2][i3];
            }
        }
    }
}

int checkXsize(int imap) //* Function to find the X size of the array
{
    int size = 0, iy = 0;

    for (int ix = 0; ix <= 29; ix++)
    {
        if (map[imap][iy][ix] == 'C')
        {
            size = ix;
            break;
        }
    }

    return size;
}


int checkYsize(int imap, int Xsize) //* Function to find the Y size of the array
{
    int size = 0;
    int ix = Xsize - 1;

    for (int iy = 0; iy <= 29; iy++)
    {
        if (map[imap][iy][ix] == 'C')
        {
            size = iy;
            break;
        }
    }

    return size;
}


void get_player_pos(void) //* Function to find the player's location
{
    for (int iy = 0; iy < checkYsize(current_map_no, checkXsize(current_map_no)); iy++)
    {
        for (int ix = 0; ix < checkXsize(current_map_no); ix++)
        {
            switch(nowPlayMap[iy][ix])
            {
                case '@':
                    current_player_pos[0] = ix;
                    current_player_pos[1] = iy;
            }
        }
        printf("\n");
    }
}

void check_goals(int imap)
{
    int goals_achieved = 0;
    for (int iy = 0; iy < checkYsize(imap, checkXsize(imap)); iy++)
    {
        for (int ix = 0; ix < checkXsize(imap); ix++)
        {
            if ((nowPlayMap[iy][ix] == '$') && map[imap][iy][ix] == 'O')
                goals_achieved ++;
        }
    }

    printf("GOALS ACHIEVED: %d\nCURR GOALS: %d\n", goals_achieved, current_goals);

    if (goals_achieved == current_goals)
        selectmap(++current_map_no);
}

void move_player(char move,int imap) //* Function to move the player
{
    
    if(map[imap][current_player_pos[1]][current_player_pos[0]]=='O') // If the player's position was originally O (uppercase o)
    {
        nowPlayMap[current_player_pos[1]][current_player_pos[0]] = 'O'; // Change to O
    }
    else
    {
        nowPlayMap[current_player_pos[1]][current_player_pos[0]] = '.'; // Otherwise change to .
    }

    switch (move)
    {
        case 'h':// left
            current_player_pos[0]-=1;
            if (!is_undoing) record_history('l');
            break;
        case 'j':// under
            current_player_pos[1]+=1;
            if (!is_undoing) record_history('k');
            break;
        case 'k':// top
            current_player_pos[1]-=1;
            if (!is_undoing) record_history('j');
            break;
        case 'l' :// right 
            current_player_pos[0]+=1;
            if (!is_undoing) record_history('h');
            break;
    }
    // Take a new picture of the golbaengi location
    nowPlayMap[current_player_pos[1]][current_player_pos[0]] = '@'; // Express player movement

    check_goals(imap); //! Check whether there is a goal every time the box moves
}

_Bool is_box_moved = 0;

void move_box(char c,int imap) //* 1. Check whether a box exists in front of the player's direction of movement. 2. If there is a wall or another box in front of the box, it will not move.
{
    //check dollar front
    switch (c)
    {
        case 'h':// left
            if (nowPlayMap[current_player_pos[1]][current_player_pos[0]-2] != '#' && nowPlayMap[current_player_pos[1]][current_player_pos[0]-2] != '$' )
            {
                nowPlayMap[current_player_pos[1]][current_player_pos[0]-2] = '$';
                if (!is_undoing) is_box_moved = 1;
                move_player(c,imap);
            }else
            {
                if (!is_undoing) is_box_moved = 0;
            }
            break;
        case 'j':// under
            if ((nowPlayMap[current_player_pos[1]+2][current_player_pos[0]] != '#' && nowPlayMap[current_player_pos[1]+2][current_player_pos[0]] != '$' ))
            {
                nowPlayMap[current_player_pos[1]+2][current_player_pos[0]] = '$';
                if (!is_undoing) is_box_moved = 1;;
                move_player(c,imap);
            }else
            {
                if (!is_undoing) is_box_moved = 0;
            }
            break;
        case 'k':// top
            if ((nowPlayMap[current_player_pos[1]-2][current_player_pos[0]] != '#' && nowPlayMap[current_player_pos[1]-2][current_player_pos[0]] != '$' ))
            {
                nowPlayMap[current_player_pos[1]-2][current_player_pos[0]] = '$';
                if (!is_undoing) is_box_moved = 1;
                move_player(c,imap);
            }else
            {
                if (!is_undoing) is_box_moved = 0;
            }
            break;
        case 'l' :// right
            if ((nowPlayMap[current_player_pos[1]][current_player_pos[0]+2] != '#' && nowPlayMap[current_player_pos[1]][current_player_pos[0]+2] != '$' ))
            {
                nowPlayMap[current_player_pos[1]][current_player_pos[0]+2] = '$';
                if (!is_undoing) is_box_moved = 1;
                move_player(c,imap);
            }else
            {
                if (!is_undoing) is_box_moved = 0;
            }
            break;
    }
    if (!is_undoing) is_box_moved = 0;
}
void decide_move(char c,int imap) //* Function to check the object in front and decide whether to move or not
{
    switch (c)
    {
        case 'h':// left
            if (nowPlayMap[current_player_pos[1]][current_player_pos[0]-1] != '#')
            {
                if(nowPlayMap[current_player_pos[1]][current_player_pos[0]-1] == '$') {
                    move_box(c,imap);
                }
                else
                {
                    move_player(c,imap);
                }
            }
            break;
        case 'j':// under
            if (nowPlayMap[current_player_pos[1]+1][current_player_pos[0]] != '#')
            {
                if (nowPlayMap[current_player_pos[1]+1][current_player_pos[0]] == '$')
                {
                    move_box(c,imap);
                }
                else
                {
                    move_player(c,imap);
                }
            }
            break;
        case 'k':// top
            if (nowPlayMap[current_player_pos[1]-1][current_player_pos[0]] != '#')
            {
                if (nowPlayMap[current_player_pos[1]-1][current_player_pos[0]] == '$')
                {
                    move_box(c,imap);
                }
                else
                {
                    move_player(c,imap);
                }
            }
            break;
        case 'l' :// right
            if (nowPlayMap[current_player_pos[1]][current_player_pos[0]+1] != '#')
            {
                if (nowPlayMap[current_player_pos[1]][current_player_pos[0]+1] == '$')
                {
                    move_box(c,imap);
                }
                else
                {
                    move_player(c,imap);
                }
            }
            break;
    }
}





void printmap(int imap) //* Prints the map currently being played
{
    system("clear");
    for (int iy = 0; iy < checkYsize(imap, checkXsize(imap)); iy++)
    {
        for (int ix = 0; ix < checkXsize(imap); ix++)
        {
            printf("%c", nowPlayMap[iy][ix]);
        }
        printf("\n");
    }
    printf("\n\n");
}

char cmd_history[5] = {'\0'}; //Stack variable that records the movement commands in reverse and stores 5
_Bool box_history[5];

void selectmap(int imap) // Select the map to play
{
    for (int i = 0; i <= 4; ++i)
        cmd_history[i] = box_history[i] = '\0';

    for (int iy = 0; iy < checkYsize(imap, checkXsize(imap)); iy++)
    {
        for (int ix = 0; ix < checkXsize(imap); ix++)
        {
            nowPlayMap[iy][ix] = map[imap][iy][ix];

            //Counting target point
            if (nowPlayMap[iy][ix] == 'O')
                current_goals++;
        }
    }

    get_player_pos();
}

void newgame(int imap) // Restart from the first map
{
    int imap=0;

    selectmap(imap);
    printmap(imap);
}

int history_idx = 0; //* Variable that reads the history array backwards

void record_history(char move) //* Function to record player movements
{
     //If a general command is input
     for (int i = 0; i <= 3; ++i)
     {
         cmd_history[i] = cmd_history[i+1];
         box_history[i] = box_history[i+1];
     }

     cmd_history[4] = move;
     box_history[4] = is_box_moved;
}

void undo()
{
     is_undoing = true;

     decide_move(cmd_history[4], current_map_no);



     if (box_history[4])
     {
         switch (cmd_history[4])
         {
             case 'h':// left
                 if (nowPlayMap[current_player_pos[1]][current_player_pos[0]-2] != '#' && nowPlayMap[current_player_pos[1]][current_player_pos[0]-2] != '$' )
                 {
                     nowPlayMap[current_player_pos[1]][current_player_pos[0]+1] = '$';
                     nowPlayMap[current_player_pos[1]][current_player_pos[0]+2] = '.';
                 }
                 break;
             case 'j':// ha
                 if ((nowPlayMap[current_player_pos[1]-2][current_player_pos[0]] != '#' && nowPlayMap[current_player_pos[1]+2][current_player_pos[0]] != '$' ))
                 {
                     nowPlayMap[current_player_pos[1]-1][current_player_pos[0]] = '$';
                     nowPlayMap[current_player_pos[1]-2][current_player_pos[0]] = '.';
                 }
                 break;
             case 'k':// award
                 if ((nowPlayMap[current_player_pos[1]+2][current_player_pos[0]] != '#' && nowPlayMap[current_player_pos[1]-2][current_player_pos[0]] != '$' ))
                 {
                     nowPlayMap[current_player_pos[1]+1][current_player_pos[0]] = '$';
                     nowPlayMap[current_player_pos[1]+2][current_player_pos[0]] = '.';
                 }
                 break;
             case 'l' :// right
                 if ((nowPlayMap[current_player_pos[1]][current_player_pos[0]+2] != '#' && nowPlayMap[current_player_pos[1]][current_player_pos[0]+2] != '$' ))
                 {
                     nowPlayMap[current_player_pos[1]][current_player_pos[0]-1] = '$';
                     nowPlayMap[current_player_pos[1]][current_player_pos[0]-2] = '.';
                 }
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

void ranking(int move_count, char imap)
{

    int i,j,change,k;

    if (imap == '\n')
        printf("%d\n", move_count);
    else if (imap == '1')
        printf("%d\n", move_count);
    else if (imap == '2')
        printf("%d\n", move_count);
    else if (imap == '3')
        printf("%d\n", move_count);
    else if (imap == '4')
        printf("%d\n", move_count);
    else if (imap == '5')
        printf("%d\n", move_count);

}

void save(void){
    FILE *ofp;

    ofp = fopen("sokoban","w");
    fprintf(ofp, "%s %d", name, move_count);

    fclose(ofp);
}

void load(void){
    FILE *ifp;

    ifp = fopen("sokoban","r");
    fscanf(ifp, "%s %d", name, &move_count);
    printf("%s %d", name, move_count);

    fclose(ifp);
}

int main(void)
{
    char command;
    int imap = 0;
    char name[10] = {'\0'};
    int move_count=0; // Variables to be used in the leaderboard

    load_map();

    if (check_error == 1)
        return 0;
    
    printf("Input your name, please : ");
    scanf("%10s", name);
    printf("%s", name);

    current_map_no = 0;

    selectmap(current_map_no);
    printmap(current_map_no);
    printf("history: ");

    while(1)
    {
        //? Assume map file number 1, Map selection function scheduled to be added in the future
        command = getch();
        printf("history: ");
        for (int i = 0; i <= 4; ++i) {
            printf("%c", history[i]);
        }
        printf("\n");

        switch(command)
        {
            case 'f':
                load();
                break;
            
            case 's':
                save();
                break;

            case 'n':
                newgame(0);
                move_count=0;
                break;

            case 'r':
                newgame(current_map_no);
                break;

            case 'u':
                get_player_pos();
                undo();
                break;

            case 'o':
                goto end;
                break;

            case 't':
                command = getch();
                ranking(move_count,command);
                break;

            case 'h':
            case 'j':
            case 'k':
            case 'l':
                get_player_pos();
                is_undoing = false;
                move_count++;
                decide_move(command, imap);
                printmap(current_map_no);
                break;
        }


        printf("\n");
        // TESTING
        // i++;
        printf("commands: ");
        for (int  i = 0;  i <= 4; ++ i) {
            printf("%c", cmd_history[i]);
        }
        printf("\n");
        printf("box: ");
        for (int  i = 0;  i <= 4; ++ i) {
            printf("%d", box_history[i]);
        }
    }

    end:
    return 0;
}



/*

Main function



int move_count=0; // Variables to be used in the leaderboard
printf("User name: ");
scanf("%s", &player_ranking[player_name]);
getch();
case 't':
    command = getch();
    if(command=='\n')
        ranking(move_count,current_map_no,player_ranking[player_name]);
else if(command=='1')
        ranking(move_count,0,player_ranking[player_name]);
    else if(command=='2')
        ranking(move_count,1,player_ranking[player_name]);
    else if(command=='3')
        ranking(move_count,2,player_ranking[player_name]);
    else if(command=='4')
        ranking(move_count,3,player_ranking[player_name]);
else if(command=='5')
        ranking(move_count,4,player_ranking[player_name]);
    break;
case 'h':
case 'j':
case 'k':
case 'l':
    move_count++;
    break;
void ranking(int move_count, int imap, char player_ranking[]){
    int i,j,change,k;
    if(imap == current_map_no){
        for(i=0;i<player;i++){
            for(j=0;j<player;j++){
player_ranking[j] = move_count;
                player_ranking[j+1] = move_count;
                if(player_ranking[j]> player_ranking[j+1]) {
                    change = player_ranking[player_name];
                    player_ranking[j]=player_ranking[j+1];
                    player_ranking[j+1]=change;
                }
}
        }
        for(k=0;k<player;k++) {
            printf("Number of moves %s above %d%d",k, player_ranking[player_name], move_count);
        }
    }
    else if(imap == 1){
        printf("%d\n",move_count);
    }
    else if(imap == 2){
        printf("%d\n",move_count);
    }
    else if(imap == 3){
        printf("%d\n",move_count);
    }
else if(imap == 4){
        printf("%d\n",move_count);
    }
}
*/