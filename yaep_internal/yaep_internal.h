#ifndef __YAEP_INTERNAL__
#define __YAEP_INTERNAL__

#include "allocate.h"

#ifndef YAEP_MAX_ERROR_MESSAGE_LENGTH
#define YAEP_MAX_ERROR_MESSAGE_LENGTH 200
#endif

/* The following is major structure which stores information about
   grammar. */
struct grammar
{
    /* The following member is TRUE if the grammar is undefined (you
       should set up the grammar by yaep_read_grammar or
       yaep_parse_grammar) or bad (error was occured in setting up the
       grammar). */
    int undefined_p;

    /* This member always contains the last occurred error code for
       given grammar. */
    int error_code;
    /* This member contains message are always contains error message
       corresponding to the last occurred error code. */
    char error_message[YAEP_MAX_ERROR_MESSAGE_LENGTH + 1];

    /* The following is grammar axiom.  There is only one rule with axiom
       in lhs. */
    struct symb* axiom;
    /* The following auxiliary symbol denotes EOF. */
    struct symb* end_marker;
    /* The following auxiliary symbol is used for describing error
       recovery. */
    struct symb* term_error;
    /* And its internal number. */
    int term_error_num;
    /* The level of usage of lookaheads:
       0    - no usage
       1    - static lookaheads
       >= 2 - dynamic lookaheads */
    int lookahead_level;
    /* The following value means how much subsequent tokens should be
       successfuly shifted to finish error recovery. */
    int recovery_token_matches;

    /* The following value is debug level:
       <0 - print translation for graphviz.
       0 - print nothing.
       1 - print statistics.
       2 - print parse tree.
       3 - print rules, parser list
       4 - print sets.
       5 - print also nonstart situations.
       6 - print additionally lookaheads. */
    int debug_level;

    /* The following value is TRUE if we need only one parse. */
    int one_parse_p;

    /* The following value is TRUE if we need parse(s) with minimal
       costs. */
    int cost_p;

    /* The following value is TRUE if we need to make error recovery. */
    int error_recovery_p;

    /* The following vocabulary used for this grammar. */
    struct symbs* symbs_ptr;
    /* The following rules used for this grammar. */
    struct rules* rules_ptr;
    /* The following terminal sets used for this grammar. */
    struct term_sets* term_sets_ptr;
    /* Allocator. */
    YaepAllocator* alloc;
};

#endif // __YAEP_INTERNAL__
