#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"
#include <ctype.h>
#include <limits.h>

char *FILENAME = "input/input.txt";

void fill_line(int xini, int yini, int xfin, int yfin, int MAXX);
int any_non_green_tile_in(int xini, int yini, int xfin, int yfin, int MAXX, int MAXY);

typedef struct {
    int xini, yini;
    int xfin, yfin;
    long area;
} pair;

int compare_desc(const void *a, const void *b) {
    pair *aa = (pair *) a;
    pair *bb = (pair *) b;
    if (bb->area > aa->area) return 1;
    if (bb->area < aa->area) return -1;
    return 0;
}

typedef struct {
    int key;
    int horiz;
    int line;
    int min, max;
    UT_hash_handle hh;
} line_info;
line_info *lines = NULL;

line_info *get_line(int horizontal, int line) {
    //printf("get line: %d %d\n", horizontal, line);
    line_info *li;
    int key = line;
    if (horizontal) {
        key += 100000;
    }
    HASH_FIND_INT(lines, &key, li);
    return li;
}
void set_line(int horizontal, int line, int min, int max) {
    //printf("set line: %d %d, %d, %d\n", horizontal, line, min, max);
    line_info *li = get_line(horizontal, line);
    if (li == NULL) {
        li = malloc(sizeof *li);
        int key = line;
        if (horizontal) {
            key += 100000;
        }
        li->key = key;
        li->horiz = horizontal;
        li->line = line;
        HASH_ADD_INT(lines, key, li);
    }
    li->min = min;
    li->max = max;
}

typedef struct {
    long key;
    char c;
    UT_hash_handle hh;
} tile;
tile *tiles = NULL;
void add_tile(int x, int y, int maxx, char c) {
    tile *s;
    s = malloc(sizeof *s);
    long lx = (long) x;
    long ly = (long) y;
    long lmaxx = (long) maxx+1;
    s->key = ly * lmaxx + lx;
    s->c = c;
    HASH_ADD_INT(tiles, key, s);
}

char cat(int x, int y, int maxx) {
    tile *t = NULL;
    long lx = (long) x;
    long ly = (long) y;
    long lmaxx = (long) maxx+1;
    long key = ly * lmaxx + lx;
    HASH_FIND_INT(tiles, &key, t);
    if (t == NULL) {
        return ' ';
    }
    return t->c;
}


