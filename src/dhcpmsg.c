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
#include <strings.h>

#include "dhcpmsg.h"
#include "../config.h"  /* for CONFIG_FILE  */

int main (int argc, char *argv[], char *envp[])
{
  extern bool noneth, known;
  extern char *action, *macaddr, *ip, *hostname, *uri, *uri_data_path;

  char buff[MAX_BUFF_SIZE] = "";
  char *str_json;
  int http_verb;
  int res;

  if ( do_args (argc, argv, envp) == 1 )
    return 1;
  else /* correct arguments */
  {
    if ( read_config ( CONFIG_FILE ) == 1 )
    {
      fprintf (stderr, "error reading configuration file, aborting\n");
      return 1;
    }

    if ( ! uri )
    {
      fprintf (stderr, "uri not defined in configuration file, aborting\n");
      return 1;
    }

    /* FIXME: no use for this? */
    if ( noneth )
      printf ("protocol type for the mac address is non ethernet\n");
    /* END FIXME */

    http_verb = http_verb_from_action ( action );

    do_env ( action, &str_json );

    if ( strlen (uri_data_path) == 0 || ! uri_data_path )
    {
      /* FIXME should send to syslog as info */
      fprintf (stderr, "no uri data path defined in configuration file using default value \"%s\"\n", URI_DATA_PATH);
      sprintf ( buff, "%s%s/%s", uri, URI_DATA_PATH, macaddr );
    } else {
      sprintf ( buff, "%s%s/%s", uri, uri_data_path, macaddr );
    }
      
    res = http_request ( buff, http_verb, json_output ( str_json ) );

    /* FIXME: use syslog to report this since dnsmasq will drop stderr */
    if ( res != 0 )
    {
      fprintf (stderr, "failed pushing content to %s:\n%s\n",
          buff, json_output ( str_json ) );
    }

    free(str_json);
    free(uri);

    return res;
  }
}
