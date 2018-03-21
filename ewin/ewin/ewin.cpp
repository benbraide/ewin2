#include <windows.h>

#include "property/value_ref_property.h"
#include "property/point_property.h"
#include "property/transform_property.h"
#include "property/variadic_property.h"

#include "common/error_object.h"

using namespace std;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow){
	ewin::common::error_object err;
	bool has = err;
	ewin::common::error eval = err;
	DWORD dwe = err;
	HRESULT hr = err;
	std::wstring swe = err;
	ewin::common::error_object::error_throw_policy_type etp = err;
	try{
		err = ERROR_ACCESS_DENIED;
	}
	catch (ewin::common::error_object *e){
		has = *e;
		eval = *e;
		dwe = *e;
		hr = *e;
		swe = *e;
	}

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

	struct man3{
		ewin::property::single_transform<man3, int, bool> is_even;
		ewin::property::transform<
			man3,
			ewin::property::object::access_type::nil,
			ewin::common::variadic_type_pair<int, bool>,
			ewin::common::variadic_type_pair<float, int>
		> query;
		man3(){
			is_even.set_manager_(EWIN_PROP_HANDLER_DEF(man3));
			query.set_manager_(EWIN_PROP_HANDLER_DEF(man3));
		}

		void handle_property_(void *prop, void *arg, ewin::property::object::access_type access){
			if (EWIN_IS_ANY(access, ewin::property::object::access_type::alert | ewin::property::object::access_type::validate))
				return;

			auto tinfo = static_cast<ewin::property::object::indexed_target_info_type *>(arg);
			if (prop == &query){
				if (tinfo->index == 0u){
					auto info = static_cast<ewin::property::object::query_return_info_type<const int *, bool> *>(tinfo->target);
					info->return_value = ((*info->query % 2) != 0);
				}
				else{
					auto info = static_cast<ewin::property::object::query_return_info_type<const float *, int> *>(tinfo->target);
					info->return_value = static_cast<int>(*info->query + 0.5f);
				}
			}
			else{
				auto info = static_cast<ewin::property::object::query_return_info_type<const int *, bool> *>(tinfo->target);
				info->return_value = ((*info->query % 2) == 0);
			}
		}
	};

	man3 mani3;
	auto m3v = mani3.is_even[36];
	auto m3v2 = mani3.is_even[9];

	auto m3v3 = mani3.query[36];
	auto m3v4 = mani3.query[9];

	auto m3v5 = mani3.query[36.4f];
	auto m3v6 = mani3.query[9.5f];

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
