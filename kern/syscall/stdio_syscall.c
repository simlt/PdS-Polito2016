/*
 * Copyright (c) 2000, 2001, 2002, 2003, 2004, 2005, 2008, 2009
 *	The President and Fellows of Harvard College.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <types.h>
#include <copyinout.h>
#include <syscall.h>
#include <lib.h>
#include <kern/unistd.h>
#include <kern/errno.h>

int sys_write(int fd, const_userptr_t user_buf, size_t count, ssize_t* retval)
{
    int result = 0;
    char* buffer = kmalloc(count);
    //struct uio uio;
    unsigned int i = 0;

    // Initialize retval
    *retval = 0;
    // Copy user buffer
    result = copyin(user_buf, buffer, count);
    if (result)
        return result;

    // should get entry from a file descriptor table in process struct
    if ((fd == STDIN_FILENO) ||
        (fd == STDOUT_FILENO) ||
        (fd == STDERR_FILENO))
    {
        for (i=0; i < count; ++i)
        {
            (*retval)++;
            putch(buffer[i]);
        }
    }
    else
        result = EBADF;
    if (result)
        return result;
          

    return 0;
}

int sys_read(int fd, userptr_t user_buf, size_t count, ssize_t* retval)
{
    int result = 0;
    char* buffer = kmalloc(count);
    //struct uio uio;
    unsigned int i = 0;

    // Initialize retval
    *retval = 0;

    // should get entry from a file descriptor table in process struct
    if ((fd == STDIN_FILENO) ||
        (fd == STDOUT_FILENO) ||
        (fd == STDERR_FILENO))
    {
        for (i=0; i < count; ++i)
        {
            (*retval)++;
            buffer[i] = getch();
        }
    }
    else
        result = EBADF;
    if (result)
        return result;

    // Write user buffer
    result = copyout(buffer, user_buf, count);
    if (result)
        return result;

    return 0;
}
