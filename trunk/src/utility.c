
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
      dstrerrno = DSTR_UNINITIALIZED;
      return DSTR_UNINITIALIZED;
   }

   /* out of bounds */
   if (index >= dstrlen(str)) {
      dstrerrno = DSTR_OUT_OF_BOUNDS;
      return DSTR_OUT_OF_BOUNDS;
   }

   /* the index is within str's bounds */
   dstrerrno = DSTR_SUCCESS;
   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrtrunc(dstring_t str, size_t size) {

   size_t i;
   size_t length;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      dstrerrno = DSTR_UNINITIALIZED;
      return 0;
   }

   /* get the length of the string */
   length = dstrlen(str);

   /* if the specified size is 0, wipe out the string */
   if (0 == size) {
      DSTRBUF(str)[0] = '\0';
      dstrerrno = DSTR_SUCCESS;
      return dstrlen(str);
   }

   /* if size is greater than or equal to the length of the string,
      we have nothing to do */
   else if (size >= length) {
      dstrerrno = DSTR_SUCCESS;
      return dstrlen(str);
   }

   /* let's truncate, baby! */
   for (i = 0; i < size; i++);
   DSTRBUF(str)[i] = '\0';

   dstrerrno = DSTR_SUCCESS;
   return dstrlen(str);
}

/* ************************************************************************* */

int dstrtrimleft(dstring_t str, size_t n) {

   size_t i;
   size_t length;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      dstrerrno = DSTR_UNINITIALIZED;
      return 0;
   }

   /* get the length of the string */
   length = dstrlen(str);

   /* if the specified size is 0, there's nothing to do. */
   if (0 == n) {
      dstrerrno = DSTR_SUCCESS;
      return dstrlen(str);
   }

   /* if n is greater than or equal to the length of the string,
      wipe out the whole string */
   else if (n >= length) {
      DSTRBUF(str)[0] = '\0';
      dstrerrno = DSTR_SUCCESS;
      return dstrlen(str);
   }

   /* truncate from the left */
   for (i = 0; DSTRBUF(str)[i + n] != '\0'; i++) {
      DSTRBUF(str)[i] = DSTRBUF(str)[i + n];
   }

   /* null terminate newly truncated string */
   DSTRBUF(str)[i] = '\0';

   dstrerrno = DSTR_SUCCESS;
   return dstrlen(str);
}

/* ************************************************************************* */

int dstrdel(dstring_t str, size_t index) {

   size_t i;
   char   c;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      dstrerrno = DSTR_UNINITIALIZED;
      return -1;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(str)) {
      dstrerrno = DSTR_OUT_OF_BOUNDS;
      return -1;
   }

   /* save the character we're going to delete so we can return it */
   c = DSTRBUF(str)[i];

   /* starting at index, move each character left one position */
   for (i = index; '\0' != DSTRBUF(str)[i]; i++) {
      DSTRBUF(str)[i] = DSTRBUF(str)[i + 1];
   }

   dstrerrno = DSTR_SUCCESS;
   return c;
}

/* ************************************************************************* */

int dstrndel(dstring_t str, size_t index, size_t n) {

   size_t i;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      dstrerrno = DSTR_UNINITIALIZED;
      return dstrlen(str);
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(str)) {
      dstrerrno = DSTR_OUT_OF_BOUNDS;
      return dstrlen(str);
   }

   /* if n is 0, return DSTR_SUCCESS without doing anything */
   if (n == 0) {
      dstrerrno = DSTR_SUCCESS;
      return dstrlen(str);
   }

   /* If n is greater than the size of the string, zero it out */
   if (n > dstrlen(str)) {
      DSTRBUF(str)[0] = '\0';
      dstrerrno = DSTR_SUCCESS;
      return dstrlen(str);
   }

   /* starting at index, move each character left n positions */
   for (i = index; '\0' != DSTRBUF(str)[i]; i++) {
      DSTRBUF(str)[i] = DSTRBUF(str)[i + n];
      if ('\0' == DSTRBUF(str)[i]) {
         break;
      }
   }

   dstrerrno = DSTR_SUCCESS;
   return dstrlen(str);
}

/* ************************************************************************* */

