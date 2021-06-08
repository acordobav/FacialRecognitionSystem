#include <stdio.h>
#include <dirent.h> // Show directory files

#include "linked_list.c"

llist *get_directory_content(char *folder_path)
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(folder_path)) == NULL)
    {
        /* could not open directory */
        perror("");
        return NULL;
    }

    /* print all the files and directories within directory */
    llist* list = create_list();
    while ((ent = readdir(dir)) != NULL)
    {
        char *filename = ent->d_name;
        if (strcmp(filename, ".") != 0 &&
            strcmp(filename, "..") != 0)
        {
            // Copy the filename
            char *filename = (char *)malloc(strlen(ent->d_name));
            strcpy(filename, ent->d_name);

            // Store filename into list
            insert_first(list, filename);
        }
    }

    closedir(dir);
    return list;
}
