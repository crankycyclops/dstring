
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

/* note that it's a good idea to initialize newly declared dstring_t variables
   to NULL.  If you do so, and you later accidentally pass it to a dstring
   function, the error will be immediately caught and signaled before any NULL
   pointer dereferences can occur. */


typedef void * dstring_t;


/****************\
 * status codes *
\****************/

enum {
   DSTR_SUCCESS = 0, DSTR_NOMEM = -1, DSTR_INVALID_BUFLEN = -2,
   DSTR_UNINITIALIZED = -3, DSTR_UNOPENED_FILE = -4, DSTR_EOF = -5
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

int dfreadl(dstring_t dest, FILE *fp);
int dreadl(dstring_t dest);
int dfreadn(dstring_t dest, FILE *fp, size_t size);
int dreadn(dstring_t dest, size_t size);

int dfcatl(dstring_t dest, FILE *fp);
int dcatl(dstring_t dest);
int dfcatn(dstring_t dest, FILE *fp, size_t size);
int dcatn(dstring_t dest, size_t size);


/************************\
 * conversion functions *
\************************/

int dstrtocstr(char *dest, dstring_t src, size_t size);
int cstrtodstr(dstring_t dest, const char *src);
