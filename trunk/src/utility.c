
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
      _setdstrerrno(DSTR_UNINITIALIZED);
      return DSTR_UNINITIALIZED;
   }

   /* out of bounds */
   if (index >= dstrlen(str)) {
      _setdstrerrno(DSTR_OUT_OF_BOUNDS);
      return DSTR_OUT_OF_BOUNDS;
   }

   /* the index is within str's bounds */
   _setdstrerrno(DSTR_SUCCESS);
   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrtrunc(dstring_t str, size_t size) {

   size_t i;
   size_t length;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return 0;
   }

   /* get the length of the string */
   length = dstrlen(str);

   /* if the specified size is 0, wipe out the string */
   if (0 == size) {
      DSTRBUF(str)[0] = '\0';
      _setdstrerrno(DSTR_SUCCESS);
      return dstrlen(str);
   }

   /* if size is greater than or equal to the length of the string,
      we have nothing to do */
   else if (size >= length) {
      _setdstrerrno(DSTR_SUCCESS);
      return dstrlen(str);
   }

   /* let's truncate, baby! */
   for (i = 0; i < size; i++);
   DSTRBUF(str)[i] = '\0';

   _setdstrerrno(DSTR_SUCCESS);
   return dstrlen(str);
}

/* ************************************************************************* */

int dstrtrimleft(dstring_t str, size_t n) {

   size_t i;
   size_t length;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return 0;
   }

   /* get the length of the string */
   length = dstrlen(str);

   /* if the specified size is 0, there's nothing to do. */
   if (0 == n) {
      _setdstrerrno(DSTR_SUCCESS);
      return dstrlen(str);
   }

   /* if n is greater than or equal to the length of the string,
      wipe out the whole string */
   else if (n >= length) {
      DSTRBUF(str)[0] = '\0';
      _setdstrerrno(DSTR_SUCCESS);
      return dstrlen(str);
   }

   /* truncate from the left */
   for (i = 0; DSTRBUF(str)[i + n] != '\0'; i++) {
      DSTRBUF(str)[i] = DSTRBUF(str)[i + n];
   }

   /* null terminate newly truncated string */
   DSTRBUF(str)[i] = '\0';

   _setdstrerrno(DSTR_SUCCESS);
   return dstrlen(str);
}

/* ************************************************************************* */

int dstrdel(dstring_t str, size_t index) {

   size_t i;
   char   c;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return -1;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(str)) {
      _setdstrerrno(DSTR_OUT_OF_BOUNDS);
      return -1;
   }

   /* save the character we're going to delete so we can return it */
   c = DSTRBUF(str)[i];

   /* starting at index, move each character left one position */
   for (i = index; '\0' != DSTRBUF(str)[i]; i++) {
      DSTRBUF(str)[i] = DSTRBUF(str)[i + 1];
   }

   _setdstrerrno(DSTR_SUCCESS);
   return c;
}

/* ************************************************************************* */

int dstrndel(dstring_t str, size_t index, size_t n) {

   size_t i;       /* for loop index */
   int charcount;  /* number of characters at and beyond the specified index */

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return dstrlen(str);
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(str)) {
      _setdstrerrno(DSTR_OUT_OF_BOUNDS);
      return dstrlen(str);
   }

   /* if n is 0, return DSTR_SUCCESS without doing anything */
   if (n == 0) {
      _setdstrerrno(DSTR_SUCCESS);
      return dstrlen(str);
   }

   /* If n is greater than the number of characters beyond the specified
      index, we want to set n so that everything at and beyond the
      specified index will be deleted */
   for (i = index; '\0' != DSTRBUF(str)[i]; i++)
      charcount++;
   if (n > charcount) {
      n = charcount;
   }

   /* starting at index, move each character left n positions */
   for (i = index; '\0' != DSTRBUF(str)[i]; i++) {
      DSTRBUF(str)[i] = DSTRBUF(str)[i + n];
      if ('\0' == DSTRBUF(str)[i]) {
         break;
      }
   }

   _setdstrerrno(DSTR_SUCCESS);
   return dstrlen(str);
}

/* ************************************************************************* */

