#include <stdio.h>
#include <emscripten.h>

int main() {
  MAIN_THREAD_EM_ASM(FS.writeFile('file', 'foobar'); console.log(FS.stat('file')););
  return 0;
}
