#include <stdlib.h>
#include <ctype.h>
#include <ctype.h>

#include "qsort.h"
#include "my_assert.h"
#include "logging.h"

int compare (const void* const a, const void* const b, const bool reverse)
{
    ASSERT(a != NULL, "Invalid argument for function my_strcmp\n");
    ASSERT(b != NULL, "Invalid argument for function my_strcmp\n");

    const char* s1 = (const char*) a;
    const char* s2 = (const char*) b;

    LOG(stderr, DEBUG, " s1 = %p \n s2 = %p\n", s1, s2);

    const int step = reverse ? -1 : 1;

    while (!(isalpha (*s1)))
    {
        s1 += step;
    }

    while (!(isalpha (*s2)))
    {
        s2 += step;
    }

    while ((tolower(*s1) == tolower(*s2)) && (*s1 != '\0'))
    {
        s1 += step;
        s2 += step;
    }

    return tolower(*s1) - tolower(*s2);
}


void my_qsort(void *const base, const size_t nmemb, const size_t size,
           int(*compar)(const void *, const void *, bool), const bool reverse)
{
    ASSERT(base   != NULL, "Invalid argument for function qsort\n");
    ASSERT(compar != NULL, "Invalid argument for function qsort\n");

    char** s1  = (char**) base;
    char** s2  = ((char**) base) + (nmemb - 1) * size;
    char** mid = ((char**) base) + (nmemb - 1) / 2 * size;

    do
    {
        LOG(stderr, DEBUG, " s1 = %p \n s2 = %p\n", s1, s2);

        while (compar (*s1, *mid, reverse) < 0)
        {
            LOG(stderr, DEBUG, " s1 = %p \n s2 = %p\n", s1, s2);

            s1 += size;
        }

        LOG(stderr, DEBUG, " s1 = %p \n s2 = %p\n", s1, s2);

        while (compar (*s2, *mid, reverse) > 0)
        {
            s2 -= size;
        }

        LOG(stderr, DEBUG, " s1 = %p \n s2 = %p\n", s1, s2);

        for (size_t i = 0; i < size; i++)
        {
            char* change = s1 [i];
                           s1 [i] = s2 [i];
                                    s2 [i] = change;
        }

        s1 += size;
        s2 -= size;

        LOG(stderr, DEBUG, " s1 = %p \n s2 = %p\n", s1, s2);
    }
    while (s1 < s2);

    if (s2 > (char**) base)
    {
        my_qsort (base, (s1 - (char**) base) / size, size, compar, reverse);
    }

    if ((char**) base + (nmemb - 1) * size > s1)
    {
        my_qsort (base, ((char**) base + (nmemb - 1) * size - s1) / size, size, compar, reverse);
    }
}
