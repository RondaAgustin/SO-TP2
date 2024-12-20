// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "test_sync.h"

#define TOTAL_PAIR_PROCESSES 5

int64_t global; // shared memory
char sem_id;;

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  sys_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3)
    return -1;

  if ((n = satoi(argv[0])) <= 0)
    return -1;
  if ((inc = satoi(argv[1])) == 0)
    return -1;
  if ((use_sem = satoi(argv[2])) < 0)
    return -1;

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem) {
      sys_sem_wait(sem_id);
    }
    slowInc(&global, inc);
    if (use_sem) {
      sys_sem_post(sem_id);
    }
  }
    
  return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 2) {
    return -1;
  }

  char *argvDec[] = {argv[0], "-1", argv[1], NULL};
  char *argvInc[] = {argv[0], "1", argv[1], NULL};

  global = 0;

  
  sem_id = sys_sem_open(1);
  if (sem_id == -1) {
    printf("test_sync: ERROR opening semaphore\n");
    return -1;
  }

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = sys_create_process((uint64_t)my_process_inc, 3, argvDec, 1, 0, NULL);
    pids[i + TOTAL_PAIR_PROCESSES] = sys_create_process((uint64_t) my_process_inc, 3, argvInc, 1, 0, NULL);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    sys_wait(pids[i]);
    sys_wait(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  sys_sem_close(sem_id);

  printf("Final value: %d\n", global);

  return 0;
}
