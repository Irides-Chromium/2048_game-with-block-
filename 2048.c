#include <stdio.h>
#include <string.h>
#include <math.h>
#include <termios.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

struct termios set_non_canonical ();
char get_key ();
int main (int argc, char **argv);
int move_tile (int *tile[16], char direction);
void map_key (char key);
void place_tile (int *tile[16]);
void print_border ();
void print_status (int score);
void print_tile (int num, int tile_num);
//void print_tile_num (int *tile[16]);
void set_canonical (struct termios old);

enum {UP = 1,
      DOWN,
      LEFT,
      RIGHT};

//CANONICAL
struct termios set_non_canonical () {
    struct termios old_info;
    tcgetattr(0, &old_info);
    struct termios info = old_info;
    info.c_lflag &= ~(ICANON | ECHO);
    info.c_cc[VMIN] = 1;
    info.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &info);
    return old_info;
}

void set_canonical (struct termios old) {
    tcsetattr(0, TCSANOW, &old);
}

//BORDER
void print_border () {
    int i, j;
    char horizontal[44] = "-------------------------------------------";
    //Print horizontally
    printf ("\e[6;1H%s", horizontal);
    printf ("\e[12;1H%s", horizontal);
    printf ("\e[18;1H%s", horizontal);

    //Print vertically
    for (j=1;j<4;j++) {
        for (i=1;i<24;i++) {
            printf ("\e[%d;%dH|", i, 11*j);
        }
    }
    //Print dot
    for (j=1;j<4;j++) {
        for (i=1;i<4;i++) {
            printf ("\e[%d;%dH%c", j*6, i*11, '+');
        }
    }
}

//PRINT_TILE
void print_tile (int num, int tile_num) {
    char color[20];
    char before[8];
    char after[8];
    int row, col;
    int p_y, p_x;
    switch (num) {
        //case 0:
        //    strcpy (color, "\e[0m");
        case 2:
            strcpy (color, "\e[48;2;134;222;132m");
            strcpy (before, "    ");
            strcpy (after, "     ");           
            break;
        case 4:
            strcpy (color, "\e[48;2;103;204;252m");
            strcpy (before, "    ");
            strcpy (after, "     ");   
            break;
        case 8:
            strcpy (color, "\e[48;2;153;51;255m");
            strcpy (before, "    ");
            strcpy (after, "     ");
            break;
        case 16:
            strcpy (color, "\e[48;2;255;154;154m");
            strcpy (before, "    ");
            strcpy (after, "    ");
            break;
        case 32:
            strcpy (color, "\e[48;2;255;227;153m");
            strcpy (before, "    ");
            strcpy (after, "    ");
            break;
        case 64:
            strcpy (color, "\e[48;2;163;205;73m");
            strcpy (before, "    ");
            strcpy (after, "    ");
            break;
        case 128:
            strcpy (color, "\e[48;2;13;169;175m");
            strcpy (before, "   ");
            strcpy (after, "    ");
            break;
        case 256:
            strcpy (color, "\e[48;2;108;56;153m");
            strcpy (before, "   ");
            strcpy (after, "    ");
            break;
        case 512:
            strcpy (color, "\e[48;2;238;197;221m");
            strcpy (before, "   ");
            strcpy (after, "    ");
            break;
        case 1024:
            strcpy (color, "\e[48;2;148;24;24m");
            strcpy (before, "   ");
            strcpy (after, "   ");
            break;
        case 2048:
            strcpy (color, "\e[48;2;0;0;0m");
            strcpy (before, "   ");
            strcpy (after, "   ");
            break;
        case 4096:
            strcpy (color, "\e[48;2;200;200;200m");
            strcpy (before, "   ");
            strcpy (after, "   ");
            break;
        default:
            break;
    }

    row = (int) (tile_num/4+1);       //there was a floor ()
    col = tile_num % 4 + 1;
    p_y = (row - 1) * 6 + 1;
    p_x = (col - 1) * 11 + 1;
    if (num == 0) {
        printf ("\e[0m");
        printf ("\e[%d;%dH          ", p_y, p_x);   
        printf ("\e[%d;%dH          ", p_y+1, p_x);
        printf ("\e[%d;%dH          ", p_y+2, p_x);
        printf ("\e[%d;%dH          ", p_y+3, p_x);
        printf ("\e[%d;%dH          ", p_y+4, p_x);
        printf ("\e[0m");
    } else {
        printf ("%s", color);
        printf ("\e[%d;%dH          ", p_y, p_x);   
        printf ("\e[%d;%dH          ", p_y+1, p_x);
        printf ("\e[%d;%dH%s%d%s", p_y+2, p_x, before, num, after);
        printf ("\e[%d;%dH          ", p_y+3, p_x);
        printf ("\e[%d;%dH          ", p_y+4, p_x);
        printf ("\e[0m");
    }
    /*  0   1   2   3
     *  4   5   6   7
     *  8   9   10  11
     *  12  13  14  15
     */
}

//TEST PRINT_TILE_NUM
void print_tile_num (int *tile[16]) {
    int i;
    FILE *g = fopen ("log", "a");
    fprintf (g, "%s", "Tiles: ");
    for (i=0;i<16;i++) {
        fprintf (g, "%d ", *tile[i]);
    }
    fprintf (g, "\n");
    fclose (g);
}

