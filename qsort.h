#ifndef QSORT_H
#define QSORT_H

int  compare  (const void* const a, const void* const b, const bool reverse);
void my_qsort    (void *const base, const size_t nmemb, const size_t size,
               int(*compare)(const void *, const void *, bool), const bool reverse);

#endif // QSORT_H
