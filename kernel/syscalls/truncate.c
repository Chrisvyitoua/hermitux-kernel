#include <hermit/syscall.h>
#include <hermit/logging.h>
#include <asm/page.h>
#include <asm/uhyve.h>
#include <hermit/minifs.h>
/*
The implementation of the fonction truncate which is a call handler was made by taking inspiration on the creat.c 
located at hermitux-kernel/kernel/syscalls/creat.c.

- truncate system call implementation | int truncate(const char *path, off_t length);
What was added to its structure : 3 fields 

path: is a pointer to the path of the file to be truncated.
length: is the new length of the file.
ret: is the return value of the system call.
*/
typedef struct {
	char *path;
	off_t length;
	int ret;
} __attribute__((packed)) uhyve_truncate_t;

int sys_truncate(const char *path, off_t length) {
    if(minifs_enabled)
        return minifs_truncate(path, length);

    uhyve_truncate_t arg;
    arg.path = (char *)virt_to_phys((size_t)path);
    arg.length = length;
    arg.ret = -1;

    uhyve_send(UHYVE_PORT_TRUNCATE, (unsigned)virt_to_phys((size_t)&arg));
    return arg.ret;
}
