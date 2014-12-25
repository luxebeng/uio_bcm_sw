/*
 * @file uio_user_common.h -- UIO common API for user.
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
