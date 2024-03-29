
/* ************************************************************************* *\
   * File: cstdlib.c                                                       *
   * Purpose:                                                              *
   *    Provides functions for dstring_t objects that are meant to emulate *
   *    the behavior of the ordinary standard C library functions for      *
   *    strings.                                                           *
   *                                                                       *
   * ***Note***                                                            *
   *    Many of the standard library C functions can still be used by      *
   *    passing dstrview(dstring_t) in place of char *.  Thus, only those  *
   *    functions which must modify a dstring_t object will be written     *
   *    here.                                                              *
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

size_t dstrlen(const dstring_t str) {

   size_t count;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return 0;
   }

   /* get the size of the string */
   for (count = 0; DSTRBUF(str)[count] != '\0'; count++);

   /* indicate success and return */
   _setdstrerrno(DSTR_SUCCESS);
   return count;
}

/* ************************************************************************* */

int dstrcat(dstring_t dest, const dstring_t src) {

   size_t i, j;
   int retval;

   /* make sure dest and src are both initialized */
   if (NULL == src) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return 0;
   } else if (NULL == dest) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return 0;
   }

   /* nothing to do if the source string is empty */
   if (0 == dstrlen(src)) {
      _setdstrerrno(DSTR_SUCCESS);
      return 0;
   }

   /* if dest isn't big enough, we'll have to give it more memory */
   if (DSTRBUFLEN(dest) <= dstrlen(dest) + dstrlen(src)) {
      if (DSTR_SUCCESS != (retval = dstrealloc(&dest, DSTRBUFLEN(dest) + \
dstrlen(src) + 1))) {
         /* if the allocation was not successful, the string is untouched */
         _setdstrerrno(retval);
         return 0;
      }
   }

   /* append src to dest */
   for (i = dstrlen(dest), j = 0; DSTRBUF(src)[j] != '\0'; i++, j++) {
      DSTRBUF(dest)[i] = DSTRBUF(src)[j];
   }

   /* null terminate the newly appended string */
   DSTRBUF(dest)[i] = '\0';

   /* indicate success and return */
   _setdstrerrno(DSTR_SUCCESS);
   return dstrlen(src);
}

/* ************************************************************************* */

int dstrncat(dstring_t dest, const dstring_t src, size_t n) {

   size_t i, j;
   int retval;

   /* make sure dest and src are both initialized */
   if (NULL == src) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return 0;
   } else if (NULL == dest) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return 0;
   }

   /* nothing to do if the source string is empty or if n is 0 */
   if (0 == dstrlen(src) || 0 == n) {
      _setdstrerrno(DSTR_SUCCESS);
      return 0;
   }

   /* if n is >= size of the string, just append all of src */
   if (n >= dstrlen(src)) {
      n = dstrlen(src);
   }

   /* if dest isn't big enough, we'll have to give it more memory */
   if (DSTRBUFLEN(dest) <= dstrlen(dest) + n) {
      if (DSTR_SUCCESS != (retval = dstrealloc(&dest, DSTRBUFLEN(dest) + \
n + 1))) {
         /* if the allocation was not successful, the string is untouched */
         _setdstrerrno(retval);
         return 0;
      }
   }

   /* append src to dest */
   for (i = dstrlen(dest), j = 0; j < n; i++, j++) {
      DSTRBUF(dest)[i] = DSTRBUF(src)[j];
   }

   /* null terminate the newly appended string */
   DSTRBUF(dest)[i] = '\0';

   /* indicate success and return */
   _setdstrerrno(DSTR_SUCCESS);
   return n;
}

/* ************************************************************************* */

int dstrcatcs(dstring_t dest, const char *src) {

   size_t i, j;
   int retval;

   /* make sure dest is initialized */
   if (NULL == dest) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return 0;
   }

   /* make sure src is not a NULL pointer */
   if (NULL == src) {
      _setdstrerrno(DSTR_NULL_CPTR);
      return 0;
   }

   /* nothing to do if the source string is empty */
   if (0 == strlen(src)) {
      _setdstrerrno(DSTR_SUCCESS);
      return 0;
   }

   /* if dest isn't big enough, we'll have to give it more memory */
   if (DSTRBUFLEN(dest) <= dstrlen(dest) + strlen(src)) {
      if (DSTR_SUCCESS != (retval = dstrealloc(&dest, DSTRBUFLEN(dest) + \
strlen(src) + 1))) {
         /* if the allocation was not successful, the string is untouched */
         _setdstrerrno(retval);
         return 0;
      }
   }

   /* append src to dest */
   for (i = dstrlen(dest), j = 0; src[j] != '\0'; i++, j++) {
      DSTRBUF(dest)[i] = src[j];
   }

   /* null terminate the newly appended string */
   DSTRBUF(dest)[i] = '\0';

   /* indicate success and return */
   _setdstrerrno(DSTR_SUCCESS);
   return strlen(src);
}

