/*
 * Copyright (c) 2017 Minqi Pan <pmq2001@gmail.com>
 *
 * This file is part of fast-hessian, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include "hessian.h"

#define INT_DIRECT_MIN -0x10
#define INT_DIRECT_MAX 0x2f
#define INT_ZERO 0x90

#define INT_BYTE_MIN -0x800
#define INT_BYTE_MAX 0x7ff
#define INT_BYTE_ZERO 0xc8

#define INT_SHORT_MIN -0x40000
#define INT_SHORT_MAX 0x3ffff
#define INT_SHORT_ZERO 0xd4

short hessian_decode_int(uint8_t * const buf, const size_t buf_length, const v8::FunctionCallbackInfo<v8::Value>& args)
{
	uint8_t code = buf[0];
	if (code >= 0x80 && code <= 0xbf) {
		args.GetReturnValue().Set(code - 0x90);
		return 1;
	}
	if (buf_length >= 2 && code >= 0xc0 && code <= 0xcf) {
		args.GetReturnValue().Set(((code - 0xc8) << 8) + buf[1]);
		return 1;
	}
	if (buf_length >= 3 && code >= 0xd0 && code <= 0xd7) {
		args.GetReturnValue().Set(((code - 0xd4) << 16) + (buf[1] << 8) + buf[2]);
		return 1;
	}
	if (buf_length >= 5 && code == 0x49) {
		args.GetReturnValue().Set(ntohl(*(int32_t *)(buf + 1)));
		return 1;
	}
	return 0;
}

short hessian_encode_int(int32_t val, uint8_t **out, size_t *len)
{
	if (INT_DIRECT_MIN <= val && val <= INT_DIRECT_MAX) {
		*len = 1;
		*out = (uint8_t*)malloc(*len);
		if (NULL == *out) {
			return 0;
		}
		*out[0] = val + INT_ZERO;
		return 1;
	} else if (INT_BYTE_MIN <= val && val <= INT_BYTE_MAX) {
		*len = 2;
		*out = (uint8_t*)malloc(*len);
		if (NULL == *out) {
			return 0;
		}
		*out[1] = val & 0xff;
		*out[0] = (val >> 8) + INT_BYTE_ZERO;
		return 1;
	} else if (INT_SHORT_MIN <= val && val <= INT_SHORT_MAX) {
		*len = 3;
		*out = (uint8_t*)malloc(*len);
		if (NULL == *out) {
			return 0;
		}
		*out[0] = (val >> 16) + INT_SHORT_ZERO;
		*(uint16_t *)(*out + 1) = htons(val & 0xffff);
		return 1;
	} else {
		*len = 5;
		*out = (uint8_t*)malloc(*len);
		if (NULL == *out) {
			return 0;
		}
		*out[0] = 0x49;
		*(int32_t *)(*out+1) = htonl(val);
		return 1;
	}
}
