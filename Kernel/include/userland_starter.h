#ifndef __USERLAND_CALLER__
#define __USERLAND_CALLER__

extern void* USERLAND_CODE_ADDRESS;
extern void* USERLAND_DATA_ADDRESS;

void start_userland();
void restart_userland();

#endif
