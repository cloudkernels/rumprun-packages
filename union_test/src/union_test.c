#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN 256
#define DEFAULT_MAX 5
#define EMPTY_LAYER_STR "from empty.layer\n"

void listdir(const char *path) {
    DIR *dirp;
    struct dirent *dp;

    printf("Listing %s:\n", path);

    dirp = opendir(path);
    if (!dirp)
        return;

    while (dp = readdir(dirp)) {
        printf("\t%s\n", dp->d_name);
    }

    printf("\n");
    closedir(dirp);
}

int get_random(int max) {
    return 1 + rand() % max;
}

int
main(int argc, char **argv)
{
    int fd, n, max;
    char buf[BUF_LEN];

    printf("Union test ...\n");

    if (argc != 2) {
        max = DEFAULT_MAX;
    } else { 
        max = strtol(argv[1], NULL, 10);
    }

    listdir("/");

    snprintf(buf, BUF_LEN, "/%d", get_random(max));
    printf("Reading from %s\n", buf);
    fd = open(buf, O_RDONLY);
    if (fd < 0)
        warn("reading %s", buf);
    else {
        n = read(fd, buf, BUF_LEN);
        write(2, buf, n);
        close(fd);
    }

    snprintf(buf, BUF_LEN, "/%d", get_random(max));
    printf("Writing to %s\n", buf);
    fd = open(buf, O_WRONLY | O_TRUNC);
    if (fd < 0)
        warn("writing %s", buf);
    else {
        n = write(fd, EMPTY_LAYER_STR, strlen(EMPTY_LAYER_STR));
        printf("Wrote %d bytes to %s\n", n, buf);
        close(fd);
    }

    printf("Reading from %s\n", buf);
    fd = open(buf, O_RDONLY);
    if (fd < 0)
        warn("reading %s", buf);
    else {
        n = read(fd, buf, BUF_LEN);
        write(2, buf, n);
        close(fd);
    }

    snprintf(buf, BUF_LEN, "/%d", max + 1);
    printf("Creating %s\n", buf);
    fd = open(buf, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    if (fd < 0)
        warn("creating %s", buf);
    else {
        n = write(fd, EMPTY_LAYER_STR, strlen(EMPTY_LAYER_STR));
        printf("Wrote %d bytes to %s\n", n, buf);

        if (lseek(fd, 0, SEEK_SET) < 0) {
            warn("lseek %s", buf);
        }

        n = read(fd, buf, BUF_LEN);
        write(2, buf, n);
        close(fd);
    }

    snprintf(buf, BUF_LEN, "/%d", get_random(max));
    printf("Unlinking %s\n", buf);
    if (unlink(buf) < 0)
        warn("unlinking %s", buf);

    listdir("/");

    sleep(2);

    return 0;
}
