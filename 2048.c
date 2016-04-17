#include <stdio.h>
#include <string.h>
#include <math.h>
#include <termios.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

struct termios set_non_canonical ();
char get_key ();
bool check_end (int tile[16]);
int main (int argc, char **argv);
int move_tile (int tile[16], char direction);
void map_key (char key);
void place_tile (int tile[16]);
void print_border ();
void print_status (int score);
void print_tile (int num, int tile_num);
void print_info (int status);
void set_canonical (struct termios old);

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
    int c, r;
    char horizontal[44] = "-------------------------------------------";
    //Print horizontally
    printf ("\e[6;1H%s", horizontal);
    printf ("\e[12;1H%s", horizontal);
    printf ("\e[18;1H%s", horizontal);

    //Print vertically
    for (c=1;c<5;c++) {
        for (r=1;r<24;r++) {
            printf ("\e[%d;%dH|", r, 11*c);
        }
    }
    //Print dot
    for (c=1;c<5;c++) {
        for (r=1;r<4;r++) {
            printf ("\e[%d;%dH%c", r*6, c*11, '+');
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

    row = (int) (tile_num/4+1);
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
            case 'A': key = 'U'; break;
            case 'B': key = 'D'; break;
            case 'C': key = 'R'; break;
            case 'D': key = 'L'; break;
            default:   key = 'x'; break;
        }
    } else {
        switch (signal) {
            case 'w': case 'W': key = 'U'; break;
            case 's': case 'S': key = 'D'; break;
            case 'd': case 'D': key = 'R'; break;
            case 'a': case 'A': key = 'L'; break;
            case 'r': case 'R': key = 'r'; break;
            case 'q': case 'Q': key = 'q'; break;
            case 'i': case 'I':
            case 'h': case 'H': key = 'h'; break;
            default:            key = 'x'; break;
        }
    }
    return key;
}

//MOVE_TILE
int move_tile (int tile[16], char direction) {
    int i, j;
    int delta_score = 0;

    switch (direction) {
        case 'U':
            for (j=0;j<=5;j++) {
                if (j!=4) {
                    for (i=0;i<12;i++) {
                        if (tile[i] == 0) {
                            tile[i] = tile[i+4];
                            tile[i+4] = 0;
                        }
                    }
                } else {
                    for (i=0;i<12;i++) {
                        if (tile[i] == tile[i+4]) {
                            tile[i] = 2 * tile[i];
                            tile[i+4] = 0;
                            delta_score += tile[i];
                        }
                    }
                }
            }
            break;
        case 'D':
            for (j=0;j<=5;j++) {
                if (j!=4) {
                    for (i=11;i>=0;i--) {
                        if (tile[i+4] == 0) {
                            tile[i+4] = tile[i];
                            tile[i] = 0;
                        }
                    }
                } else {
                    for (i=11;i>=0;i--) {
                        if (tile[i] == tile[i+4]) {
                            tile[i+4] = 2 * tile[i+4];
                            tile[i] = 0;
                            delta_score += tile[i+4];
                        }
                    }
                }
            }
            break;
        case 'R':
            for (j=0;j<=5;j++) {
                if (j!=4) {
                    for (i=14;i>=0;i--) {
                        if (i == 3 || i == 7 || i == 11) {
                            continue;
                        }
                        if (tile[i+1] == 0) {
                            tile[i+1] = tile[i];
                            tile[i] = 0;
                        }
                    }
                } else {
                    for (i=14;i>=0;i--) {
                        if (i == 3 || i == 7 || i == 11) {
                            continue;
                        }
                        if (tile[i] == tile[i+1]) {
                            tile[i+1] = 2 * tile[i+1];
                            tile[i] = 0;
                            delta_score += tile[i+1];
                        }
                    }
                }
            }
            break;
        case 'L':
            for (j=0;j<=5;j++) {
                if (j!=4) {
                    for (i=0;i<=14;i++) {
                        if (i == 3 || i == 7 || i == 11) {
                            continue;
                        }
                        if (tile[i] == 0) {
                            tile[i] = tile[i+1];
                            tile[i+1] = 0;
                        }
                    }
                } else {
                    for (i=0;i<=14;i++) {
                        if (i == 3 || i == 7 || i == 11) {
                            continue;
                        }
                        if (tile[i] == tile[i+1]) {
                            tile[i] = 2 * tile[i];
                            tile[i+1] = 0;
                            delta_score += tile[i];
                        }
                    }
                }
            }
            break;
    }
    return delta_score;
}

//PLACE_TILE
void place_tile (int tile[16]) {
    int i;
    int empty_loc;
    int tile_loc;
    int tile_num;
    int empty_tile[16];
    int empty_num=0;

    srandom (time (NULL));
    for (i=0;i<16;i++) {
        if (0 == tile[i]) {
            empty_tile[empty_num] = i;
            ++empty_num;
        }
    }

    if (empty_num > 0) {
        tile_num = random () % 10 ? 2 : 4;
        empty_loc = random () % (empty_num);
        tile_loc = empty_tile[empty_loc];
        tile[tile_loc] = tile_num;
    }
}

//STATUS
void print_status (int score) {
    printf ("\e[24;1H");
    printf ("\e[1;7m");
    printf ("%80s\r%70d\r%60s\r%s", "          ", score, "YOUR SCORE: ", "-- THE 2048 GAME --");
    printf ("\e[24;80H\e[0m");
}

