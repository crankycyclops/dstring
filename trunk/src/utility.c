
/* ************************************************************************* *\
   * File: utility.c                                                       *
   * Purpose:                                                              *
   *    Provides utility functions for the DString Library                 *
   *************************************************************************
   * Project:    DString                                                   *
   * Programmer: James Colannino                                           *
   * Email:      james@colannino.org                                       *
   * Homepage:   http://james.colannino.org/                               *
   *                                                                       *
   * Description:                                                          *
   *     The purpose of this library is to provide facilities for easily   *
   * dealing with dynamically allocated strings.                           *
   ************************************************************************* 
   * DString Library Copyright 2006 by James Colannino                     *
   *                                                                       *
   * This program is free software; you can redistribute it and/or         *
   * modify it under the terms of the GNU Lesser General Public            *
   * License as published by the Free Software Foundation; either          *
   * version 2.1 of the License, or (at your option) any later version.    *
   *                                                                       *
   * This program is distributed in the hope that it will be useful,       *
   * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
   * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
   * Lesser General Public License for more details.                       *
   *                                                                       *
   * You should have received a copy of the GNU Lesser General Public      *
   * License along with this library; if not, write to:                    *
   *                                                                       *
   * The Free Software Foundation, Inc.                                    *
   * 51 Franklin St, Fifth Floor                                           *
   * Boston, MA 02110-1301 USA                                             * 
\* ************************************************************************* */

#include <string.h>

#include "static.h"
#include "dstring.h"

/* ************************************************************************* */

int dstrtrunc(dstring_t str, size_t size) {

   int i;
   size_t length;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      return DSTR_UNINITIALIZED;
   }

   /* make sure size is valid */
   if (size < 0) {
      return DSTR_INVALID_ARGUMENT;
   }

   /* get the length of the string */
   length = dstrlen(str);

   /* if the specified size is 0, there's nothing to do. */
   if (0 == size) {
      return DSTR_SUCCESS;
   }

   /* if the size is greater than or equal to the size of the string,
      wipe out the whole string */
   else if (size >= length) {
      DSTRBUF(str)[0] = '\0';
      return DSTR_SUCCESS;
   }

   /* let's truncate, baby! */
   for (i = 0; i < size; i++);
   DSTRBUF(str)[i] = '\0';

   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrtruncleft(dstring_t str, size_t size) {

   int i;
   size_t length;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      return DSTR_UNINITIALIZED;
   }

   /* make sure size is valid */
   if (size < 0) {
      return DSTR_INVALID_ARGUMENT;
   }

   /* get the length of the string */
   length = dstrlen(str);

   /* if the specified size is 0, there's nothing to do. */
   if (0 == size) {
      return DSTR_SUCCESS;
   }

   /* if the size is greater than or equal to the size of the string,
      wipe out the whole string */
   else if (size >= length) {
      DSTRBUF(str)[0] = '\0';
      return DSTR_SUCCESS;
   }

   /* truncate from the left */
   for (i = 0; DSTRBUF(str)[i + size] != '\0'; i++) {
      DSTRBUF(str)[i] = DSTRBUF(str)[i + size];
   }

   /* null terminate newly truncated string */
   DSTRBUF(str)[i] = '\0';

   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrdel(dstring_t str, int index) {

   int i;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(str) || index < 0) {
      return DSTR_OUT_OF_BOUNDS;
   }

   /* starting at index, move each character left one position */
   for (i = index; '\0' != DSTRBUF(str)[i]; i++) {
      DSTRBUF(str)[i] = DSTRBUF(str)[i + 1];
   }

   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrndel(dstring_t str, int index, int n) {

   int i;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(str) || index < 0) {
      return DSTR_OUT_OF_BOUNDS;
   }

   /* make sure n is valid */
   if (n < 0) {
      return DSTR_INVALID_ARGUMENT;
   }

   /* if n is 0, return DSTR_SUCCESS without doing anything */
   if (n == 0) {
      return DSTR_SUCCESS;
   }

   /* If n is greater than the size of the string, zero it out */
   if (n > dstrlen(str)) {
      DSTRBUF(str)[0] = '\0';
      return DSTR_SUCCESS;
   }

   /* starting at index, move each character left n positions */
   for (i = index; '\0' != DSTRBUF(str)[i]; i++) {
      DSTRBUF(str)[i] = DSTRBUF(str)[i + n];
      if ('\0' == DSTRBUF(str)[i]) {
         break;
      }
   }

   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrinsertch(dstring_t dest, int index, char c) {

   int i;
   int retval;          /* for the return value of dstrealloc() */

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == dest) {
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(dest) || index < 0) {
      return DSTR_OUT_OF_BOUNDS;
   }

   /* is our current allocation big enough? */
   if (DSTRBUFLEN(dest) <= dstrlen(dest) + 2) {
      /* if not, we need to add extra space */
      if (DSTR_SUCCESS != (retval = dstrealloc(&dest, DSTRBUFLEN(dest) + 10 ))) {
         /* if the allocation was not successful, the string is untouched */
         return retval;
      }
   }

   /* shift everything over one unit to the right, including the '\0' */
   for (i = dstrlen(dest); i >= index; i--) {
      DSTRBUF(dest)[i + 1] = DSTRBUF(dest)[i];
   }

   /* insert our character at the specified index */
   DSTRBUF(dest)[index] = c;

   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrcinsert(dstring_t dest, const char *src, int index) {

   int i, j;
   int retval;          /* for the return value of dstrealloc() */

   int srclen = strlen(src);

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == dest) {
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(dest) || index < 0) {
      return DSTR_OUT_OF_BOUNDS;
   }

   /* is our current allocation big enough? */
   if (DSTRBUFLEN(dest) <= dstrlen(dest) + srclen + 1) {
      /* if not, we need to add extra space */
      if (DSTR_SUCCESS != (retval = dstrealloc(&dest, DSTRBUFLEN(dest) + \
srclen + 1))) {
         /* if the allocation was not successful, the string is untouched */
         return retval;
      }
   }

   /* shift everything over srclen units to the right, including '\0' */
   for (i = dstrlen(dest); i >= index; i--) {
      DSTRBUF(dest)[i + srclen] = DSTRBUF(dest)[i];
   }

   /* insert the source string in dest at the proper position */
   for (i = index, j = 0; j < srclen; i++, j++) {
      DSTRBUF(dest)[i] = src[j];
   }

   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrinsert(dstring_t dest, const dstring_t src, int index) {

   /* dstrcinsert() will check to make sure dest is initialized, but we must
      first make sure that src is also initialized! */
   if (NULL == src) {
      return DSTR_UNINITIALIZED;
   }

   return dstrcinsert(dest, (const char *)DSTRBUF(src), index);
}

