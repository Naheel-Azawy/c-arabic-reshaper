#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <wchar.h>
#include <string.h>
#include <stdbool.h>
#include "arabic-glyphies.h"
#include "arabic-reshaper.h"

bool is_ar(wchar_t c) {
  return (c >= 0x0622 && c <= 0x063A)
    || (c >= 0x0641 && c <= 0x064A)
    || (c == 0x066E) || (c == 0x0671)
    || (c == 0x06AA) || (c == 0x06C1)
    || (c == 0x06E4);
}

bool is_tashkeel(wchar_t c) {
  return
    (c >= 0x0610 && c <= 0x061A) ||
    (c >= 0x064B && c <= 0x065F) ||
    (c == 0x0670)                ||
    (c >= 0x06D6 && c <= 0x06DC) ||
    (c >= 0x06DF && c <= 0x06E8) ||
    (c >= 0x06EA && c <= 0x06ED) ||
    (c >= 0x08D4 && c <= 0x08E1) ||
    (c >= 0x08D4 && c <= 0x08ED) ||
    (c >= 0x08E3 && c <= 0x08FF);
}

//#define GET_UNSHAPED_GLPHY_OLD
wchar_t get_unshaped_glphy(wchar_t target) {
#ifdef GET_UNSHAPED_GLPHY_OLD
  for (int i = 0; i < ARABIC_GLPHIES_LEN; ++i) {
    for (int j = 1; j < 5; ++j) {
      if (ARABIC_GLPHIES[i][j] == target) {
        return ARABIC_GLPHIES[i][0];
      }
    }
  }
#else
  if (target >= ARABIC_GLPHIES_REV_1_MIN && target <= ARABIC_GLPHIES_REV_1_MAX) {
    return ARABIC_GLPHIES_REV_1[target - ARABIC_GLPHIES_REV_1_MIN];
  } else if (target >= ARABIC_GLPHIES_REV_2_MIN && target <= ARABIC_GLPHIES_REV_2_MAX) {
    return ARABIC_GLPHIES_REV_2[target - ARABIC_GLPHIES_REV_2_MIN];
  } else if (target >= ARABIC_GLPHIES_REV_0_MIN && target <= ARABIC_GLPHIES_REV_0_MAX) {
    for (int i = 0; i < ARABIC_GLPHIES_REV_0_LEN; ++i) {
      if (target == ARABIC_GLPHIES_REV_0[i][0]) {
        return ARABIC_GLPHIES_REV_0[i][1];
      }
    }
  }
#endif
  return target;
}

wchar_t get_reshaped_glphy(wchar_t target, int location) {
  if (target < ARABIC_GLPHIES_MIN || target > ARABIC_GLPHIES_MAX) {
    return target;
  } else {
    int res = ARABIC_GLPHIES[target - ARABIC_GLPHIES_MIN][location];
    return res == 0 ? target : res;
  }
}

int get_glphy_type(wchar_t target) {
  if (target < ARABIC_GLPHIES_MIN || target > ARABIC_GLPHIES_MAX) {
    return 2;
  } else {
    return ARABIC_GLPHIES[target - ARABIC_GLPHIES_MIN][5];
  }
}

wchar_t* reshape(wchar_t* str, size_t len, bool unshape) {

  // Double check
  if (str == 0 || *str == 0 || len == 0) {
    return str;
  }

  // Keep the previous char
  wchar_t prev = str[0], tmp;
  if (unshape) {
    prev = get_unshaped_glphy(prev);
  }

  // First char is always a starting char
  str[0] = get_reshaped_glphy(str[0], 2);

  // Iterate from the second till the second to last
  int i;
  for (i = 1; i < len - 1; ++i) {
    if (unshape) {
      str[i] = get_unshaped_glphy(str[i]);
      str[i + 1] = get_unshaped_glphy(str[i + 1]);
    }
    // Store the current char as it will be needed in the next iteration as 'prev'
    tmp = str[i];
    if(get_glphy_type(prev) == 2 || !is_ar(prev)) { // If prev has only 2 types OR it is not arabic (start of the word)
      if (!is_ar(str[i + 1])) { // If not arabic (end of the word)
        str[i] = get_reshaped_glphy(str[i], 1);
      } else {
        // If the letter has only 2 shapes, then it doesnt matter which position it is, It'll be always the second form
        str[i] = get_reshaped_glphy(str[i], 2);
      }
    } else if (!is_ar(str[i + 1])) { // If not arabic (end of the word)
      // Put the right form of the character, 4 for the last letter in the str
      str[i] = get_reshaped_glphy(str[i], 4);
    } else {
      // Then it should be in the middle which should be placed in its right form 3
      str[i] = get_reshaped_glphy(str[i], 3);
    }
    prev = tmp;
  }

  // check for the last letter before last has 2 forms
  if (len >= 2) {
    if (unshape) {
      str[i] = get_unshaped_glphy(str[i]);
    }
    if(get_glphy_type(prev) == 2) {
      // If the letter has only 2 shapes, then it doesnt matter which position it is, It'll be always the second form
      str[i] = get_reshaped_glphy(str[len - 1], 1);
    } else {
      // Put the right form of the character, 4 for the last letter in the str
      str[i] = get_reshaped_glphy(str[len - 1], 4);
    }
  }

  return str;
}



wchar_t* unshape(wchar_t* str, size_t len) {

  // Double check
  if (str == 0 || *str == 0 || len == 0) {
    return str;
  }

  int i;
  for (i = 0; i < len; ++i) {
    str[i] = get_unshaped_glphy(str[i]);
  }

  return str;
}

void reverse_str(wchar_t *str, size_t len) {
  if (str == 0 || *str == 0) {
    return;
  }

  wchar_t* start = str;
  wchar_t* end = start + len - 1; // -1 for \0
  wchar_t  tmp;

  while (end > start) {
    tmp = *start;
    *start = *end;
    *end = tmp;

    ++start;
    --end;
  }
}

wchar_t* wstr_clone(wchar_t* in, size_t* len) {
  *len = wcslen(in);
  wchar_t* out = malloc(sizeof(wchar_t) * (*len + 1));
  wcscpy(out, in);
  return out;
}

wchar_t* new_str(char* in, size_t* len) {
  *len = strlen(in);
  wchar_t* out = malloc(sizeof(wchar_t) * (*len + 1));
  *len = mbstowcs(out, in, *len + 1);
  return out;
}

wchar_t* new_str_n(char* in, size_t in_len, size_t* len) {
  wchar_t* out = malloc(sizeof(wchar_t) * (in_len + 1));
  *len = mbstowcs(out, in, in_len + 1);
  return out;
}

