#ifndef  _NOSECOWIFI_H

#include <nos_scheduler.h>

#define SYSCFG_NS "ripeatlas"

extern bool DEBUG;

#define debug_printf(fmt, ...) { if (DEBUG) \
	fprintf(stderr, "%s:%s:%d: "fmt"\n", \
	__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); }
#define debug_print(s) debug_printf("%s",s)
#define debug_trace() debug_printf("","")


bool ripeatlas_bootup();
bool ripeatlas_get_enable();
void ripeatlas_set_enable(bool state);

bool ripeatlas_get_status(char *status, unsigned int status_size);

bool ripeatlas_get_key(char *key, unsigned int key_size);

#endif
