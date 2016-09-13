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
#ifndef DYN_ARRAY_H_
#define DYN_ARRAY_H_

#define DA_SUCCESS      (0)
#define DA_FAILURE      (1)

/*
 * A dynamic array is an array able to hold elements of the
 * same type. The elements should also be comparable if one
 * intends to call da_sort.
 * Different instances of a dynamic array can hold elements
 * of different types, but each instance is limited to one
 * particular type. This is enforced by being required to
 * pass the size of the elements to be held by an array upon
 * creation thereof.
 */
typedef struct da_dyn_array_s da_dyn_array_t;
/*
 * Creates a new dynamic array, able to hold an initial nmemb
 *   elements of size size.
 * Returns the newly created array, or NULL in case of failure.
 */
da_dyn_array_t *da_create(size_t nmemb, size_t size);
/*
 * Destroys a dynamic array. If the array is NULL, of was not
 * created with da_create, the behaviour is undefined.
 */
void da_destroy(da_dyn_array_t *);
/*
 * Resizes a dynamic array. If nmemb is greater than the current
 * capacity of an array, new *uninitialized* elements are added.
 * On the other hand, if size if smaller than the current
 * capacity of an array, the tailing elements are slashed.
 *
 * Note that it does not bring much to resize an array to a
 * greater size. In principle, a dynamic array is *not* limited
 * in size. This function is only useful to keep memory usage in
 * check, and reduce the size of a growing array.
 */
int da_resize(da_dyn_array_t *, size_t nmemb);
/*
 * Adds a new element to the end of an array. The end is considered
 * to be one past the highest index were an element was ever put in
 * the array, or the new capacity of an array when it was reduced
 * with da_resize.
 */
int da_add(da_dyn_array_t *, void *);
/*
 * Sets an element in the array at the specified index. If the element
 * had already been set, it is replaced. If the index specified is the
 * highest set index so far, the next call to da_add will set the
 * element one past this index.
 */
int da_set(da_dyn_array_t *, void *, size_t);
/*
 * Sorts the current dynamic array with the specified comparison
 * function. Note that it is *your* responsibility to ensure that
 * all elements in the array until are actually sets. The elements
 * considered for sorting by da_sort range from element at index 0
 * to element at the highest index set so (be it through da_add,
 * da_set or da_resize).
 */
void da_sort(da_dyn_array_t *, int (*)(const void *, const void *));

/*
 * An iterator lets you browse through the elements of an
 * array, one by one.
 */
typedef struct da_iterator da_iterator_t;
/*
 * Creates a new iterator for an array.
 */
da_iterator_t *da_it_create(da_dyn_array_t *);
/*
 * Returns a pointer to the next element in the array, or NULL
 * if the array has been fully traversed. Note that it is *your*
 * responsibility to manage possibly uninitialized elements. If
 * you didn't set an element, it *will* be returned by the iterator
 * anyways.
 */
void *da_it_next(da_iterator_t *);
/*
 * Releases the resource allocated to an iterator object. If the
 * iterator passed is NULL or wasn't created with da_it_create, the
 * behaviour is undefined.
 */
void da_it_destroy(da_iterator_t *);

#endif /* DYN_ARRAY_H_ */
