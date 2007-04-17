
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

#include <stdio.h>
#include <string.h>

#include "static.h"
#include "dstring.h"


int dstrfreadl(dstring_t dest, FILE *fp) {

   char *start;                     /* points to beginning of buffer */
   char *bufpos;                    /* our current position in DSTRBUF(dest) */
   char *status;                    /* tests for NULL when we encounter EOF */

   ptrdiff_t bufcount = 0;          /* how many characters we've read so far */

   int retval;                        /* returns the return value of dstrealloc() */

   /* make sure dest is initialized */
   if (NULL == dest) {
      return DSTR_UNINITIALIZED;
   }

   /* make sure fp is an opened file */
   if (NULL == fp) {
      return DSTR_UNOPENED_FILE;
   }

   bufpos = start = DSTRBUF(dest);

   /* zero out the previous string, so that if we encounter EOF, the
      caller will know whether or not a partial line was read */
   *start = '\0';

   read:
   status = fgets(bufpos, DSTRBUFLEN(dest) - bufcount, fp);

   /* EOF encountered */
   if (status == NULL) {
      return DSTR_EOF;
   }

   /* make sure bufpos points to our current position */
   bufpos = start + strlen(DSTRBUF(dest));

   /* need more memory to complete line */
   if (bufpos[-1] != '\n') {

      /* if we can't get more memory, we can't finish the line */
      if (DSTR_SUCCESS != (retval = dstrealloc(&dest, DSTRBUFLEN(dest) * 2))) {
         return retval;
      }

      /* make sure you update start lest you suffer the wrath of glibc... */
      start = DSTRBUF(dest);
      bufpos = start + strlen(DSTRBUF(dest));

      /* the +1 at the end is to make sure the '\0' is counted */
      bufcount = bufpos - start + 1;
      goto read;
   };

   return DSTR_SUCCESS;
}


int dstrfreadn(dstring_t dest, FILE *fp, size_t size) {

   char *status;     /* lets us know when we hit EOF */
   int   retval;     /* stores the return value of dstrealloc() */

  /* make sure dest is initialized */
   if (NULL == dest) {
      return DSTR_UNINITIALIZED;
   }

   /* make sure fp is an opened file */
   if (NULL == fp) {
      return DSTR_UNOPENED_FILE;
   }


   /* our current buffer won't accomodate the string; reallocate! */
   if (DSTRBUFLEN(dest) < size) {
      if (DSTR_SUCCESS != (retval = dstrealloc(&dest, size))) {
         return retval;
      }
   }

   /* now, zero out the previous string and read a new one */
   DSTRBUF(dest)[0] = '\0';
   status = fgets(DSTRBUF(dest), size, fp);

   /* if we got EOF, let the caller know */
   if (status == NULL) {
      return DSTR_EOF;
   } else {
      return DSTR_SUCCESS;
   }
}
