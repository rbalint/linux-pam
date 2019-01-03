/* pam_session.c - PAM Session Management */

/*
 * $Id: pam_session.c,v 1.4 2003/07/13 20:01:44 vorlon Exp $
 */

#include "pam_private.h"

#include <stdio.h>

int pam_open_session(pam_handle_t *pamh, int flags)
{
    D(("called"));

    IF_NO_PAMH("pam_open_session", pamh, PAM_SYSTEM_ERR);

    if (__PAM_FROM_MODULE(pamh)) {
	D(("called from module!?"));
	return PAM_SYSTEM_ERR;
    }

    return _pam_dispatch(pamh, flags, PAM_OPEN_SESSION);
}

int pam_close_session(pam_handle_t *pamh, int flags)
{
    D(("called"));

    IF_NO_PAMH("pam_close_session", pamh, PAM_SYSTEM_ERR);

    if (__PAM_FROM_MODULE(pamh)) {
	D(("called from module!?"));
	return PAM_SYSTEM_ERR;
    }

    return _pam_dispatch(pamh, flags, PAM_CLOSE_SESSION);
}