//INFO
void print_info (int status) {
    switch (status) {
        case 0: //normal
            printf ("\e[3;49H%s", "Use your arrow key to play      ");
            printf ("\e[4;49H%s", "                                ");
            printf ("\e[5;49H%s", "         _____                  ");
            printf ("\e[6;49H%s", "        |     |                 ");
            printf ("\e[7;49H%s", "        |  ^  |                 ");
            printf ("\e[8;49H%s", "        |  |  |                 ");
            printf ("\e[9;49H%s", "  -------------------           ");
            printf("\e[10;49H%s", "  |     |     |     |           ");
            printf("\e[11;49H%s", "  |  <- |  |  | ->  |           ");
            printf("\e[12;49H%s", "  |     |  v  |     |           ");
            printf("\e[13;49H%s", "  -------------------           ");
            printf("\e[14;49H%s", "  Or W, S, A, D instead         ");
            break;
        case 1: //win
            printf ("\e[3;49H%s", "                                ");
            printf ("\e[4;49H%s", "                                ");
            printf ("\e[5;49H%s", "                                ");
            printf ("\e[6;49H%s", "                                ");
            printf ("\e[7;49H%s", "                                ");
            printf ("\e[8;49H%s", "                                ");
            printf ("\e[9;49H%s", "                                ");
            printf("\e[10;49H%s", "                                ");
            printf("\e[11;49H%s", "                                ");
            printf("\e[34m");
            printf("\e[12;49H%s", "           HURRAY!!!            ");
            printf("\e[1;35m");
            printf("\e[13;49H%s", "      You reached 2048!!        ");
            printf("\e[0m");
            printf("\e[14;51H%s", "       Move to continue         ");
            break;
        case 2: //lose
            printf ("\e[3;49H%s", "                                ");
            printf ("\e[4;49H%s", "                                ");
            printf ("\e[5;49H%s", "                                ");
            printf ("\e[6;49H%s", "                                ");
            printf ("\e[7;49H%s", "                                ");
            printf ("\e[8;49H%s", "                                ");
            printf ("\e[9;49H%s", "                                ");
            printf("\e[10;49H%s", "                                ");
            printf("\e[11;49H%s", "                                ");
            printf("\e[12;49H%s", "                                ");
            printf("\e[1;31m");
            printf("\e[13;49H%s", "            OH NO!!             ");
            printf("\e[0m");
            printf("\e[14;49H%s", "          You lose!!!           ");
            break;
        default:
            break;
    }
    printf ("\e[20;49H%s", "    Press 'r' to retry");
    printf ("\e[21;49H%s", "          'q' to quit");
}

//OVER
bool check_end (int tile[16]) {
    int i;
    int tile_bak[16];
    bool changed = false;
    for (i=0;i<16;i++) {
        if (tile[i] == 0) {
            return false;
        }
    }

    for (i=0;i<16;i++) {
        tile_bak[i] = tile[i];
    }

    move_tile (tile, 'U');
    move_tile (tile, 'D');
    move_tile (tile, 'R');
    move_tile (tile, 'L');
    for (i=0;i<16;i++) {
        if (tile_bak[i] != tile[i]) {
            changed = true;
        }
    }

    if (changed) {
        for (i=0;i<16;i++) {
            tile[i] = tile_bak[i];
        }
        return false;
    }
    return true;
}

int main (int argc, char **argv) {
    char key;
    bool changed = false;
    bool reached = false;
    int i;
    int turn;
    int score = 0;
    int tile[16];
    int tile_bak[16];

    printf ("\e[s");            //store cursor position
    printf ("\e[?1049h");       //store window in buffer
    printf ("\e[2J");           //clear screen
    print_border ();
    struct termios saved_state = set_non_canonical ();
start:
    //tile initialization
    for (i=0;i<16;i++) {
        tile[i] = 0;
    }

    //for (i=0;i<16;i++) {
    //    tile_p[i] = &tile[i];
    //}

    place_tile (tile);
    place_tile (tile);

    for (i=0;i<16;i++) {
        print_tile (tile[i], i);
    }

    score = 0;
    print_info (0);
    print_status (score);
    //main loop
    for (turn=1;;turn++) {

        //backup
        for (i=0;i<16;i++) {
            tile_bak[i] = tile[i];
        }

        //check end
        if (check_end (tile)) {
            goto lose;
        }

        //check 2048
        if (!reached) {
            for (i=0;i<16;i++) {
                if (tile[i] == 2048) {
                    print_info (1);
                    reached = true;
                }
            }
        } else {
            print_info (0);
        }

        //Get key
        key = get_key ();
        switch (key) {
            case 'U': case 'D': case 'R': case 'L':
                score += move_tile (tile, key);
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

        changed = false;
        for (i=0;i<16;i++) {
            if (tile[i] != tile_bak[i]) {
                changed = true;
            }
        }

        if (changed) {
            place_tile (tile);
        }

        for (i=0;i<16;i++) {
            print_tile (tile[i], i);
        }

        print_status (score);
    }

lose:
    print_info (2);
    while (true) {
        switch (get_key ()) {
            case 'q':
                goto end;
            case 'r':
                goto start;
            default:
                continue;
        }
    }

end:
    //exit
    set_canonical (saved_state);//reset terminal
    printf ("\e[u\n");          //restore cursor position
    printf ("\e[?1049l");       //restore the terminal window
    return 0;
}
