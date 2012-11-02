/* ************************************************************************* *\
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

#ifndef DSTRING_H_INCLUDED
#define DSTRING_H_INCLUDED

#include <stdio.h>
#include <stdarg.h>

/* HINT: it's a VERY good idea to set newly declared and uninitialized
   dstring_t variables to NULL.  If you do so, and you later accidentally
   pass it to a dstring function, the error will be immediately caught and
   signaled before any NULL pointer dereferences can occur. */



/* ************************************************************************* */


/* the library's current version */
#define DSTR_VER_CURRENT  1
#define DSTR_VER_REVISION 0
#define DSTR_VER_AGE      0


/* ************************************************************************* */


/* does the build environment support IEEE-754 floating point arithmetic? */
#ifdef __STDC_VERSION__
   #if __STDC_VERSION__ >= 199901
      #define DSTR_IEEE-754
   #endif
#endif


/* ************************************************************************* */


/* dstring_t is actually a "black-box" type */
typedef void * dstring_t;



/* ************************************************************************* */



/* dstrerrno is set by dstring functions to indicate success or error status */
#ifdef DSTR_WIN32THREAD
   #include <windows.h>
   /* declare win32 thread macro for dstrerrno value */
#endif
 
#ifdef DSTR_PTHREAD
   #include <pthread.h>
   extern pthread_key_t _dstrerrno_key;
   #define dstrerrno *(int *)pthread_getspecific(_dstrerrno_key)
#endif

#ifndef DSTR_WIN32THREAD
#ifndef DSTR_PTHREAD
   extern int dstrerrno;
#endif
#endif



/* ************************************************************************* */



/****************\
 * status codes *
\****************/

enum STATUS_CODES {

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
   DSTR_EOF = -5,

   /* returned if there was some read error for a file other than EOF */
   DSTR_FILE_ERROR = -6,

   /* returned when an index into a dstring_t is out of bounds */
   DSTR_OUT_OF_BOUNDS = -7,

   /* when an invalid argument is passed to a DString function */
   DSTR_INVALID_ARGUMENT = -8,

   /* when a NULL char * is passed to a function that expects a string */
   DSTR_NULL_CPTR = -9,

   /* when a dstring_t object is empty, but a non-empty string is expected */
   DSTR_EMPTY_STRING = -10
};



/* ************************************************************************* */



/* this is the default number of bytes to allocate to a dstring_t object */
#define DSTRALLOC_DEFAULT_SIZE 20



/************************\
 * allocation functions *
\************************/


/* **** dstralloc **********************************************************

   This function initializes a variable of type dstring_t by allocating
   space for the object as well as an initial buffer with a default size.
   If passed a size of 0 bytes, it will call dstrfree() to free the object
   and set it to NULL.

   In addition to the return value, dstrerrno will be set to indicate
   success or failure.

   Found in alloc.c

   *************************************************************************

   Input:
      dstring_t * (points to the object to be allocated)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstralloc(dstring_t *dstrptr);


/* **** dstrnalloc *********************************************************

   This function initializes a variable of type dstring_t by allocating
   space for the object as well as an initial buffer for string data of
   size 'bytes.'  If passed a size of 0 bytes, it will call dstrfree() to
   free the object and set it to NULL.

   In addition to the return value, dstrerrno will be set to indicate
   success or failure.

   Found in alloc.c

   *************************************************************************

   Input:
      dstring_t * (points to the object to be allocated)
      size_t (the number of bytes for the initial allocation)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrnalloc(dstring_t *dstrptr, size_t bytes);


/* **** dstrealloc **********************************************************

   This function grows or shrinks the number of bytes allocated to the buffer
   of an initialized dstring_t.  Like dstralloc, a size of 0 bytes will
   invoke dstrfree() to free the object and set it to NULL.  If an
   uninitialized dstring_t is passed to dstrealloc, it will be passed to
   dstralloc instead, where it will be initialized with the specified number
   of bytes.

   In addition to the return value, dstrerrno will be set to indicate
   success or failure.

   Found in alloc.c

   *************************************************************************

   Input:
      dstring_t * (points to the object to be allocated)
      size_t (the number of bytes for the initial allocation)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrealloc(dstring_t *dstrptr, size_t bytes);


/* **** dstrfree ***********************************************************

   This function frees all memory dynamically allocated to an object of type
   dstring_t and sets the variable to NULL (denoting an uninitialized
   state.)

   In addition to the return value, dstrerrno will be set to indicate
   success or failure.

   Found in alloc.c

   *************************************************************************

   Input:
      dstring_t * (points to the object to be allocated)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrfree(dstring_t *dstrptr);


/**********************\
 * accessor functions *
\**********************/


