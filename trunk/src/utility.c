
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

int dstrboundscheck(dstring_t str, size_t index) {

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      return DSTR_UNINITIALIZED;
   }

   /* out of bounds */
   if (index >= dstrlen(str)) {
      return DSTR_OUT_OF_BOUNDS;
   }

   /* the index is within str's bounds */
   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrtrunc(dstring_t str, size_t size) {

   size_t i;
   size_t length;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      return DSTR_UNINITIALIZED;
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

   size_t i;
   size_t length;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      return DSTR_UNINITIALIZED;
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

int dstrdel(dstring_t str, size_t index) {

   size_t i;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(str)) {
      return DSTR_OUT_OF_BOUNDS;
   }

   /* starting at index, move each character left one position */
   for (i = index; '\0' != DSTRBUF(str)[i]; i++) {
      DSTRBUF(str)[i] = DSTRBUF(str)[i + 1];
   }

   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrndel(dstring_t str, size_t index, size_t n) {

   size_t i;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(str)) {
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

int dstrinsertc(dstring_t dest, size_t index, char c) {

   size_t i;
   int retval;          /* for the return value of dstrealloc() */

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == dest) {
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(dest)) {
      return DSTR_OUT_OF_BOUNDS;
   }

   /* make sure we're not attempting to insert a \0 */
   if ('\0' == c) {
      return DSTR_INVALID_ARGUMENT;
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

int dstrinsertcs(dstring_t dest, const char *src, size_t index) {

   size_t i, j;
   int retval;          /* for the return value of dstrealloc() */

   int srclen = strlen(src);

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == dest) {
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(dest)) {
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

int dstrinserts(dstring_t dest, const dstring_t src, size_t index) {

   /* dstrcinsert() will check to make sure dest is initialized, but we must
      first make sure that src is also initialized! */
   if (NULL == src) {
      return DSTR_UNINITIALIZED;
   }

   return dstrinsertcs(dest, (const char *)DSTRBUF(src), index);
}

/* ************************************************************************* */

int dstrninsertcs(dstring_t dest, const char *src, size_t index, size_t n) {

   size_t i, j;
   int retval;          /* for the return value of dstrealloc() */

   size_t srclen;       /* number of chars in src to insert */

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == dest) {
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(dest)) {
      return DSTR_OUT_OF_BOUNDS;
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

int dstrninserts(dstring_t dest, const dstring_t src, size_t index, size_t n) {

   /* dstrcinsert() will check to make sure dest is initialized, but we must
      first make sure that src is also initialized! */
   if (NULL == src) {
      return DSTR_UNINITIALIZED;
   }

   return dstrninsertcs(dest, (const char *)DSTRBUF(src), index, n);
}

/* ************************************************************************* */

int dstrxchg(dstring_t str, size_t index, char c) {

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(str)) {
      return DSTR_OUT_OF_BOUNDS;
   }

   /* make sure we're not attempting to insert a \0 */
   if ('\0' == c) {
      return DSTR_INVALID_ARGUMENT;
   }

   /* exchange the old character for the new */
   DSTRBUF(str)[index] = c;

   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrgetc(dstring_t str, size_t index) {

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(str)) {
      return DSTR_OUT_OF_BOUNDS;
   }

   return DSTRBUF(str)[index];
}

/* ************************************************************************* */

int dstryankc(dstring_t str, size_t index) {

   size_t i;
   char returnval;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(str)) {
      return DSTR_OUT_OF_BOUNDS;
   }

   /* this is the value we'll return */
   returnval = DSTRBUF(str)[index];

   /* now, we must remove this character from the string */
   dstrdel(str, index);

   return returnval;
}

/* ************************************************************************* */

int dstreplacec(dstring_t str, char oldc, char newc) {

   size_t i;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      return DSTR_UNINITIALIZED;
   }

   /* make sure we're not trying to remove or insert \0's */
   if ('\0' == oldc || '\0' == newc) {
      return DSTR_INVALID_ARGUMENT;
   }

   /* replace all instances of oldc with newc */
   for (i = 0; i < dstrlen(str); i++) {
      if (oldc == DSTRBUF(str)[i]) {
         DSTRBUF(str)[i] = newc;
      }
   }

   return DSTR_SUCCESS;
}