int main(int argc, char **argv){
    int MAX = 0;
    int MAXX = 0;
    int MAXY = 0;
    if (strcmp(FILENAME, "input/input.txt") == 0) {
    }
    FILE *f;
    int bufferLength = 255;
    char buffer[bufferLength];

    f = fopen(FILENAME, "r");
    if (f == NULL)
        exit(EXIT_FAILURE);

    long xs[1000];
    long ys[1000];
    for (int ii = 0; ii < 1000; ii++) {
        xs[ii] = -1;
        ys[ii] = -1;
    }
    MAX = 0;
    while(fgets(buffer, bufferLength, f)) {
        printf("%s\n", buffer);
        long x, y;
        sscanf(buffer, "%ld,%ld", &x, &y);
        xs[MAX] = x;
        ys[MAX] = y;
        if (x > MAXX) {
            MAXX = x;
        }
        if (y > MAXY) {
            MAXY = y;
        }
        MAX++;
    }
    printf("maxx: %d, maxy: %d\n", MAXX, MAXY);
    long max_area = 0;
    long ALL_MAX = (MAX * (MAX-1))/2;
    pair all_pairs[ALL_MAX];
    for (int ii = 0; ii < ALL_MAX; ii++) {
            all_pairs[ii].area = 0;
    }
    int all_index = 0;
    for (int ii = 0; ii < MAX; ii++) {
        for (int jj = ii + 1; jj < MAX; jj++) {
            long sidex = xs[ii] - xs[jj];
            if (sidex < 0) {
                sidex = -sidex;
            }
            long sidey = ys[ii] - ys[jj];
            if (sidey < 0) {
                sidey = -sidey;
            }
            long area = (sidex + 1) * (sidey + 1);
            all_pairs[all_index].xini = xs[ii];
            all_pairs[all_index].yini = ys[ii];
            all_pairs[all_index].xfin = xs[jj];
            all_pairs[all_index].yfin = ys[jj];
            all_pairs[all_index].area = area;
            if (max_area < area) {
                max_area = area;
                printf("max area (%ld) between points %d (%ld, %ld) and %d (%ld, %ld)\n",
                    max_area, ii, xs[ii], ys[ii], jj, xs[jj], ys[jj]);
            }
            all_index++;
//            areas[ii] = area;
        }
    }
    qsort(all_pairs, ALL_MAX, sizeof(pair), compare_desc);

    printf("part 1: %ld\n\n", max_area);
    printf("part 1: area: %ld ini: %d, %d fin: %d, %d\n\n", all_pairs[0].area, all_pairs[0].xini, all_pairs[0].yini, all_pairs[0].xfin, all_pairs[0].yfin);
    printf("part 1: area: %ld ini: %d, %d fin: %d, %d\n\n", all_pairs[1].area, all_pairs[1].xini, all_pairs[1].yini, all_pairs[1].xfin, all_pairs[1].yfin);
    //part 2

    for (int ii = 0; ii < MAX; ii++) {
        long xini = xs[ii];
        long yini = ys[ii];
        long xfin, yfin;
        if (ii == MAX-1) {
            xfin = xs[0];
            yfin = ys[0];
        } else {
            xfin = xs[ii+1];
            yfin = ys[ii+1];
        }
        fill_line((int)xini, (int)yini, (int)xfin, (int)yfin, MAXX);
    }
    printf("lines filled\n\n");
    if (MAXX < 30) {
        for (int yy = 0; yy <= MAXY; yy++) {
            for (int xx = 0; xx <= MAXX; xx++) {
                char c = cat(xx, yy, MAXX);
                printf("%c",c);
            }
            printf("\n");
        }
        for (int yy = 0; yy <= MAXY; yy++) {
            printf("h line %d\t = ", yy);
            line_info *lih = get_line(1, yy);
            if (lih == NULL) {
                printf("no data\n");
            } else {
                printf("min: %d \t max %d\n", lih->min, lih->max);
            }
        }
        for (int xx = 0; xx <= MAXX; xx++) {
            printf("v line %d\t = ", xx);
            line_info *liv = get_line(0, xx);
            if (liv == NULL) {
                printf("no data\n");
            } else {
                printf("min: %d \t max %d\n", liv->min, liv->max);
            }
        }
    }

    long max_area_2 = 0;
    //can do binary search with ii?
    //21508
    for (int ii = 11027; ii < ALL_MAX; ii++) {
        pair p = all_pairs[ii];
        long area = p.area;
        printf("------>current max area for (%d) =  %ld\t current area examined: %ld\n\n", ii, max_area_2, area);
        if (area > max_area_2) {
            printf("\nany non green for area %ld in %d, %d and %d, %d\n", p.area, p.xini, p.yini, p.xfin, p.yfin);
            int angti = any_non_green_tile_in((int)p.xini, (int)p.yini, (int)p.xfin, (int)p.yfin, MAXX, MAXY);
            long bsup = ii/2;
            long bsdown = (ii + ALL_MAX)/2;
            printf("angti: %d   %d of %ld\t bsup: %ld bsdown: %ld\n", angti, ii, ALL_MAX, bsup, bsdown);
            if (angti) {
                printf("a non green :(\n");
//                continue;
            } else {
                printf("only greens! area: %ld  vs  max area: %ld\n", area, max_area_2);
                if (max_area_2 < area) {
                    max_area_2 = area;
                    printf("only greens! max area: %ld\n", area);
                    break;
                }
            }
            printf("max area (%ld) between points %d (%d, %d) and (%d, %d)\n",
                max_area_2, ii, p.xini, p.yini, p.xfin, p.yfin);
        }
    }
    printf("part 2 max area: %ld\n", max_area_2);
    //max area (133705) between points 0 (97997, 50117) and 4 (97943, 52549)
    //133705 too low
    //186690 too low
    //3023223824 too high
    //1577956170 right
}

