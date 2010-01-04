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
#include <string.h>
#include <curl/curl.h>

#include "dhcpmsg.h"

#define CURL_TIMEOUT 10

int curldatalen = 0;

/** curl readfunction callback
 * taken from WebAware libpurple plugin.
 * Copyright (C) 2007 Vishal Rao <vishal.rao@lahsiv.net>
 * slightly modified
 * (code in GPL v2 or later)
 */
static size_t
curlreader(void *ptr, size_t size, size_t nmemb, void *stream)
{
  extern int curldatalen;
  static int total = 0;
  int current = 0;

  while (total < curldatalen && current < size*nmemb)
  {
    *(((char*)ptr)+current) = *(((char*)stream)+total);
    total++;
    current++;
  }

  if (current == 0) total = 0;

  return current;
}

/** does an http request
 * return 0 if successful, non-zero values otherwise
 */
int http_request ( char *uri, int http_verb, char *data )
{
  extern int curldatalen;

  long http_code = 0;
  int ret = 1;  /* return code */

  CURL *curl;
  CURLcode res;

  if ( http_verb != HTTP_POST
      && http_verb != HTTP_PUT
      && http_verb != HTTP_DELETE )
    return 1;

  if ( ! data )
  {
    curldatalen = 0;
    data = "";
  }
  else
  {
    curldatalen = strlen ( data );
  }

  curl = curl_easy_init ();
  if ( curl )
  {
    curl_easy_setopt ( curl, CURLOPT_URL, uri );

    if ( http_verb == HTTP_POST )
    {
      /* FIXME is it valid? */
      curl_easy_setopt ( curl, CURLOPT_POST, 1L );
      curl_easy_setopt ( curl, CURLOPT_CUSTOMREQUEST, "POST" );
      curl_easy_setopt ( curl, CURLOPT_POSTFIELDS, NULL );
    }
    else if ( http_verb == HTTP_PUT )
      curl_easy_setopt ( curl, CURLOPT_PUT, 1L );
    else if ( http_verb == HTTP_DELETE )
      curl_easy_setopt ( curl, CURLOPT_CUSTOMREQUEST, "DELETE" );

    /* we don't push data when we delete */
    if ( http_verb == HTTP_PUT || http_verb == HTTP_POST)
    {
      curl_easy_setopt ( curl, CURLOPT_UPLOAD, 1L );
      curl_easy_setopt ( curl, CURLOPT_READDATA, data );
      curl_easy_setopt ( curl, CURLOPT_READFUNCTION, &curlreader );
    }

    /* FIXME need to add support for timeouts in the config file */
    curl_easy_setopt( curl, CURLOPT_TIMEOUT, CURL_TIMEOUT );

    res = curl_easy_perform ( curl );

    curl_easy_getinfo ( curl, CURLINFO_RESPONSE_CODE, &http_code );

    /* FIXME for now only consider 200 to be ok.
     * Will need the app to provide some json return value
     */
    if (http_code == 200 && res != CURLE_ABORTED_BY_CALLBACK )
    {
      ret = 0;
    }

    /* always cleanup */ 
    curl_easy_cleanup ( curl );

    return ret;
  }

  return 1;
}