/* **** dstrview ***********************************************************

   This function returns a read-only constant character pointer to the
   buffer inside of a dstring_t object that can in turn be passed to
   standard library functions such as printf, strlen, etc.

   dstrerrno will be set to indicate success or failure.

   Found in access.c

   *************************************************************************

   Input:
      const dstring_t (our dstring_t object)

   Output:
      A constant character pointer to the string buffer, or NULL to signal
      error (check dstrerrno)

   ************************************************************************* */
const char * const dstrview(const dstring_t dstr);


/* **** dstrallocsize ******************************************************

   This function is an accessor function that returns the number of bytes
   currently allocated to the string buffer of a dstring_t object, NOT THE
   LENGTH OF THE STRING!  To find the length of the string, see dstrlen
   further below.

   Since size_t is an unsigned type, it cannot return a status code.
   However, it is also a convenient fact that an initialized dstring_t
   object will always have an allocation greater than 0.  Thus, if the
   return value was 0, it means the object was not initialized.

   Also, dstrerrno will be set to indicate success or failure.

   Found in access.c

   *************************************************************************

   Input:
      const dstring_t (our dstring_t object)

   Output:
      > 0: The number of bytes allocated to the buffer of a dstring_t object
     == 0:  error (check dstrerrno)

   ************************************************************************* */
size_t dstrallocsize(const dstring_t dstr);


/*****************\
 * I/O functions *
\*****************/


/* **** dstrfreadl *********************************************************

   This function reads an entire line of input from FILE *fp, terminated by
   the '\n' character (the newline is included as part of the string), and
   stores it in the buffer of a dstring_t object.

   New data overwrites anything previously stored in the buffer.  If EOF is
   encountered before any data can be read, the previous string will remain
   untouched.

   dstrerrno will be set to indicate success or failure.

   Found in io.c

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      FILE * (our input stream)

   Output:
      number of characters successfully read (0 on EOF or error - check
      dstrerrno)

   ************************************************************************* */
size_t dstrfreadl(dstring_t dest, FILE *fp);


/* **** dstrreadl **********************************************************

   Implemented as a macro, this call wraps around dstrfreadl, using stdin
   as the input file.

   dstrerrno will be set to indicate success or failure.

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)

   Output:
      number of characters successfully read (0 on EOF or error - check
      dstrerrno)

   ************************************************************************* */
#define dstreadl(DEST) dstrfreadl(DEST, stdin)


/* **** dstrfreadn *********************************************************

   This function reads up to one less than n characters (this does NOT
   include the NULL terminating character), including \n's, from FILE *fp
   and stores them in the buffer of a dstring_t object.

   If size is 0, nothing will be done and dstrerrno will be set to
   DSTR_SUCCESS.

   New data overwrites anything previously stored in the buffer.  In the
   event of a DSTR_NOMEM error, whatever was contained in the buffer
   before the function call is preserved.  If EOF is encountered before
   any data can be read, the contents of dest will be preserved.  Otherwise,
   it will be overwritten by whatever was successfully read from fp, even
   when EOF is encountered.

   dstrerrno will be set to indicate success or failure.

   Found in io.c

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      FILE (our input stream)
      size_t (the number of characters to read)

   Output:
      Number of characters successfully read (0 if size = 0, or on EOF or
      error - check dstrerrno)

   ************************************************************************* */
size_t dstrfreadn(dstring_t dest, FILE *fp, size_t n);


