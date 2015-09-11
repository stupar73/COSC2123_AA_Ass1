/*******************************************************************************
 * COSC2123 - Algorithms and Analysis
 * Semester 2 2015 Assignment #1
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC2123 - Algorithms and Analysis
 * Program Code     : BP094 - Bachelor of Computer Science
 * Skeleton code provided by Jeffrey Chan
 *
 * main - Main method and I/O functions
 ******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>

#include "bipartiteGraph.h"
#include "commonDefs.h"


/* declarations */

#define GETOPT_FINISHED -1

/* command line options, you may add more to this if you wish. */
struct cmdline_opt
{
    int verbose;    /* if true, print out results */
    int profile;    /* optional flag, do what you want with this */
};


/* Static function declarations */
void print_usage(const char *name);
int parse_args(int argc, char **argv, struct cmdline_opt *optvals);
void process_datafile(const char *fname, bpGraph_t **ppGraph,
                      struct cmdline_opt *optvals);
unsigned long getTime();


/*
 * Main function, the entry point of program.
 */
int main(int argc, char *argv[])
{
    int i = 0;
    int arg_data_index = 0;
    struct cmdline_opt options;
    bpGraph_t *pGraph = NULL;

    /* process the command line arguments */
    arg_data_index = parse_args(argc, argv, &options);

    /* process the commands */
    for (i = arg_data_index; i < argc; i++)
    {
        process_datafile(argv[i], &pGraph, &options);
    }


    return EXIT_SUCCESS;
} /* end of main() */


void process_datafile(const char *fname, bpGraph_t **ppGraph,
    struct cmdline_opt *optvals)
{
    FILE *file;
    char cmd, cmdType;
    int retVal, val1, val2, val3;
    int verboseOutput, profileOutput;
    unsigned long start, end;

    /* sanity check for pointers */
    assert(optvals);

    verboseOutput = optvals->verbose;
    profileOutput = optvals->profile;



    if ((file = fopen(fname, "r")) == NULL)
    {
        fprintf(stderr, "Error: cannot open file %s.\n", fname);
        return;
    }

