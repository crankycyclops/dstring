
/* ************************************************************************* *\
   * File: sprintf.c                                                       *
   * Purpose:                                                              *
   *    Provides the dstring version of vsprintf and sprintf.              *
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


#include <ctype.h>
#include <string.h>
#include <stdarg.h>

#include "dstring.h"
#include "static.h"
#include "sprintf.h"

static char *flags       = FLAGSC;
static char *modifiers   = MODIFIERSC;
static char *conversions = CONVERSIONSC;


/* ************************************************************************* */

int dstrsprintf(dstring_t str, const char *format, ...) {

   va_list args;
   long int chars;

   va_start(args, format);
   chars = dstrvsprintf(str, format, args);
   va_end(args);

   return chars;
}

/* ************************************************************************* */

int dstrvsprintf(dstring_t str, const char *format, va_list args) {

   /* flags containing information about a single conversion specifier */
   struct specifier conversion;

   /* keeps track of our position in format */
   char *curpos;
   char *end;

   /* keeps track of the return value of append* functions */
   int r;

   curpos = (char *)format;

   /* make sure the string is empty */
   if (dstrlen(str) != 0) {
      dstrtrunc(str, 0);
      if (dstrerrno != DSTR_SUCCESS) {
         return -1;
      }
   }

   while (*curpos != '\0') {

      /* possible conversion specifier */
      if ('%' == *curpos) {

         end = parsearg(curpos, &conversion);
         if (dstrerrno != DSTR_SUCCESS) {
            return -1;
         }

         /* it wasn't a valid specifier */
         if (end == curpos) {
            dstrinsertc(str, dstrlen(str), *curpos);
            if (dstrerrno != DSTR_SUCCESS) {
               return -1;
            }
            curpos++;
            continue;
         }

         /* it was a valid specifier */
         else {
            /* make sure we update curpos */
            curpos = end;
            switch(CONVERSIONBITS & conversion.format) {
               case PERCENT:
                  dstrinsertc(str, dstrlen(str) - 1, '%');
                  if (DSTR_SUCCESS != dstrerrno) {
                     return -1;
                  }
                  break;

               case SIGNED_INT:

                  if (conversion.format & LONG) {
                     r = appendsignedint(str, conversion,
                            va_arg(args, long int));
                  } else if (conversion.format & SHORT) {
                     r = appendsignedint(str, conversion,
                            (long int)va_arg(args, int));
                  } else {
                     r = appendsignedint(str, conversion,
                            (long int)va_arg(args, int));
                  }

                  if (r < 0) {
                     return -1;
                  }

                  break;

               case UNSIGNED_INT:

                  if (conversion.format & LONG) {
                     r = appendunsignedint(str, conversion,
                            va_arg(args, unsigned long int));
                  } else if (conversion.format & SHORT) {
                     r = appendsignedint(str, conversion,
                            (unsigned long)va_arg(args, unsigned int));
                  } else {
                     r = appendsignedint(str, conversion,
                            (unsigned long)va_arg(args, unsigned int));
                  }

                  if (r < 0) {
                     return -1;
                  }

                  break;

               case OCTAL:

                  if (conversion.format & LONG) {
                     r = appendoctal(str, conversion,
                            va_arg(args, unsigned long int));
                  } else if (conversion.format & SHORT) {
                     r = appendoctal(str, conversion,
                            (unsigned long)va_arg(args, unsigned int));
                  } else {
                     r = appendoctal(str, conversion,
                            (unsigned long)va_arg(args, unsigned int));
                  }

                  if (r < 0) {
                     return -1;
                  }

                  break;

               case HEX_LOWERCASE:
               case HEX_UPPERCASE:

                  if (conversion.format & LONG) {
                     r = appendhex(str, conversion,
                            va_arg(args, long int));
                  } else if (conversion.format & SHORT) {
                     r = appendhex(str, conversion,
                            (long)va_arg(args, int));
                  } else {
                     r = appendhex(str, conversion,
                            (long)va_arg(args, int));
                  }

                  if (r < 0) {
                     return -1;
                  }

                  break;

               case CHAR:

                  dstrinsertc(str, dstrlen(str) - 1,
                     (unsigned char)va_arg(args, int));
                  if (DSTR_SUCCESS != dstrerrno) {
                     return -1;
                  }

                  break;

               case C_STRING:

                  if (conversion.format & PRECISION) {
                     dstrncatcs(str, va_arg(args, char *),
                        conversion.precision);
                  } else {
                     dstrcatcs(str, va_arg(args, char *));
                  }

                  if (DSTR_SUCCESS != dstrerrno) {
                     return -1;
                  }

                  break;

               case DSTRING:

                  if (conversion.format & PRECISION) {
                     dstrncat(str, va_arg(args, dstring_t),
                        conversion.precision);
                  } else {
                     dstrcat(str, va_arg(args, dstring_t));
                  }

                  if (DSTR_SUCCESS != dstrerrno) {
                     return -1;
                  }

                  break;

               case FLOAT:

                  if (conversion.format & LONG_DOUBLE) {
                     r = appendfloat(str, conversion,
                            va_arg(args, long double));
                  } else {
                     r = appendfloat(str, conversion,
                            (long double)va_arg(args, double));
                  }

                  if (r < 0) {
                     return -1;
                  }

                  break;

               case FLOAT_EXP_LOWER:
               case FLOAT_EXP_UPPER:

                  if (conversion.format & LONG_DOUBLE) {
                     r = appendfloatexp(str, conversion,
                            va_arg(args, long double));
                  } else {
                     r = appendfloatexp(str, conversion,
                            (long double)va_arg(args, long double));
                  }

                  if (r < 0) {
                     return -1;
                  }

                  break;

               case FLOAT_G_LOWER:
               case FLOAT_G_UPPER:

                  break;

               case POINTER:

                  r = appendptr(str, conversion, va_arg(args, void *));

                  if (r < 0) {
                     return -1;
                  }

                  break;

               case NUM_CHARS:

                  r = appendunsignedint(str, conversion,
                         (unsigned long)dstrlen(str));

                  if (r < 0) {
                     return -1;
                  }

                  break;

               default:
                  break;
            }
            continue;
         }
      }

      /* just another ordinary character to insert into str */
      dstrinsertc(str, dstrlen(str), *curpos);
      if (dstrerrno != DSTR_SUCCESS) {
         return -1;
      }
      curpos++;
   }

   _setdstrerrno(DSTR_SUCCESS);
   return dstrlen(str);
}

