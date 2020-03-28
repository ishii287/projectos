int resource_count, process_count;
int *resources, *safeSeq;
int **allotted_val, **need_left;

// fetch safe sequence is there is one else return false
bool fetch_safe_seq() {
	// get safe sequence
        int tempRes[resource_count];
        for(int i=0; i<resource_count; i++) tempRes[i] = resources[i];

        bool finished[process_count];
        for(int i=0; i<process_count; i++) finished[i] = false;
        int nfinished=0;
        while(nfinished < process_count) {
                bool safe = false;

                for(int i=0; i<process_count; i++) {
                        if(!finished[i]) {
                                bool possible = true;

                                for(int j=0; j<resource_count; j++)
                                        if(need_left[i][j] > tempRes[j]) {
                                                possible = false;
                                                break;
                                        }

                                if(possible) {
                                        for(int j=0; j<resource_count; j++)
                                                tempRes[j] += allotted_val[i][j];
                                        safeSeq[nfinished] = i;
                                        finished[i] = true;
                                        ++nfinished;
                                        safe = true;
                                }
                        }
                }

                if(!safe) {
                        for(int k=0; k<process_count; k++) safeSeq[k] = -1;
                        return false; // if no safe sequence is there
                }
        }
        return true; // if safe srquence found
}
