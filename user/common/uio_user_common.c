/*
 * @file uio_user_common.c -- UIO common API for user.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) luxebeng <luxebeng@gmail.com>, 2014
 *
 */

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sched.h>
#include <limits.h>

#include <dirent.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sal/core/spl.h>
#include <sal/appl/pci.h>
#include <ibde.h>
#include "uio_user_common.h"

#define UIO_FOLDER "/sys/class/uio"

/**
 * identify the device name by read the first line of the file
 * and compare to device name.
 * @param filename
 *  the target fiile name
 * @param dev_name
 *  the string of device wanted.
 * @return
 *  -true, get the uio name
 *  -false, not target file or failure
 */
static bool
identify_name_from_file(char *filename, const char * dev_name)
{
    char c, *out;
    int fd, len;
    bool result = false;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        return result;
    }

    for (len = 0; ((read (fd, &c, 1) == 1) && (c != '\n')); len++);
    lseek(fd, 0, SEEK_SET);

    out = malloc(len + 1);
    if (!out)
    {
        goto exit;
    }

    len = read(fd, out, len);
    if (len < 0)
    {
        goto free;
    }

    out[len] = 0;
    if ((len != sizeof(dev_name)) || (strcmp(out, dev_name)))
    {
        goto free;
    }
    result = true;

free:
    free(out);
    out = NULL;
exit:
    close(fd);
    return result;
}

/**
 * get the uio name from device name
 * @param dev_name
 *  the device name
 * @param uio_name
 *  the uio name string if success. NULL for failed.
 * @return
 *  bool true for result. ture for success.false for failure.
 */
bool
uio_get_uio_name(const char *dev_name, char * uio_name)
{
    struct dirent **namelist;
    int32_t i, nr;
    char filename[PATH_MAX];
    bool result = false;

    nr = scandir(UIO_FOLDER, &namelist, 0, alphasort);
    if (nr < 0)
    {
        return result;
    }

    for (i = 0; i < nr; i++)
    {
        if (!strcmp(namelist [i]->d_name, ".") ||
            !strcmp(namelist [i]->d_name, ".."))
            continue;

        snprintf(uio_name, PATH_MAX, "%s/%s", UIO_FOLDER, namelist[i]->d_name);
        snprintf(filename, PATH_MAX, "%s/name", uio_name);
        if (identify_name_from_file(filename,dev_name))
        {
            result = true;
            break;
        }
    }

    for (i = 0; i < nr; i++)
        free(namelist[i]);
    free(namelist);

    return result;
}

