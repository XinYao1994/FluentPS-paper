#include "ps/ps.h"
// test_connection
int main(int argc, char *argv[]) {
  ps::Start(0);
  // do nothing
  ps::Finalize(0, true);
  return 0;
}
