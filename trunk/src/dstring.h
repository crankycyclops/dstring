
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


#include <stdio.h>

/* HINT: it's a VERY good idea to initialize newly declared dstring_t variables
   to NULL.  If you do so, and you later accidentally pass it to a dstring
   function, the error will be immediately caught and signaled before any NULL
   pointer dereferences can occur. */



/* ************************************************************************* */



/* dstring_t is actually a "black-box" type */
typedef void * dstring_t;



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

   /* returned when an index into a dstring_t is out of bounds */
   DSTR_OUT_OF_BOUNDS = -6,

   /* when an invalid argument is passed to a DString function */
   DSTR_INVALID_ARGUMENT = -7
};



/* ************************************************************************* */



/************************\
 * allocation functions *
\************************/


/* **** dstralloc **********************************************************

   This function initializes a variable of type dstring_t by allocating
   space for the object as well as an initial buffer for string data.  If
   passed a size of 0 bytes, it will call dstrfree() to free the object and
   set it to NULL.

   Found in alloc.c

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
   invoke dstrfree() to free the object and set it to NULL.  If an
   uninitialized dstring_t is passed to dstrealloc, it will be passed to
   dstralloc instead, where it will be initialized with the specified number
   of bytes.

   Found in alloc.c

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

   Found in alloc.c

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

   Found in access.c

   *************************************************************************

   Input:
      const dstring_t (our dstring_t object)

   Output:
      A constant character pointer to the string buffer, or NULL to signal
      that the dstring_t object is uninitialized.

   ************************************************************************* */
const char * const dstrview(const dstring_t str);


/* **** dstrallocsize ******************************************************

   This function is an accessor function that returns the number of bytes
   currently allocated to the string buffer of a dstring_t object, NOT THE
   LENGTH OF THE STRING!  To find the length of the string, see dstrlen
   further below.

   Since size_t is an unsigned type, it cannot return a status code.
   However, it is also a convenient fact that an initialized dstring_t
   object will always have an allocation greater than 0.  Thus, if the
   return value was 0, it means the object was not initialized.

   Found in access.c

   *************************************************************************

   Input:
      const dstring_t (our dstring_t object)

   Output:
      > 0: The number of bytes allocated to the buffer of a dstring_t object
     == 0:  The dstring_t object was not initialized

   ************************************************************************* */
size_t dstrallocsize(const dstring_t str);


/*****************\
 * I/O functions *
\*****************/


/* **** dstrfreadl *********************************************************

   This function reads an entire line of input from FILE *fp, terminated by
   the '\n' character (the newline is included as part of the string), and
   stores it in the buffer of a dstring_t object.

   New data overwrites anything previously stored in the buffer.

   Found in io.c

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      FILE * (our input stream)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrfreadl(dstring_t dest, FILE *fp);


/* **** dstrreadl **********************************************************

   Implemented as a macro, this call wraps around dstrfreadl, using stdin
   as the input file.

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
#define dstreadl(DEST) dstrfreadl(DEST, stdin)


/* **** dstrfreadn *********************************************************

   This function reads up to one less than n characters (the last space is
   reserved for the NULL terminating character), including \n's, from FILE
   *fp and stores them in the buffer of a dstring_t object.

   If size is 0, nothing will be done and DSTR_SUCCESS will be returned.

   New data overwrites anything previously stored in the buffer.  In the
   event of a DSTR_NOMEM error, the buffer will be empty.

   Found in io.c

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      FILE (our input stream)
      int (the number of characters to read)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrfreadn(dstring_t dest, FILE *fp, int size);


/* **** dstrreadn **********************************************************

   Implemented as a macro, this wraps around dstrfreadn, using stdin as the
   input file.

   If size is 0, nothing will be done and DSTR_SUCCESS will be returned.

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      int (the number of characters to read)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
#define dstreadn(DEST, SIZE) dstrfreadn(DEST, stdin, SIZE)


/* **** dstrfcatl **********************************************************

   This function reads an entire line of input from FILE *fp, terminated by
   the '\n' character (the newline is included as part of the string), and
   appends it to the buffer of a dstring_t object, overwriting the previous
   '\n' character.

   Found in io.c

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      FILE * (our input stream)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrfcatl(dstring_t dest, FILE *fp);


/* **** dstrcatl **********************************************************

   Implemented as a macro, this call wraps around dstrfcatl, using stdin
   as the input file.

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
#define dstrcatl(DEST) dstrfcatl(DEST, stdin)


/* **** dstrfcatn **********************************************************

   This function reads up to one less than n characters (the last space is
   reserved for the NULL terminating character), including \n's, from FILE
   *fp and appends them to the buffer of a dstring_t object.

   If size is 0, nothing will be done and DSTR_SUCCESS will be returned.

   New data is appended to anything previously stored in the buffer.  In
   the event of a DSTR_NOMEM error, the buffer will be unmodified.

   Found in io.c

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      FILE (our input stream)
      int (the number of characters to read)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrfcatn(dstring_t dest, FILE *fp, int size);


/* **** dstrcatn ***********************************************************

   Implemented as a macro, this wraps around dstrcatn, using stdin as the
   input file.

   If size is 0, nothing will be done and DSTR_SUCCESS will be returned.

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      int (the number of characters to read)

   Output:
      An integer status (see enum above)

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

   Found in cstdlib.c

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)

   Output:
      The length of the string
      (BE CAREFUL! A size of 0 is an ambiguous case; since size_t is
       unsigned, we can't return a status code.  Thus, 0 could indicate a
       size of 0 OR the fact that the dstring_t object is uninitialized.)

   ************************************************************************* */
