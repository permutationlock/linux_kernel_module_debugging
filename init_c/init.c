/* crt.h: exports `_start` symbol to init runtime and call `main` */
#include <crt.h>
/* sys.h: defines `reboot` function */
#include <sys.h>
/* unistd.h: defines `sleep`, `read`, and `write` functions */
#include <unistd.h>

int main(void) {
    /* try to wait for kernel boot messages to finish */
    sleep(1);

    /* write our message to standard output */
    const char msg[] = "Hello, World!\n";
    write(STDOUT_FILENO, msg, sizeof(msg));

    /* wait for user to press enter by reading from standard input */
    char buff[1];
    read(STDIN_FILENO, buff, sizeof(buff));

    /* reboot the machine */
    reboot(LINUX_REBOOT_CMD_RESTART);
}