/* ************************************************************************* */

char *parsearg(char *format, struct specifier *conversion) {

   int i;
   char *pos = format;

   /* store string versions of the field width and precision */
   dstring_t fieldstr;
   dstring_t precisionstr;

   /* make sure we zero out the format structure each time */
   conversion->format    = 0;
   conversion->field     = 0;
   conversion->precision = 0;

   /* let us begin */
   pos++;

   /* are we just trying to print a % sign? */
   if ('%' == *pos) {
      conversion->format |= PERCENT;
      pos++;
      return pos;
   }

   /* initialize the temporary field width and precision strings */
   if (DSTR_SUCCESS != dstralloc(&fieldstr)) {
      return format;
   }

   if (DSTR_SUCCESS != dstralloc(&precisionstr)) {
      dstrfree(&fieldstr);
      return format;
   }

   /* check for flags */
   for (i = 0; flags[i] != '\0'; i++) {

      /* we found a flag */
      if (*pos == flags[i]) {

         /* make sure it's not a duplicate */
         if (FLAG(flags[i]) == (conversion->format & FLAG(flags[i]))) {
            dstrfree(&fieldstr), dstrfree(&precisionstr);
            return format;
         } else {
            conversion->format |= FLAG(flags[i]);
         }

         /* look to see if there are more flags */
         pos++, i = 0;
      }
   }

   /* is there a field width? */
   for (; isdigit(*pos); pos++) {
      dstrinsertc(fieldstr, dstrlen(fieldstr), *pos);
      if (DSTR_SUCCESS != dstrerrno) {
         dstrfree(&fieldstr), dstrfree(&precisionstr);
         return format;
      }
   }

   /* we got a field width, so convert it into an integer */
   if (dstrlen(fieldstr) > 0) {
      conversion->field = str2int(fieldstr);
      if (conversion->field < 0) {
         dstrfree(&fieldstr), dstrfree(&precisionstr);
         return format;
      }
      conversion->format |= FIELDWIDTH;
   }

   /* is there a precision? */
   if ('.' == *pos) {
      for (pos++; isdigit(*pos); pos++) {
         dstrinsertc(precisionstr, dstrlen(precisionstr), *pos);
         if (DSTR_SUCCESS != dstrerrno) {
            dstrfree(&fieldstr), dstrfree(&precisionstr);
            return format;
         }
      }
   }

   /* we got a precision, so convert it into an integer */
   if (dstrlen(precisionstr) > 0) {
      conversion->precision = str2int(precisionstr);
      if (conversion->field < 0) {
         dstrfree(&fieldstr), dstrfree(&precisionstr);
         return format;
      }
      conversion->format |= PRECISION;
   }

   /* now, check for modifiers */
   for (i = 0; modifiers[i] != '\0'; i++) {

      /* we found a modifier */
      if (*pos == modifiers[i]) {

         /* make sure it's not a duplicate */
         /* NOTE: we'll have to make an exception for C99 'll' */
         if (MOD(modifiers[i]) == (conversion->format & MOD(modifiers[i]))) {
            dstrfree(&fieldstr), dstrfree(&precisionstr);
            return format;
         } else {
            conversion->format |= MOD(modifiers[i]);
         }

         /* look to see if there are more flags */
         pos++, i = 0;
      }
   }

   /* lastly, make sure we have a conversion specifier */
   for (i = 0; conversions[i] != '\0'; i++) {

      /* we found a conversion specifier */
      if (*pos == conversions[i]) {
            conversion->format |= CONVERSION(conversions[i]);
            dstrfree(&fieldstr), dstrfree(&precisionstr);
            pos++;
            return pos;
      }
   }

   /* there was no conversion specifier, so we did all that for nothing! */
   dstrfree(&fieldstr), dstrfree(&precisionstr);
   return format;
}

