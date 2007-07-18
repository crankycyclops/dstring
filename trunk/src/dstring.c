
/* ************************************************************************* *\
   * File: dstring.c                                                       *
   * Purpose:                                                              *
   *    Provides misc functions for the DString Library                    *
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

#include "dstring.h"
#include <stdlib.h>


/* error messages to accompany return codes */
const char *errormsgs[] = {
   "success",
   "out of memory",
   "invalid buffer length",
   "dstring_t uninitialized",
   "unopened file",
   "eof has been reached",
   "error reading file",
   "out of bounds",
   "invalid argument",
   "pointer to char is NULL",
   "unknown error"
};

/* Win32 version of our thread-safe dstrerrno */
#ifdef DSTR_WIN32THREAD
   /* etc */
#endif

/* Pthreads version of our thread-safe dstrerrno */
#ifdef DSTR_PTHREAD
   pthread_key_t _dstrerrno_key;                      /* key value for TLS */
   static void _dstrfree_dstrerrno(void *dstrptr);    /* pthread destructor */
#endif

/* dstrerrno for non-Win32 and non-pthreads systems (not thread-safe) */
#ifndef DSTR_WIN32THREAD
#ifndef DSTR_PTHREAD
   int dstrerrno;
#endif
#endif

/* ************************************************************************* */

const char * const dstrerrormsg(int code) {

   if (abs(code) > 9) {
      return errormsgs[10];
   } else {
      return errormsgs[abs(code)];
   }
}

/* ************************************************************************* */

/* FOR LIBRARY'S INTERNAL USE ONLY! */
void _setdstrerrno(int status) {

   /* if we're using threads, once the key is set, this will be 1 */
   static int iskeyset = 0;

   #ifdef DSTR_WIN32THREAD
      /* etc */
   #endif

   #ifdef DSTR_PTHREAD
      int   keystatus;
      int  *errvalptr;

      if (0 == iskeyset) {

         /* allocate space for a key and make sure it was successful */
         keystatus = pthread_key_create(&_dstrerrno_key, _dstrfree_dstrerrno);
         if (0 != keystatus) {
            fprintf(stderr, "__FILE__: __LINE__: error: could not allocate");
            fprintf(stderr, " dstrerrno\n");
            exit(EXIT_FAILURE);
         }

         /* allocate space for dstrerrno itself */
         if (NULL == (errvalptr = calloc(1, sizeof(int)))) {
            fprintf(stderr, "__FILE__: __LINE__: error: could not allocate");
            fprintf(stderr, " dstrerrno\n");
            exit(EXIT_FAILURE);
         }

         /* associate the key with the data for "dstrerrno" */
         if (0 != pthread_setspecific(_dstrerrno_key, errvalptr)) {
            fprintf(stderr, "__FILE__: __LINE__: error: could not allocate");
            fprintf(stderr, " dstrerrno\n");
            exit(EXIT_FAILURE);
         }

         iskeyset = 1;
      }

      errvalptr = (int *)pthread_getspecific(_dstrerrno_key);
      *errvalptr = status;
   #endif

   #ifndef DSTR_WIN32THREAD
   #ifndef DSTR_PTHREAD
      dstrerrno = status;
   #endif
   #endif

   return;
}

/* ************************************************************************* */

#ifdef DSTR_PTHREAD
/* pthread destructor for dstrerrno - FOR INTERNAL USE ONLY! */
static void _dstrfree_dstrerrno(void *dstrptr) {

   free(dstrptr);
   return;
}
#endif
