/*
 * @file uio_user_common.h -- UIO common API for user.
 *
 * @brief
 *   This program is free software; you can redistribute it and/or modify it
 *   under the terms of the GNU General Public License version 2 as published
 *   by the Free Software Foundation.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc., 59
 *   Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __UIO_USER_COMMON_H_
#define __UIO_USER_COMMON_H_

/**
 * @file uio_user_common.h
 *
 * @brief
 *  the application just has the deive name that is need to be operated.
 *  but the open/write function need the uio name for the device.
 *  this API is created to ge the uio name by device name.
 */

/**
 * get the uio name by device name
 * @param dev_name
 *  the device name
 * @param uio_name
 *  the uio name string if success. NULL if failed.
 * @return
 *  bool true for result. ture for success.false for failure.
 */
bool uio_get_uio_name(const char *dev_name, char * uio_name);

#endif /* __UIO_USER_COMMON_H_ */