size_t dstrlen(const dstring_t str);


/* **** dstrcat ************************************************************

   This function emulates the behavior of the C standard library function
   strcat() by appending one dstring_t object to another.

   dstrcat() is safe, unlike strcat(), because the destination buffer will
   grow to accomodate the required size.

   Found in cstdlib.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const dstring_t (source)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrcat(dstring_t dest, const dstring_t src);


/* **** dstrncat ***********************************************************

   This function emulates the behavior of the C standard library function
   strncat() by appending one dstring_t object to another, up to n chars.

   Found in cstdlib.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const dstring_t (source)
      size_t (number of characters to concatenate in source)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrncat(dstring_t dest, const dstring_t src, size_t n);


/* **** dstrcatcs **********************************************************

   This function emulates the behavior of the C standard library function
   strncat() by appending an ordinary C string to a dstring_t object.

   Found in cstdlib.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const char * (source)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrcatcs(dstring_t dest, const char *src);


/* **** dstrncatcs *********************************************************

   This function emulates the behavior of the C standard library function
   strncat() by appending a C string to a dstring_t object, up to n chars.

   Found in cstdlib.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const char * (source)
      size_t (number of characters to concatenate in source)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrncatcs(dstring_t dest, const char *src, size_t n);


/* **** dstrcpy ************************************************************

   This function emulates the behavior of the C standard library function
   strcpy() by copying the contents of one initialized dstring_t object to
   another.  Both dstring_t objects must be initialized.

   Unlike strcpy(), dstrcpy() is safe, because the destination buffer will
   grow to accomodate the required size.

   Found in cstdlib.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const dstring_t (source)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrcpy(dstring_t dest, const dstring_t src);


/* **** dstrncpy ************************************************************

   This function emulates the behavior of the C standard library function
   strncpy() by copying the contents of one initialized dstring_t object to
   another, up to n characters.  Both dstring_t objects must be initialized.

   If n is greater than the size of the source string, like strncpy(), the
   remaining positions in the string will be padded with '\0' characters.

   Found in cstdlib.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const dstring_t (source)
      size_t (number of characters to copy)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrncpy(dstring_t dest, const dstring_t src, size_t n);


/*********************\
 * utility functions *
\*********************/


/* **** dstrboundscheck ****************************************************

   This function returns DSTR_SUCCESS if the specified index is within the
   bounds of the passed dstring_t object, DSTR_OUT_OF_BOUNDS if it is not
   and DSTR_INVALID_ARGUMENT if an invalid index is given (less than 0.)

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

   If the size is 0, nothing will be done and DSTR_SUCCESS will be returned.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t (string to truncate)
      size_t (number of characters to keep in the string)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrtrunc(dstring_t str, size_t size);


/* **** dstrtruncleft ******************************************************

   This function truncates a string on the left side by the specified number
   of characters.

   If the size is 0, nothing will be done and DSTR_SUCCESS will be returned.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t (string to truncate)
      size_t (number of characters to keep in the string)

   Output:
      An integer status (see enum above)

   ************************************************************************* */
int dstrtruncleft(dstring_t str, size_t size);


/* **** dstrdel ************************************************************

   This function removes a character from a dstring_t buffer at the
   specified 0-based index

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      size_t (index)

   Output:
      A status code

   ************************************************************************* */
int dstrdel(dstring_t str, size_t index);


/* **** dstrndel ***********************************************************

   This function removes n characters from a dstring_t buffer at the
   specified 0-based index.

   If n is 0, nothing will be done and DSTR_SUCCESS will be returned.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t (our dstring_t object)
      size_t (index)
      size_t (number of items to remove)

   Output:
      A status code

   ************************************************************************* */
int dstrndel(dstring_t str, size_t index, size_t n);


/* **** dstrinsertc ********************************************************

   This function inserts a single character into a dstring object at the
   specified 0-based index.  Inserting \0's is not allowed and will result
   in a return value of DSTR_INVALID_ARGUMENT if attempted.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t (destination)
      size_t (index)
      char (source)

   Output:
      A status code

   ************************************************************************* */