/* **** dstreadn **********************************************************

   Implemented as a macro, this wraps around dstrfreadn, using stdin as the
   input file.

   If size is 0, nothing will be done and DSTR_SUCCESS will be returned.

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      size_t (the number of characters to read)

   Output:
      Number of characters successfully read (0 if size = 0, or on EOF or
      error - check dstrerrno)

   ************************************************************************* */
#define dstreadn(DEST, SIZE) dstrfreadn(DEST, stdin, SIZE)


/* **** dstrfcatl **********************************************************

   This function reads an entire line of input from FILE *fp, terminated by
   the '\n' character (the newline is included as part of the string), and
   appends it to the buffer of a dstring_t object (does NOT overwrite the
   previous '\n' character if there is one.)

   Found in io.c

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      FILE * (our input stream)

   Output:
      Number of characters successfully read (0 on error or EOF - check
      dstrerrno)

   ************************************************************************* */
size_t dstrfcatl(dstring_t dest, FILE *fp);


/* **** dstrcatl **********************************************************

   Implemented as a macro, this call wraps around dstrfcatl, using stdin
   as the input file.

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)

   Output:
      Number of characters successfully read (0 on error or EOF - check
      dstrerrno)

   ************************************************************************* */
#define dstrcatl(DEST) dstrfcatl(DEST, stdin)


/* **** dstrfcatn **********************************************************

   This function reads up to one less than n characters (not including the
   NULL terminating character), including \n's, from FILE *fp and appends
   them to the buffer of a dstring_t object.

   If size is 0, nothing will be done and dstrerrno will be set to
   DSTR_SUCCESS.

   New data is appended to anything previously stored in the buffer.  In
   the event of a DSTR_NOMEM error, the buffer will be unmodified.

   Found in io.c

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      FILE (our input stream)
      size_t (the number of characters to read)

   Output:
      Number of characters successfully read (0 on error or EOF - check
      dstrerrno)

   ************************************************************************* */
size_t dstrfcatn(dstring_t dest, FILE *fp, size_t n);


/* **** dstrcatn ***********************************************************

   Implemented as a macro, this wraps around dstrcatn, using stdin as the
   input file.

   If size is 0, nothing will be done and DSTR_SUCCESS will be returned.

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      size_t (the number of characters to read)

   Output:
      Number of characters successfully read (0 on error or EOF - check
      dstrerrno)

   ************************************************************************* */
#define dstrcatn(DEST, SIZE) dstrfcatn(DEST, stdin, SIZE)


/************************\
 * conversion functions *
\************************/


/* **** dstrtocstr **********************************************************

   This function copies the string stored in a dstring_t buffer into an
   ordinary character array, up to one less than size characters (the last
   space being reserved for \0).

   If size is 0, an empty string (just a single '\0') will be created and
   DSTR_SUCCESS will be returned.

   dstrerrno will be set to indicate success or failure, matching the return
   value.

   If dest is a NULL pointer, dstrerrno will be set to DSTR_NULL_CPTR.

   Found in convert.c

   *************************************************************************

   Input:
      char * (destination string)
      const dstring_t (source string)
      size_t (number of characters to copy minus one)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrtocstr(char *dest, const dstring_t src, size_t size);


/* **** cstrtodstr **********************************************************

   This function copies the string stored in an ordinary character array into
   the buffer of a dstring_t object.

   The source string overwrites any previous data stored in the dstring_t
   buffer.

   dstrerrno will be set to indicate success or failure, matching the return
   value.

   If src is a NULL pointer, dstrerrno will be set to DSTR_NULL_CPTR.

   Found in convert.c

   *************************************************************************

   Input:
      dstring_t (destination string)
      const char * (source string)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int cstrtodstr(dstring_t dest, const char *src);


/**************************\
 * C Library Replacements *
\**************************/


/* **** dstrlen ************************************************************

   This function takes as an argument a dstring_t object and returns the
   length of the string.  This is the dstring_t equivalent of strlen in the
   standard library.

   dstrerrno will be set to indicate success or type of error.

   Found in cstdlib.c

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)

   Output:
      The length of the string
      (BE CAREFUL! A size of 0 is an ambiguous case; since size_t is
       unsigned, we can't return a status code.  Thus, 0 could indicate a
       size of 0 OR the fact that the dstring_t object is uninitialized - 
       on a return of 0, checking dstrerrno is a good idea ;)

   ************************************************************************* */
