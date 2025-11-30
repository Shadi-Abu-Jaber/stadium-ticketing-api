#ifndef MATAM_HOMEWORK_3_STADIUM_H
#define MATAM_HOMEWORK_3_STADIUM_H

#include "Stand.h"

#define MAX_STAND_NAME_LEN 513

typedef struct Stadium *Stadium;

Stadium StadiumCreate(char **stands_names, unsigned int *stands_capacities, unsigned int num_stands);

void create_stands(Stadium stadium, char **stands_names, unsigned int *stands_capacities);

Stadium StadiumCreateFromFile(const char *file_name);

unsigned int get_num_of_stands(const char *file_name);

void StadiumDestroy(Stadium stadium);

unsigned int StadiumNumStands(Stadium stadium);

unsigned int StadiumCapacity(Stadium stadium);

Stand StadiumGetStandByName(Stadium stadium, char *stand_name);

Stand StadiumGetStandByIndex(Stadium stadium, unsigned int index);

#endif //MATAM_HOMEWORK_3_STADIUM_H