/* ************************************************************************* */

int dstrncatcs(dstring_t dest, const char *src, size_t n) {

   size_t i, j;
   int retval;

   /* make sure dest is initialized */
   if (NULL == dest) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return 0;
   }

   /* make sure src is not a NULL pointer */
   if (NULL == src) {
      _setdstrerrno(DSTR_NULL_CPTR);
      return 0;
   }

   /* nothing to do if the source string is empty or if n is 0 */
   if (0 == strlen(src) || 0 == n) {
      _setdstrerrno(DSTR_SUCCESS);
      return 0;
   }

   /* if n is >= size of the string, just append all of src */
   if (n >= strlen(src)) {
      n = strlen(src);
   }

   /* if dest isn't big enough, we'll have to give it more memory */
   if (DSTRBUFLEN(dest) <= dstrlen(dest) + n) {
      if (DSTR_SUCCESS != (retval = dstrealloc(&dest, DSTRBUFLEN(dest) + \
n + 1))) {
         /* if the allocation was not successful, the string is untouched */
         _setdstrerrno(retval);
         return 0;
      }
   }

   /* append src to dest */
   for (i = dstrlen(dest), j = 0; j < n; i++, j++) {
      DSTRBUF(dest)[i] = src[j];
   }

   /* null terminate the newly appended string */
   DSTRBUF(dest)[i] = '\0';

   /* indicate success and return */
   _setdstrerrno(DSTR_SUCCESS);
   return n;
}

/* ************************************************************************* */

int dstrcpy(dstring_t dest, const dstring_t src) {

   size_t count;
   int retval;

   /* make sure dest and src are both initialized */
   if (NULL == src) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return 0;
   } else if (NULL == dest) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return 0;
   }

   /* nothing to do */
   if (0 == dstrlen(src)) {
      _setdstrerrno(DSTR_SUCCESS);
      return 0;
   }

   /* check to see if dest needs to "grow" */
   if (DSTRBUFLEN(dest) <= dstrlen(src)) {
      if (DSTR_SUCCESS != (retval = dstrealloc(&dest, DSTRBUFLEN(dest) + \
dstrlen(src) + 1))) {
         /* if the allocation was not successful, the string is untouched */
         _setdstrerrno(retval);
         return 0;
      }

   }

   /* copy source to destination */
   for (count = 0; DSTRBUF(src)[count] != '\0'; count++) {
      DSTRBUF(dest)[count] = DSTRBUF(src)[count];
   }

   /* NULL terminate the copied string */
   DSTRBUF(dest)[count] = '\0';

   _setdstrerrno(DSTR_SUCCESS);
   return count;
}

/* ************************************************************************* */

int dstrncpy(dstring_t dest, const dstring_t src, size_t n) {

   size_t i;
   int retval;

   /* make sure dest and src are both initialized */
   if (NULL == src) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return 0;
   } else if (NULL == dest) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return 0;
   }

   /* if n is larger than the size of src, just append all of src */
   if (n > dstrlen(src)) {
      n = dstrlen(src);
   }

   /* nothing to do */
   if (0 == dstrlen(src) || 0 == n) {
      _setdstrerrno(DSTR_SUCCESS);
      return 0;
   }

   /* check to see if dest needs to "grow" */
   if (DSTRBUFLEN(dest) <= n) {
      if (DSTR_SUCCESS != (retval = dstrealloc(&dest, DSTRBUFLEN(dest) + \
n + 1))) {
         /* if the allocation was not successful, the string is untouched */
         _setdstrerrno(retval);
         return 0;
      }

   }

   /* copy source to destination */
   for (i = 0; DSTRBUF(src)[i] != '\0' && i < n; i++) {
      DSTRBUF(dest)[i] = DSTRBUF(src)[i];
   }

   /* NULL terminate the newly appended string */
   DSTRBUF(dest)[i] = '\0';

   _setdstrerrno(DSTR_SUCCESS);
   return n;
}

