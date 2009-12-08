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
#include <string.h>
#include <regex.h>
#include "dhcpmsg.h"

/** check if string matched regex
 * returns:
 * 0 if it matches
 * 1 if it doesn't match
 * 2 if error */
int regex_match (const char *regex, char *string)
{
  int match;

  if ( regex == NULL || string == NULL)
    return 2;

  regex_t preg;
  if ( regcomp (&preg, regex, REG_NOSUB | REG_EXTENDED) == 0 )
  {
    match = regexec (&preg, string, 0, NULL, 0);
    regfree (&preg);
    if (match == 0)
      return 0;
    else if (match == REG_NOMATCH)
      return 1;
    else
      return 2;
  }
}

/** check that the mac format is ok and set the flag noneth if needed 
 * a correct format is:
 * 01:23:45:67:89:ab if protocol is ethernet
 * 06-01:23:45:67:89:ab if protocol is not ethernet
 *
 * return 0 if mac has a correct format, 1 otherwise */
int check_mac ( char *mac_address )
{
  extern bool noneth;

  int res = 1;
  const char *regex_eth = "^([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2}$";
  const char *regex_noneth = "^[0-9]+-([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2}$";

  if ( mac_address == NULL)
    return 1;

  if ( regex_match (regex_noneth, mac_address) == 0 )
  {
    res = 0;
    noneth = true;
  }

  if ( regex_match (regex_eth, mac_address) == 0 )
    res = 0;

  return res;
}

  
/** check if arguments are corrects
 * return 0 if ok, 1 otherwise */
int do_args (int argc, char *argv[], char *envp[])
{
  extern char *action, *macaddr, *ip, *hostname;

  if ( argv == NULL || envp == NULL )
    return 1;

  if ( argc < 4 || argc > 5 )
  {
    fprintf (stderr, "invalid numbers of arguments.\n");
    fprintf (stderr, "Usage: %s add|del|old <mac> <ip> [<hostname>]\n", argv[0]);
    return 1;
  }
  else
  {
    if ( regex_match ("^add|del|old$", argv[1]) != 0 )
    {
      fprintf (stderr, "incorrect action '%s'\n", argv[1]); /* DEBUG */
      return 1;
    }

    action = argv[1];
    macaddr = argv[2];
    ip = argv[3];

    if ( argc == 5 )
      hostname = argv[4];

    if ( check_mac(macaddr) != 0 )
    {
        fprintf (stderr, "macaddress type is not correct\n"); /* DEBUG */
        return 1;
    }

    return 0;
  }
}

/*
 */
char * json_from_env ( char *string, const char *env_name, const char *name )
{
  char *var = NULL;
  char buffer[MAX_BUFF_SIZE] = ""; /* FIXME should do dynamic allocation */

  if ( strlen (env_name) == 0 || strlen (name) == 0 )
  {
    return string;
  }

  var = getenv (env_name);

  if ( var != NULL && strlen (string) == 0 ) /* start from scratch */
  {
    if ( (strlen (name) + strlen (var) + 1) < sizeof(buffer) - 1 ) 
      sprintf (buffer, "%s:%s", name, var);
    else
    {
      fprintf(stderr, "key:value pair to big to fit in buffer\n");
      return NULL;
    }
  }
  else if ( var != NULL && strlen (string) != 0 ) /* add stuff to existing value */
  {
    if ( (strlen (name) + strlen (var) + strlen (string) + 2)  < sizeof(buffer)-1 )
      sprintf (buffer, "%s,%s:%s", string, name, var);
    else
    {
      fprintf(stderr, "cannot append another key:value\n");
      return NULL;
    }
  }
  else if ( strlen (string) != 0 ) /* var == NULL, preserve existing string */
  {
    sprintf (buffer, "%s", string);
  }

  strcpy (string, buffer);
  return string;
}

/** check environment variables, set variable list accordingly if needed
 * and returns the values as json data
 * environments variables that can be set are:
 * - DNSMASQ_CLIENT_ID
 * - DNSMASQ_DOMAIN
 * - DNSMASQ_LEASE_LENGTH  (if compiled with HAVE_BROKEN_RTC)
 * - DNSMASQ_LEASE_EXPIRES (otherwise)
 * - DNSMASQ_TIME_REMAINING
 * - DNSMASQ_RELAY_ADDRESS
 * for add|del
 * - DNSMASQ_INTERFACE
 * for old|add actions:
 * - DNSMASQ_VENDOR_CLASS
 * - DNSMASQ_SUPPLIED_HOSTNAME
 * - DNSMASQ_USER_CLASS0..DNSMASQ_USER_CLASSn 
 * for old only (if new lease without name)
 * - DNSMASQ_OLD_HOSTNAME`
 */
int do_env ( char *action_type, char **res )
{
  char *clientid, *domain;
  char json[MAX_BUFF_SIZE] = ""; /* FIXME should do dynamic allocation */
  int n = 0;

  json_from_env ( json, "DNSMASQ_CLIENT_ID", "client_id" );
  json_from_env ( json, "DNSMASQ_DOMAIN", "domain" );
  json_from_env ( json, "DNSMASQ_RELAY_ADDRESS", "relay_address" );
  json_from_env ( json, "DNSMASQ_TIME_REMAINING", "time_remaining");

  /* mutually exclusives */
  /* FIXME should check that they are not set at the same time */
  json_from_env ( json, "DNSMASQ_LEASE_LENGTH", "lease_length" );
  json_from_env ( json, "DNSMASQ_LEASE_EXPIRES", "lease_expires" );

  if ( strcmp (action_type, "del") == 0 )
  {
    json_from_env ( json, "DNSMASQ_INTERFACE", "interface" );
  }

  if ( strcmp (action_type, "add") == 0 )
  {
    json_from_env ( json, "DNSMASQ_INTERFACE", "interface" );
    json_from_env ( json, "DNSMASQ_VENDOR_CLASS", "vendor_class" );
    json_from_env ( json, "DNSMASQ_SUPPLIED_HOSTNAME", "supplied_hostname" );
    /* FIXME how to deal with N USER_CLASS? */
  }

  if ( strcmp (action_type, "old") == 0 )
  {
    json_from_env ( json, "DNSMASQ_VENDOR_CLASS", "vendor_class" );
    json_from_env ( json, "DNSMASQ_SUPPLIED_HOSTNAME", "supplied_hostname" );
    json_from_env ( json, "DNSMASQ_OLD_HOSTNAME", "old_hostname" );
    /* FIXME how to deal with N USER_CLASS? */
  }

  n = strlen (json);
  *res = calloc (MAX_BUFF_SIZE, sizeof(char));
  memcpy(*res, json, n);

  return n;
}
