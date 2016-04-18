#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

char get_key ();
bool check_end (int *);
int main (int, char **);
int move_tile (int *, char);
void map_key (char);
void place_tile (int *);
void print_border ();
void print_status (int);
void print_tile (int, int);
void print_info (int);

//BORDER
void print_border () {
    int r, c;
    char horizontal[44] = "-------------------------------------------";
    //Print horizontally
    for (r=1;r<4;r++)
        mvprintw (r * 6 - 1, 0, horizontal);

    //Print vertically
    for (c=1;c<5;c++)
        for (r=1;r<24;r++)
            mvprintw (r - 1, 11 * c - 1, "|");

    //Print dot
    for (c=1;c<5;c++)
        for (r=1;r<4;r++)
            mvprintw (r * 6 - 1, 11 * c - 1, "+");

    refresh ();
}

//PRINT_TILE
void print_tile (int num, int tile_num) {
    //Use COLOR_RED as custom color
    char before[8];
    char after[8];
    int row, col;
    int p_y, p_x;

    init_color (1, 523, 867, 515);
    init_color (2, 402, 796, 984);
    init_color (3, 597, 199, 996);
    init_color (4, 996, 601, 601);
    init_color (5, 996, 886, 597);
    init_color (6, 636, 800, 285);
    init_color (7, 50, 660, 683);
    init_color (8, 421, 218, 597);
    init_color (9, 929, 769, 863);
    init_color (10, 578, 93, 93);
    init_color (11, 0, 0, 0);
    init_color (12, 781, 781, 781);

    switch (num) {
        case 2:
            strcpy (before, "    ");
            strcpy (after, "     ");
            break;
        case 4:
            strcpy (before, "    ");
            strcpy (after, "     ");
            break;
        case 8:
            strcpy (before, "    ");
            strcpy (after, "     ");
            break;
        case 16:
            strcpy (before, "    ");
            strcpy (after, "    ");
            break;
        case 32:
            strcpy (before, "    ");
            strcpy (after, "    ");
            break;
        case 64:
            strcpy (before, "    ");
            strcpy (after, "    ");
            break;
        case 128:
            strcpy (before, "   ");
            strcpy (after, "    ");
            break;
        case 256:
            strcpy (before, "   ");
            strcpy (after, "    ");
            break;
        case 512:
            strcpy (before, "   ");
            strcpy (after, "    ");
            break;
        case 1024:
            strcpy (before, "   ");
            strcpy (after, "   ");
            break;
        case 2048:
            strcpy (before, "   ");
            strcpy (after, "   ");
            break;
        case 4096:
            strcpy (before, "   ");
            strcpy (after, "   ");
            break;
        default:
            break;
    }

    int count = 0;
    if (num != 0) {
        int _num = num;
        while (_num != 2) {
            _num = _num >> 1;
            count += 1;
        }
    }
    row = (int) (tile_num/4);
    col = tile_num % 4;
    p_y = row * 6;
    p_x = col * 11;
    use_default_colors ();
    assume_default_colors (-1, -1);

    init_pair (1, -1, -1);
    init_pair (2, -1, 1);
    init_pair (3, -1, 2);
    init_pair (4, -1, 3);
    init_pair (5, -1, 4);
    init_pair (6, -1, 5);
    init_pair (7, -1, 6);
    init_pair (8, -1, 7);
    init_pair (9, -1, 8);
    init_pair (10, -1, 9);
    init_pair (11, -1, 10);
    init_pair (12, -1, 11);
    init_pair (13, -1, 12);

    if (num == 0) {
        attron (COLOR_PAIR(1));
        mvprintw (p_y, p_x, "          ");
        mvprintw (p_y+1, p_x, "          ");
        mvprintw (p_y+2, p_x, "          ");
        mvprintw (p_y+3, p_x, "          ");
        mvprintw (p_y+4, p_x, "          ");
        attroff (COLOR_PAIR(1));
    } else {
        attron (COLOR_PAIR(count+2));
        mvprintw (p_y, p_x, "          ");
        mvprintw (p_y+1, p_x, "          ");
        mvprintw (p_y+2, p_x, "%s%d%s", before, num, after);
        mvprintw (p_y+3, p_x, "          ");
        mvprintw (p_y+4, p_x, "          ");
        attroff (COLOR_PAIR(count+2));
    }
    refresh ();
}

