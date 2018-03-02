#include <windows.h>

#include "property/value_ref_property.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow){
	struct man{
		man(){
			val.set_ref_(&val_);
		}

		ewin::property::value_ref<man, int> val;
		int val_ = 9;
	};

	struct man2{
		struct s{
			int a, b;
		};

		man2(){
			val.set_ref_(&val_);
		}

		ewin::property::value_ref<man2, s> val;
		s val_ = { 9, 18 };
	};

	man mani;
	mani.val + 18;
	mani.val += 18;
	auto b = (mani.val == 18), c = (27 == mani.val);

	man2 mani2;
	mani2.val->b += 18;
	auto b2 = (mani2.val->b == 18), c2 = (27 == mani2.val->b);

	return 0;
}
