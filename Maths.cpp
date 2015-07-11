#include "Maths.h"

void calcPowers(ll deg[], ll base, int n) {
	deg[0] = 1;
	for (int i = 1; i <= n; ++i) {
		deg[i] = deg[i - 1] * base;
	}
}
