#ifndef __ANSI_COLOR_H
#define __ANSI_COLOR_H

/* 
 For ANSI color code definition, refer to Wikipedia:
 http://en.wikipedia.org/wiki/ANSI_escape_code#Colors 
 
 I do not prefix the macro with ANSI_, so if we need to switch to another
 color implementation, we can just create another header file with the same
 macro definition and change the included header file.
 */

#define _COLOR_RESET   "\x1b[0m"

#define COLOR_RED(s)     "\x1b[31m" s _COLOR_RESET
#define COLOR_GREEN(s)   "\x1b[32m" s _COLOR_RESET
#define COLOR_YELLOW(s)  "\x1b[33m" s _COLOR_RESET
#define COLOR_BLUE(s)    "\x1b[34m" s _COLOR_RESET
#define COLOR_MAGENTA(s) "\x1b[35m" s _COLOR_RESET
#define COLOR_CYAN(s)    "\x1b[36m" s _COLOR_RESET

#endif /* __ANSI_COLOR_H */
