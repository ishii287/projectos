pthread_mutex_t lock_resrc;
pthread_cond_t condition;

int resource_count, process_count;
int *resources, *safeSeq;
int **allotted_val, **max_requirement, **need_left;
int run_process_count;

// function for processing the code
void* process_code(void *arg) {
        int p = *((int *) arg);

	// locking resources using mutex lock
        pthread_mutex_lock(&lock_resrc);

        // checking condition
        while(p != safeSeq[run_process_count])
                pthread_cond_wait(&condition, &lock_resrc);

	// process
        printf("\n--> Process %d", p+1);
        printf("\n\tallotted_val : ");
        for(int i=0; i<resource_count; i++)
                printf("%3d", allotted_val[p][i]);

        printf("\n\tneed_lefted    : ");
        for(int i=0; i<resource_count; i++)
                printf("%3d", need_left[p][i]);

        printf("\n\tAvailable : ");
        for(int i=0; i<resource_count; i++)
                printf("%3d", resources[i]);

        printf("\n"); 

        printf("\tResource allotted_val!");
        printf("\n"); 
        printf("\tProcess Code Running...");
        printf("\n"); 
        printf("\tProcess Code Completed...");
        printf("\n"); 
        printf("\tProcess Releasing Resource...");
        printf("\n"); 
        printf("\tResource Released!");

	for(int i=0; i<resource_count; i++)
                resources[i] += allotted_val[p][i];

        printf("\n\tNow Available : ");
        for(int i=0; i<resource_count; i++)
                printf("%3d", resources[i]);
        printf("\n\n");


        run_process_count++;
        pthread_cond_broadcast(&condition);
        pthread_mutex_unlock(&lock_resrc);
	pthread_exit(NULL);
}
