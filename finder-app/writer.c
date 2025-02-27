#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>

/* Possible exit status */
int success = 0;
int invalid_args_num = 1;
int unable_to_write  = 1;
int file_is_empty    = 1;

int main(int argc, char* argv[])
{
    /* REQUIREMENT: Setup syslog logging to your utility 
    using the LOG_USER facility */
    openlog("writer", LOG_PID, LOG_USER);

    /* REQUIREMENT: Use the syslog capability to log any 
    unexpected errors with LOG_ERR level */
    if (argc != 3) 
    {
        syslog(LOG_ERR, "Both file and write string are required.");
        return invalid_args_num;
    }

    char* writefile = argv[1];
    char* writestr  = argv[2];

    FILE* file = fopen(writefile, "w");

    /* REQUIREMENT: Use the syslog capability to write a message.
    This should be written with LOG_DEBUG level. */
    syslog(LOG_DEBUG, "Writing %s to %s", writefile, writestr);

    /* ERROR: File could not be open. */
    if (file == NULL)
    {
        syslog(LOG_ERR, "Unable to open file %s: %s", writefile, strerror(errno));
        closelog();
        return unable_to_write;
    } 

    /* ERROR: File content could not be written. */
    if (fprintf(file, "%s", writestr) < 0)
    {
        syslog(LOG_ERR, "Unable to write to file %s: %s", writefile, strerror(errno));
        fclose(file);
        closelog();
        return file_is_empty;
    } 

    fclose(file);
    closelog();

    return success;
}