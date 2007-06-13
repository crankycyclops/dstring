
/* ************************************************************************* *\
   * File: format.c                                                        *
   * Purpose:                                                              *
   *    Provides functions for formatting strings                          *
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

int dstrpadl(dstring_t str, size_t n, char c) {

   size_t i;
   int retval;

   size_t oldstrlen = dstrlen(str);

   /* make sure str is initialized */
   if (NULL == str) {
      return DSTR_UNINITIALIZED;
   }

   /* if n is 0, do nothing */
   if (0 == n) {
      return DSTR_SUCCESS;
   }

   /* we can't insert '\0' characters */
   if ('\0' == c) {
      return DSTR_INVALID_ARGUMENT;
   }

   /* make sure we have enough space in the string */
   if (DSTRBUFLEN(str) <= oldstrlen + n) {
      if (DSTR_SUCCESS != (retval = dstrealloc(&str, DSTRBUFLEN(str) + n + 1))) {
         /* if the allocation was not successful, the string is untouched */
         return retval;
      }
   }

   /* shift everything to the left n places */
   for (i = oldstrlen + n - 1; i >= n; i--) {
      DSTRBUF(str)[i] = DSTRBUF(str)[i - n];
   }

   /* insert the padding characters at the left */
   for (i = 0; i < n; i++) {
      DSTRBUF(str)[i] = c;
   }

   /* NULL terminate new string */
   DSTRBUF(str)[oldstrlen + n] = '\0';

   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrpadr(dstring_t str, size_t n, char c) {

   size_t i, j;
   int retval;

   /* make sure str is initialized */
   if (NULL == str) {
      return DSTR_UNINITIALIZED;
   }

   /* if n is 0, do nothing */
   if (0 == n) {
      return DSTR_SUCCESS;
   }

   /* we can't insert '\0' characters */
   if ('\0' == c) {
      return DSTR_INVALID_ARGUMENT;
   }

   /* make sure we have enough space in the string */
   if (DSTRBUFLEN(str) <= dstrlen(str) + n) {
      if (DSTR_SUCCESS != (retval = dstrealloc(&str, DSTRBUFLEN(str) + n + 1))) {
         /* if the allocation was not successful, the string is untouched */
         return retval;
      }
   }

   /* append n fill characters to the end of the string */
   for (i = dstrlen(str), j = 0; j < n; i++, j++) {
       DSTRBUF(str)[i] = c;
   }

   /* NULL terminate the string */
   DSTRBUF(str)[i] = '\0';

   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrcenter(dstring_t str, size_t len) {


}

/* ************************************************************************* */

int dstrright(dstring_t str, size_t len) {


}

/* ************************************************************************* */

int dstrleft(dstring_t str, size_t len) {


}
