#include "mapped-log.h"

typedef struct {
    int x;
    int y;
} Foo;

#define INC_SIZE 4096
#define LOGFILE "data"

int main(int argc, const char *argv[])
{
    MappedLog log;
    int i;

    // create a log and write some data.
    if (new_mapped_log(LOGFILE, &log, INC_SIZE) != 0) {
        exit(1);
    }
    for (i = 0; i < 2*INC_SIZE; i++) {
        Foo *e = (Foo *)next_log_entry(&log, sizeof(*e));
        if (e == NULL) {
            exit(1);
        }
        e->x = i;
        e->y = i + 1;
    }
    // write -1 to indicate end of log.
    Foo *e = (Foo *)next_log_entry(&log, sizeof(*e));
    if (e == NULL) {
        exit(1);
    }
    e->x = -1;
    unmap_log(&log);

    // read logged data and check for correctness.
    if (open_mapped_log(LOGFILE, &log) != 0) {
        exit(1);
    }
    for (i = 0; ; i++) {
        Foo *e = (Foo *)next_log_entry(&log, sizeof(*e));
        if (e == NULL) {
            exit(1);
        }
        if (e->x == -1) {
            break;
        }
        if (e->x != i || e->y != i+1) {
            printf("logged data in correct. i=%d x=%d y=%d\n", i, e->x, e->y);
            exit(1);
        }
    }
    unmap_log(&log);

    return 0;
}
