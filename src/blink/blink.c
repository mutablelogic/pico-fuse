
#include <picofuse/picofuse.h>

int main() {
    // Initialize the picofuse object
    picofuse_t *picofuse = picofuse_init(PICOFUSE_DEBUG);

    // Call main loop
    int errorCode = picofuse_main(picofuse);

    // Free resources
    picofuse_free(picofuse);

    // Return error code
    return errorCode;
}
