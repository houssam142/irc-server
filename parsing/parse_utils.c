#include "../includes/parse.h"

int get_array_size(char **arr)
{
  int i;

  i = 0;
  while (arr[i] != NULL)
    i++;
  return (i);
}
