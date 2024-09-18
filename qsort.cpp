#include <stdlib.h>
#include <ctype.h>
#include <ctype.h>

#include "qsort.h"
#include "my_assert.h"
#include "logging.h"

static void skip_non_abc (const char** const s, const int step);

static void skip_non_abc (const char** const s, const int step)
{
    ASSERT(s != NULL, "Invalid argument for function skip_non_sym\n");

    while (!(isalpha (**s)) && (**s != '\0'))
    {
        *s += step;
    }
}

int compare (const void* const a, const void* const b)
{
    ASSERT(a != NULL, "Invalid argument for function my_strcmp\n");
    ASSERT(b != NULL, "Invalid argument for function my_strcmp\n");

    const char* s1 = *((const char**) a);
    const char* s2 = *((const char**) b);

//    LOG(stderr, DEBUG, " s1 = %p \n s2 = %p\n", s1, s2);

    skip_non_abc (&s1, 1);
    skip_non_abc (&s2, 1);

    while ((tolower(*s1) == tolower(*s2)) && (*s1 != '\0'))
    {
        s1++;
        s2++;

        skip_non_abc (&s1, 1);
        skip_non_abc (&s2, 1);
    }

    return tolower(*s1) - tolower(*s2);
}

int compare_r (const void* const a, const void* const b)
{
    ASSERT(a != NULL, "Invalid argument for function my_strcmp\n");
    ASSERT(b != NULL, "Invalid argument for function my_strcmp\n");

    const char* s1 = *((const char**) a);
    const char* s2 = *((const char**) b);

//    LOG(stderr, DEBUG, " s1 = %p \n s2 = %p\n", s1, s2);

    skip_non_abc (&s1, -1);
    skip_non_abc (&s2, -1);

    while ((tolower(*s1) == tolower(*s2)) && (*s1 != '\0'))
    {
        s1--;
        s2--;

        skip_non_abc (&s1, -1);
        skip_non_abc (&s2, -1);
    }

    return tolower(*s1) - tolower(*s2);
}

void my_qsort(void *const base, const size_t nmemb, const size_t size,
              int(*compar)(const void *, const void *))
{
    ASSERT(base   != NULL, "Invalid argument for function qsort\n");
    ASSERT(compar != NULL, "Invalid argument for function qsort\n");

    char* left   = (char*) base;
    char* right  = ((char*) base) + ((nmemb - 1) * size);
    char* mid    = ((char*) base) + ((nmemb - 1) / 2 * size);

    size_t count = 1;
    do
    {
        LOG(stderr, DEBUG, " left = %p \n right = %p\n mid = %p\n"
                           " *mid = %p \n Iteration № %lu\n",
                           left, right, mid, *mid, count);
        count++;

        while (compar (left, mid) < 0)
        {
            left += size;
        }

        LOG(stderr, DEBUG, " left = %p \n right = %p\n", left, right);

        while (compar (right, mid) > 0)
        {
            right -= size;
        }

        LOG(stderr, DEBUG, " left = %p \n right = %p\n size = %lu\n", left, right, size);

        for (size_t i = 0; i < size; i++)
        {
            char change =  left [i];
                           left [i] = right [i];
                                      right [i] = change;
        }

        left  += size;
        right -= size;

        LOG(stderr, DEBUG, " left = %p \n right = %p\n", left, right);
    }
    while (left < right);

    if (right > (char*) base)
    {
        my_qsort (base, (right - (char*) base) / size + 1, size, compar);
    }

    if ((char*) base + (nmemb - 1) * size > left)
    {
        my_qsort (left, ((char*) base + (nmemb - 1) * size - left) / size + 1, size, compar);
    }
}
