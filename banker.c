#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include "fetch_safe_seq.h"
#include "process_code.h"

pthread_mutex_t lock_resrc;
pthread_cond_t condition;

int resource_count, process_count;
int *resources, *safeSeq;
int **allotted_val, **max_requirement, **need_left;
int run_process_count = 0;

int main() {

        printf("\nEnter no of process? ");
        scanf("%d", &process_count);

        printf("\nEnter number of resources? ");
        scanf("%d", &resource_count);

        resources = (int *)malloc(resource_count * sizeof(*resources));
        printf("\nAvailable Resources (R1 R2 ... and so on)? ");
        for(int i=0; i<resource_count; i++)
                scanf("%d", &resources[i]);

        allotted_val = (int **)malloc(process_count * sizeof(*allotted_val));
        for(int i=0; i<process_count; i++)
                allotted_val[i] = (int *)malloc(resource_count * sizeof(**allotted_val));

        max_requirement = (int **)malloc(process_count * sizeof(*max_requirement));
        for(int i=0; i<process_count; i++)
                max_requirement[i] = (int *)malloc(resource_count * sizeof(**max_requirement));

        // get allotted_val from user
        printf("\n");
        for(int i=0; i<process_count; i++) {
                printf("\nResource allotted_val to process %d (R1 R2 ...)? ", i+1);
                for(int j=0; j<resource_count; j++)
                        scanf("%d", &allotted_val[i][j]);
        }
        printf("\n");

	// get maximum required resources from user
        for(int i=0; i<process_count; i++) {
                printf("\nMaximum resource required by process %d (R1 R2 ...)? ", i+1);
                for(int j=0; j<resource_count; j++)
                        scanf("%d", &max_requirement[i][j]);
        }
        printf("\n");

	// make a 2D array of need_left
        need_left = (int **)malloc(process_count * sizeof(*need_left));
        for(int i=0; i<process_count; i++)
                need_left[i] = (int *)malloc(resource_count * sizeof(**need_left));

        for(int i=0; i<process_count; i++)
                for(int j=0; j<resource_count; j++)
                        need_left[i][j] = max_requirement[i][j] - allotted_val[i][j];

	// calculate the safe sequence
	safeSeq = (int *)malloc(process_count * sizeof(*safeSeq));
        for(int i=0; i<process_count; i++) safeSeq[i] = -1;

        if(!fetch_safe_seq()) {
                printf("\nUnsafe State Found!! These process leads the system to unsafe state..\n\n");
                exit(-1);
        }

        printf("\n\nSafe Sequence Exist. The order is following : ");
        for(int i=0; i<process_count; i++) {
                printf("%-3d", safeSeq[i]+1);
        }

        printf("\nExecuting Processes...\n\n");
        
	
	// running the n threads
	pthread_t processes[process_count];
        pthread_attr_t attr;
        pthread_attr_init(&attr);

	int processNumber[process_count];
	for(int i=0; i<process_count; i++) processNumber[i] = i;

        for(int i=0; i<process_count; i++)
                pthread_create(&processes[i], &attr, process_code, (void *)(&processNumber[i]));

        for(int i=0; i<process_count; i++)
                pthread_join(processes[i], NULL);

        printf("\nAll Processes Finished\n");	
	
	// making free all the resources
        free(resources);
        for(int i=0; i<process_count; i++) {
                free(allotted_val[i]);
                free(max_requirement[i]);
		free(need_left[i]);
        }
        free(allotted_val);
        free(max_requirement);
	free(need_left);
        free(safeSeq);
}
