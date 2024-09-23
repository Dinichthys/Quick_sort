#include <stdlib.h>
#include <ctype.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

#include "qsort.h"
#include "my_assert.h"
#include "logging.h"
#include "helpful.h"

static void skip_non_abc (const char** const s, const int step);
static void swap (void* const a, void* const b, const size_t size);
static void vec_swap (void* a, void* b, const size_t size);
static void memcpy_for_swap (void* const ptr1, void* const ptr2, void* const change, size_t size);

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

    LOG(DEBUG, " s1 = %p \n s2 = %p\n", s1, s2);

    skip_non_abc (&s1, 1);
    skip_non_abc (&s2, 1);

    LOG(DEBUG, s1);
    LOG(DEBUG, s2);

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

    LOG(DEBUG, " s1 = %p \n s2 = %p\n", s1, s2);

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

    if (nmemb < 2)
    {
        return;
    }

    LOG(INFO, " Parameters for function my_qsort:\n"
              " base = %p \n nmemb = %lu\n size = %lu\n",
              base, nmemb, size);

    char* left   = (char*) base;
    char* right  = (char*) base + ((nmemb - 1) * size);
    char* mid    = (char*) base + ( nmemb / 2  * size);

    size_t count = 1;
    while (left < right)
    {
        LOG(DEBUG, " left = %p \n right = %p\n mid = %p\n"
                   " Iteration № %lu\n",
                   left, right, mid, count);
        count++;

        while ((left < right) && (compar (left, mid) <= 0) && (left < mid))
        {
            left += size;
        }

        LOG(DEBUG, " left = %p \n right = %p\n", left, right);

        while ((left < right) && (compar (right, mid) >= 0) && (right > mid))
        {
            right -= size;
        }

        LOG(DEBUG, " left = %p \n right = %p\n size = %lu\n", left, right, size);

        if (left < right)
        {
            vec_swap (left, right, size);

            if (left == mid)
            {
                mid = right;
            }
            else if (right == mid)
            {
                mid = left;
            }
        }

        LOG(DEBUG, " left = %p \n right = %p\n", left, right);
    }

    LOG(DEBUG, " left = %p \n right = %p\n", left, right);

    if (left > (char*) base)
    {
        my_qsort (base, (left - (char*) base) / size, size, compar);
    }

    if (right < (char*) base + (nmemb - 1) * size)
    {
        my_qsort (right + size, ((char*) base + (nmemb - 1) * size - right) / size, size, compar);
    }
}

static void swap (void* const a, void* const b, const size_t size)
{
    ASSERT(a != NULL, "Invalid argument for function swap\n");
    ASSERT(b != NULL, "Invalid argument for function swap\n");

    LOG(DEBUG, " Parameters for function swap:\n"
                       " a = %p \n b = %p\n size = %lu\n",
                       a, b, size);

    char* ptr_1 = (char*) a;
    char* ptr_2 = (char*) b;

    for (size_t i = 0; i < size; i++)
    {
        char change =  ptr_1 [i];
                       ptr_1 [i] = ptr_2 [i];
                                   ptr_2 [i] = change;
    }
}

static void vec_swap (void* a, void* b, size_t size)
{
    ASSERT(a != NULL, "Invalid argument for function vec_swap\n");
    ASSERT(b != NULL, "Invalid argument for function vec_swap\n");

    while (size > sizeof (uint64_t))
    {
        uint64_t change = 0;

        memcpy_for_swap (a, b, &change, sizeof (uint64_t));

        a = (uint8_t*) a + sizeof (uint64_t);
        b = (uint8_t*) b + sizeof (uint64_t);
        size -= sizeof (uint64_t);
    }

    if (size > sizeof (uint32_t))
    {
        uint32_t change = 0;

        memcpy_for_swap (a, b, &change, sizeof (uint32_t));

        a = (uint8_t*) a + sizeof (uint32_t);
        b = (uint8_t*) b + sizeof (uint32_t);
        size -= sizeof (uint32_t);
    }

    if (size > sizeof (uint16_t))
    {
        uint16_t change = 0;

        memcpy_for_swap (a, b, &change, sizeof (uint16_t));

        a = (uint8_t*) a + sizeof (uint16_t);
        b = (uint8_t*) b + sizeof (uint16_t);
        size -= sizeof (uint16_t);
    }

    if (size > sizeof (uint8_t))
    {
        uint8_t change = 0;

        memcpy_for_swap (a, b, &change, sizeof (uint8_t));

        a = (uint8_t*) a + sizeof (uint8_t);
        b = (uint8_t*) b + sizeof (uint8_t);
        size -= sizeof (uint8_t);
    }
}

static void memcpy_for_swap (void* const ptr1, void* const ptr2, void* const change, const size_t size)
{
    ASSERT(ptr1   != NULL, "Invalid argument for function memcpy_for_swap\n");
    ASSERT(ptr2   != NULL, "Invalid argument for function memcpy_for_swap\n");
    ASSERT(change != NULL, "Invalid argument for function memcpy_for_swap\n");

    memcpy (change, ptr1, size);
            memcpy (ptr1, ptr2, size);
                  memcpy (ptr2, change, size);
}
