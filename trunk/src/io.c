
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

/* ************************************************************************* */

size_t dstrfreadl(dstring_t dest, FILE *fp) {

   char     *start;                 /* points to beginning of buffer */
   char     *bufpos;                /* our current position in DSTRBUF(dest) */
   char     *status;                /* tests for NULL when we encounter EOF */
   ptrdiff_t bufcount = 0;          /* how many characters we've read so far */

   int       retval;                /* returns the return value of dstrealloc() */
   dstring_t temp;                  /* temporary dstring_t object */

   /* make sure dest is initialized */
   if (NULL == dest) {
      dstrerrno = DSTR_UNINITIALIZED;
      return 0;
   }

   /* make sure fp is an opened file */
   if (NULL == fp) {
      dstrerrno = DSTR_UNOPENED_FILE;
      return 0;
   }

   /* allocate space for temp */
   if (DSTR_SUCCESS != dstralloc(&temp, dstrallocsize(dest))) {
      dstrerrno = DSTR_NOMEM;
      return 0;
   }

   bufpos = start = DSTRBUF(temp);

   /* *start = '\0'; */

   /* loop until we've read a whole line */
   while (1) {

      status = fgets(bufpos, DSTRBUFLEN(temp) - bufcount, fp);

      /* EOF or read error encountered */
      if (status == NULL) {

         /* if we read something, we should keep it */
         if (dstrlen(temp) > 0) {
            break;
         }

         /* there's nothing to read */
         if (feof(fp)) {
            dstrerrno = DSTR_EOF;
         } else {
            dstrerrno = DSTR_FILE_ERROR;
         }

         dstrfree(&temp);
         return 0;
      }

      /* make sure bufpos points to our current position */
      bufpos = start + dstrlen(temp);

      /* we're done */
      if ('\n' == bufpos[-1]) {
         break;
      }

      /* if we can't get more memory, we can't finish the line */
      if (DSTR_SUCCESS != (retval = dstrealloc(&temp, DSTRBUFLEN(temp) * 2))) {
         dstrerrno = retval;
         dstrfree(&temp);
         return 0;
      }

      /* make sure you update start, lest you suffer the wrath of glibc... */
      start = DSTRBUF(temp);
      bufpos = start + dstrlen(temp);

      /* the +1 at the end is to make sure the '\0' is counted */
      bufcount = bufpos - start + 1;
   }

   /* make sure dest has enough space to store our new string */
   if (DSTR_SUCCESS != (retval = dstrealloc(&dest, dstrlen(temp) + 1))) {
      dstrerrno = retval;
      dstrfree(&temp);
      return 0;
   }

   /* copy our new string into dest */
   dstrcpy(dest, temp);

   /* indicate success and return */
   dstrerrno = DSTR_SUCCESS;
   dstrfree(&temp);
   return dstrlen(dest);
}

/* ************************************************************************* */

size_t dstrfreadn(dstring_t dest, FILE *fp, size_t n) {

   int   count;      /* number of characters read from fp */
   int   c;          /* temporary storage for a character */
   int   retval;

  /* make sure dest is initialized */
   if (NULL == dest) {
      dstrerrno = DSTR_UNINITIALIZED;
      return 0;
   }

   /* make sure fp is an opened file */
   if (NULL == fp) {
      dstrerrno = DSTR_UNOPENED_FILE;
      return 0;
   }

   /* make sure size is a valid digit (>= 0) */
   if (n < 0) {
      dstrerrno = DSTR_INVALID_ARGUMENT;
      return 0;
   }

   /* if size is 0, return DSTR_SUCCESS without doing anything */
   if (0 == n) {
      dstrerrno = DSTR_SUCCESS;
      return 0;
   }

   /* our current buffer won't accomodate the string; reallocate! */
   if (DSTRBUFLEN(dest) < n) {
      if (DSTR_SUCCESS != (retval = dstrealloc(&dest, n))) {
         dstrerrno = retval;
         return 0;
      }
   }

   /* now, read our new string, up to n characters */
   count = fread((void *)(DSTRBUF(dest)), sizeof(char), n, fp);

   /* make sure we got something; if not, find out what happened */
   if (0 == count) {
      if (feof(fp)) {
         dstrerrno = DSTR_EOF;
      } else {
         dstrerrno = DSTR_FILE_ERROR;
      }
      return 0;
   }

   /* NULL terminate the new string */
   if (count > 0) {
      DSTRBUF(dest)[count] = '\0';
   }

   /* indicate success and return */
   dstrerrno = DSTR_SUCCESS;
   return dstrlen(dest);
}

/* ************************************************************************* */

size_t dstrfcatl(dstring_t dest, FILE *fp) {

   size_t    count;  /* number of characters successfully read from fp */
   dstring_t temp;   /* will store the string which will be appended to dest */

   /* allocate space for our temporary string */
   if (DSTR_SUCCESS != dstralloc(&temp, dstrlen(dest))) {
      /* 0 items are successfully read if we can't get the memory ;)
         dstrerrno should have been set by dstralloc() */
      return 0;
   }

   /* read a line of input from the file */
   count = dstrfreadl(temp, fp);
   if (DSTR_SUCCESS != dstrerrno) {
      dstrfree(&temp);
      return 0;
   }

   /* cat temp to dest - dstrcat will see if dest is initialized for us :) */
   dstrcat(dest, temp);
   if (DSTR_SUCCESS != dstrerrno) {
      dstrfree(&temp);
      return 0;
   }

   /* free our temp string and return successfully */
   dstrfree(&temp);
   dstrerrno = DSTR_SUCCESS;
   return count;
}

/* ************************************************************************* */

/* the only difference between this function and dstrfcatl() is that we call
   dstrfreadn() instead of dstrfreadl() */

size_t dstrfcatn(dstring_t dest, FILE *fp, size_t n) {

   size_t    count;  /* number of characters successfully read from fp */
   dstring_t temp;   /* will store the string which will be appended to dest */

   /* allocate space for our temporary string */
   if (DSTR_SUCCESS != dstralloc(&temp, dstrlen(dest))) {
      /* 0 items are successfully read if we can't get the memory ;)
         dstrerrno should have been set by dstralloc() */
      return 0;
   }

   /* read a line of input from the file */
   count = dstrfreadn(temp, fp, n);
   if (DSTR_SUCCESS != dstrerrno) {
      dstrfree(&temp);
      return 0;
   }

   /* cat temp to dest - dstrcat will see if dest is initialized for us :) */
   dstrcat(dest, temp);
   if (DSTR_SUCCESS != dstrerrno) {
      dstrfree(&temp);
      return 0;
   }

   /* free our temp string and return successfully */
   dstrfree(&temp);
   dstrerrno = DSTR_SUCCESS;
   return count;
}