    /*
     * We are assuming a well-formed datafile for this project
     * However you are welcome to do some error checking (note that the given
     * implemenation can return error codes, which you may want to handle).
     * But when we test, we will only provide well formed input.
     */
    while (fscanf(file, "%c %c %d %d %d\n", &cmd, &cmdType, &val1, &val2, &val3) > 0)
    {
        switch (cmd)
        {
            /* create graph */
            case 'C':
                if (*ppGraph == NULL)
                {
                    
                    start = getTime();
                    *ppGraph = bipartGraphCreate(val1, val2);
                    end = getTime();
                    if (verboseOutput)
                    {
                        printf("create graph: %d %d\n", val1, val2);
                    }
                    if (profileOutput)
                    {
                        printf("\ttime taken = %.2f sec\n", (double)(end - start) / 1E6);
                    }
                }
                break;
            /* destroy graph */
            case 'D':
                if (*ppGraph != NULL)
                {
                    start = getTime();
                    bipartGraphDestroy(*ppGraph);
                    end = getTime();
                    if (verboseOutput)
                    {
                        printf("destroyed graph\n");
                    }
                    if (profileOutput)
                    {
                        printf("\ttime taken = %.2f sec\n", (double)(end - start) / 1E6);
                    }
                }
                break;
            /* insert vertex or edge */
            case 'I':
                switch (cmdType)
                {
                    /* insert vertex */
                    case 'V':
                        start = getTime();
                        retVal = bipartGraphInsertVertex(*ppGraph, val1, val2);
                        end = getTime();
                        if (verboseOutput)
                        {
                            printf("insert vertex: %d %d %s\n", val1, val2,
                                (retVal > 0 ? "new" : "repeat"));
                        }
                        if (profileOutput)
                        {
                            printf("\ttime taken = %.2f sec\n", (double)(end - start) / 1E6);
                        }
                        break;
                    /* insert edge */
                    case 'E':
                        start = getTime();
                        retVal = bipartGraphInsertEdge(*ppGraph, val1, val2, val3);
                        end = getTime();
                        if (verboseOutput)
                        {
                            printf("insert edge: %d %d %d %s\n", val1, val2, val3,
                                (retVal > 0 ? "new" : "failed"));
                        }
                        if (profileOutput)
                        {
                            printf("\ttime taken = %.2f sec\n", (double)(end - start) / 1E6);
                        }
                        break;
                }
                break;
            /* remove vertex or edge */
            case 'R':
                switch (cmdType)
                {
                    /* delete vertex */
                    case 'V':
                        start = getTime();
                        retVal = bipartGraphDeleteVertex(*ppGraph, val1, val2);
                        end = getTime();
                        if (verboseOutput)
                        {
                            printf("removed vertex: %d %d %s\n", val1, val2,
                                (retVal > 0 ? "delete" : "missing"));
                        }
                        if (profileOutput)
                        {
                            printf("\ttime taken = %.2f sec\n", (double)(end - start) / 1E6);
                        }
                        break;
                    /* delete edge */
                    case 'E':
                        start = getTime();
                        retVal = bipartGraphDeleteEdge(*ppGraph, val1, val2, val3);
                        end = getTime();
                        if (verboseOutput)
                        {
                            printf("removed edge: %d %d %d %s\n", val1, val2, val3,
                                (retVal > 0 ? "delete" : "missing"));
                        }
                        if (profileOutput)
                        {
                            printf("\ttime taken = %.2f sec\n", (double)(end - start) / 1E6);
                        }
                        break;
                }
                break;
            case 'S':
                /* search */
                switch (cmdType)
                {
                    case 'V':
                        start = getTime();
                        retVal = bipartGraphFindVertex(*ppGraph, val1, val2);
                        end = getTime();
                        if (verboseOutput)
                        {
                            printf("search: %d %d %s\n", val1, val2,
                                (retVal > 0 ? "present" : "absent"));
                        }
                        if (profileOutput)
                        {
                            printf("\ttime taken = %.2f sec\n", (double)(end - start) / 1E6);
                        }
                        break;
                    case 'E':
                        start = getTime();
                        retVal = bipartGraphFindEdge(*ppGraph, val1, val2, val3);
                        end = getTime();
                        if (verboseOutput)
                        {
                            printf("search: %d %d %d %s\n", val1, val2, val3,
                                (retVal > 0 ? "present" : "absent"));
                        }
                        if (profileOutput)
                        {
                            printf("\ttime taken = %.2f sec\n", (double)(end - start) / 1E6);
                        }
                        break;
                }
                break;
            /* define your own commands if it helps with testing */
            case 'P':
                /* print, useful for debugging */
                if (verboseOutput)
                {
                    start = getTime();
                    bipartGraphPrint(*ppGraph);
                    end = getTime();
                }
                if (profileOutput)
                {
                    printf("\ttime taken = %.2f sec\n", (double)(end - start) / 1E6);
                }
                break;
            default:
                /* ignore any unrecognised commands */
                printf("Unknown command: %c %c", cmd, cmdType);
                break;
        }
    }

    fclose(file);
} /* end of process_datafile() */


/*
 * Parse the command line arguments.
 */
int parse_args(int argc, char **argv, struct cmdline_opt *optvals)
{
    extern int optind;

    int opt = GETOPT_FINISHED;

    if (argc <= 1)
    {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    /* set options to default */
    assert(optvals);
    optvals->verbose = 1;
    optvals->profile = 0;

    /*
     * This uses getopt to read command line options.
     * See the man page to learn more about getopt.
     */
    while ((opt = getopt(argc, argv, "qvp")) != GETOPT_FINISHED)
    {
        switch (opt)
        {
            case 'q':
                /* quiet mode */
                optvals->verbose = 0;
                break;
            case 'v':
                /* verbose mode (default) */
                optvals->verbose = 1;
                break;
            case 'p':
                /* profiling mode */
                optvals->profile = 1;
                break;
            default:
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    /* profiling mode requires verbose mode */
    if (optvals->profile)
    {
        optvals->verbose = 1;
    }

    return optind;
} /* End of parse_args() */


/*
 * Print usage message.
 */
void print_usage(const char *name)
{
    fprintf(stderr, "Usage: %s [OPTIONS]... [FILES]...\n", name);
    fprintf(stderr, "Implements a basic bipartite graph ADT, "
        "performing the\noperations defined in FILES.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -q\t\tquiet mode "
        "(no output from operations)\n");
    fprintf(stderr, "  -v\t\tverbose mode "
        "(default, output from operations)\n");
    fprintf(stderr, "  -p\t\tprofile mode "
        "(print time taken for each command)\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Multiple files can be specified "
        "and they will run sequentially.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Example:\n  "
        "%s insert_list.txt search_list.txt\n", name);

    return;
} /* End of print_usage() */


unsigned long getTime()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);

    return (tp.tv_sec*1E6) + tp.tv_usec;
}
