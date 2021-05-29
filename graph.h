#include <cassert>
#include <vector>
using namespace std;

const bool DEBUG = 1;
const bool LOG = 1;
#define ASSERT(x) if (DEBUG) assert(x)
#define debug_printf(...) do { if (DEBUG) fprintf(stderr,  __VA_ARGS__); } while (0)
#define log_printf(...) do { if (LOG) printf(__VA_ARGS__); } while (0)

const int NODE_NUM_MAX = 200005;
const int PKG_NUM_MAX = 200005;
const int PKG_SIZE = 1572;

int read_graph(char* edge_path, char* weight_path);
int output(char* answer_path);