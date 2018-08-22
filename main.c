#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <stdbool.h>
#include "arabic-reshaper.h"

int main(int argc, char** argv) {
  errorif (!setlocale(LC_ALL, "en_US.UTF-8"),
           L"Can't set the specified locale! Check LANG, LC_CTYPE, LC_ALL.\n");

  char* opt = getenv("ARRESHOPT");
  bool rev = false;
  bool uns = false;
  if (opt) for (char* o = opt; *o; ++o) {
    switch (*o) {
    case 'r': rev = true; break;
    case 'u': uns = true; break;
    }
  }

  char     buffer[BUFSIZ];
  size_t   len;
  wchar_t* str;

  while(fgets(buffer, sizeof(buffer), stdin) != 0) {
    str = new_str(buffer, &len);
    if (uns) unshape(str, len);
    else reshape(str, len);
    if (rev) reverse_str(str, len);
    wprintf(L"%S", str);
    if (str) free(str);
  }

  return 0;
}
