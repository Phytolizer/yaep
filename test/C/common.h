#ifndef YAEP_TEST_C_COMMON_H_
#define YAEP_TEST_C_COMMON_H_

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>

#include"yaep.h"

static void *
test_parse_alloc (int size)
{
  void * result;

  assert ((size > 0) && ((unsigned int) size == (size_t) size));
  result = malloc (size);
  assert (result != NULL);

  return result;
}

static void
test_parse_free (void * mem)
{
  free( mem );
}

/* Printing syntax error. */
static void
test_syntax_error (int err_tok_num, void *err_tok_attr,
		   int start_ignored_tok_num, void *start_ignored_tok_attr,
		   int start_recovered_tok_num, void *start_recovered_tok_attr)
{
  if (start_ignored_tok_num < 0)
    fprintf (stderr, "Syntax error on token %d\n", err_tok_num);
  else
    fprintf
      (stderr,
       "Syntax error on token %d:ignore %d tokens starting with token = %d\n",
       err_tok_num, start_recovered_tok_num - start_ignored_tok_num,
       start_ignored_tok_num);
}

static const char *input;
static const char *description;

/* The following function imported by YAEP (see comments in the interface file). */
static int
test_read_token (void **attr)
{
  static int ntok = 0;

  *attr = NULL;
  if (input [ntok])
    {
      return input [ntok++];
    }
  else
    {
      return -1;
    }
}

static void
test_standard_parse (void)
{
  struct grammar *g;
  struct yaep_tree_node *root;
  int ambiguous_p;

  if ((g = yaep_create_grammar ()) == NULL)
    {
      fprintf (stderr, "yaep_create_grammar: No memory\n");
      exit (1);
    }
  if (yaep_parse_grammar (g, 1, description) != 0)
    {
      fprintf (stderr, "%s\n", yaep_error_message (g));
      exit (1);
    }
  if (yaep_parse
      (g, test_read_token, test_syntax_error, test_parse_alloc,
       test_parse_free, &root, &ambiguous_p))
    {
      fprintf (stderr, "yaep parse: %s\n", yaep_error_message (g));
      exit (1);
    }
  yaep_free_grammar (g);
}

static void
test_standard_read
  (const char *(*read_terminal) (int *),
   const char *(*read_rule) (const char ***, const char **, int *, int **))
{
  struct grammar *g;
  struct yaep_tree_node *root;
  int ambiguous_p;

  if ((g = yaep_create_grammar ()) == NULL)
    {
      fprintf (stderr, "yaep_create_grammar: No memory\n");
      exit (1);
    }
  if (yaep_read_grammar (g, 1, read_terminal, read_rule) != 0)
    {
      fprintf (stderr, "%s\n", yaep_error_message (g));
      exit (1);
    }
  if (yaep_parse
      (g, test_read_token, test_syntax_error, test_parse_alloc,
       test_parse_free, &root, &ambiguous_p))
    {
      fprintf (stderr, "yaep parse: %s\n", yaep_error_message (g));
      exit (1);
    }
  yaep_free_grammar (g);
}

static void
test_complex_parse
  (int one_parse, int ambiguous, int print_cost, int recovery_match,
   int argc, char **argv)
{
  struct grammar *g;
  struct yaep_tree_node *root;
  int ambiguous_p;

  if ((g = yaep_create_grammar ()) == NULL)
    {
      fprintf (stderr, "yaep_create_grammar: No memory\n");
      exit (1);
    }
  yaep_set_one_parse_flag (g, one_parse);
  if (print_cost)
    {
      yaep_set_cost_flag (g, 1);
    }
  if (argc > 1)
    yaep_set_lookahead_level (g, atoi (argv [1]));
  if (argc > 2)
    yaep_set_debug_level (g, atoi (argv [2]));
  else
    yaep_set_debug_level (g, 3);
  if (argc > 3)
    yaep_set_error_recovery_flag (g, atoi (argv [3]));
  if (argc > 4)
    yaep_set_one_parse_flag (g, atoi (argv [4]));
  if (recovery_match)
    {
      yaep_set_recovery_match (g, recovery_match);
    }
  if (yaep_parse_grammar (g, 1, description) != 0)
    {
      fprintf (stderr, "%s\n", yaep_error_message (g));
      exit (1);
    }
  if (yaep_parse (g, test_read_token, test_syntax_error, test_parse_alloc,
                  test_parse_free, &root, &ambiguous_p))
    {
      fprintf (stderr, "yaep parse: %s\n", yaep_error_message (g));
      exit (1);
    }
  if (ambiguous != ambiguous_p)
    {
      fprintf (stderr, "Grammar should be %sambiguous\n", ambiguous ? "" : "un");
      exit (1);
    }
  if (print_cost)
    {
      fprintf (stderr, "cost = %d\n", root->val.anode.cost);
    }
  yaep_free_grammar (g);
}

#endif