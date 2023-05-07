#ifndef __LINK_H
#define __LINK_H
#define NAME(name) name
#define NAME_STR(name) #name
#define NAME_LABEL(name) name##:
#define ENTRY(name) \
    .global NAME(name); \
    NAME_LABEL(name)
#endif
