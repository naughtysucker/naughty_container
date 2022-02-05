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

/**_Description
 *  @Get the address of the container which contains the member.
 * _Parameters
 *  @member_ptr: Address of member.
 *  @type: Type of container.
 *  @member: The name of member.
 * _Return
 *  @Address of the Container.
 */
#define naughty_container_of(member_ptr, type, member) ((type *)((uint8_t *)member_ptr - (uint8_t *)&((type *)0)->member))

#define naughty_member_of(container_ptr, member) (&container_ptr->member)

#endif
