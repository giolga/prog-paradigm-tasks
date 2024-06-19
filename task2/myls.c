#include <dirent.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

bool is_dir(int filetype) {
    return filetype == DT_DIR;
}

int is_fold(const struct dirent *entry) {
    return is_dir(entry->d_type);
}

int is_file(const struct dirent *entry) {
    return !is_fold(entry);
}

int wi_dot(const struct dirent *entry) {
    return (entry->d_name)[0] == '.';
}

int wo_dot(const struct dirent *entry) {
    return !wi_dot(entry);
}

int fold_wo_dot(const struct dirent *entry) {
    return wo_dot(entry) && is_fold(entry);
}

int file_wo_dot(const struct dirent *entry) {
    return wo_dot(entry) && is_file(entry);
}

int dir_comp(const struct dirent **a, const struct dirent **b) {
    return strcasecmp((*a)->d_name, (*b)->d_name);
}

int type_comp(const struct dirent **a, const struct dirent **b) {
    int type_a = is_dir((*a)->d_type) ? 0 : 1;
    int type_b = is_dir((*b)->d_type) ? 0 : 1;
    if (type_a == type_b) {
        return strcasecmp((*a)->d_name, (*b)->d_name);
    }
    return type_a - type_b;
}

void ls(const char *dir, bool show_all, bool sort_by_type) {
    struct dirent **lists[2] = {NULL, NULL};
    int nums[2] = {-2, -2};

    if (show_all == true && sort_by_type == true) {
        nums[0] = scandir(dir, &lists[0], is_fold, type_comp);
        nums[1] = scandir(dir, &lists[1], is_file, type_comp);
    } else if (show_all == true && sort_by_type == false) {
        nums[0] = scandir(dir, &lists[0], NULL, dir_comp);
    } else if (show_all == false && sort_by_type == false) {
        nums[0] = scandir(dir, &lists[0], wo_dot, dir_comp);
    } else { // if (show_all == false && sort_by_type == true)
        nums[0] = scandir(dir, &lists[0], fold_wo_dot, type_comp);
        nums[1] = scandir(dir, &lists[1], file_wo_dot, type_comp);
    }

    for (int i = 0; i < 2; i++) {
        if (nums[i] == -1) {
            printf("./myls: cannot access %s: Permission denied\n", dir);
            continue;
        }
        if (lists[i] == NULL || nums[i] == -2)
            continue;
        for (int j = 0; j < nums[i]; j++) {
            struct dirent *entry = lists[i][j];
            if (is_fold(entry))
                printf("%s/\n", entry->d_name);
            else
                printf("%s\n", entry->d_name);
            free(entry);
        }
        free(lists[i]);
    }
}

int main(int argc, char *argv[]) {
    bool show_all = false;
    bool dirs_first = false;

    int opt;
    while ((opt = getopt(argc, argv, "az")) != -1) {
        switch (opt) {
            case 'a': show_all = true; 
                break;
            case 'z': dirs_first = true; 
                break;
            default: exit(1);
        }
    }

    if (optind < argc) {
        for (int i = optind; i < argc; i++) {
            printf("%s:\n", argv[i]);
            ls(argv[i], show_all, dirs_first);
            printf("\n");
        }
    } 
    else {
        ls(".", show_all, dirs_first);
    }
    
    return 0;
}
