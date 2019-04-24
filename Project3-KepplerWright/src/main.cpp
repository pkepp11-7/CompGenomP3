//Patrick Keppler and Connor Wright, 2019
#include "ReadMapApp.h"
int main(int argc, char * argv[])
{
  assert(argc > 3);

  ReadMapApp mapApp(argv[1], argv[2], argv[3]);
  mapApp.run();

  return 0;
}
