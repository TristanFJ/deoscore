/* ΔOS: Decentralized Operating System

Copyright (c) 2013-2017 Andrew DeSantis <atd@gmx.it>
Copyright (c) 2016-2017 DeSantis Inc. <inc@gmx.it>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

This file incorporates work covered by the BSD 2-Clause License,
as well as the following copyright, and permission notice:
---
ΔOS may be used and distributed under the terms of the GPLv3,
which are available at: <http://www.gnu.org/licenses/gpl-3.0.html>

If you would like to embed ΔOS within a commercial application or
redistribute it in a modified binary form, contact DeSantis Inc.
*/

#include "deArray.h"

#ifdef __cplusplus
extern "C" {
#endif

#define _correctArrayIndex(T,x,s,a) _correctArrayIndex_##T(x,s,a)
#define _growArray(T,x) _growArray_##T(x)
#define deArrayPolymorphismC(T)                                               \
                                                                              \
static void _correctArrayIndex_##T(deArray_##T *self, char *side, char *act); \
static void _growArray_##T(deArray_##T *self);                                \
                                                                              \
deArray_##T *newArray_##T(void)                                               \
{                                                                             \
    deArray_##T *self = (deArray_##T *) malloc(sizeof(deArray_##T));          \
    self->factor = FACTOR;                                                    \
    self->capacity = CAPACITY;                                                \
    self->store = (T *) malloc(self->capacity * sizeof(T));                   \
    self->head = self->tail = self->size = 0;                                 \
    return self;                                                              \
}                                                                             \
                                                                              \
void freeArray_##T(deArray_##T *self)                                         \
{                                                                             \
    if (self == NULL) return;                                                 \
    free(self->store);                                                        \
    free(self);                                                               \
}                                                                             \
                                                                              \
void appendArray_##T(deArray_##T *self, int value)                            \
{                                                                             \
    if (self->size == 0)                                                      \
        prependArray_##T(self, value);                                        \
    else                                                                      \
    {                                                                         \
        if (getArraySize_##T(self) == getArrayCapacity_##T(self))             \
            _growArray_##T(self);                                             \
        else                                                                  \
            _correctArrayIndex_##T(self, "tail", "increment");                \
        self->store[getArrayTail_##T(self)] = value;                          \
        self->size++;                                                         \
    }                                                                         \
}                                                                             \
                                                                              \
void prependArray_##T(deArray_##T *self, int value)                           \
{                                                                             \
    if (getArraySize_##T(self) == 0)                                          \
    {                                                                         \
        self->store[getArrayTail_##T(self)] = value;                          \
        self->size++;                                                         \
    }                                                                         \
    else                                                                      \
    {                                                                         \
        if (getArraySize_##T(self) == getArrayCapacity_##T(self))             \
            _growArray_##T(self);                                             \
        _correctArrayIndex_##T(self, "head", "decrement");                    \
        self->store[getArrayHead_##T(self)] = value;                          \
        self->size++;                                                         \
    }                                                                         \
}                                                                             \
                                                                              \
double getArrayFactor_##T(deArray_##T *self)                                  \
{                                                                             \
    return self->factor;                                                      \
}                                                                             \
                                                                              \
int getArrayCapacity_##T(deArray_##T *self)                                   \
{                                                                             \
    return self->capacity;                                                    \
}                                                                             \
                                                                              \
int getArrayHead_##T(deArray_##T *self)                                       \
{                                                                             \
    return self->head;                                                        \
}                                                                             \
                                                                              \
int getArrayTail_##T(deArray_##T *self)                                       \
{                                                                             \
    return self->tail;                                                        \
}                                                                             \
                                                                              \
int getArraySize_##T(deArray_##T *self)                                       \
{                                                                             \
    return self->size;                                                        \
}                                                                             \
                                                                              \
static void _correctArrayIndex_##T(deArray_##T *self, char *side, char *act)  \
{                                                                             \
    if (strcmp(side,"tail") == 0)                                             \
    {                                                                         \
        if (strcmp(act,"increment") == 0)                                     \
            self->tail = (++self->tail) % getArrayCapacity_##T(self);         \
    }                                                                         \
    else                                                                      \
    {                                                                         \
        if (strcmp(act,"decrement") == 0)                                     \
        {                                                                     \
            if (self->head - 1 < 0)                                           \
                self->head = ((--self->head) % getArrayCapacity_##T(self))    \
                                             + getArrayCapacity_##T(self);    \
            else                                                              \
                self->head = ((--self->head) % getArrayCapacity_##T(self));   \
        }                                                                     \
    }                                                                         \
}                                                                             \
                                                                              \
static void _growArray_##T(deArray_##T *self)                                 \
{                                                                             \
    self->head = 0;                                                           \
    self->tail = getArraySize_##T(self);                                      \
    self->capacity = getArrayFactor_##T(self) * getArrayCapacity_##T(self);   \
    self->store = (T *) realloc(self->store,                                  \
                                getArrayCapacity_##T(self) * sizeof(T));      \
}

#define X(T) deArrayPolymorphismC(T);
X(char)                                                                       \
X(double)                                                                     \
X(int)
#undef X
#undef deArrayPolymorphismC

#ifdef __cplusplus
}
#endif
