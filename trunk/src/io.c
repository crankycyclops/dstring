
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


int dfreadl(dstring_t dest, FILE *fp) {

   char *start;                     /* points to beginning of buffer */
   char *bufpos;                    /* our current position in DSTRBUF(dest) */
   char *status;                    /* tests for NULL when we encounter EOF */

   ptrdiff_t bufcount = 0;          /* how many characters we've read so far */

   /* make sure dest is initialized */
   if (NULL == dest) {
      return DSTR_UNINITIALIZED;
   }

   /* make sure fp is an opened file */
   if (NULL == fp) {
      return DSTR_UNOPENED_FILE;
   }

   bufpos = start = DSTRBUF(dest);

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
      if (DSTR_SUCCESS != dstrealloc(&dest, DSTRBUFLEN(dest))) {
         return DSTR_NOMEM;
      }

      bufpos = start + strlen(DSTRBUF(dest));

      /* the +1 at the end is to make sure the '\0' is counted */
      bufcount = bufpos - start + 1;
      goto read;
   };

   return DSTR_SUCCESS;
}


/* implement as a dstring.h macro ??? */
int dreadl(dstring_t dest) {

   return(dfreadl(dest, stdin));
}
