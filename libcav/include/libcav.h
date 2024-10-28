#ifndef LIBCAV_H
#define LIBCAV_H

#include <stddef.h>

/* * get_request: make a get request populating a buffer in return (depends on heap alloc)
 * - url: specify url to get from.
 * - (0) data_size: specifies the data length of gotten content, is 0 if request fails
 * - (0) status_code: status_code gotten.
 *
 * * () indicates values required declared value.
 */

char *get_request(char *url, size_t data_size, int status_code);

#endif // !LIBCAV_H