int dstrinsertc(dstring_t dest, size_t index, char c) {

   size_t i;
   int retval;          /* for the return value of dstrealloc() */

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == dest) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(dest)) {
      _setdstrerrno(DSTR_OUT_OF_BOUNDS);
      return DSTR_OUT_OF_BOUNDS;
   }

   /* make sure we're not attempting to insert a \0 */
   if ('\0' == c) {
      _setdstrerrno(DSTR_INVALID_ARGUMENT);
      return DSTR_INVALID_ARGUMENT;
   }

   /* is our current allocation big enough? */
   if (DSTRBUFLEN(dest) <= dstrlen(dest) + 2) {
      /* if not, we need to add extra space */
      if (DSTR_SUCCESS != (retval = dstrealloc(&dest, DSTRBUFLEN(dest) + 10 ))) {
         /* if the allocation was not successful, the string is untouched */
         _setdstrerrno(retval);
         return retval;
      }
   }

   /* shift everything over one unit to the right, including the '\0' */
   for (i = dstrlen(dest); i > index; i--) {
      DSTRBUF(dest)[i + 1] = DSTRBUF(dest)[i];
   }
   /* i can only be decremented down to 0 before overflowing, since it's
      unsigned; thus, we must execute the loop body one more time */
   DSTRBUF(dest)[i + 1] = DSTRBUF(dest)[i];

   /* insert our character at the specified index */
   DSTRBUF(dest)[index] = c;

   _setdstrerrno(DSTR_SUCCESS);
   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrinsertcs(dstring_t dest, const char *src, size_t index) {

   size_t i, j;
   int retval;          /* for the return value of dstrealloc() */

   int srclen = strlen(src);

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == dest) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return dstrlen(dest);
   }

   /* make sure src is not a NULL pointer */
   if (NULL == src) {
      _setdstrerrno(DSTR_NULL_CPTR);
      return dstrlen(dest);
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(dest)) {
      _setdstrerrno(DSTR_OUT_OF_BOUNDS);
      return dstrlen(dest);
   }

   /* is our current allocation big enough? */
   if (DSTRBUFLEN(dest) <= dstrlen(dest) + srclen + 1) {
      /* if not, we need to add extra space */
      if (DSTR_SUCCESS != (retval = dstrealloc(&dest, DSTRBUFLEN(dest) + \
srclen + 1))) {
         /* if the allocation was not successful, the string is untouched */
         _setdstrerrno(retval);
         return dstrlen(dest);
      }
   }

   /* shift everything over srclen units to the right, including '\0' */
   for (i = dstrlen(dest); i > index; i--) {
      DSTRBUF(dest)[i + srclen] = DSTRBUF(dest)[i];
   }
   /* we can't let i be less than 0, since it's of type size_t (unsigned);
      thus, we execute the for loop statement one more time */
   DSTRBUF(dest)[i + srclen] = DSTRBUF(dest)[i];

   /* insert the source string in dest at the proper position */
   for (i = index, j = 0; j < srclen; i++, j++) {
      DSTRBUF(dest)[i] = src[j];
   }

   _setdstrerrno(DSTR_SUCCESS);
   return dstrlen(dest);
}

/* ************************************************************************* */

int dstrinserts(dstring_t dest, const dstring_t src, size_t index) {

   /* dstrcinsert() will check to make sure dest is initialized, but we must
      first make sure that src is also initialized! */
   if (NULL == src) {
      _setdstrerrno(DSTR_UNINITIALIZED);
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
      _setdstrerrno(DSTR_UNINITIALIZED);
      return dstrlen(dest);
   }

   /* make sure src is not a NULL pointer */
   if (NULL == src) {
      _setdstrerrno(DSTR_NULL_CPTR);
      return dstrlen(dest);
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(dest)) {
      _setdstrerrno(DSTR_OUT_OF_BOUNDS);
      return dstrlen(dest);
   }

   /* if n is 0, return DSTR_SUCCESS without doing anything */
   if (n == 0) {
      _setdstrerrno(DSTR_SUCCESS);
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
         _setdstrerrno(retval);
         return dstrlen(dest);
      }
   }

   /* shift everything over srclen units to the right, including '\0' */
   for (i = dstrlen(dest); i > index; i--) {
      DSTRBUF(dest)[i + srclen] = DSTRBUF(dest)[i];
   }
   /* above test can only decrement down to 0, because i is unsigned; so we
      execute the inner statements of the loop just one more time */
   DSTRBUF(dest)[i + srclen] = DSTRBUF(dest)[i];

   /* insert the source string in dest at the proper position */
   for (i = index, j = 0; j < srclen; i++, j++) {
      DSTRBUF(dest)[i] = src[j];
   }

   _setdstrerrno(DSTR_SUCCESS);
   return dstrlen(dest);
}