int dstrinsertc(dstring_t dest, size_t index, char c);


/* **** dstrinserts ********************************************************

   This function inserts one string into another at the specified 0-based
   index.

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
      A status code

   ************************************************************************* */
int dstrinserts(dstring_t dest, const dstring_t src, size_t index);


/* **** dstrinsertcs *******************************************************

   This function inserts a C string into a dstring_t object at the specified
   0-based index.

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
      A status code

   ************************************************************************* */
int dstrinsertcs(dstring_t dest, const char *src, size_t index);


/* **** dstrninserts *******************************************************

   This function inserts n characters from a dstring_t object into another
   at the specified 0-based index.  If n is larger than the number of
   characters in the source string, then all characters up to, but not
   including '\0', will be inserted.

   If size is 0, nothing will be done and DSTR_SUCCESS will be returned.

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
      A status code

   ************************************************************************* */
int dstrninserts(dstring_t dest, const dstring_t src, size_t index, size_t n);


/* **** dstrninsertcs ******************************************************

   This function inserts n characters from a C string into a dstring_t
   object at the specified 0-based index.  If n is larger than the number
   of characters in the C string, then all characters in the C string up to,
   but not including '\0', will be inserted.

   If size is 0, nothing will be done and DSTR_SUCCESS will be returned.

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
      A status code

   ************************************************************************* */
int dstrninsertcs(dstring_t dest, const char *src, size_t index, size_t n);


/* **** dstrxchg **********************************************************

   This function exchanges the character at the specified 0-based index for
   another (old character is overwritten with the new.)   \0's cannot be
   inserted into the string.  Attempting to do so will not work and will
   result in a return value of DSTR_INVALID_ARGUMENT.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t
      size_t (index)
      char (character to exchange)

   Output:
      A status code

   ************************************************************************* */
int dstrxchg(dstring_t str, size_t index, char c);


/* **** dstrgetc ***********************************************************

   This function returns the character found at the specified index in the
   passed dstring_t object.  If the index is less than 0, a return value of
   DSTR_INVALID_ARGUMENT will be returned.  If the index is out of bounds,
   a value of DSTR_OUT_OF_BOUNDS will be returned.

   The programmer should note that the index of the \0 character is
   considered out of bounds.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t
      size_t (index)

   Output:
      > 0: a character
      < 0: status code indicating some failure

   ************************************************************************* */
int dstrgetc(dstring_t str, size_t index);


/* **** dstryankc **********************************************************

   This function returns the character found at the specified index in the
   passed dstring_t object and removes it from the string.  If the index is
   less than 0, a return value of DSTR_INVALID_ARGUMENT will be returned.
   If the index is out of bounds, a value of DSTR_OUT_OF_BOUNDS will be
   returned.

   The programmer should note that the index of the \0 character is
   considered out of bounds.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t
      size_t (index)

   Output:
      > 0: a character
      < 0: status code indicating some failure

   ************************************************************************* */
int dstryankc(dstring_t str, size_t index);


/* **** dstrreplacec *******************************************************

   This function combs through an entire dstring_t object, replacing all
   instances of the character contained in oldc with the character contained
   in newc.  If there are no instances of oldc in the dstring_t object, the
   function will do nothing and will return DSTR_SUCCESS.

   Attempting to pass \0 as either oldc or newc will result in the function
   doing nothing and returning DSTR_INVALID_ARGUMENT.

   Found in utility.c

   *************************************************************************

   Input:
      dstring_t
      char (old character to change)
      char (new character to swap in place of the old)

   Output:
      A status code

   ************************************************************************* */
int dstreplacec(dstring_t str, char oldc, char newc);


/*************************\
 *  Formatting Functions  *
\**************************/


/* **** dstrpadl ***********************************************************

   This function pads the left side of a string with n amount of the
   specified fill character.

   Found in format.c

   *************************************************************************

   Input:
      dstring_t
      size_t (number of padding characters)
      char (pad character)

   Output:
      A status code

   ************************************************************************* */
int dstrpadl(dstring_t str, size_t n, char c);


/* **** dstrpadr ***********************************************************

   This function pads the right side of a string with n amount of the
   specified fill character.

   Found in format.c

   *************************************************************************

   Input:
      dstring_t
      size_t (number of padding characters)
      char (pad character)

   Output:
      A status code

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
      size_t (max width in which the non-whitespace text will be centered)

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
      size_t (max width in which the non-whitespace text will be centered)

   Output:
      A status code

   ************************************************************************* */
int dstrleft(dstring_t str, size_t len);


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


/* IMPLEMENT! */

/* Utility */
int dstreplaces(dstring_t str, const char *old, const char *new);

/* Format */
int dstrtoupper(dstring_t str);
int dstrtolower(dstring_t str);
/* more in format.c that need prototypes to be made */
