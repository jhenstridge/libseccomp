/**
 * Enhanced Seccomp Architecture/Machine Specific Code
 *
 * Copyright (c) 2012 Red Hat <pmoore@redhat.com>
 * Author: Paul Moore <pmoore@redhat.com>
 */

/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <errno.h>
#include <stdlib.h>
#include <asm/bitsperlong.h>
#include <linux/audit.h>

#include "arch.h"
#include "arch-i386.h"

const struct arch_def arch_def_native = {
#if __i386__
	.token = AUDIT_ARCH_I386,
#elif __x86_64__
	.token = AUDIT_ARCH_X86_64,
#else
#error the arch code needs to know about your machine type
#endif /* machine type guess */

#if __BITS_PER_LONG == 32
	.size = ARCH_SIZE_32,
#elif __BITS_PER_LONG == 64
	.size = ARCH_SIZE_64,
#else
	.size = ARCH_SIZE_UNSPEC,
#endif /* BITS_PER_LONG */

#if __BYTE_ORDER == __LITTLE_ENDIAN
	.endian = ARCH_ENDIAN_LITTLE,
#elif __BYTE_ORDER == __BIG_ENDIAN
	.endian = ARCH_ENDIAN_BIG,
#else
	.endian = ARCH_ENDIAN_UNSPEC,
#endif /* __BYTE_ORDER */
};

/**
 * Determine the argument offset for the lower 32 bits
 * @param arch the architecture definition
 * @param arg the argument number
 *
 * Determine the correct offset for the low 32 bits of the given argument based
 * on the architecture definition.  Returns the offset on success, negative
 * values on failure.
 *
 */
int arch_arg_offset_lo(const struct arch_def *arch, unsigned int arg)
{
	switch (arch->token) {
	case AUDIT_ARCH_I386:
		return i386_arg_offset_lo(arch, arg);
	default:
		return -EDOM;
	}
}

/**
 * Determine the argument offset for the high 32 bits
 * @param arch the architecture definition
 * @param arg the argument number
 *
 * Determine the correct offset for the high 32 bits of the given argument
 * based on the architecture definition.  Returns the offset on success,
 * negative values on failure.
 *
 */
int arch_arg_offset_hi(const struct arch_def *arch, unsigned int arg)
{
	return -EDOM;
}

/**
 * Determine the argument offset
 * @param arch the architecture definition
 * @param arg the argument number
 *
 * Determine the correct offset of the given argument based on the architecture
 * definition.  Returns the offset on success, negative values on failure.
 *
 */
int arch_arg_offset(const struct arch_def *arch, unsigned int arg)
{
	switch (arch->token) {
	case AUDIT_ARCH_I386:
		return i386_arg_offset(arch, arg);
	default:
		return -EDOM;
	}
}