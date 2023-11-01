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
    LBRACE = 258,                  /* LBRACE  */
    RBRACE = 259,                  /* RBRACE  */
    SEMI = 260,                    /* SEMI  */
    IDENTIFIER = 261,              /* IDENTIFIER  */
    LPAR = 262,                    /* LPAR  */
    RPAR = 263,                    /* RPAR  */
    CHAR = 264,                    /* CHAR  */
    INT = 265,                     /* INT  */
    VOID = 266,                    /* VOID  */
    SHORT = 267,                   /* SHORT  */
    DOUBLE = 268,                  /* DOUBLE  */
    ASSIGN = 269,                  /* ASSIGN  */
    WHILE = 270,                   /* WHILE  */
    RETURN = 271,                  /* RETURN  */
    BITWISEAND = 272,              /* BITWISEAND  */
    BITWISEOR = 273,               /* BITWISEOR  */
    BITWISEXOR = 274,              /* BITWISEXOR  */
    AND = 275,                     /* AND  */
    MUL = 276,                     /* MUL  */
    COMMA = 277,                   /* COMMA  */
    DIV = 278,                     /* DIV  */
    EQ = 279,                      /* EQ  */
    GE = 280,                      /* GE  */
    GT = 281,                      /* GT  */
    LE = 282,                      /* LE  */
    LT = 283,                      /* LT  */
    MINUS = 284,                   /* MINUS  */
    MOD = 285,                     /* MOD  */
    NE = 286,                      /* NE  */
    NOT = 287,                     /* NOT  */
    OR = 288,                      /* OR  */
    PLUS = 289,                    /* PLUS  */
    DECIMAL = 290,                 /* DECIMAL  */
    NATURAL = 291,                 /* NATURAL  */
    CHRLIT = 292,                  /* CHRLIT  */
    LOWER_THAN_ELSE = 293,         /* LOWER_THAN_ELSE  */
    ELSE = 294,                    /* ELSE  */
    IF = 295                       /* IF  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define LBRACE 258
#define RBRACE 259
#define SEMI 260
#define IDENTIFIER 261
#define LPAR 262
#define RPAR 263
#define CHAR 264
#define INT 265
#define VOID 266
#define SHORT 267
#define DOUBLE 268
#define ASSIGN 269
#define WHILE 270
#define RETURN 271
#define BITWISEAND 272
#define BITWISEOR 273
#define BITWISEXOR 274
#define AND 275
#define MUL 276
#define COMMA 277
#define DIV 278
#define EQ 279
#define GE 280
#define GT 281
#define LE 282
#define LT 283
#define MINUS 284
#define MOD 285
#define NE 286
#define NOT 287
#define OR 288
#define PLUS 289
#define DECIMAL 290
#define NATURAL 291
#define CHRLIT 292
#define LOWER_THAN_ELSE 293
#define ELSE 294
#define IF 295

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