/* ************************************************************************* */

int dstrninserts(dstring_t dest, const dstring_t src, size_t index, size_t n) {

   /* dstrcinsert() will check to make sure dest is initialized, but we must
      first make sure that src is also initialized! */
   if (NULL == src) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return dstrlen(dest);
   }

   return dstrninsertcs(dest, (const char *)DSTRBUF(src), index, n);
}

/* ************************************************************************* */

int dstrxchg(dstring_t str, size_t index, char c) {

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(str)) {
      _setdstrerrno(DSTR_OUT_OF_BOUNDS);
      return DSTR_OUT_OF_BOUNDS;
   }

   /* make sure we're not attempting to insert a \0 */
   if ('\0' == c) {
      _setdstrerrno(DSTR_INVALID_ARGUMENT);
      return DSTR_INVALID_ARGUMENT;
   }

   /* exchange the old character for the new */
   DSTRBUF(str)[index] = c;

   _setdstrerrno(DSTR_SUCCESS);
   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrgetc(dstring_t str, size_t index) {

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return DSTR_UNINITIALIZED;
   }

   /* check to see if the index is out of bounds */
   if (index >= dstrlen(str)) {
      _setdstrerrno(DSTR_OUT_OF_BOUNDS);
      return DSTR_OUT_OF_BOUNDS;
   }

   /* indicate success and return the character */
   _setdstrerrno(DSTR_SUCCESS);
   return DSTRBUF(str)[index];
}

/* ************************************************************************* */

int dstreplacec(dstring_t str, char oldc, char newc) {

   size_t i;
   size_t replacements = 0;          /* number of replacements made */

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return 0;
   }

   /* make sure we're not trying to remove or insert \0's */
   if ('\0' == oldc || '\0' == newc) {
      _setdstrerrno(DSTR_INVALID_ARGUMENT);
      return 0;
   }

   /* replace all instances of oldc with newc */
   for (i = 0; i < dstrlen(str); i++) {
      if (oldc == DSTRBUF(str)[i]) {
         replacements++;
         DSTRBUF(str)[i] = newc;
      }
   }

   _setdstrerrno(DSTR_SUCCESS);
   return replacements;
}

/* ************************************************************************* */

int dstreplaces(dstring_t str, const char *olds, const char *news) {

   dstring_t temp;          /* temporary string */
   char *match;             /* points to next occurence of olds in str */

   int i, j;                /* for loop indices */
   int replacements = 0;    /* number of replacements made */

   /* make sure str is initialized */
   if (NULL == str) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return 0;
   }

   /* make sure olds points to something */
   if (NULL == olds) {
      _setdstrerrno(DSTR_NULL_CPTR);
      return 0;
   }

   /* allocate space for temp */
   if (DSTR_SUCCESS != dstralloc(&temp, dstrlen(str))) {
      _setdstrerrno(DSTR_NOMEM);
      return 0;
   }

   /* make a temporary copy of the string */
   dstrcpy(temp, str);
   if (dstrerrno != DSTR_SUCCESS) {
      dstrfree(&temp);
      return 0;
   }

   /* replace or remove all instances of olds from the string */
   while (NULL != (match = strstr(DSTRBUF(temp), olds))) {
      dstrndel(temp, match - DSTRBUF(temp), strlen(olds));
      if (dstrerrno != DSTR_SUCCESS) {
         dstrfree(&temp);
	 return 0;
      }
      dstrinsertcs(temp, news, match - DSTRBUF(temp));
      if (dstrerrno != DSTR_SUCCESS) {
         dstrfree(&temp);
	 return 0;
      }
      replacements++;
   }

   /* copy the temporary string back to str */
   dstrcpy(str, temp);
   if (dstrerrno != DSTR_SUCCESS) {
      return 0;
   }

   dstrfree(&temp);
   _setdstrerrno(DSTR_SUCCESS);
   return replacements;
}
