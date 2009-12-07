/*
 * Copyright (C) 2009 Gildas Le Nadan <gildas@endemics.info>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; version 2.1 or version 3.0 only.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dhcpmsg.h"

int main (int argc, char *argv[], char *envp[])
{
  extern bool noneth;
  extern char *action, *mac, *ip, *hostname;

  char *str_json;

  if ( do_args (argc, argv, envp) == 1 )
    return 1;
  else /* correct arguments */
  {
    if ( noneth )
        printf ("protocol type for the mac address is non ethernet\n");

    if ( argc == 4 )
      printf ("action:\t%s\nmac:\t%s\nip:\t%s\n", action, macaddr, ip);
    else if ( argc == 5 )
      printf ("action:\t%s\nmac:\t%s\nip:\t%s\nhostname:\t%s\n", action, macaddr, ip,hostname);

    do_env ( action, &str_json );
    printf ("environment variables:\t%s\n", str_json);

    return 0;
  }
}