size_t dstrlen(const dstring_t str);


/* **** dstrcat ************************************************************

   This function emulates the behavior of the C standard library function
   strcat() by appending one dstring_t object to another.

   dstrcat() is safe, unlike strcat(), because the destination buffer will
   grow to accomodate the required size.

   dstrerrno will be set to indicate success or the type of error.

   Found in cstdlib.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const dstring_t (source)

   Output:
      number of characters concatenated to src

   ************************************************************************* */
int dstrcat(dstring_t dest, const dstring_t src);


/* **** dstrncat ***********************************************************

   This function emulates the behavior of the C standard library function
   strncat() by appending one dstring_t object to another, up to n chars.

   dstrerrno will be set to indicate success or the type of error.

   Found in cstdlib.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const dstring_t (source)
      size_t (number of characters to concatenate in source)

   Output:
      number of characters appended to dest (could be less than n if src has
      less than n characters)

   ************************************************************************* */
int dstrncat(dstring_t dest, const dstring_t src, size_t n);


/* **** dstrcatcs **********************************************************

   This function emulates the behavior of the C standard library function
   strcat() by appending an ordinary C string to a dstring_t object.

   dstrerrno will be set to indicate success or the type of error.

   If src is a NULL pointer, dstrerrno will be set to DSTR_NULL_CPTR.

   Found in cstdlib.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const char * (source)

   Output:
      number of characters successfully appended to dest

   ************************************************************************* */
int dstrcatcs(dstring_t dest, const char *src);


/* **** dstrncatcs *********************************************************

   This function emulates the behavior of the C standard library function
   strncat() by appending a C string to a dstring_t object, up to n chars.

   dstrerrno will be set to indicate success or the type of error.

   If src is a NULL pointer, dstrerrno will be set to DSTR_NULL_CPTR.

   Found in cstdlib.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const char * (source)
      size_t (number of characters to concatenate in source)

   Output:
      number of characters successfully appended to dest (up to one less
      than n)

   ************************************************************************* */
int dstrncatcs(dstring_t dest, const char *src, size_t n);


/* **** dstrcpy ************************************************************

   This function emulates the behavior of the C standard library function
   strcpy() by copying the contents of one initialized dstring_t object to
   another.  Both dstring_t objects must be initialized.

   Unlike strcpy(), dstrcpy() is safe, because the destination buffer will
   grow to accomodate the required size.

   dstrerrno will be set to indicate status or the type of error.

   Found in cstdlib.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const dstring_t (source)

   Output:
      number of characters copied to dest

   ************************************************************************* */
int dstrcpy(dstring_t dest, const dstring_t src);


/* **** dstrncpy ************************************************************

   This function emulates the behavior of the C standard library function
   strncpy() by copying the contents of one initialized dstring_t object to
   another, up to n characters.  Both dstring_t objects must be initialized.

   dstrerrno will be set to indicate status or the type of error.

   Found in cstdlib.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const dstring_t (source)
      size_t (number of characters to copy)

   Output:
      number of characters successfully copied to dest

   ************************************************************************* */
int dstrncpy(dstring_t dest, const dstring_t src, size_t n);


/* **** dstrcmp ************************************************************

   A replacement for strcmp() that compares two dstring_t objects.

   Found in cstdlib.c

   *************************************************************************

   Input:
      dstring_t (first string)
      dstring_t (second string)

   Output:
      What the output of strcmp() would be on two equivalent C-strings.

   ************************************************************************* */
int dstrcmp(dstring_t str1, dstring_t str2);


/* **** dstrcmpcs **********************************************************

   A replacement for strcmp() that compares a dstring_t object and a C-style
   string.

   Found in cstdlib.c

   *************************************************************************

   Input:
      dstring_t (first string)
      const char * (second string)

   Output:
      What the output of strcmp() would be on two equivalent C-strings.

   ************************************************************************* */
int dstrcmpcs(dstring_t str1, const char *str2);


