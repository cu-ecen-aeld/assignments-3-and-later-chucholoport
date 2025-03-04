#include "systemcalls.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/

    int system_call_status = system(cmd);

    return system_call_status == -1 ? false : true;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/

    bool do_exec_result = false;

    /* Fork: creates a new process by duplicating the calling process. */
    pid_t pid = fork();

    /* On failure, -1 is returned in the parent, no child process is created, 
       and errno is set to indicate the error. */
    if (pid == -1) 
    {
        syslog(LOG_ERR, "Error on fork step: %s", strerror(errno));
        
        va_end(args);

        exit(EXIT_FAILURE);
    } 
    /* On success, the PID of the child process is returned in the
       parent, and 0 is returned in the child. */
    else if (pid == 0) 
    {
        /* Execv: Replace child process image with execv with new process image */
        execv(command[0], command);
        
        /* The exec() functions only return if an error has occurred. 
           The return value is -1, and errno is set to indicate the error. */
        syslog(LOG_ERR, "Error on execv step: %s", strerror(errno));
        
        exit(EXIT_FAILURE);
    }
    else 
    {
        int status;

        /* Waitpid: suspends execution of the calling process until state changes */
        if (waitpid(pid, &status, 0) == -1)
        {
            /* on error, -1 is returned. Each of these calls sets errno 
               to an appropriate value in the case of an error. */
            syslog(LOG_ERR, "Error on waitpid step: %s", strerror(errno));
            
            do_exec_result = false;
        }

        /* on success, returns the process ID of the child whose state has changed; 
           if WNOHANG was specified and one or more child(ren) specified by pid exist, 
           but have not yet changed state, then 0 is returned. */
        if (status == 0) 
        {
            do_exec_result = true;
        }
        else 
        {
            syslog(LOG_ERR, "Command failed. Status code: %d", status);
            
            do_exec_result = false;
        }

        va_end(args);
        
        return do_exec_result;

    }

}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/
    bool do_exec_result = false;    

    int fd = open(outputfile, O_WRONLY|O_TRUNC|O_CREAT, 0644);
    
    if (fd < 0) 
    { 
        syslog(LOG_ERR, "Unable to open %s: %s", outputfile, strerror(errno));
        
        va_end(args);

        return do_exec_result;
    }

    /* Fork: creates a new process by duplicating the calling process. */
    pid_t pid = fork();

    /* On failure, -1 is returned in the parent, no child process is created, 
       and errno is set to indicate the error. */
    if (pid == -1) 
    {
        syslog(LOG_ERR, "Error on fork step: %s", strerror(errno));
        
        close(fd);
    
        va_end(args);

        exit(EXIT_FAILURE);
    } 
    /* On success, the PID of the child process is returned in the
       parent, and 0 is returned in the child. */
    else if (pid == 0) 
    {
        if (dup2(fd, STDOUT_FILENO) < 0) 
        { 
            syslog(LOG_ERR, "Error on dup2 %d step: %s", fd, strerror(errno)); 
            
            close(fd);

            va_end(args);

            exit(EXIT_FAILURE);
        }
        
        close(fd);

        /* Execv: Replace child process image with execv with new process image */
        execvp(command[0], command);
        
        /* The exec() functions only return if an error has occurred. 
           The return value is -1, and errno is set to indicate the error. */
        syslog(LOG_ERR, "Error on execv step: %s", strerror(errno));
        
        exit(EXIT_FAILURE);
    }
    else 
    {
        close(fd);

        int status;

        /* Waitpid: suspends execution of the calling process until state changes */
        if (waitpid(pid, &status, 0) == -1)
        {
            /* on error, -1 is returned. Each of these calls sets errno 
               to an appropriate value in the case of an error. */
            syslog(LOG_ERR, "Error on waitpid step: %s", strerror(errno));
            
            do_exec_result = false;
        }

        /* on success, returns the process ID of the child whose state has changed; 
           if WNOHANG was specified and one or more child(ren) specified by pid exist, 
           but have not yet changed state, then 0 is returned. */
        if (status == 0) 
        {
            do_exec_result = true;
        }
        else 
        {
            syslog(LOG_ERR, "Command failed. Status code: %d", status);
            
            do_exec_result = false;
        }

        va_end(args);
        
        return do_exec_result;

    }
    
    va_end(args);

    return true;
}
