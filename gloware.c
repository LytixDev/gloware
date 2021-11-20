#include <stdio.h>
#include <string.h>
#include <dirent.h>

// d_type = 4 -> directory
#define FOLDER 4
const char *FILES_TO_ENCRYPT = "dummy_files";
const char *ENCRYPTION_KEY = "NFTs_are_a_scam";

void find_files(const char *dir_name)
{
    DIR *dir = opendir(dir_name);
    if (dir == NULL) {
        printf("Dir = NULL");
        return;
    }
    
    struct dirent *entity;
    entity = readdir(dir);
    while (entity != NULL) {
        char relative[255] = {0};
        strcat(relative, dir_name);
        strcat(relative, "/");
        strcat(relative, entity->d_name);
        if (strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0)
            printf("%hhd %s\n", entity->d_type, relative);
        if (entity->d_type == FOLDER && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0) {
            char path[100] = {0};
            strcat(path, dir_name);
            strcat(path, "/");
            strcat(path, entity->d_name);
            find_files(path);
        }
        entity = readdir(dir);
    }
    closedir(dir);
}

int main()
{
    find_files(FILES_TO_ENCRYPT);
}

// AES stuff
// encrypt, pad
// decrypt, unpad


char *get_file_content()
{
}

char *encrypt_file()
{
}

char *decrypt_file()
{
}

void write_to_file()
{
}