/* **** dstrsprintf ********************************************************

   An implementation of sprintf() that supports writing to dstring_t
   objects.  This function supports every aspect of the standard library
   function it aims to replace, and like its counterpart, returns the
   number of characters written to the dstring_t object on success, or
   a negative value on error (in the case of an error, the return value
   will match the value of dstrerrno.)

   dstrerrno will be set to indicate status or the type of error.

   Found in sprintf.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const char * (format string)
      possible variable list of arguments

   Output:
      number of characters successfully copied to dest or a status code
      on error

   ************************************************************************* */
int dstrsprintf(dstring_t str, const char *format, ...);


/* **** dstrvsprintf *******************************************************

   An implementation of vsprintf() that supports writing to dstring_t
   objects.  This function supports every aspect of the standard library
   function it aims to replace, and like its counterpart, returns the
   number of characters written to the dstring_t object on success, or
   a negative value on error (in the case of an error, the return value
   will match the value of dstrerrno.)

   dstrerrno will be set to indicate status or the type of error.

   Found in sprintf.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const char * (format string)
      va_list containing possible additional arguments

   Output:
      number of characters successfully copied to dest or a status code
      on error

   ************************************************************************* */
int dstrvsprintf(dstring_t str, const char *format, va_list args);


/*********************\
 * utility functions *
\*********************/


/* **** dstrboundscheck ****************************************************

   This function returns DSTR_SUCCESS if the specified index is within the
   bounds of the passed dstring_t object, DSTR_OUT_OF_BOUNDS if it is not
   and DSTR_INVALID_ARGUMENT if an invalid index is given (less than 0.)

   dstrerrno will be set to the return value of this function.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t (string to check)
      size_t (index)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrboundscheck(dstring_t str, size_t index);


/* **** dstrtrunc **********************************************************

   This function truncates a string down to the specified number of
   characters.

   dstrerrno will be set to indicate success or the type of error.

   If the size passed is greater than or equal to the length of the string
   already in the buffer, nothing will change, and dstrerrno will be set to
   DSTR_SUCCESS.

   0 is a valid size, and if passed will result in an empty (NULL) string.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t (string to truncate)
      size_t (number of characters to keep in the string)

   Output:
      length of the truncated string

   ************************************************************************* */
int dstrtrunc(dstring_t str, size_t size);


/* **** dstrtrimleft *******************************************************

   This function trims n characters off the left of a string.

   If n is 0, nothing will be done and dstrerrno will be set to
   DSTR_SUCCESS.  If n is greater than the length of the string, the string
   will become an empty (NULL) string.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t (string to truncate)
      size_t (number of characters to keep in the string)

   Output:
      size of the trimmed string

   ************************************************************************* */
int dstrtrimleft(dstring_t str, size_t n);


/* **** dstrdel ************************************************************

   This function removes a character from a dstring_t buffer at the
   specified 0-based index.  Note that removing the terminating '\0' is
   not allowed and will result in dstrerrno being set to
   DSTR_INVALID_ARGUMENT.

   dstrerrno will be set to indicate success or type of error.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      size_t (index)

   Output:
      < 0: an error occured (check dstrerrno)
      > 0: the character that was removed

   ************************************************************************* */
int dstrdel(dstring_t str, size_t index);


/* **** dstrndel ***********************************************************

   This function removes n characters from a dstring_t buffer, starting at
   the specified 0-based index.

   dstrerrno will be set to indicate success or type of error.

   If n is 0, nothing will be done and dstrerrno will be set to
   DSTR_SUCCESS.  If n is greater than the number of characters beyond the
   specified index, everything at the index and beyond will be deleted from
   the string.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      size_t (index)
      size_t (number of items to remove)

   Output:
      length of the new string

   ************************************************************************* */
int dstrndel(dstring_t str, size_t index, size_t n);


/* **** dstrinsertc ********************************************************

   This function inserts a single character into a dstring object at the
   specified 0-based index.  Inserting \0's is not allowed and will result
   in dstrerrno being set to DSTR_INVALID_ARGUMENT if attempted.  The user
   is allowed to insert at one index beyond the last character in the string
   (index = dstrlen(str)) in order to append a character to the end.

   dstrerrno will be set to indicate success or error type.  This function
   will also return the same status code.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t (destination)
      size_t (index)
      char (source)

   Output:
      A status code (see enum above)

   ************************************************************************* */
