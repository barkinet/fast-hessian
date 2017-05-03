/*
 * Copyright (c) 2017 Minqi Pan <pmq2001@gmail.com>
 *
 * This file is part of v8-hessian, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include "hessian.h"

short hessian_decode_null(uint8_t *buffer)
{
        return 'N' == buffer[0];
}

short hessian_encode_null(uint8_t **out, size_t *len)
{
        *len = 1;
        *out = (uint8_t*)malloc(*len);
        if (NULL == *out) {
                return 0;
        }
        *out[0] = 'N';
        return 1;
}