/* ************************************************************************* */

int appendsignedint(dstring_t dest, const struct specifier conversion,
   long int arg) {

   /* this is the index in which we insert digits in reverse order */
   int index = 0;

   int digit;
   char fillc = ' ';
   dstring_t tempint;


   if (DSTR_SUCCESS != dstralloc(&tempint)) {
      return -1;
   }

   /* does the user want to pad with 0's instead of spaces? */
   if (conversion.format & FLAG_0) {
      fillc = '0';
   }

   /* is the integer negative? */
   if (arg < 0) {
      if (DSTR_SUCCESS != dstrinsertc(tempint, 0, '-')) {
         dstrfree(&tempint);
         return -1;
      }
      index = 1;
      arg *= -1;
   }

   /* + flag set, so append the sign even if it's positive */
   else if (conversion.format & FLAG_PLUS) {
      if (DSTR_SUCCESS != dstrinsertc(tempint, 0, '+')) {
         dstrfree(&tempint);
         return -1;
      }
      index = 1;
   }

   /* space flag set, so if there's no sign, prefix with a space */
   else if (conversion.format & FLAG_SPACE) {
      if (DSTR_SUCCESS != dstrinsertc(tempint, 0, ' ')) {
         dstrfree(&tempint);
         return -1;
      }
      index = 1;
   }

   /* insert each digit */
   while ((digit = arg % 10) > 0) {
      if (DSTR_SUCCESS != dstrinsertc(tempint, index, DIGITC(digit))) {
         dstrfree(&tempint);
         return -1;
      }
      arg /= 10;
   }

   /* did the user specify a field width? */
   if (conversion.format & FIELDWIDTH) {
      while (dstrlen(tempint) < conversion.field) {

         /* the user wants to left justify his int in its field */
         if (conversion.format & FLAG_MINUS) {
            if (DSTR_SUCCESS != dstrinsertc(tempint, dstrlen(dest), fillc)) {
               dstrfree(&tempint);
               return -1;
            }
         }

         /* by default, the int will be right justified in its field */
         else {
            if (DSTR_SUCCESS != dstrinsertc(tempint, index, fillc)) {
               dstrfree(&tempint);
               return -1;
            }
         }
      }
   }

   /* append the temporary string to the dest string and return */
   dstrcat(dest, tempint);
   if (DSTR_SUCCESS != dstrerrno) {
      dstrfree(&tempint);
      return -1;
   }

   dstrfree(&tempint);
   return 0;
}

