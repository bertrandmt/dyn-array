/*
 * Copyright (c) 2003-2016 - Bertrand Mollinier Toublet.
 * bertrandmt@gmail.com
 * http://www.bmt-online.org/
 *
 * Dynamic array - dyn_array
 *
 * This program is  free software;  you can  redistribute it  and/or
 * modify it  under the  terms of  the GNU  General  Public  License
 * as published  by the Free  Software Foundation;  either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is  distributed in the hope that  it will be useful,
 * but WITHOUT  ANY WARRANTY;  without even the  implied warranty of
 * MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR  PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along  with this  program; if  not,  write to  the Free  Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "dyn_array.h"

struct da_dyn_array_s
{
    void *base;
    size_t nmemb;
    size_t size;
    size_t index;
};

da_dyn_array_t *da_create(size_t nmemb, size_t size)
{
    da_dyn_array_t *new_da = NULL;

    new_da = malloc(sizeof *new_da);
    if (NULL != new_da)
    {
        new_da->base = NULL;
        new_da->size = size;
        if (DA_FAILURE == da_resize(new_da, nmemb))
        {
            free(new_da);
            new_da = NULL;
        }
        else
        {
            new_da->index = 0;
        }
    }
    return new_da;
}

void da_destroy(da_dyn_array_t *da)
{
    if (NULL != da)
    {
        free(da->base);
        free(da);
    }
}

int da_resize(da_dyn_array_t *da, size_t nmemb)
{
    int status = DA_FAILURE;
    void *tmp;
    size_t byte_size;

    assert(NULL != da);

    /* this is to test for overflow */
    byte_size = nmemb * da->size;
    if (byte_size / da->size == nmemb)
    {
        tmp = realloc(da->base, byte_size);
        if (NULL != tmp)
        {
            da->base = tmp;
            da->nmemb = nmemb;
            if (nmemb < da->index)
            {
                da->index = nmemb;
            }
            status = DA_SUCCESS;
        }
    }
    return status;
}

int da_add(da_dyn_array_t *da, void *obj)
{
    assert(NULL != da);
    return da_set(da, obj, da->index);
}

int da_set(da_dyn_array_t *da, void *obj, size_t index)
{
    int status = DA_SUCCESS;

    assert(NULL != da);
    assert(NULL != obj);

    if (index >= da->nmemb)
    {
        /*
         * Allocate enough space plus a cushion of da->nmemb.
         * The +1 takes care of the corner case where the array
         * has no allocated elements (da->nmemb == 0) and we
         * are trying to write at index 0.
         */
        status = da_resize(da, da->nmemb + index + 1);
    }

    if (DA_FAILURE != status)
    {
        memcpy((char *)da->base + index * da->size, obj, da->size);
        if (index >= da->index)
        {
            da->index = index + 1;
        }
    }

    return status;
}

void da_sort(da_dyn_array_t *da, int (*compar)(const void *, const void *))
{
    assert(NULL != da);
    assert(NULL != compar);

    qsort(da->base, da->index, da->size, compar);
}

struct da_iterator
{
    da_dyn_array_t *da;
    size_t index;
};

da_iterator_t *da_it_create(da_dyn_array_t *da)
{
    da_iterator_t *new_it;

    assert(NULL != da);

    new_it = malloc(sizeof *new_it);
    if (NULL != new_it)
    {
        new_it->da = da;
        new_it->index = 0;
    }
    return new_it;
}

void *da_it_next(da_iterator_t *it)
{
    void *next;

    assert(NULL != it);
    assert(NULL != it->da);
    assert(NULL != it->da->base);

    if (it->index >= it->da->index)
    {
        next = NULL;
    }
    else
    {
        next = (char *)it->da->base + it->index * it->da->size;
        it->index += 1;
    }

    return next;
}

void da_it_destroy(da_iterator_t *it)
{
    if (NULL != it)
    {
        free(it);
    }
}
