
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


size_t dstrlen(dstring_t str) {

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      return DSTR_UNINITIALIZED;
   }

   return strlen(DSTRBUF(str));
}


int dstrtrunc(dstring_t str, size_t size) {

   int i;
   size_t length;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == str) {
      return DSTR_UNINITIALIZED;
   }

   /* get the length of the string */
   length = dstrlen(str);

   /* if the specified size is greater than or equal to the length of the
      string, there's nothing to do. */
   if (size >= length) {
      return DSTR_SUCCESS;
   }

   /* let's truncate, baby! */
   for (i = 0; i < size; i++);
   DSTRBUF(str)[i] = '\0';

   return DSTR_SUCCESS;
}
