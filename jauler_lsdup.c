/*
 * Author: Rytis Karpuška
 * 			rytis.karpuska@gmail.com
 * 			2015
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "errno.h"
#include "thread_pool.h"
#include "lf_map.h"
#include "dir_trav_task.h"
#include "calc_hash_task.h"
#include "compare_task.h"
#include "writer.h"

int main(int argc, char *argv[])
{
	struct timespec ts = {0, 1000000};

	//create writer
	struct writer *w = w_create();
	if(w == NULL){
		fprintf(stderr, "Could not create writer\n");
		return -ENOMEM;
	}

	//Create thread pool
	struct thread_pool *tp = tp_create(4);
	if(tp == NULL){
		fprintf(stderr, "Could not create thread pool\n");
		return -ENOMEM;
	}

	//Create empty map of potential matches by size
	struct map *m = map_create();
	if(m == NULL){
		fprintf(stderr, "Could not create map\n");
		return -ENOMEM;
	}

	//Traverse directory
	char *path = argc < 2 ? "." : argv[1];
	if(dtt_start(path, tp, m) != 0){
		fprintf(stderr, "Could not traverse directory\n");
		return -EINVAL;
	}

	//Wait for end of traversing
	while(tp->num_enqueued_tasks != 0)
		nanosleep(&ts, NULL);

	//Calculate hashes of potential matches
	if(cht_start(tp, m) != 0){
		fprintf(stderr, "Could not calculate hashes\n");
		return -EINVAL;
	}

	//Wait for end of hashing
	while(tp->num_enqueued_tasks != 0)
		nanosleep(&ts, NULL);

	//Calculate hashes of potential matches
	if(ct_start(tp, m, w) != 0){
		fprintf(stderr, "Could not calculate hashes\n");
		return -EINVAL;
	}

	//Wait for end of hashing
	while(tp->num_enqueued_tasks != 0)
		nanosleep(&ts, NULL);

	return 0;
}