int dstrinsertc(dstring_t dest, size_t index, char c);


/* **** dstrinserts ********************************************************

   This function inserts one string into another at the specified 0-based
   index.

   dstrerrno will be set to indicate success or the type of error.

   WARNING: It is the programmer's responsibility to delete any \n
   characters in the source string that would cause undesired line breaks
   if inserted into the destination string!

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const dstring_t (source)
      size_t (index)

   Output:
      the length of the resulting string

   ************************************************************************* */
int dstrinserts(dstring_t dest, const dstring_t src, size_t index);


/* **** dstrinsertcs *******************************************************

   This function inserts a C string into a dstring_t object at the specified
   0-based index.

   dstrerrno will be set to indicate success or the type of error.

   If src is a NULL pointer, dstrerrno will be set to DSTR_NULL_CPTR.

   WARNING: It is the programmer's responsibility to delete any \n
   characters in the source string that would cause undesired line breaks
   if inserted into the destination string!

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const char * (source)
      size_t (index)

   Output:
      the length of the resulting string

   ************************************************************************* */
int dstrinsertcs(dstring_t dest, const char *src, size_t index);


/* **** dstrninserts *******************************************************

   This function inserts n characters from a dstring_t object into another
   at the specified 0-based index.  If n is larger than the number of
   characters in the source string, then all characters up to, but not
   including '\0', will be inserted.

   dstrerrno will be set to indicate success or type of error.

   If n is 0, nothing will be done and dstrerrno will be set to DSTR_SUCCESS.

   WARNING: It is the programmer's responsibility to delete any \n
   characters in the source string that would cause undesired line breaks
   if inserted into the destination string!

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const dstring_t (source)
      size_t (index)
      size_t (number of characters to insert)

   Output:
      length of the resulting string

   ************************************************************************* */
int dstrninserts(dstring_t dest, const dstring_t src, size_t index, size_t n);


/* **** dstrninsertcs ******************************************************

   This function inserts n characters from a C string into a dstring_t
   object at the specified 0-based index.  If n is larger than the number
   of characters in the C string, then all characters in the C string up to,
   but not including '\0', will be inserted.


   dstrerrno will be set to indicate success or type of error.

   If n is 0, nothing will be done and dstrerrno will be set to DSTR_SUCCESS.

   If src is a NULL pointer, dstrerrno will be set to DSTR_NULL_CPTR.

   WARNING: It is the programmer's responsibility to delete any \n
   characters in the source string that would cause undesired line breaks
   if inserted into the destination string!

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const char * (source)
      size_t (index)
      size_t (number of characters to insert)

   Output:
      length of the resulting string

   ************************************************************************* */
int dstrninsertcs(dstring_t dest, const char *src, size_t index, size_t n);


/* **** dstrxchg **********************************************************

   This function exchanges the character at the specified 0-based index for
   another (old character is overwritten with the new.)   \0's cannot be
   inserted into the string.  Attempting to do so will not work and will
   result in a return value of DSTR_INVALID_ARGUMENT.

   dstrerrno will be set to indicate success or type of error.  The return
   value of this function will also be the same status code.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t
      size_t (index)
      char (character to exchange)

   Output:
      a status code (see enum above)

   ************************************************************************* */
int dstrxchg(dstring_t str, size_t index, char c);


/* **** dstrgetc ***********************************************************

   This function returns the character found at the specified index in the
   passed dstring_t object.  If the index is less than 0, or if the index
   is out of bounds, '\0' will be returned to indicate failure.

   dstrerrno will be set to indicate success or type of error.

   The programmer should note that the index of the '\0' character is
   considered out of bounds.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t
      size_t (index)

   Output:
      Not NULL: a character
          NULL: an error occured (dstrerrno will be set)

   ************************************************************************* */
char dstrgetc(dstring_t str, size_t index);


