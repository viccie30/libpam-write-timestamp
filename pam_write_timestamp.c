/*
 * Copyright 2020 Victor Westerhuis
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <https://www.gnu.org/licenses/>.
 */
#include <dlfcn.h>
#include <security/pam_ext.h>
#include <security/pam_modules.h>
#include <syslog.h>

int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc,
                        const char **argv) {
  void *dl_handle = dlopen(PAM_DIRECTORY "/pam_timestamp.so", RTLD_NOW);
  if (!dl_handle) {
    const char *const error = dlerror();
    pam_syslog(pamh, LOG_ERR, "could not dlopen `%s': %s",
               PAM_DIRECTORY "/pam_timestamp.so", error ? error : "");
    return PAM_MODULE_UNKNOWN;
  }

  int (*const function)(pam_handle_t *, int, int, const char **) =
      dlsym(dl_handle, "pam_sm_open_session");
  if (!function) {
    const char *const error = dlerror();
    pam_syslog(pamh, LOG_ERR, "could not dlsym `pam_sm_open_session': %s",
               error ? error : "");
    return PAM_SYMBOL_ERR;
  }

  const int ret = function(pamh, flags, argc, argv);

  if (dlclose(dl_handle)) {
    const char *const error = dlerror();
    pam_syslog(pamh, LOG_WARNING, "could not dlclose `%s': %s",
               PAM_DIRECTORY "/pam_timestamp.so", error ? error : "");
  }

  return ret;
}

int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  (void)pamh;
  (void)flags;
  (void)argc;
  (void)argv;

  return PAM_SUCCESS;
}
