#include "Stadium.h"

/******************************************************************************/
struct Stadium {
    Stand *stands;
    unsigned int num_stands;
};

/******************************************************************************/
Stadium StadiumCreate(char **stands_names, unsigned int *stands_capacities,
                      unsigned int num_stands) {
    Stadium new_stadium = (Stadium) malloc(sizeof(struct Stadium));
    if (!new_stadium) {
        fprintf(stderr, "Failed To Allocate Memory!\n");
        fprintf(stderr, "File name is %s\n", __FILE__);
        fprintf(stderr, "Line is %d\n", __LINE__);
        exit(1);
    }

    new_stadium->num_stands = num_stands;

    new_stadium->stands = (Stand *) malloc(sizeof(Stand) * num_stands);
    if (!new_stadium->stands) {
        free(new_stadium);
        fprintf(stderr, "Failed To Allocate Memory!\n");
        fprintf(stderr, "File name is %s\n", __FILE__);
        fprintf(stderr, "Line is %d\n", __LINE__);
        exit(1);
    }
    create_stands(new_stadium, stands_names, stands_capacities);

    return new_stadium;
}

/******************************************************************************/
void create_stands(Stadium new_stadium, char **stands_names,
                   unsigned int *stands_capacities) {
    int i;
    for (i = 0; i < new_stadium->num_stands; i++) {
        new_stadium->stands[i] = StandCreate(stands_names[i],
                                             stands_capacities[i]);
    }
}

/******************************************************************************/
Stadium StadiumCreateFromFile(const char *file_name) {
    unsigned int capacity;
    unsigned int num_stands = get_num_of_stands(file_name);
    char *name = malloc(sizeof(char) * MAX_STAND_NAME_LEN);
    if (!name) {
        fprintf(stderr, "Failed To Allocate Memory!\n");
        fprintf(stderr, "File name is %s\n", __FILE__);
        fprintf(stderr, "Line is %d\n", __LINE__);
        exit(1);
    }
    char **stands_names = calloc(num_stands, sizeof(char *));
    if (!stands_names) {
        fprintf(stderr, "Failed To Allocate Memory!\n");
        fprintf(stderr, "File name is %s\n", __FILE__);
        fprintf(stderr, "Line is %d\n", __LINE__);
        exit(1);
    }
    unsigned int *stands_capacities = malloc(sizeof(int) * num_stands);
    if (!stands_capacities) {
        fprintf(stderr, "Failed To Allocate Memory!\n");
        fprintf(stderr, "File name is %s\n", __FILE__);
        fprintf(stderr, "Line is %d\n", __LINE__);
        exit(1);
    }
    FILE *file = fopen(file_name, "r");
    if (!file) {
        fprintf(stderr, "Failed To Open File!\n");
        fprintf(stderr, "File name is %s\n", __FILE__);
        fprintf(stderr, "Line is %d\n", __LINE__);
        exit(1);
    }
    int i;
    for (i = 0; i < num_stands; i++) {
        if (fscanf(file, "%s%d", name, &capacity) != 2) {
            fprintf(stderr, "Error reading from file\n");
            break;
        }
        stands_names[i] = (char *) malloc(sizeof(char) * (strlen(name) + 1));
        if (!stands_names[i]) {
            fprintf(stderr, "Failed To Allocate Memory!\n");
            fprintf(stderr, "File name is %s\n", __FILE__);
            fprintf(stderr, "Line is %d\n", __LINE__);
            exit(1);
        }

        strcpy(stands_names[i], name);
        stands_capacities[i] = capacity;
    }
    fclose(file);

    Stadium stadium = StadiumCreate(stands_names, stands_capacities, num_stands);

    free(name);
    free(stands_capacities);
    for (i = 0; i < num_stands; i++) {
        free(stands_names[i]);
    }
    free(stands_names);
    return stadium;
}

/******************************************************************************/
unsigned int get_num_of_stands(const char *file_name) {
    char *buff = NULL;
    size_t buff_size = 0;
    unsigned int num_stands = 0;
    FILE *file = fopen(file_name, "r");
    if (!file) {
        fprintf(stderr, "Failed To Open File!\n");
        fprintf(stderr, "File name is %s\n", __FILE__);
        fprintf(stderr, "Line is %d\n", __LINE__);
        exit(1);
    }
    while (getline(&buff, &buff_size, file) != EOF) {
        if (strcmp(buff, "\n") == 0) {
            continue;
        }
        // each line is a stand
        num_stands++;
    }
    fclose(file);
    free(buff);
    return num_stands;
}

/******************************************************************************/
void StadiumDestroy(Stadium stadium) {
    int i;
    for (i = 0; i < stadium->num_stands; i++) {
        StandDestroy(stadium->stands[i]);
    }
    free(stadium->stands);
    free(stadium);
}

/******************************************************************************/
unsigned int StadiumNumStands(Stadium stadium) {
    return stadium->num_stands;
}

/******************************************************************************/
unsigned int StadiumCapacity(Stadium stadium) {
    int i;
    unsigned int stadium_capacity = 0;
    for (i = 0; i < stadium->num_stands; i++) {
        stadium_capacity += StandMaxCapacity(stadium->stands[i]);
    }
    return stadium_capacity;
}

/******************************************************************************/
Stand StadiumGetStandByName(Stadium stadium, char *stand_name) {
    int i;
    const char *curr_stand_name;
    for (i = 0; i < stadium->num_stands; i++) {
        curr_stand_name = StandName(stadium->stands[i]);
        int is_same_name = strcmp(curr_stand_name, stand_name);
        if (is_same_name == 0) {
            return stadium->stands[i];
        }
    }
    return NULL;
}

/******************************************************************************/
Stand StadiumGetStandByIndex(Stadium stadium, unsigned int index) {

    if (index > stadium->num_stands - 1) {
        return NULL;
    }

    int i;
    for (i = 0; i < stadium->num_stands; i++) {
        if (i == index) {
            return stadium->stands[index];
        }
    }

    return NULL;
}

/******************************************************************************/