void fill_line(int xini, int yini, int xfin, int yfin, int MAXX) {
    int ax = xini;
    int ay = yini;
    int bx = xfin;
    int by = yfin;
    int horizontal = 0;
    if (xini == xfin) {
        horizontal = 1;
        if (yini > yfin) {
            ay = yfin;
            by = yini;
        }
    }
    if (yini == yfin) {
        horizontal = 0;
        if (xini > xfin) {
            ax = xfin;
            bx = xini;
        }
    }
    printf("filling with green on line %d, %d to %d, %d\n", ax, ay, bx, by);
    for (int xx = ax; xx <= bx; xx++) {
        for (int yy = ay; yy <= by; yy++) {
            long pos = (long)yy * (long)MAXX + (long)xx;
            //printf("green at %d,%d = %ld\n", xx,yy, pos);
            add_tile(xx, yy, MAXX, 'g');
            line_info *li_h = get_line(1, yy);
            //printf("lines: lih: %p\n", li_h);
            line_info *li_v = get_line(0, xx);
            //printf("lines: liv: %p\n", li_v);
            if (li_h == NULL) {
                set_line(1, yy, ax, bx);
            } else {
                int minx = li_h->min;
                int maxx = li_h->max;
                if (ax < minx) {
                    minx = ax;
                }
                if (bx > maxx) {
                    maxx = bx;
                }
                set_line(1, yy, minx, maxx);
            }
            if (li_v == NULL) {
                set_line(0, xx, ay, by);
            } else {
                int miny = li_v->min;
                int maxy = li_v->max;
                if (ay < miny) {
                    miny = ay;
                }
                if (by > maxy) {
                    maxy = by;
                }
                set_line(0, xx, miny, maxy);
            }
        }
    }
}

int check_tile(int xx, int yy, int MAXX) {
    char c = cat(xx, yy, MAXX);
    if (c == 'g') {
        return 0;
    }
    line_info *lh = get_line(1, yy);
    line_info *lv = get_line(0, xx);
    if (xx < lh->min || xx > lh->max) {
        printf("xx (%d) not inside %d and %d   at height %d\n", xx, lh->min, lh->max, yy);
        return 1;
    }
    if (yy < lv->min || yy > lv->max) {
        printf("yy (%d) not inside %d and %d    at width %d\n", yy, lv->min, lv->max, xx);
        return 1;
    }
    return 0;
}

int any_non_green_tile_in(int xini, int yini, int xfin, int yfin, int MAXX, int MAXY) {
    int ax = xini;
    int ay = yini;
    int bx = xfin;
    int by = yfin;
    if (yini > yfin) {
        ay = yfin;
        by = yini;
    }
    if (xini > xfin) {
        ax = xfin;
        bx = xini;
    }
    int count = 0;
    printf("l search: ax = %d, from %d to %d y\n", ax, ay, by);
    for (int yy = ay; yy <= by; yy++) {
        int result_for_tile = check_tile(ax, yy, MAXX);
        if (result_for_tile == 1) {
            return 1;
        }
    }
    printf("t search: ay = %d, from %d to %d x\n", ay, ax, bx);
    for (int xx = ax; xx <= bx; xx++) {
        int result_for_tile = check_tile(xx, ay, MAXX);
        if (result_for_tile == 1) {
            return 1;
        }
    }
    printf("r search: bx = %d, from %d to %d y\n", bx, ay, by);
    for (int yy = ay; yy <= by; yy++) {
        int result_for_tile = check_tile(bx, yy, MAXX);
        if (result_for_tile == 1) {
            return 1;
        }
    }
    printf("b search: ay = %d, from %d to %d x\n", by, ax, bx);
    for (int xx = ax; xx <= bx; xx++) {
        int result_for_tile = check_tile(xx, by, MAXX);
        if (result_for_tile == 1) {
            return 1;
        }
    }
    return 0;
}
