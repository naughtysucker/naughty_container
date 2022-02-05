/* *****************************************************************************
 * @ Encoding: UTF-8
 * @ File Name: naughty_exception.h
 * @ Author: carl
 * @ Email: naughtygeng@qq.com
 * @ Created Time: 2021-Aug-03(Tuesday) 11:08:16
 * @ All Rights Reserved
 * *****************************************************************************/

#ifndef NAUGHTY_EXCEPTION_H
#define NAUGHTY_EXCEPTION_H

typedef enum naughty_exception
{
	naughty_exception_no = 0,
	naughty_exception_nullptr = -1,
	naughty_exception_alloc = -2,
	naughty_exception_unknown = -3,
	naughty_exception_runout = -4,
	naughty_exception_outofrange = -5,
	naughty_exception_notfound = -6,
	naughty_exception_wrongparameter = -7,
}naughty_exception;


#endif
