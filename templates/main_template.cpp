#include <fstream>
#include <iostream>
#include <ctime>
#include <cstdlib>

extern class IO io;
bool useFastIO = false;
istream * pin;
ostream * pout;

void solve(std::istream& in, std::ostream& out);
int main() {
    srand(time(NULL));
    using namespace std;
    ios_base::sync_with_stdio(0);
    cin.tie(0);

#ifdef CAIDE_STDIN
    istream& in = cin;
    useFastIO = true;
#else
    ifstream in(CAIDE_IN_FILE);
    useFastIO = false;
#endif

#ifdef CAIDE_STDOUT
    ostream& out = cout;
#else
    ofstream out(CAIDE_OUT_FILE);
#endif
    out << fixed << setprecision(16);
    pin = &in; pout = &out;
    solve(in, out);
    return 0;
}

