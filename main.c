#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 1000

typedef struct {
    char *producer;
    char *model;
    int bunker_capacity;
    int weight;
    float loading_speed;
    float speed;
    float price[2];
} combine;

int compare(const char *str, const char *substr) {
    int i;
    int flag = 1;
    for (i = 0; str[i] && substr[i] && flag!=0; i++) {
            if ((str[i]) != (substr[i])) {
                flag = 0;
            }
    }
    return flag;
}
void free_memory(combine **combines, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if ((*combines)[i].producer != NULL) {
            free((*combines)[i].producer);
            (*combines)[i].producer = NULL;
        }
        if ((*combines)[i].model != NULL) {
            free((*combines)[i].model);
            (*combines)[i].model = NULL;
        }
    }
    if ((*combines) != NULL) {
        free(*combines);
        *combines = NULL;
    }
}

void add_new_position(combine *combines, char *s1, char *s2, int a1, int a2, float b1, float b2, float b3, float b4,
                      int *mem) {
    combines->producer = (char *) malloc((strlen(s1) + 1) * sizeof(char));
    combines->model = (char *) malloc((strlen(s2) + 1) * sizeof(char));
    if (combines->producer != NULL && combines->model != NULL) {
        strcpy(combines->producer, s1);
        strcpy(combines->model, s2);
        combines->bunker_capacity = a1;
        combines->weight = a2;
        combines->loading_speed = b1;
        combines->speed = b2;
        combines->price[0] = b3;
        combines->price[1] = b4;
    } else {
        (*mem) = 0;
    }
}

int input_from_file(combine **combines, int *mem) {
    FILE *f;
    int cnt = 0;
    char pathfile[N];
    char s1[N], s2[N];
    int a1, a2;
    int i;
    float b1, b2, b3, b4;
    printf("Enter the path to the file:\n");
    fgets(pathfile, N, stdin);
    pathfile[strcspn(pathfile, "\n")] = '\0';
    f = fopen(pathfile, "r");
    while (f == NULL) {
        printf("Invalid file name \nEnter the path to the file again: ");
        fgets(pathfile, N, stdin);
        pathfile[strcspn(pathfile, "\n")] = '\0';
        f = fopen(pathfile, "r");
    }
    while (fscanf(f, "%[^,],%[^,],%d,%d,%f,%f,%f,%f\n", s1, s2, &a1, &a2, &b1, &b2, &b3, &b4) == 8) {
        cnt++;
    }
    rewind(f);
    *combines = (combine *) malloc((cnt) * sizeof(combine));
    if (*combines != NULL) {
        for (i = 0; i < cnt; i++) {
            fscanf(f, "%[^,],%[^,],%d,%d,%f,%f,%f,%f\n", s1, s2, &a1, &a2, &b1, &b2, &b3, &b4);
            add_new_position(&(*combines)[i], s1, s2, a1, a2, b1, b2, b3, b4, mem);
        }
    } else {
        (*mem = 0);
    }
    fclose(f);
    return cnt;
}


void input_from_consol(combine **combines, int n, int k, int *mem) {
    *combines = (combine *) realloc(*combines, (n + k) * sizeof(combine));
    char s1[N], s2[N];
    int a1, a2;
    int i;
    float b1, b2, b3, b4;
    if (*combines != NULL) {
        printf("Input the following information about the combines, separated by commas:\n");
        printf("Producer, Model, Bunker_capacity, Weight, Loading speed, Speed, Price in dollars, Price in rubles\n");
        for (i = 0; i < k; i++) {
            printf("%d combine: ", i + 1);
            scanf("%[^,], %[^,],%d,%d,%f,%f,%f,%f", s1, s2, &a1, &a2, &b1, &b2, &b3, &b4);
            memmove(s1, s1 + 1, strlen(s1));
            add_new_position(&(*combines)[n + i], s1, s2, a1, a2, b1, b2, b3, b4, mem);
        }
    } else {
        (*mem) = 0;
    }
}

void sort_last(combine **combines, int n) {
    combine temp;
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if ((*combines)[j].speed < (*combines)[j + 1].speed) {
                temp = (*combines)[j];
                (*combines)[j] = (*combines)[j + 1];
                (*combines)[j + 1] = temp;
            }
        }
    }
}

void output_console(combine **combines, int n) {
    int field;
    int found = 0;
    char search[N];
    int i;
    char s1[N],s2[N];
    if (n!=0) {
        printf("| Producer           | Model          | Bunker capacity  | Weight | Loading speed | Speed |     Price ($ and P)    |\n");
        printf("+--------------------+----------------+------------------+--------+---------------+-------+------------------------+\n");
        for (i = 0; i < n; i++) {
            printf("| %-18s | %-14s | %-16d | %-6d | %-14.2f| %-5.2f | %-6.2f  %-6.2f | \n", (*combines)[i].producer,
                   (*combines)[i].model, (*combines)[i].bunker_capacity, (*combines)[i].weight,
                   (*combines)[i].loading_speed, (*combines)[i].speed,
                   (*combines)[i].price[0], (*combines)[i].price[1]);
        }
        printf("+--------------------+----------------+------------------+--------+---------------+-------+------------------------+\n");
        printf("Choose search field (1 - by producer, 2 - by model):\n");
        scanf("%d", &field);
        getchar();
        if (field == 1) {
            printf("Enter the producer:\n");
            fgets(search, sizeof(s1), stdin);
        } else {
            printf("Enter the model:\n");
            fgets(search, sizeof(s1), stdin);
        }
        printf("\n");
        for (i = 0; i < n; i++) {
            strcpy(s1,(*combines)[i].producer);
            strcpy(s2,(*combines)[i].model);
            if ((field == 1 && compare(&s1, &search) == 1) ||
                (field == 2 && compare(&s2, &search) == 1)) {
                if (found == 0) {
                    printf("| Producer           | Model          | Bunker capacity  | Weight | Loading speed | Speed |     Price ($ and P)    |\n");
                    printf("+--------------------+----------------+------------------+--------+---------------+-------+------------------------+\n");
                }
                printf("| %-18s | %-14s | %-16d | %-6d | %-14.2f| %-5.2f | %-6.2f  %-6.2f | \n",
                       (*combines)[i].producer,
                       (*combines)[i].model, (*combines)[i].bunker_capacity, (*combines)[i].weight,
                       (*combines)[i].loading_speed, (*combines)[i].speed,
                       (*combines)[i].price[0], (*combines)[i].price[1]);
                found = 1;
            }
        }
        if (!found) {
            printf("There are no suggestions for the specified parameters.\n");
        } else {
            printf("+--------------------+----------------+------------------+--------+---------------+-------+------------------------+\n");
        }
    } else{
        printf("No data available\n");
    }
}


int main() {
    combine *combines = NULL;
    int n = 0, mem = 1, choice;
    int c;
    printf("Do you want to read data from a file (0 - no, 1 - yes)?\n");
    scanf("%d", &c);
    getchar();
    if (c == 1){
        n = input_from_file(&combines, &mem);
    }
    if (mem != 0) {
        printf("Do you want to add elements? Enter the number of elements to add :\n");
        scanf("%d", &choice);
        printf("\n");
        if (choice != 0) {
            input_from_consol(&combines, n, choice, &mem);
            n += choice;
            printf("\n");
        }
    }
    if (mem != 0) {
        sort_last(&combines, n);
        output_console(&combines, n);
    } else {
        printf("Error at memory allocation!");
    }
    free_memory(&combines, n);
    return 0;
}
