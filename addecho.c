#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HEADER_SIZE 11

int main(int argc, char *argv[])
{
    //Setting up for reading the command line
    int delay = 8000;
    int volume = 4;
    int opt;
    int error;
    if (argc < 3 || argc > 7 || argc == 4 || argc == 5)
    {
        fprintf(stderr, "Usage: %s [-d delay time] [-v volume of echo] <filename1> <filename2>\n", argv[0]);
        return 1;
    }
    while ((opt = getopt(argc, argv, "d:v:")) != -1) {
        switch (opt) {
        case 'd': ;
            char *p;
            int d = strtol(optarg, &p, 10);
            if (d>0 && p[0] == '\0')
            {
                delay = d;
            }
            else
            {
                fprintf(stderr, "Invalid number for -d. Delay has to be a number more than 0\n");
                return 1;
            }
            break;
        case 'v':;
            char *p1;
            int v = strtol(optarg, &p1, 10);
            if (v > 0 && p1[0]=='\0')
            {
                volume = v;
            }
            else
            {
                fprintf(stderr, "Invalid number for -v. Volume scale has to be a number more than 0\n");
                return 1;
            }
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-d delay time] [-v volume of echo] <filename1> <filename2>\n",
                    argv[0]);
            return 1;
        }
    }
    char *input_name = argv[argc-2];
    char *output_name = argv[argc-1];

    if (strcmp(input_name, output_name) == 0)
     {
         fprintf(stderr, "ERROR: You cannot overwrite your original file\n");
         return 1;
     }

    if (optind >= argc) {
        fprintf(stderr, "Expected argument after options\n");
        return 1;
    }
    //Setting up input and output file

    FILE *orig = fopen(input_name, "rb");
    if (orig == NULL)
    {
        fprintf(stderr, "ERROR: Cannot open file\n");
        return 1;
    }

    FILE *echo = fopen(output_name, "wb");
    if (echo == NULL)
    {
        fprintf(stderr, "ERROR: Cannot open file\n");
    }

    int r;
    unsigned int value;
    int sample_size;
    for(int x=0;x<HEADER_SIZE;x++)
    {
        r = fread(&value, sizeof(int), 1, orig);
        if(r == 0)
        {
            fprintf(stderr, "ERROR: File cannot read\n");
        }
        if (x == 1 || x ==10)
        {
            value += 2*delay;
            sample_size = value;
        }
        error = fwrite(&value, sizeof(int), r, echo);

        if(r != error)
        {
            fprintf(stderr, "ERROR: File cannot write\n");
        }

    }

    short *echo_buffer = malloc(sizeof(short)*delay);

    short *buffer = malloc(sizeof(short)*delay);

    sample_size/=2;


    r = fread(echo_buffer, sizeof(short), delay, orig);

    if (2*delay-sample_size>0)
    {
        fwrite(echo_buffer, sizeof(short), r, echo);

        for (int x=r;x<delay;x++)
        {
            echo_buffer[x] = 0;
            error = fwrite(&echo_buffer[x], 2, 1, echo);
            if(error!=1)
            {
                fprintf(stderr, "ERROR: File cannot write\n");
                return 1;
            }
        }

        for(int i=0;i<r;i++)
        {
            echo_buffer[i] /= volume;
            fwrite(&echo_buffer[i], 2, 1, echo);
        }
    }
    else{
        fwrite(echo_buffer, sizeof(short), delay, echo);
        while ((r = fread(buffer, 2, delay, orig)))
        {
            for(int i=0;i<r;i++)
            {
                echo_buffer[i] = buffer[i] + echo_buffer[i]/volume;
            }

            error = fwrite(echo_buffer, sizeof(short), r, echo);
            for(int k=0; k<r; k++)
            {
                echo_buffer[k] = buffer[k];
            }
            if(error!=r){
            fprintf(stderr, "ERROR: File cannot write\n");
            return 1;
            }
        }
        fseek(orig, -delay*2, SEEK_END);
        fread(echo_buffer, 2, delay, orig);
        for(int u=0;u<delay;u++){
            echo_buffer[u]/=volume;
        }
        fwrite(echo_buffer, sizeof(short), delay, echo);
    }


    free(echo_buffer);
    free(buffer);

    error = fclose(orig);
    if (error!=0)
    {
        fprintf(stderr, "Error: File cannot close");
        return 1;
    }
    error = fclose(echo);
    if (error!=0)
    {
        fprintf(stderr, "Error: File cannot close");
        return 1;
    }

   return 0;
}