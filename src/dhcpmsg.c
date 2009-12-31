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
#include "../config.h"  /* for CONFIG_FILE  */

int main (int argc, char *argv[], char *envp[])
{
  extern bool noneth, known;
  extern char *action, *macaddr, *ip, *hostname, *uri;

  char *str_json;

  if ( do_args (argc, argv, envp) == 1 )
    return 1;
  else /* correct arguments */
  {
    if ( read_config ( CONFIG_FILE ) == 1 )
    {
      fprintf (stderr, "error reading configuration file, aborting\n");
      return 1;
    }

    if ( uri )
      printf ("uri for the webapp is: %s\n",uri);
    else
    {
      fprintf (stderr, "uri not defined in configuration file, aborting\n");
      return 1;
    }

    if ( noneth )
      printf ("protocol type for the mac address is non ethernet\n");

    if ( argc == 4 )
      printf ("action:\t%s\nmac:\t%s\nip:\t%s\n", action, macaddr, ip);
    else if ( argc == 5 )
      printf ("action:\t%s\nmac:\t%s\nip:\t%s\nhostname:\t%s\n", action, macaddr, ip,hostname);

    do_env ( action, &str_json );

    if ( known )
      printf ("address statically assigned\n");

    printf ("environment variables:\t%s\n\n", str_json);

    printf ( "%s", json_output (str_json) );

    return 0;
  }
}
