#include <hermit/syscall.h>
#include <hermit/logging.h>
#include <asm/page.h>
#include <asm/uhyve.h>
#include <hermit/minifs.h>

/*
- ftruncate system call function implementation | int ftruncate(int fd, off_t length);
What was added to its structure : 3 fields 

fd: is the file descriptor of the file to be truncated.
length: is the new length of the file.
ret: is the return value of the system call.

*/
typedef struct {
	int fd;
	off_t length;
	int ret;
} __attribute__((packed)) uhyve_ftruncate_t;

int sys_ftruncate(int fd, off_t length) {
    if(minifs_enabled)
        return minifs_ftruncate(fd, length);

    uhyve_ftruncate_t arg;
    arg.fd = fd;
    arg.length = length;
    arg.ret = -1;

    uhyve_send(UHYVE_PORT_FTRUNCATE, (unsigned)virt_to_phys((size_t)&arg));
    return arg.ret;
}
