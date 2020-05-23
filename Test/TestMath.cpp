#include "TestNet.h"


void TestNet::TestMath()
{
	double d = 50 * 1.414f;
	int32_t md = d;
	cout << md << endl;
	cout << 19322 % (md) << endl;
	cout << 19322 / (md*0.1) << endl;
}