/* ************************************************************************* */

int appendunsignedint(dstring_t dest, const struct specifier conversion,
   unsigned long int arg) {

   /* this is the index in which we insert digits in reverse order */
   int index = 0;

   int digit;
   char fillc = ' ';
   dstring_t tempint;


   if (DSTR_SUCCESS != dstralloc(&tempint)) {
      return -1;
   }

   /* does the user want to pad with 0's instead of spaces? */
   if (conversion.format & FLAG_0) {
      fillc = '0';
   }

   /* + flag set, so append the sign even if it's positive */
   if (conversion.format & FLAG_PLUS) {
      if (DSTR_SUCCESS != dstrinsertc(tempint, 0, '+')) {
         dstrfree(&tempint);
         return -1;
      }
      index = 1;
   }

   /* space flag set, so if there's no sign, prefix with a space */
   else if (conversion.format & FLAG_SPACE) {
      if (DSTR_SUCCESS != dstrinsertc(tempint, 0, ' ')) {
         dstrfree(&tempint);
         return -1;
      }
      index = 1;
   }

   /* insert each digit */
   while ((digit = arg % 10) > 0) {
      if (DSTR_SUCCESS != dstrinsertc(tempint, index, DIGITC(digit))) {
         dstrfree(&tempint);
         return -1;
      }
      arg /= 10;
   }

   /* did the user specify a field width? */
   if (conversion.format & FIELDWIDTH) {
      while (dstrlen(tempint) < conversion.field) {

         /* the user wants to left justify his int in its field */
         if (conversion.format & FLAG_MINUS) {
            if (DSTR_SUCCESS != dstrinsertc(tempint, dstrlen(dest), fillc)) {
               dstrfree(&tempint);
               return -1;
            }
         }

         /* by default, the int will be right justified in its field */
         else {
            if (DSTR_SUCCESS != dstrinsertc(tempint, index, fillc)) {
               dstrfree(&tempint);
               return -1;
            }
         }
      }
   }

   /* append the temporary string to the dest string and return */
   dstrcat(dest, tempint);
   if (DSTR_SUCCESS != dstrerrno) {
      dstrfree(&tempint);
      return -1;
   }

   dstrfree(&tempint);
   return 0;
}

/* ************************************************************************* */

int appendoctal(dstring_t dest, const struct specifier conversion,
   unsigned long int arg) {

   return 0;
}

/* ************************************************************************* */

int appendhex(dstring_t dest, const struct specifier conversion,
   long int arg) {

   return 0;
}

/* ************************************************************************* */

int appendfloat(dstring_t dest, const struct specifier conversion,
   long double arg) {

   return 0;
}

/* ************************************************************************* */

int appendfloatexp(dstring_t dest, const struct specifier conversion,
   long double arg) {

   return 0;
}

/* ************************************************************************* */

int appendptr(dstring_t dest, const struct specifier conversion,
   void *ptr) {

   return 0;
}

/* ************************************************************************* */

int str2int(dstring_t intstr) {

   int intval = 0;

   while (dstrlen(intstr) > 0) {
      intval *= 10;
      intval += DIGITI(dstrgetc(intstr, dstrlen(intstr) - 1));
      dstrdel(intstr, dstrlen(intstr) - 1);
      if (DSTR_SUCCESS != dstrerrno) {
         return -1;
      }
   }

   return intval;
}
