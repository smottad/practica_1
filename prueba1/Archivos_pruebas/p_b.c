#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF 1024

typedef struct {
    int sourceID;
    int dstID;
    int hod;
    double mean_travel_time;
} Trip;

int main()
{
    char *myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);

    int fd;
    char buf[MAX_BUF];

    Trip trip;
    int sourceID, dstID, hod;

    while (1) {
        fd = open(myfifo, O_RDONLY);
        read(fd, buf, MAX_BUF);

        sscanf(buf, "%d %d %d", &sourceID, &dstID, &hod);

        FILE *fp = fopen("trips.bin", "rb");
        if (!fp) {
            printf("Error opening file: No such file or directory\n");
            write(fd, "NA", 2);
            close(fd);
            continue;
        }

        int found = 0;
        while (fread(&trip, sizeof(Trip), 1, fp)) {
            if (trip.sourceID == sourceID && trip.dstID == dstID && trip.hod == hod) {
                printf("The mean travel time for the trip from source ID %d to destination ID %d at hour of day %d is %lf\n",
                        trip.sourceID, trip.dstID, trip.hod, trip.mean_travel_time);
                char result[20];
                snprintf(result, sizeof(result), "%lf", trip.mean_travel_time);
                write(fd, result, strlen(result)+1);
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("No trips found for the given parameters\n");
            write(fd, "NF", 2);
        }

        fclose(fp);
        close(fd);
    }

    return 0;
}
