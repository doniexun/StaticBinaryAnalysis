/*
** test.c for  in /home/bartholomew/programmation/c/prototype
**
** Made by Bartholomew
** Bartholomew de la Villardiere <exabot@gmail.com>
**
** Started on  mer. mars 12 02:33:04 2014
** Last update mer. mars 12 02:33:04 2014
*/

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "get_nb_params.h"

int main(int argc, char** argv)
{
    char* fct_name = 0;
    size_t size = 0;
    int nb_params = 0;
    void* handle = 0;
    const uint8_t* fct;
    char* error = 0;

    if (argc != 2)
    {
        printf("usage: %s lib_path\n", argv[0]);
        return (1);
    }

    handle = dlopen(argv[1], RTLD_LOCAL | RTLD_LAZY);
    if (handle == 0)
    {
        fprintf(stderr, "%s\n", dlerror());
        return (1);
    }
    dlerror();

    while (1)
    {
        fct_name = 0;
        printf("fct_name: ");
        fflush(stdout);
        size = getline(&fct_name, &size, stdin);
        if (size <= 0)
            break;

        fct_name[size - 1] = 0;
        fct = dlsym(handle, fct_name);
        error = dlerror();
        if (error != NULL)
        {
            fprintf(stderr, "%s\n", dlerror());
            return (1);
        }

        nb_params = get_nb_params(fct);
        if (nb_params != -1)
            printf("Parameters number: %d.\n", nb_params);
        else
            printf("Error.\n");
        free(fct_name);
    }

    dlclose(handle);
    return (0);
}