/* **** dstrpopc ***********************************************************

   This function returns the last character in the string, afterwhich it
   is removed.  This behavior treats the string like a stack, where the
   last value is popped from the top.  If the string is empty, '\0' will be
   returned.

   dstrerrno will be set to indicate success or type of error.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t

   Output:
      Not NULL: a character
          NULL: there was an error (dstrerrno will be set)

   ************************************************************************* */
char dstrpopc(dstring_t str);


/* **** dstrdequeuec *******************************************************

   This function returns the first character in the string, afterwhich it
   is removed.  This behavior treats the string like a queue, where the
   first value is "dequeued."  If the string is empty, '\0' will be
   returned.

   dstrerrno will be set to indicate success or type of error.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t

   Output:
      Not NULL: a character
          NULL: there was an error (dstrerrno will be set)

   ************************************************************************* */
char dstrdequeuec(dstring_t str);


/* **** dstrreplacec *******************************************************

   This function combs through an entire dstring_t object, replacing all
   instances of the character contained in oldc with the character contained
   in newc.  If there are no instances of oldc in the dstring_t object, the
   function will return 0 and dstrerrno will be set to DSTR_SUCCESS.

   dstrerrno will be set to indicate success or type of error.

   Attempting to pass \0 as either oldc or newc will result in the function
   doing nothing and dstrerrno being set to DSTR_INVALID_ARGUMENT.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t
      char (old character to change)
      char (new character to swap in place of the old)

   Output:
      number of characters replaced

   ************************************************************************* */
int dstreplacec(dstring_t str, char oldc, char newc);


/* **** dstrreplaces *******************************************************

   This function combs through an entire dstring_t object, replacing all
   instances of the substring contained in olds with the substring contained
   in news.  If there are no instances of olds in the dstring_t object, the
   function will return 0 and dstrerrno will be set to DSTR_SUCCESS.

   dstrerrno will be set to indicate success or type of error.

   If olds is a NULL pointer, dstrerrno will be set to DSTR_NULL_CPTR and
   a value of 0 will be returned.

   If news is set to NULL, or if it points to an empty string ("\0"), all
   instances of olds will be removed from the dstring_t object.  This
   functionality is taken advantage of by dstremoves(), which is actually
   a macro that wraps around this function.

   Passing a 0-length string as news is legal and will simply result in the
   removal of all instances of olds from the dstring_t object. Passing a
   0-length string as olds is illegal, and will result in the function
   returning 0 and dstrerrno being set to DSTR_INVALID_ARGUMENT.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t
      const char * (old substring)
      const char * (new char (new substring)

   Output:
      number of substrings replaced

   ************************************************************************* */
int dstreplaces(dstring_t str, const char *olds, const char *news);

#define dstremoves(STR, SUBSTR) dstreplaces(STR, SUBSTR, "\0")


/*************************\
 *  Formatting Functions  *
\**************************/


/* **** dstrpadl ***********************************************************

   This function pads the left side of a string with n amount of the
   specified fill character.

   dstrerrno will be set to indicate success or the type of error.

   Found in format.c

   *************************************************************************

   Input:
      dstring_t
      size_t (number of padding characters)
      char (pad character)

   Output:
      length of the resulting string

   ************************************************************************* */
int dstrpadl(dstring_t str, size_t n, char c);


/* **** dstrpadr ***********************************************************

   This function pads the right side of a string with n amount of the
   specified fill character.

   dstrerrno will be set to indicate success or the type of error.

   Found in format.c

   *************************************************************************

   Input:
      dstring_t
      size_t (number of padding characters)
      char (pad character)

   Output:
      length of the resulting string

   ************************************************************************* */
int dstrpadr(dstring_t str, size_t n, char c);



/* **** dstrcenter *********************************************************

   This function will center justify non-whitespace text assuming a max
   width of len and will fill the surrounding whitespace with space
   characters.

   If len is less than the number of characters in the string, the function
   will do nothing and will return DSTR_SUCCESS.

   If a single string contains more than one line (more than a single \n),
   each one will be center justified.

   Found in format.c

   *************************************************************************

   Input:
      dstring_t
      size_t (max width in which the non-whitespace text will be centered)

   Output:
      A status code

   ************************************************************************* */
