#include <stdio.h>
#define true 1
#define false 0

char temp[5][30][30]; //* Variable for first reading the map file and storing the data
char map[5][30][30]; //* Variable that processes and stores the temp variable
int current_player_pos[2];


void load_map(void){ //* Function to load data from a map file, store it in temp, process it, and save it in map

    FILE *ifp;
    /*
     * imap: map number
     * iy: Map row (Y-axis) number
     * ix: Column (X-axis) number of the map
     * will_load: Variable that sets whether to load characters
    */
    int imap = 0, iy = 0, ix = 0, c = 0;
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
                will_load = false;
                break;

            case '3':
                imap = 2;
                iy = 0;
                ix = 0;
                will_load = false;
                break;

            case '4':
                imap = 3;
                iy = 0;
                ix = 0;
                will_load = false;
                break;

            case '5':
                imap = 4;
                iy = 0;
                ix = 0;
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
                goto load_map_end;

            default:
                temp[imap][iy][ix] = c;
                ix++;
                break;
        }
    }

    load_map_end:

    fclose(ifp);

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


void get_player_pos(int imap)
{
    for (int iy = 0; iy < checkYsize(imap, checkXsize(imap)); iy++)
    {
        for (int ix = 0; ix < checkXsize(imap); ix++)
        {
            switch(map[imap][iy][ix])
            {
                case '@':
                    current_player_pos[0] = ix;
                    current_player_pos[1] = iy;
            }
        }
        printf("\n");
    }
}




int main(void)
{
    load_map();

      for (int imap = 0; imap <= 4; imap++)
    {
        get_player_pos(imap);
        printf("PLAYER_POSITION: %d, %d\n", current_player_pos[0], current_player_pos[1]);
        for (int iy = 0; iy < checkYsize(imap, checkXsize(imap)); iy++)
        {
            for (int ix = 0; ix < checkXsize(imap); ix++)
            {
                printf("%c", map[imap][iy][ix]);
            }
        }
            printf("\n");
    }
        printf("\n\n");
}



    return 0;
}