//GET_KEY
char get_key () {
    char signal;
    char key;
    bool escape;
    signal = getch ();
    if (0x1b == signal) {
        escape = true;
        if (0x5b == getch ())
            signal = getch ();
    } else
        escape = false;

    if (escape)
        switch (signal) {
            case 'A':
                key = 'U';
                break;
            case 'B':
                key = 'D';
                break;
            case 'C':
                key = 'R';
                break;
            case 'D':
                key = 'L';
                break;
            default:
                key = 'x';
                break;
        }
    else
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
    for (i=0;i<16;i++)
        if (0 == tile[i]) {
            empty_tile[empty_num] = i;
            ++empty_num;
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
    move (23, 0);
    attron (A_REVERSE | A_BOLD);
    printw ("\r%80s", " ");
    printw ("\r%70d", score);
    printw ("\r%60s", "YOUR SCORE: ");
    printw ("\r%s", "-- THE 2048 GAME --");
    move (23, 79);
    attroff (A_REVERSE | A_BOLD);
    refresh ();
}

//INFO
void print_info (int status) {
    switch (status) {
        case 0: //normal
            mvprintw (3, 45, "%s", "    Use your arrow key to play     ");
            mvprintw (4, 45, "%s", "                                   ");
            mvprintw (5, 45, "%s", "             _____                 ");
            mvprintw (6, 45, "%s", "            |     |                ");
            mvprintw (7, 45, "%s", "            |  ^  |                ");
            mvprintw (8, 45, "%s", "            |  |  |                ");
            mvprintw (9, 45, "%s", "      -------------------          ");
            mvprintw(10, 45, "%s", "      |     |     |     |          ");
            mvprintw(11, 45, "%s", "      |  <- |  |  | ->  |          ");
            mvprintw(12, 45, "%s", "      |     |  v  |     |          ");
            mvprintw(13, 45, "%s", "      -------------------          ");
            mvprintw(14, 45, "%s", "      Or W, S, A, D instead        ");
            break;
        case 1: //win
            mvprintw (3, 45, "%s", "                                   ");
            mvprintw (4, 45, "%s", "                                   ");
            mvprintw (5, 45, "%s", "                                   ");
            mvprintw (6, 45, "%s", "                                   ");
            mvprintw (7, 45, "%s", "                                   ");
            mvprintw (8, 45, "%s", "                                   ");
            mvprintw (9, 45, "%s", "                                   ");
            mvprintw(10, 45, "%s", "                                   ");
            mvprintw(11, 45, "%s", "                                   ");
            mvprintw(12, 45, "%s", "                                   ");
            mvprintw(13, 45, "%s", "               HURRAY!!!           ");
            mvprintw(14, 45, "%s", "          You reached 2048!!       ");
            mvprintw(15, 45, "%s", "           Move to continue        ");
            break;
        case 2: //lose
            mvprintw (3, 45, "%s", "                                   ");
            mvprintw (4, 45, "%s", "                                   ");
            mvprintw (5, 45, "%s", "                                   ");
            mvprintw (6, 45, "%s", "                                   ");
            mvprintw (7, 45, "%s", "                                   ");
            mvprintw (8, 45, "%s", "                                   ");
            mvprintw (9, 45, "%s", "                                   ");
            mvprintw(10, 45, "%s", "                                   ");
            mvprintw(11, 45, "%s", "                                   ");
            mvprintw(12, 45, "%s", "                                   ");
            mvprintw(13, 45, "%s", "                OH NO!!            ");
            mvprintw(14, 45, "%s", "              You lose!!!          ");
            break;
        default:
            break;
    }
    mvprintw (20, 49, "%s", "    Press 'r' to retry");
    mvprintw (21, 49, "%s", "          'q' to quit");
    refresh ();
}

//OVER
bool check_end (int tile[16]) {
    int i;
    int tile_bak[16];
    bool changed = false;
    for (i=0;i<16;i++)
        if (tile[i] == 0)
            return false;

    for (i=0;i<16;i++)
        tile_bak[i] = tile[i];

    move_tile (tile, 'U');
    move_tile (tile, 'D');
    move_tile (tile, 'R');
    move_tile (tile, 'L');
    for (i=0;i<16;i++)
        if (tile_bak[i] != tile[i])
            changed = true;

    if (changed) {
        for (i=0;i<16;i++)
            tile[i] = tile_bak[i];
        return false;
    }
    return true;
}

void init_scr () {
    initscr ();
    start_color ();
    noecho ();
    raw();
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

    init_scr ();
    print_border ();
start:
    //tile initialization
    for (i=0;i<16;i++)
        tile[i] = 0;

    place_tile (tile);
    place_tile (tile);

    for (i=0;i<16;i++)
        print_tile (tile[i], i);

    score = 0;
    print_info (0);
    print_status (score);
    //main loop
    for (turn=1;!check_end (tile);turn++) {

        //backup
        for (i=0;i<16;i++)
            tile_bak[i] = tile[i];

        //check 2048
        if (!reached) {
            for (i=0;i<16;i++) {
                if (tile[i] == 2048) {
                    print_info (1);
                    reached = true;
                }
            }
        } else
            print_info (0);

        //Get key
        key = get_key ();
        switch (key) {
            case 'U': case 'D': case 'R': case 'L':
                score += move_tile (tile, key);
                break;
            case 'q': goto end; break;
            case 'r': goto start; break;
            case 'h': break;
            default: break;
        }

        changed = false;
        for (i=0;i<16;i++)
            if (tile[i] != tile_bak[i])
                changed = true;

        if (changed)
            place_tile (tile);

        for (i=0;i<16;i++)
            print_tile (tile[i], i);

        print_status (score);
    }

    print_info (2);
    while (true) {
        switch (get_key ()) {
            case 'q': goto end;
            case 'r': goto start;
            default:  continue;
        }
    }

end:
    //exit
    endwin ();
    return 0;
}
