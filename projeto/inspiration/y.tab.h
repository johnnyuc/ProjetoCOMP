/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    CHRLIT = 258,                  /* CHRLIT  */
    INTLIT = 259,                  /* INTLIT  */
    REALLIT = 260,                 /* REALLIT  */
    ID = 261,                      /* ID  */
    VOID = 262,                    /* VOID  */
    CHAR = 263,                    /* CHAR  */
    INT = 264,                     /* INT  */
    SHORT = 265,                   /* SHORT  */
    DOUBLE = 266,                  /* DOUBLE  */
    SEMI = 267,                    /* SEMI  */
    COMMA = 268,                   /* COMMA  */
    LBRACE = 269,                  /* LBRACE  */
    RBRACE = 270,                  /* RBRACE  */
    LPAR = 271,                    /* LPAR  */
    RPAR = 272,                    /* RPAR  */
    ASSIGN = 273,                  /* ASSIGN  */
    PLUS = 274,                    /* PLUS  */
    MINUS = 275,                   /* MINUS  */
    MUL = 276,                     /* MUL  */
    DIV = 277,                     /* DIV  */
    MOD = 278,                     /* MOD  */
    OR = 279,                      /* OR  */
    AND = 280,                     /* AND  */
    BITWISEAND = 281,              /* BITWISEAND  */
    BITWISEOR = 282,               /* BITWISEOR  */
    BITWISEXOR = 283,              /* BITWISEXOR  */
    EQ = 284,                      /* EQ  */
    NE = 285,                      /* NE  */
    LE = 286,                      /* LE  */
    GE = 287,                      /* GE  */
    LT = 288,                      /* LT  */
    GT = 289,                      /* GT  */
    NOT = 290,                     /* NOT  */
    WHILE = 291,                   /* WHILE  */
    IF = 292,                      /* IF  */
    ELSE = 293,                    /* ELSE  */
    RETURN = 294,                  /* RETURN  */
    RESERVED = 295                 /* RESERVED  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define CHRLIT 258
#define INTLIT 259
#define REALLIT 260
#define ID 261
#define VOID 262
#define CHAR 263
#define INT 264
#define SHORT 265
#define DOUBLE 266
#define SEMI 267
#define COMMA 268
#define LBRACE 269
#define RBRACE 270
#define LPAR 271
#define RPAR 272
#define ASSIGN 273
#define PLUS 274
#define MINUS 275
#define MUL 276
#define DIV 277
#define MOD 278
#define OR 279
#define AND 280
#define BITWISEAND 281
#define BITWISEOR 282
#define BITWISEXOR 283
#define EQ 284
#define NE 285
#define LE 286
#define GE 287
#define LT 288
#define GT 289
#define NOT 290
#define WHILE 291
#define IF 292
#define ELSE 293
#define RETURN 294
#define RESERVED 295

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 34 "uccompiler.y"

int intValue;
double doubleValue;
char* stringValue;

ast_node* ast;

#line 155 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
