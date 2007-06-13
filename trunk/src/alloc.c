
/* ************************************************************************* *\
   * File: alloc.c                                                         *
   * Purpose:                                                              *
   *    Provides code for allocating and deallocating dstring_t objects    *
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

#include <stdlib.h>
#include <stdio.h>

#include "static.h"
#include "dstring.h"

/* ************************************************************************* */

int dstralloc(dstring_t *strptr, size_t bytes) {

   /* if a value of 0 is given, we must free the string */
   if (0 == bytes) {
      return dstrfree(strptr);
   }

   /* attempt to allocate the structure itself */
   if ((*strptr = (dstrptr)calloc(1, sizeof(dstr))) == NULL) {
      return DSTR_NOMEM;
   }

   /* attempt to allocate space for the buffer inside str */
   if ((DSTRBUF(*strptr) = calloc(bytes, sizeof(char))) == NULL) {
      free(DSTRREF(*strptr)), *strptr = NULL;
      return DSTR_NOMEM;
   }

   /* update the buflen data member */
   DSTRBUFLEN(*strptr) = bytes;

   /* null terminate the buffer, so we start out with an "empty" string */
   DSTRBUF(*strptr)[0] = '\0';

   /* the structure was initialized successfully */
   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrealloc(dstring_t *strptr, size_t bytes) {

   char *tmpbuf;

   /* if it's an uninitialized string, use dstralloc instead */
   if (NULL == *strptr) {
      return dstralloc(strptr, bytes);
   }

   /* if a value of 0 is given, we must free the string */
   if (0 == bytes) {
      return dstrfree(strptr);
   }

   /* the size hasn't changed; nothing to do */
   else if (DSTRBUFLEN(*strptr) == bytes) {
      return DSTR_SUCCESS;
   }

   /* attempt to reallocate the buffer */
   tmpbuf = DSTRBUF(*strptr);
   if ((tmpbuf = realloc(tmpbuf, bytes)) == NULL) {
      /* whatever was in the string before remains untouched */
      return DSTR_NOMEM;
   }

   /* make sure we update the NULL terminating char to ensure we don't try to
      access out of bounds memory later if we are shrinking the buffer */
   tmpbuf[bytes - 1] = '\0';

   /* update the buf and buflen members and return success */
   DSTRBUF(*strptr) = tmpbuf;
   DSTRBUFLEN(*strptr) = bytes;
   return DSTR_SUCCESS;
}

/* ************************************************************************* */

int dstrfree(dstring_t *strptr) {

   /* make sure it's not an uninitialized string */
   if (NULL == *strptr) {
      return DSTR_UNINITIALIZED;
   }

   /* free allocated memory */
   free(DSTRBUF(*strptr));
   free(DSTRREF(*strptr));

   /* ensure that the caller's dstring_t is set to NULL upon return */
   *strptr = NULL;
   return DSTR_SUCCESS;
}
