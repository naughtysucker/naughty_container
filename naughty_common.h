/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: naughty_common.h
 * @ Author: carl
 * @ Email: naughtygeng@qq.com
 * @ Created Time: 2021-8月-17(星期二) 07:59:01
 * @ All Rights Reserved
 * *****************************************************************************/

#ifndef NAUGHTY_COMMON_H
#define NAUGHTY_COMMON_H

#include <stdint.h>
#include <assert.h>

/**_Description
 *  @Get the address of the container which contains the member.
 * _Parameters
 *  @member_ptr: Address of member.
 *  @type: Type of container.
 *  @member: The name of member.
 * _Return
 *  @Address of the Container.
 */
#define naughty_container_of(member_ptr, type, member) ((type *)((byte_t *)member_ptr - (byte_t *)&((type *)0)->member))

#define naughty_member_of(container_ptr, member) (&container_ptr->member)

#define NAUGHTY_ASSERT assert

#if (ARCH_BIT_WIDTH == 8)
typedef int8_t ssize_t;
#elif (ARCH_BIT_WIDTH == 16)
typedef int16_t ssize_t;
#elif (ARCH_BIT_WIDTH == 32)
typedef int32_t ssize_t;
#elif (ARCH_BIT_WIDTH == 64)
typedef int64_t ssize_t;
#endif

typedef uint8_t byte_t;

#if _WIN32
typedef int64_t ssize_t;
#endif

#endif
