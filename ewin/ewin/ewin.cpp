#include <windows.h>

#include "property/value_ref_property.h"
#include "property/point_property.h"
#include "property/rect_property.h"
#include "property/variadic_property.h"

using namespace std;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow){
	struct man{
		man()
			: ptr_(&val_){
			val.set_ref_(&val_);
			val2.set_ref_(&val2_);
			ptr.set_ref_(&ptr_);
		}

		ewin::property::value_ref<man, int> val, val2;
		ewin::property::value<man, int *> ptr;
		int val_ = 9, val2_ = 18;
		int *ptr_;
	};

	struct man2{
		struct s{
			int a, b;
		};

		man2(){
			val.set_ref_(&val_);
			val2.set_ref_(&i_);
			val2.set_ref_(&b_);
			pt.set_ref_(&pt_);
			pt2.set_ref_(&pt_);
		}

		ewin::property::value_ref<man2, s> val;
		s val_ = { 9, 18 };
		ewin::property::variadic<man2, ewin::property::object::access_type::nil, int, bool> val2;
		ewin::property::point<man2, ::POINT, int> pt, pt2;
		int i_ = 45;
		bool b_ = false;
		::POINT pt_;
	};

	int i = 9;
	const float &ic = static_cast<const float &>(i);

	man mani;
	mani.val + 18;
	mani.val += 18;
	auto b = (mani.val == 18), c = (27 == mani.val);
	mani.val = mani.val2;
	auto d = (mani.ptr == nullptr), ewin = (nullptr == mani.ptr);

	man2 mani2;
	mani2.val->b += 18;
	auto b2 = (mani2.val->b == 18), c2 = (27 == mani2.val->b);

	int iv = mani2.val2;
	bool bv = mani2.val2;

	mani2.pt = mani2.pt2;

	return 0;
}
