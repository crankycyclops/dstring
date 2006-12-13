
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
   auto NULL-terminate the string at the cutoff point to prevent out of bounds
   issues when a function tries to read up to the previous NULL beyond the
   shrunken buffer */


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


/* **** dstralloc **********************************************************

   This function initializes a variable of type dstring_t by allocating
   space for the object as well as an initial buffer for string data.  If
   passed a size of 0 bytes, it will call dstrfree() to free the object and
   set it to NULL.

   *************************************************************************

   Input:
      dstring_t * (points to the object to be allocated)
      size_t (the number of bytes for the initial allocation)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstralloc(dstring_t *strptr, size_t bytes);


/* **** dstrealloc **********************************************************

   This function grows or shrinks the number of bytes allocated to the buffer
   of an initialized dstring_t.  Like dstralloc, a size of 0 bytes will
   invoke dstrfree() to free the object and set it to NULL.

   *************************************************************************

   Input:
      dstring_t * (points to the object to be allocated)
      size_t (the number of bytes for the initial allocation)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrealloc(dstring_t *strptr, size_t bytes);


/* **** dstrfree ***********************************************************

   This function frees all memory dynamically allocated to an object of type
   dstring_t and sets the variable to NULL (denoting an uninitialized
   state.)

   *************************************************************************

   Input:
      dstring_t * (points to the object to be allocated)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrfree(dstring_t *strptr);


/**********************\
 * accessor functions *
\**********************/


/* **** dstrview ***********************************************************

   This function returns a read-only constant character pointer to the
   buffer inside of a dstring_t object that can in turn be passed to
   standard library functions such as printf, strlen, etc.

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)

   Output:
      A constant character pointer to the string buffer, or NULL to signal
      that the dstring_t object is uninitialized.

   ************************************************************************* */
const char *dstrview(dstring_t str);


/* **** dstrallocsize ******************************************************

   This function is an accessor function that returns the number of bytes
   currently allocated to the string buffer of a dstring_t object (NOT THE
   LENGTH OF THE STRING!)

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)

   Output:
      The number of bytes allocated to the buffer of a dstring_t object, or
      -1 to indicate that the dstring_t object is uninitialized.

   ************************************************************************* */
size_t dstrallocsize(dstring_t str);


/****************\
 * IO functions *
\****************/


/* **** dstrfreadl *********************************************************

   This function reads an entire line of input from FILE *fp, terminated by
   the '\n' character (the newline is included as part of the string), and
   stores it in the buffer of a dstring_t object.

   New data overwrites anything previously stored in the buffer.

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      FILE * (our input stream)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrfreadl(dstring_t dest, FILE *fp);


/* **** dstrreadl **********************************************************

   This function is a wrapper for dstrfreadl that uses stdin as the input
   stream.  The exact same documentation applies.

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrreadl(dstring_t dest);


/* **** dstrfreadn *********************************************************

   This function reads up to one less than n characters (the last space is
   reserved for the NULL terminating character), including \n's, from FILE
   *fp and stores them in the buffer of a dstring_t object.

   New data overwrites anything previously stored in the buffer.  In the
   event of a DSTR_NOMEM error, the buffer will be empty.

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      FILE (our input stream)
      size_t (the number of characters to read)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrfreadn(dstring_t dest, FILE *fp, size_t size);


/* **** dstrreadn **********************************************************

   This function is a wrapper for dstrfreadn that uses stdin as the input
   buffer.  The same documentation applies.

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      size_t (the number of characters to read)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrreadn(dstring_t dest, size_t size);


/* -- all IO functions below this line are UNIMPLEMENTED! -- */
int dstrfcatl(dstring_t dest, FILE *fp);
int dstrcatl(dstring_t dest);
int dstrfcatn(dstring_t dest, FILE *fp, size_t size);
int dstrcatn(dstring_t dest, size_t size);


/************************\
 * conversion functions *
\************************/


/* **** dstrtocstr **********************************************************

   This function copies the string stored in a dstring_t buffer into an
   ordinary character array, up to one less than size characters (the last
   space being reserved for \0).

   *************************************************************************

   Input:
      char * (destination string)
      dstring_t (source string)
      size_t (number of characters to copy minus one)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrtocstr(char *dest, dstring_t src, size_t size);


/* **** cstrtodstr **********************************************************

   This function copies the string stored in an ordinary character array into
   the buffer of a dstring_t object.

   The source string overwrites any previous data stored in the dstring_t
   buffer.

   *************************************************************************

   Input:
      dstring_t (destination string)
      char * (source string)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int cstrtodstr(dstring_t dest, const char *src);


/*********************\
 * utility functions *
\*********************/

/* truncates a string -- currently UNIMPLEMENTED! -- */
int dstrtrunc(dstring_t str, size_t size);


/* **** dstrerrormesg ******************************************************

   This function takes as input an integer status (see enum above) and
   returns a constant read-only pointer to a string describing the error.

   *************************************************************************

   Input:
      int (status code)

   Output:
      A read-only string describing the status code

   ************************************************************************* */
const char *dstrerrormsg(int code);
