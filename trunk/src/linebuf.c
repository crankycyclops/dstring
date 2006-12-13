
/* ************************************************************************* *\
   * File: linebuf.c                                                       *
   * Purpose:                                                              *
   *    Provides the code for dealing with data structures of type lineBuf *
   *************************************************************************
   * Project:    Library for Dynamically Allocated Strings                 *
   * Programmer: James Colannino                                           *
   * Email:      james@colannino.org                                       *
   * Homepage:   http://james.colannino.org/                               *
   *                                                                       *
   * Description:                                                          *
   *     The purpose of this library is to provide facilities for easily   *
   * dealing with dynamically allocated strings.                           *
   ************************************************************************* 
   * Linebuf Library Copyright 2006 by James Colannino                     *
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
#include <stdlib.h>
#include <string.h>

#include "linebuf.h"

/* INTERNAL FUNCTION: addmemory(): doubles the size of linebuf->buf.
   Returns a pointer to the newly allocated memory or exits if it fails.

   Input:
      - pointer to an initialize lineBuf structure
*/
static char *addmemory(lineBuf *linebuf);


int initline(lineBuf *linebuf, size_t buflen) {

   /* check for invalid buflen value */
   if (buflen <= 0) {
      return -1;
   } else {
      linebuf->buflen = buflen;
   }

   /* attempt to allocate memory */
   if ((linebuf->buf = calloc(buflen, sizeof(char))) == NULL) {
      return -2;
   }

   /* the structure was initialized successfully */
   return 0;
}


int initstdin(lineBuf *linebuf, size_t buflen) {

   int status = initline(linebuf, buflen);

   if (status < 0) {
      return status;
   } else {
      linebuf->fp = stdin;
      return 0;
   }
}


void freeline(lineBuf *linebuf) {

   free(linebuf->buf);
   linebuf->buflen = 0;

   return;
}


int readline(lineBuf *linebuf) {

   char *start;                     /* points to beginning of buffer */
   char *bufpos;                    /* our current position in linebuf->buf */
   char *status;                    /* tests for NULL when we encounter EOF */

   ptrdiff_t bufcount = 0;          /* how many characters we've read so far */

   /* linebuf->fp is not an opened file */
   if (linebuf->fp == NULL)
      return -2;

   bufpos = start = linebuf->buf;

   read:
   status = fgets(bufpos, linebuf->buflen - bufcount, linebuf->fp);

   /* EOF encountered */
   if (status == NULL)
      return -1;

   /* make sure bufpos points to our current position */
   bufpos = start + strlen(linebuf->buf);

   /* need more memory to complete line */
   if (bufpos[-1] != '\n') {
      /* if we can't get more memory, we can't finish the line */
      if ((start = addmemory(linebuf)) == NULL) {
         return -3;
      }
      bufpos = start + strlen(linebuf->buf);
      /* the +1 at the end is to make sure the '\0' is counted */
      bufcount = bufpos - start + 1;
      goto read;
   };

   return 0;
}


static char *addmemory(lineBuf *linebuf) {

   char  *bufcpy    = linebuf->buf;
   size_t buflencpy = linebuf->buflen;

   buflencpy *= 2;

   if ((bufcpy = (char *)realloc(bufcpy, buflencpy)) == NULL) {
      /* return NULL if we fail to reallocate */
      return NULL;
   }

   /* reallocation was successful */
   linebuf->buf    = bufcpy;
   linebuf->buflen = buflencpy;
   return linebuf->buf;
}
