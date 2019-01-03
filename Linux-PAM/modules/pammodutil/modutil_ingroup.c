/*
 * $Id: modutil_ingroup.c,v 1.1 2004/09/24 13:14:14 kukuk Exp $
 *
 * This function provides common methods for checking if a user is in a
 * specified group.
 */

#include "pammodutil.h"
#include "include/security/_pam_modutil.h"
#include <pwd.h>
#include <grp.h>

#ifdef HAVE_GETGROUPLIST
static int checkgrouplist(const char *user, gid_t primary, gid_t target)
{
	gid_t *grouplist = NULL;
	int agroups, ngroups, i;
	ngroups = agroups = 3;
	do {
		grouplist = malloc(sizeof(gid_t) * agroups);
		if (grouplist == NULL) {
			return 0;
		}
		ngroups = agroups;
		i = getgrouplist(user, primary, grouplist, &ngroups);
		if ((i < 0) || (ngroups < 1)) {
			agroups *= 2;
			free(grouplist);
		} else {
			for (i = 0; i < ngroups; i++) {
				if (grouplist[i] == target) {
					free(grouplist);
					return 1;
				}
			}
			free(grouplist);
		}
	} while (((i < 0) || (ngroups < 1)) && (agroups < 10000));
	return 0;
}
#endif

static int _pammodutil_user_in_group_common(pam_handle_t *pamh,
				            struct passwd *pwd,
					    struct group *grp)
{
	int i;

	if (pwd == NULL) {
		return 0;
	}
	if (grp == NULL) {
		return 0;
	}

	if (pwd->pw_gid == grp->gr_gid) {
		return 1;
	}

	for (i = 0; (grp->gr_mem != NULL) && (grp->gr_mem[i] != NULL); i++) {
		if (strcmp(pwd->pw_name, grp->gr_mem[i]) == 0) {
			return 1;
		}
	}

#ifdef HAVE_GETGROUPLIST
	if (checkgrouplist(pwd->pw_name, pwd->pw_gid, grp->gr_gid)) {
		return 1;
	}
#endif

	return 0;
}

int _pammodutil_user_in_group_nam_nam(pam_handle_t *pamh,
				      const char *user, const char *group)
{
	struct passwd *pwd;
	struct group *grp;

	pwd = _pammodutil_getpwnam(pamh, user);
	grp = _pammodutil_getgrnam(pamh, group);

	return _pammodutil_user_in_group_common(pamh, pwd, grp);
}

int _pammodutil_user_in_group_nam_gid(pam_handle_t *pamh,
				      const char *user, gid_t group)
{
	struct passwd *pwd;
	struct group *grp;

	pwd = _pammodutil_getpwnam(pamh, user);
	grp = _pammodutil_getgrgid(pamh, group);

	return _pammodutil_user_in_group_common(pamh, pwd, grp);
}

int _pammodutil_user_in_group_uid_nam(pam_handle_t *pamh,
				      uid_t user, const char *group)
{
	struct passwd *pwd;
	struct group *grp;

	pwd = _pammodutil_getpwuid(pamh, user);
	grp = _pammodutil_getgrnam(pamh, group);

	return _pammodutil_user_in_group_common(pamh, pwd, grp);
}

int _pammodutil_user_in_group_uid_gid(pam_handle_t *pamh,
				      uid_t user, gid_t group)
{
	struct passwd *pwd;
	struct group *grp;

	pwd = _pammodutil_getpwuid(pamh, user);
	grp = _pammodutil_getgrgid(pamh, group);

	return _pammodutil_user_in_group_common(pamh, pwd, grp);
}
