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

#include <check.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../src/dhcpmsg.h"

START_TEST (test_noop)
{
  /* do nothing, testing the test suite :) */
  fail_unless (true);
}
END_TEST

START_TEST (test_regex_match)
{
  fail_unless ( regex_match ("[a-z]", "qwerty") == 0,
     "regex match returns 0\n" );
  fail_unless ( regex_match ("^add|del|old$", "add") == 0,
      NULL );
  fail_unless ( regex_match ("[0-9]", "qwerty") == 1,
     "regex match returns 1\n" );
  fail_unless ( regex_match ("^add|del|old$", "foo") == 1,
      NULL );
  fail_unless ( regex_match (NULL, "qwerty") == 2,
     "other errors should report 2\n" );
}
END_TEST

START_TEST (test_check_mac)
{
  fail_unless ( check_mac ("01:23:45:67:89:ab") == 0,
      "correct ethernet mac address\n" );
  fail_unless ( check_mac ("01:23:45:67:89:AB") == 0,
      "correct CAPS ethernet mac address\n" );
  fail_unless ( check_mac ("06-01:23:45:67:89:ab") == 0,
      "correct non ethernet mac address\n" );
  fail_unless ( check_mac ("06-01:23:45:67:89:AB") == 0,
      "correct CAPS non ethernet mac address\n" );
  fail_unless ( check_mac (NULL) == 1,
      "mac_address is a NULL pointer\n" );
  fail_unless ( check_mac ("01-23-45-56-89-ab") == 1,
      "incorrect dash-separated ethernet mac address\n" );
  fail_unless ( check_mac ("06-01-23-45-56-89-ab") == 1,
      "incorrect dash-separated non ethernet mac address\n" );
  fail_unless ( check_mac ("012:34:56:78:9a:bc") == 1,
      "too many characters in mac address\n" );
  fail_unless ( check_mac ("012:34:56:78:9a:b") == 1,
      "incorrect mac address formatting\n" );
  fail_unless ( check_mac ("01:23:45:67:89:ag") == 1,
      "'g' is not a valid hexadecimal value for mac address\n" );
}
END_TEST

START_TEST (test_do_args)
{
  char *empty[] = { };
  char *ok_add[] = { "dhcpmsg","add","01:23:45:67:89:ab" };
  char *ok_del[] = { "dhcpmsg","del","01:23:45:67:89:ab" };
  char *ok_old[] = { "dhcpmsg","old","01:23:45:67:89:ab" };
  char *ok_add_name[] = { "dhcpmsg","add","01:23:45:67:89:ab","hostname" };
  char *ok_del_name[] = { "dhcpmsg","del","01:23:45:67:89:ab","hostname" };
  char *ok_old_name[] = { "dhcpmsg","old","01:23:45:67:89:ab","hostname" };
  char *ko_old[] = { "dhcpmsg","old","01:23:45:67:89:ab" };
  char *ko_foo[] = { "dhcpmsg","foo","01:23:45:67:89:ab" };

  fail_unless ( do_args (4, ok_add, empty) == 0,
      "add action, no hostname\n" );
  fail_unless ( do_args (4, ok_del, empty) == 0,
      "del action, no hostname\n" );
  fail_unless ( do_args (4, ok_old, empty) == 0,
      "old action, no hostname\n" );
  fail_unless ( do_args (5, ok_add_name, empty) == 0,
      "add action, hostname\n" );
  fail_unless ( do_args (5, ok_del_name, empty) == 0,
      "del action, hostname\n" );
  fail_unless ( do_args (5, ok_old_name, empty) == 0,
      "old action, hostname\n" );
  fail_unless ( do_args (4, ko_foo, empty) == 1,
      "incorrect 'foo' action\n" );
  fail_unless ( do_args (0, empty, empty) == 1,
      "incorrect number of arguments\n" );
}
END_TEST

START_TEST (test_do_env)
{
  char *action = "add";
  char *p;
  char *name="DNSMASQ_DOMAIN";
  char *value="test";
  int n = 11; /* domain:test */

  fail_unless ( do_env (action, &p) == 0,
      "correct empty environement\n" );
  fail_unless ( setenv(name, value, 0) == 0,
      "could not initialize environment\n");
  fail_unless ( do_env (action, &p) == n,
      "do_env did not return correct size\n" );
}
END_TEST

Suite * checks_suite (void)
{
  Suite *s = suite_create("utils");

  TCase *tc_core = tcase_create ("Core");
  tcase_add_test (tc_core, test_noop);
  tcase_add_test (tc_core, test_regex_match);
  tcase_add_test (tc_core, test_check_mac);
  tcase_add_test (tc_core, test_do_args);
  tcase_add_test (tc_core, test_do_env);
  suite_add_tcase (s, tc_core);

  return s;
}

int main (void)
{
    int number_failed;

    Suite *s = checks_suite ();
    SRunner *sr = srunner_create (s);
    srunner_run_all (sr, CK_NORMAL);
    number_failed = srunner_ntests_failed (sr);
    srunner_free (sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

