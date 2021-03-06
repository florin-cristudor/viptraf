/* For terms of usage/redistribution/modification see the LICENSE file */
/* For authors and contributors see the AUTHORS file */

#include "iptraf-ng-compat.h"
//end old inc
#include "close.h"
#include "viptraf.h"

// Die if we can't allocate size bytes of memory.
void *xmalloc(size_t size)
{
	void *ptr = malloc(size);

	if (ptr == NULL && size != 0)
        exit_program(ERROR_MEMORY, "");
	return ptr;
}

void *xmallocz(size_t size)
{
	void *ptr = xmalloc(size);

	memset(ptr, 0, size);
	return ptr;
}

void *xcalloc(size_t nmemb, size_t size)
{
	void *ptr = calloc(nmemb, size);

	if (!ptr && (!nmemb || !size))
        exit_program(ERROR_MEMORY, "");
	return ptr;
}

void *xrealloc(void *ptr, size_t size)
{
	void *ret = realloc(ptr, size);

	if (!ret && !size)
        exit_program(ERROR_MEMORY, "");
	return ret;
}

// Die if we can't copy a string to freshly allocated memory.
char *xstrdup(const char *s)
{
	if (!s)
		return NULL;

	char *t = strdup(s);

	if (!t)
        exit_program(ERROR_MEMORY, "");

	return t;
}

int strtoul_ui(char const *s, int base, unsigned int *result)
{
	unsigned long ul;
	char *p;

	errno = 0;
	ul = strtoul(s, &p, base);
	if (errno || *p || p == s || (unsigned int) ul != ul)
		return -1;
	*result = ul;
	return 0;
}

int strtol_i(char const *s, int base, int *result)
{
	long ul;
	char *p;

	errno = 0;
	ul = strtol(s, &p, base);
	if (errno || *p || p == s || (int) ul != ul)
		return -1;
	*result = ul;
	return 0;
}
