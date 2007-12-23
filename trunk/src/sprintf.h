
/* ************************************************************************* *\
   * File: sprintf.h                                                       *
   * Purpose:                                                              *
   *    Header file for dstrsprintf implementation.                        *
   *                                                                       *
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


#ifndef SPRINTF_H_INCLUDED
#define SPRINTF_H_INCLUDED

#include "dstring.h"


struct specifier {
   long int format;      /* conversion flags */
        int field;       /* field width */
        int precision;   /* precision */
};

enum {
   UPPERCASE, LOWERCASE
};

/* possible specifiers, flags and modifiers in character form */
#define FLAGSC        " -+0#"
#define MODIFIERSC    "hlL"
#define CONVERSIONSC  "dioxXucsSfeEgGpn"

/* conversion specifiers */
#define SIGNED_INT          0x00000001
#define OCTAL               0x00000002
#define HEX_LOWERCASE       0x00000004
#define HEX_UPPERCASE       0x00000008
#define UNSIGNED_INT        0x00000010
#define CHAR                0x00000020
#define C_STRING            0x00000040
#define DSTRING             0x00000080
#define FLOAT               0x00000100
#define FLOAT_EXP_LOWER     0x00000200
#define FLOAT_EXP_UPPER     0x00000400
#define FLOAT_G_LOWER       0x00000800
#define FLOAT_G_UPPER       0x00001000
#define POINTER             0x00002000
#define NUM_CHARS           0x00004000
#define PERCENT             0x00008000

/* flags */
#define FLAG_SPACE          0x00010000
#define FLAG_MINUS          0x00020000
#define FLAG_PLUS           0x00040000
#define FLAG_0              0x00080000
#define FLAG_POUND          0x00100000
#define FIELDWIDTH          0x00200000
#define PRECISION           0x00400000

/* modifiers */
#define SHORT               0x00800000
#define LONG                0x01000000
#define LONG_DOUBLE         0x02000000

/* convenient bitmasks */
#define CONVERSIONBITS      0x0000FFFF


/* maps modifiers to their appropriate bit flags */
#define MOD(X) ((X) == 'h' ? SHORT : ((X) == 'l' ? LONG : LONG_DOUBLE))

/* maps flags to their appropriate bit flags */
#define FLAG(X) ((X) == ' ' ? FLAG_SPACE : ((X) == '-' ? FLAG_MINUS : \
((X) == '+' ? FLAG_PLUS : ((X) == '0' ? FLAG_0 : FLAG_POUND))))

/* maps conversion specifiers to their appropriate bit flags */
#define CONVERSION(X) ((X) == 'd' || (X) == 'i' ? SIGNED_INT : \
((X) == 'o' ? OCTAL : ((X) == 'x' ? HEX_LOWERCASE : \
((X) == 'X' ? HEX_UPPERCASE : ((X) == 'u' ? UNSIGNED_INT : \
((X) == 'c' ? CHAR : ((X) == 's' ? C_STRING : ((X) == 'S' ? DSTRING : \
((X) == 'f' ? FLOAT : ((X) == 'e' ? FLOAT_EXP_LOWER : \
((X) == 'E' ? FLOAT_EXP_UPPER : ((X) == 'g' ? FLOAT_G_LOWER : \
((X) == 'G' ? FLOAT_G_UPPER : ((X) == 'p' ? POINTER : NUM_CHARS))))))))))))))

/* maps digits to their corresponding characters */
#define DIGITC(X) ((X) == 0 ? '0' : ((X) == 1 ? '1' : ((X) == 2 ? '2' : \
((X) == 3 ? '3' : ((X) == 4 ? '4' : ((X) == 5 ? '5' : ((X) == 6 ? '6' : \
((X) == 7 ? '7' : ((X) == 8 ? '8' : '9')))))))))

/* maps digit characters to their corresponding integers */
#define DIGITI(X) ((X) == '0' ? 0 : ((X) == '1' ? 1 : ((X) == '2' ? 2 : \
((X) == '3' ? 3 : ((X) == '4' ? 4 : ((X) == '5' ? 5 : ((X) == '6' ? 6 : \
((X) == '7' ? 7 : ((X) == '8' ? 8 : 9)))))))))