int dstrcenter(dstring_t str, size_t len);


/* **** dstrright **********************************************************

   This function will right justify non-whitespace text assuming a max
   width of len and will fill the whitespace on the left with spaces.

   If len is less than the number of characters in the string, the function
   will do nothing and will return DSTR_SUCCESS.

   If a single string contains more than one line (more than a single \n),
   each one will be right justified.

   Found in format.c

   *************************************************************************

   Input:
      dstring_t
      size_t (max width in which the non-whitespace text will be justified)

   Output:
      A status code

   ************************************************************************* */
int dstrright(dstring_t str, size_t len);


/* **** dstrleft ***********************************************************

   This function will left justify non-whitespace text assuming a max width
   of len and will fill the whitespace on the right with spaces.

   If len is less than the number of characters in the string, the function
   will do nothing and will return DSTR_SUCCESS.

   If a single string contains more than one line (more than a single \n),
   each one will be left justified.

   Found in format.c

   *************************************************************************

   Input:
      dstring_t
      size_t (max width in which the non-whitespace text will be justified)

   Output:
      A status code

   ************************************************************************* */
int dstrleft(dstring_t str, size_t len);


/* **** dstrtoupper ********************************************************

   This function will convert all lowercase alphabetic characters in a
   string into uppercase letters, starting at the specified index.

   An index of 0 will tell the function to convert the entire string.

   Found in format.c

   *************************************************************************

   Input:
      dstring_t
      size_t (index)

   Output:
      A status code

   ************************************************************************* */
int dstrtoupper(dstring_t str, size_t index);


/* **** dstrtolower ********************************************************

   This function will convert all uppercase alphabetic characters in a
   string into lowercase letters, starting at the specified index.

   An index of 0 will tell the function to convert the entire string.

   Found in format.c

   *************************************************************************

   Input:
      dstring_t
      size_t (index)

   Output:
      A status code

   ************************************************************************* */
int dstrtolower(dstring_t str, size_t index);


/* **** dstrtoupper ********************************************************

   This function will convert n lowercase alphabetic characters in a
   string into uppercasecase letters, starting at the specified index.

   If n is greater than the number of characters at and beyond the specified
   index, everything from the index to the end of the string will be
   converted.

   Found in format.c

   *************************************************************************

   Input:
      dstring_t
      size_t (index)

   Output:
      A status code

   ************************************************************************* */
int dstrntoupper(dstring_t str, size_t index, size_t n);


/* **** dstrtolower ********************************************************

   This function will convert n uppercase alphabetic characters in a
   string into lowercasecasecase letters, starting at the specified index.

   If n is greater than the number of characters at and beyond the specified
   index, everything from the index to the end of the string will be
   converted.

   Found in format.c

   *************************************************************************

   Input:
      dstring_t
      size_t (index)

   Output:
      A status code

   ************************************************************************* */
int dstrntolower(dstring_t str, size_t index, size_t n);


/*********************\
 *  Misc. Functions  *
\*********************/


/* **** dstrerrormesg ******************************************************

   This function takes as input an integer status (see enum above) and
   returns a constant read-only pointer to a string describing the error.

   Found in dstring.c

   *************************************************************************

   Input:
      int (status code)

   Output:
      A read-only string describing the status code

   ************************************************************************* */
const char * const dstrerrormsg(int code);


/* **** dstrbuildinfo ******************************************************

   This function takes as input a pointer to an initialized dstring_t
   object and fills it with a string (which may contain multiple lines)
   containing build information pertaining to the dstring library.

   dstrerrno will be set to indicate success or the nature of the failure.
   In the event of an error, the contents of the string is undefined.

   Found in dstring.c

   *************************************************************************

   Input:
      dstring_t * (pointer to our dstring_t object)

   Output:
      status code (see enum above)

   ************************************************************************* */
int dstrbuildinfo(dstring_t str);


/* IMPLEMENT! */
/* Format */

/* fills in n characters starting at a specified index */
int dstrfill(dstring_t str, size_t index, size_t n, char c); 
/* more in format.c that need prototypes to be made */

#endif
