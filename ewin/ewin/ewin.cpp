#include <windows.h>

#include "property/value_property.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow){
	struct man{
		man(){
			val.set_ref_(&val_);
		}

		ewin::property::value<man, int> val;
		int val_ = 9;
	};

	man mani;
	mani.val + 18;
	mani.val += 18;
	auto b = (mani.val == 18), c = (27 == mani.val);

	return 0;
}