/* **** parseargs **********************************************************

   This function parses a single format specifier and fills in a structure
   containing the appropriate information.  It is intended only for
   internal use with dstrvsprintf().

   Found in sprintf.c

   *************************************************************************

   Input:
      char * (format specifier to parse)
      struct specifier * (will contain information about the conversion)

   Output:
      Pointer to the end of the format specifier if successful, or a pointer
      back to the beginning if it's invalid

   ************************************************************************* */
char *parsearg(char *format, struct specifier *conversion);


/* **** appendsignedint ****************************************************

   This function appends a formatted integer to a dstring object.  It is
   an internal function designed for use with dstrvsprintf(), and it is
   assumed that all input is valid and that the destination string is
   initialized.

   Found in sprintf.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const struct specifier * (will contain information about the conversion)
      long int (the integer to format)

   Output:
      0 on success
     >0 if an error occured

   ************************************************************************* */
int appendsignedint(dstring_t dest, const struct specifier conversion,
   long int arg);


/* **** appendunsignedint **************************************************

   This function is the same as appendsignedint, except that it takes as
   input an unsigned integer.

   Found in sprintf.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const struct specifier * (will contain information about the conversion)
      unsigned long int (the integer to format)

   Output:
      0 on success
     >0 if an error occured

   ************************************************************************* */
int appendunsignedint(dstring_t dest, const struct specifier conversion,
   unsigned long int arg);


/* **** appendoctal ********************************************************

   This internal-only function appends the string representation of an
   unsigned integer to a dstring_t object in octal format.

   Found in sprintf.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const struct specifier * (will contain information about the conversion)
      unsigned long int (the integer to format)

   Output:
      0 on success
     >0 if an error occured

   ************************************************************************* */
int appendoctal(dstring_t dest, const struct specifier conversion,
   unsigned long int arg);


/* **** appendhex **********************************************************

   This internal-only function appends the string representation of an
   integer to a dstring_t object in hexadecimal format.

   Found in sprintf.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const struct specifier * (will contain information about the conversion)
      long int (the integer to format)

   Output:
      0 on success
     >0 if an error occured

   ************************************************************************* */
int appendhex(dstring_t dest, const struct specifier conversion,
   long int arg);


/* **** appendfloat ********************************************************

   This internal-only function appends the string representation of a
   double to a dstring_t object.

   Found in sprintf.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const struct specifier * (will contain information about the conversion)
      long double (the float to format)

   Output:
      0 on success
     >0 if an error occured

   ************************************************************************* */
int appendfloat(dstring_t dest, const struct specifier conversion,
   long double arg);


/* **** appendfloatexp *****************************************************

   This internal-only function appends the string representation of a
   double to a dstring_t object in scientific notation.

   Found in sprintf.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const struct specifier * (will contain information about the conversion)
      long double (the float to format)

   Output:
      0 on success
     >0 if an error occured

   ************************************************************************* */
int appendfloatexp(dstring_t dest, const struct specifier conversion,
   long double arg);


/* **** appendptr **********************************************************

   This internal-only function appends the string representation of a
   pointer to a dstring_t object.

   Found in sprintf.c

   *************************************************************************

   Input:
      dstring_t (destination)
      const struct specifier * (will contain information about the conversion)
      void * (the pointer to format)

   Output:
      0 on success
     >0 if an error occured

   ************************************************************************* */
int appendptr(dstring_t dest, const struct specifier conversion,
   void *ptr);


/* **** str2int ************************************************************

   This function converts an integer into a dstring_t string.  It is
   assumed that all the characters in intstr are numbers.

   Found in sprintf.c

   *************************************************************************

   Input:
      dstring_t (the string we want to convert)

   Output:
      An integer on success
     >0 if an error occured

   ************************************************************************* */
int str2int(dstring_t intstr);


#endif
