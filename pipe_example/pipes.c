#include <stdio.h>
#include <stdlib.h>

int
main ()
{
  FILE *ps_pipe;
  FILE *grep_pipe;

  int bytes_read;
  int nbytes = 100;
  char *my_string;

  /* Open our two pipes */
  ps_pipe = popen ("ls -l", "r");
  grep_pipe = popen ("grep pipes_ex", "w");

  /* Check that pipes are non-null, therefore open */
  if ((!ps_pipe) || (!grep_pipe))
    {
      fprintf (stderr,
               "One or both pipes failed.\n");
      return EXIT_FAILURE;
    }
  my_string = (char *) malloc(nbytes + 1);
  bytes_read = getdelim(&my_string, &nbytes, (int)"\n\n", ps_pipe);
  

  /* Close ps_pipe, checking for errors */
  if (pclose (ps_pipe) != 0)
    {
      fprintf (stderr,
               "Could not run 'ps', or other error.");
    }

  /* Send output of 'ps -A' to 'grep init', with two newlines */
  fprintf (grep_pipe, "%s\n\n", my_string);

  /* Close grep_pipe, checking for errors */
  if (pclose (grep_pipe) != 0)
    {
      fprintf (stderr,
               "Could not run 'grep', or other error.\n");
    }

  /* Exit! */
  return 0;
}
