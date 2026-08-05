#include "../includes/parse.h"


bool is_key_allowed(char *key, t_section sect)
{
  if (sect == SERVER)
  {
    return (!strcmp(key, "name") || !strcmp(key, "password") 
          || !strcmp(key, "port"));
  }
  else if (sect == LINK)
  {
    return (!strcmp(key, "name") || !strcmp(key, "password")
        || !strcmp(key, "port"));
  }
  return false;
}

int get_array_size(char **arr)
{
  int i;

  i = 0;
  while (arr[i] != NULL)
    i++;
  return (i);
}