/* ************************************************************************* */

int dstrncinsert(dstring_t dest, const char *src, int index, int n) {

   int i, j;
   int retval;          /* for the return value of dstrealloc() */

   int srclen;          /* number of chars in src to insert */

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == dest) {
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(dest) || index < 0) {
      return DSTR_OUT_OF_BOUNDS;
   }

   /* make sure n is valid */ 
   if (n < 0) {
      return DSTR_INVALID_ARGUMENT;
   }

   /* if n is 0, return DSTR_SUCCESS without doing anything */
   if (n == 0) {
      return DSTR_SUCCESS;
   }

   /* determine the proper amount of characters to insert */
   if (n > strlen(src)) {
      srclen = strlen(src);
   } else {
      srclen = n;
   }

   /* is our current allocation big enough? */
   if (DSTRBUFLEN(dest) <= dstrlen(dest) + srclen + 1) {
      /* if not, we need to add extra space */
      if (DSTR_SUCCESS != (retval = dstrealloc(&dest, DSTRBUFLEN(dest) + \
srclen + 1))) {
         /* if the allocation was not successful, the string is untouched */
         return retval;
      }
   }

   /* shift everything over srclen units to the right, including '\0' */
   for (i = dstrlen(dest); i >= index; i--) {
      DSTRBUF(dest)[i + srclen] = DSTRBUF(dest)[i];
   }

   /* insert the source string in dest at the proper position */
   for (i = index, j = 0; j < srclen; i++, j++) {
      DSTRBUF(dest)[i] = src[j];
   }

   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrninsert(dstring_t dest, const dstring_t src, int index, int n) {

   /* dstrcinsert() will check to make sure dest is initialized, but we must
      first make sure that src is also initialized! */
   if (NULL == src) {
      return DSTR_UNINITIALIZED;
   }

   return dstrncinsert(dest, (const char *)DSTRBUF(src), index, n);
}

/* ************************************************************************* */