//GET_KEY 
char get_key () {
    char signal;
    char key;
    bool escape;
    signal = getchar ();
    if (0x1b == signal) {
        escape = true;
        if (0x5b == getchar ()) {
            signal = getchar ();
        }
    } else {
        escape = false;
    }

    if (escape) {
        switch (signal) {
            case 0x41:
                key = 'U';
                break;
            case 0x42:
                key = 'D';
                break;
            case 0x43:
                key = 'R';
                break;
            case 0x44:
                key = 'L';
                break;
            default:
                key = 'x';
                break;
        }
    } else {
        switch (signal) {
            case 0x77: case 0x57:
                key = 'U';
                break;
            case 0x73: case 0x53:
                key = 'D';
                break;
            case 0x64: case 0x44:
                key = 'R';
                break;
            case 0x61: case 0x41:
                key = 'L';
                break;
            case 0x72: case 0x52:
                key = 'r';
                break;
            case 0x71: case 0x51:
                key = 'q';
                break;
            case 0x69: case 0x49: case 0x68: case 0x48:
                key = 'h';
                break;
            default:
                key = 'x';
                break;
        }
    }
    return key;
}

//MOVE_TILE 
int move_tile (int *tile[16], char direction) {
    int i;
    int delta_score = 0;
    int left_ava[12] =
    {0, 1, 2,
     4, 5, 6,
     8, 9, 10,
     12, 13, 14};

    switch (direction) {
        case 'U':
            for (i=0;i<4;i++) {
                if (*tile[i] == 0) {
                    *tile[i] = *tile[i+4];
                    *tile[i+4] = 0;
                }

                if (*tile[i] == *tile[i+4]) {
                    *tile[i] = 2 * *tile[i];
                    *tile[i+4] = 0;
                    delta_score += *tile[i];
                }
            }
            break;
        case 'D':
            for (i=0;i<12;i++) {

                if (*tile[i+4] == 0) {
                    *tile[i+4] = *tile[i];
                    *tile[i] = 0;
                }

                if (*tile[i] == *tile[i+4]) {
                    *tile[i+4] = 2 * *tile[i+4];
                    *tile[i] = 0;
                    delta_score += *tile[i+4];
                }
            }
            break;
        case 'R':
            for (i=0;i<12;i++) {

                if (*tile[left_ava[i]+1] == 0) {
                    *tile[left_ava[i]+1] = *tile[left_ava[i]];
                    *tile[left_ava[i]] = 0;
                }

                if (*tile[left_ava[i]] == *tile[left_ava[i]+1]) {
                    *tile[left_ava[i]+1] = 2 * *tile[left_ava[i]+1];
                    *tile[left_ava[i]] = 0;
                    delta_score += *tile[left_ava[i]+1];
                }
            }
            break;
        case 'L':
            for (i=0;i<12;i++) {

                if (*tile[left_ava[i]] == 0) {
                    *tile[left_ava[i]] = *tile[left_ava[i]+1];
                    *tile[left_ava[i]+1] = 0;
                }

                if (*tile[left_ava[i]] == *tile[left_ava[i]+1]) {
                    *tile[left_ava[i]] = 2 * *tile[left_ava[i]];
                    *tile[left_ava[i]+1] = 0;
                    delta_score += *tile[left_ava[i]];
                }
            }
            break;
    }
    return delta_score;
}

//PLACE_TILE
void place_tile (int *tile[16]) {
    int i;
    int empty_loc;
    int tile_loc;
    int tile_num;
    int empty_tile[16];
    int empty_num=0;
    
    srandom (time (NULL));
    for (i=0;i<16;i++) {
        if (0 == *tile[i]) {
            empty_tile[empty_num] = i;
            ++empty_num;
        }
    }

    if (empty_num > 0) {
        tile_num = random () % 8 ? 2 : 4;
        empty_loc = random () % (empty_num);
        tile_loc = empty_tile[empty_loc];
        print_tile (tile_num, tile_loc);
        *tile[tile_loc] = tile_num;
    }
}

//STATUS
void print_status (int score) {
    printf ("\e[24;1H");
    printf ("\e[1;7m");
    printf ("%80s\r%70d\r%60s\r%s", "          ", score, "YOUR SCORE: ", "-- THE 2048 GAME --");
    printf ("\e[24;80H\e[0m");
}

int main (int argc, char **argv) {
    char key;
    int i;
    int turn;
    int score = 0;
    int tile[16];// = {0, 2, 4, 0, 0, 8, 0, 16, 0, 128, 0, 512, 0, 1024, 0, 4096};
    int *tile_p[16];

    printf ("\e[s");          //store cursor position
    printf ("\e[?1049h");       //store window in buffer
    printf ("\e[2J");           //clear screen
    print_border ();
    print_status (score);
    struct termios saved_state = set_non_canonical ();
start:
    //tile initialization
    for (i=0;i<16;i++) {
        tile[i] = 0;
    }

    for (i=0;i<16;i++) {
        tile_p[i] = &tile[i];
    }

    place_tile (tile_p);
    place_tile (tile_p);

    //for (i=0;i<16;i++) {
    //    print_tile (*tile_p[i], i);
    //}
    
    //main loop
    for (turn=1;;turn++) {
        //Get key
        key = get_key ();
        switch (key) {
            case 'U': case 'D': case 'R': case 'L':
                score += move_tile (tile_p, key);
                break;
            case 'q':
                goto end;
                break;
            case 'r':
                goto start;
                break;
            case 'h':
                //show_help ();
                break;
            default:
                break;
        }
        place_tile (tile_p);
        for (i=0;i<16;i++) {
            print_tile (*tile_p[i], i);
        }
        print_tile_num (tile_p);
        print_status (score);
    }

end:
    //exit
    set_canonical (saved_state);//reset terminal
    printf ("\e[u\n");      //restore cursor position
    printf ("\e[?1049l");       //restore the terminal window
    return 0;
}