int dstrinsertc(dstring_t dest, size_t index, char c) {

   size_t i;
   int retval;          /* for the return value of dstrealloc() */

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == dest) {
      dstrerrno = DSTR_UNINITIALIZED;
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(dest)) {
      dstrerrno = DSTR_OUT_OF_BOUNDS;
      return DSTR_OUT_OF_BOUNDS;
   }

   /* make sure we're not attempting to insert a \0 */
   if ('\0' == c) {
      dstrerrno = DSTR_INVALID_ARGUMENT;
      return DSTR_INVALID_ARGUMENT;
   }

   /* is our current allocation big enough? */
   if (DSTRBUFLEN(dest) <= dstrlen(dest) + 2) {
      /* if not, we need to add extra space */
      if (DSTR_SUCCESS != (retval = dstrealloc(&dest, DSTRBUFLEN(dest) + 10 ))) {
         /* if the allocation was not successful, the string is untouched */
         dstrerrno = retval;
         return retval;
      }
   }

   /* shift everything over one unit to the right, including the '\0' */
   for (i = dstrlen(dest); i >= index; i--) {
      DSTRBUF(dest)[i + 1] = DSTRBUF(dest)[i];
   }

   /* insert our character at the specified index */
   DSTRBUF(dest)[index] = c;

   dstrerrno = DSTR_SUCCESS;
   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrinsertcs(dstring_t dest, const char *src, size_t index) {

   size_t i, j;
   int retval;          /* for the return value of dstrealloc() */

   int srclen = strlen(src);

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == dest) {
      dstrerrno = DSTR_UNINITIALIZED;
      return dstrlen(dest);
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(dest)) {
      dstrerrno = DSTR_OUT_OF_BOUNDS;
      return dstrlen(dest);
   }

   /* is our current allocation big enough? */
   if (DSTRBUFLEN(dest) <= dstrlen(dest) + srclen + 1) {
      /* if not, we need to add extra space */
      if (DSTR_SUCCESS != (retval = dstrealloc(&dest, DSTRBUFLEN(dest) + \
srclen + 1))) {
         /* if the allocation was not successful, the string is untouched */
         dstrerrno = retval;
         return dstrlen(dest);
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

   dstrerrno = DSTR_SUCCESS;
   return dstrlen(dest);
}

/* ************************************************************************* */

int dstrinserts(dstring_t dest, const dstring_t src, size_t index) {

   /* dstrcinsert() will check to make sure dest is initialized, but we must
      first make sure that src is also initialized! */
   if (NULL == src) {
      dstrerrno = DSTR_UNINITIALIZED;
      return dstrlen(dest);
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
      dstrerrno = DSTR_UNINITIALIZED;
      return dstrlen(dest);
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(dest)) {
      dstrerrno = DSTR_OUT_OF_BOUNDS;
      return dstrlen(dest);
   }

   /* if n is 0, return DSTR_SUCCESS without doing anything */
   if (n == 0) {
      dstrerrno = DSTR_SUCCESS;
      return dstrlen(dest);
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
         dstrerrno = retval;
         return dstrlen(dest);
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

   dstrerrno = DSTR_SUCCESS;
   return dstrlen(dest);
}

/* ************************************************************************* */

int dstrninserts(dstring_t dest, const dstring_t src, size_t index, size_t n) {

   /* dstrcinsert() will check to make sure dest is initialized, but we must
      first make sure that src is also initialized! */
   if (NULL == src) {
      dstrerrno = DSTR_UNINITIALIZED;
      return dstrlen(dest);
   }

   return dstrninsertcs(dest, (const char *)DSTRBUF(src), index, n);
}

/* ************************************************************************* */

int dstrxchg(dstring_t str, size_t index, char c) {

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      dstrerrno = DSTR_UNINITIALIZED;
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(str)) {
      dstrerrno = DSTR_OUT_OF_BOUNDS;
      return DSTR_OUT_OF_BOUNDS;
   }

   /* make sure we're not attempting to insert a \0 */
   if ('\0' == c) {
      dstrerrno = DSTR_INVALID_ARGUMENT;
      return DSTR_INVALID_ARGUMENT;
   }

   /* exchange the old character for the new */
   DSTRBUF(str)[index] = c;

   dstrerrno = DSTR_SUCCESS;
   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrgetc(dstring_t str, size_t index) {

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      dstrerrno = DSTR_UNINITIALIZED;
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(str)) {
      dstrerrno = DSTR_OUT_OF_BOUNDS;
      return DSTR_OUT_OF_BOUNDS;
   }

   /* indicate success and return the character */
   dstrerrno = DSTR_SUCCESS;
   return DSTRBUF(str)[index];
}

/* ************************************************************************* */

int dstreplacec(dstring_t str, char oldc, char newc) {

   size_t i;
   size_t replacements = 0;          /* number of replacements made */

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      dstrerrno = DSTR_UNINITIALIZED;
      return 0;
   }

   /* make sure we're not trying to remove or insert \0's */
   if ('\0' == oldc || '\0' == newc) {
      dstrerrno DSTR_INVALID_ARGUMENT;
      return 0;
   }

   /* replace all instances of oldc with newc */
   for (i = 0; i < dstrlen(str); i++) {
      if (oldc == DSTRBUF(str)[i]) {
         replacements++;
         DSTRBUF(str)[i] = newc;
      }
   }

   dstrerrno = DSTR_SUCCESS;
   return replacements;
}
