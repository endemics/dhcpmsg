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

#ifndef DHCPMSG_H
#define DHCPMSG_H

#define MAX_BUFF_SIZE 1024
#define URI_DATA_PATH "/macs"

#define HTTP_PUT  1
#define HTTP_POST 2
#define HTTP_GET  3
#define HTTP_DELETE 4

int regex_match ( const char *regex, char *string );
int check_mac ( char *mac_address );
int do_args ( int argc, char *argv[], char *envp[] );
int do_env ( char *action_type, char **res );
char *json_output ( char *keyvaluestring );
int read_config ( char *configfile );
int http_verb_from_action ( char *action_type );
int http_request ( char *uri, int http_verb, char *data );

#endif /* DHCPMSG_H */
