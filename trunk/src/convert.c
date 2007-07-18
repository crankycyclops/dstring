
/* ************************************************************************* *\
   * File: io.c                                                            *
   * Purpose:                                                              *
   *    Provides input and output facilities for dstring_t objects         *
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

#include "static.h"
#include "dstring.h"

/* ************************************************************************* */

int dstrtocstr(char *dest, const dstring_t src, size_t size) {

   size_t i;

   /* make sure we're not dealing with an uninitialized dstring */
   if (NULL == src) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return DSTR_UNINITIALIZED;
   }

   /* make sure dest is not a NULL pointer, which would cause a crash */
   if (NULL == dest) {
      _setdstrerrno(DSTR_NULL_CPTR);
      return DSTR_NULL_CPTR;
   }

   /* if size is 0, create an empty string */
   if (0 == size) {
      *dest = '\0';
      _setdstrerrno(DSTR_SUCCESS);
      return DSTR_SUCCESS;
   }

   for (i = 0; DSTRBUF(src)[i] != '\0' && i < size - 1; i++) {
      dest[i] = DSTRBUF(src)[i];
   }

   dest[++i] = '\0';
   _setdstrerrno(DSTR_SUCCESS);
   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int cstrtodstr(dstring_t dest, const char *src) {

   size_t i;
   int status;

   /* make sure we're not dealing with an uninitialized string */
   if (NULL == dest) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return DSTR_UNINITIALIZED;
   }

   /* make sure src is not a NULL pointer, which would cause a crash */
   if (NULL == src) {
      _setdstrerrno(DSTR_NULL_CPTR);
      return DSTR_NULL_CPTR;
   }

   for (i = 0; src[i] != '\0'; i++) {
      /* we're out of memory; allocate more */
      if (i >= (DSTRBUFLEN(dest) - 1)) {
         if (DSTR_SUCCESS != \
(status = dstrealloc(&dest, DSTRBUFLEN(dest) * 2))) {
            /* NULL terminate what we were able to get and return */
            DSTRBUF(dest)[i] = '\0';
            _setdstrerrno(status);
            return status;
         }
      }
      DSTRBUF(dest)[i] = src[i];
   }

   DSTRBUF(dest)[++i] = '\0';
   _setdstrerrno(DSTR_SUCCESS);
   return DSTR_SUCCESS;
}
