
/* ************************************************************************* *\
   * File: dstring.h                                                       *
   * Purpose:                                                              *
   *    Provides typedefs, macros and prototypes for interfacing with the  *
   *    DString library                                                    *
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

/* HINT: it's a VERY good idea to initialize newly declared dstring_t variables
   to NULL.  If you do so, and you later accidentally pass it to a dstring
   function, the error will be immediately caught and signaled before any NULL
   pointer dereferences can occur. */

/* security issue to take care of: if realloc to make string smaller,
   auto NULL-terminate the string at the cutoff point to prevent */

/* WRITE FUNCTION TO RETURN STRING FOR ERROR CODE LIKE DSTR_SUCCESS */


typedef void * dstring_t;


/****************\
 * status codes *
\****************/

enum {

   /* returned if a call is successful */
   DSTR_SUCCESS = 0,

   /* returned if an operation fails due to lack of memory */   
   DSTR_NOMEM = -1,

   /* returned if the caller passes an invalid size for the string's buffer */
   DSTR_INVALID_BUFLEN = -2,

   /* returned if an uninitialized dstring_t is passed to a function */
   DSTR_UNINITIALIZED = -3,

   /* returned if a NULL FILE pointer is passed to a function */
   DSTR_UNOPENED_FILE = -4,

   /* returned when a dstring read function encounters EOF */
   DSTR_EOF = -5
};


/************************\
 * allocation functions *
\************************/

/* if passed a size of 0 bytes, it will free the object */
int dstralloc(dstring_t *strptr, size_t bytes);

/* like dstralloc, a size of 0 bytes will free the object */
int dstrealloc(dstring_t *strptr, size_t bytes);

/* frees a dstring_t object and sets the pointer to NULL */
int dstrfree(dstring_t *strptr);


/**********************\
 * accessor functions *
\**********************/

/* returns a constant character pointer that can be used with standard library
   functions such as printf, strlen, etc.

   returns NULL if str is uninitialized */
const char *dstrview(dstring_t str);

/* returns the number of bytes currently allocated to the string stored in
   the dstring_t object

   returns -1 if str is uninitialized*/
size_t dstrallocsize(dstring_t str);


/****************\
 * IO functions *
\****************/

int dstrfreadl(dstring_t dest, FILE *fp);
int dstrreadl(dstring_t dest);

/* these two functions will have nothing in them if they return DSTR_NOMEM */

/* gets characters up to one less than size and stores them in the dstring_t */
int dstrfreadn(dstring_t dest, FILE *fp, size_t size);
int dstrreadn(dstring_t dest, size_t size);

/* all IO functions below this line are UNIMPLEMENTED! */
int dstrfcatl(dstring_t dest, FILE *fp);
int dstrcatl(dstring_t dest);
int dstrfcatn(dstring_t dest, FILE *fp, size_t size);
int dstrcatn(dstring_t dest, size_t size);


/************************\
 * conversion functions *
\************************/

int dstrtocstr(char *dest, dstring_t src, size_t size);
int cstrtodstr(dstring_t dest, const char *src);


/*********************\
 * utility functions *
\*********************/

/* truncates a string */
int dstrtrunc(dstring_t str, size_t size);

/* converts an integer error code into a readable string */
const char *dstrerrormsg(int code);
