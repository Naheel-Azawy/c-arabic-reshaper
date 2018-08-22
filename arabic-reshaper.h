#ifndef __ARABIC_RESHAPER_H
#define __ARABIC_RESHAPER_H

#include <wchar.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define error(s, ...) { fwprintf(stderr, L"" RED s RESET "\n", ##__VA_ARGS__); exit(EXIT_FAILURE); }
#define errorif(c, s, ...) if (c) error(s, ##__VA_ARGS__);

#define info(s, ...) { fwprintf(stdout, L"" BLU s RESET "\n", ##__VA_ARGS__); }
#define infoif(c, s, ...) if (c) info(s, ##__VA_ARGS__);

int is_ar(wchar_t c);

wchar_t get_unshaped_glphy(wchar_t target);

wchar_t get_reshaped_glphy(wchar_t target, int location);

int get_glphy_type(wchar_t target);

wchar_t* reshape(wchar_t* str, size_t len);

wchar_t* unshape(wchar_t* str, size_t len);

void reverse_str(wchar_t *str, size_t len);

wchar_t* wstr_clone(wchar_t* in, size_t* len);

wchar_t* new_str(char* in, size_t* len);

wchar_t* new_str_n(char* in, size_t in_len, size_t* len);

#endif
