#pragma once

#ifndef EWIN_FLAG_PROPERTY_H
#define EWIN_FLAG_PROPERTY_H

#include "value_property.h"

namespace ewin::property{
	template <class manager_type, class value_type, object::access_type access = object::access_type::nil>
	class flag : public value<manager_type, value_type, access>{
	public:
		typedef value<manager_type, value_type, access> value_prop_type;

		value_type operator()() const{
			return operator value_type();
		}

		template <typename target_type>
		flag &operator()(const target_type &target){
			return operator=(target);
		}

		operator value_type() const{
			return value_prop_type::operator value_type();
		}

		template <typename target_type>
		flag &operator =(const target_type &target){
			value_prop_type::operator=(target);
			return *this;
		}

		flag &operator =(const flag &target){
			return ((&target == this) ? *this : operator=(*target.operator value_type()));
		}

		template <typename target_type>
		flag &operator +=(const target_type &target){
			EWIN_PROP_CHECK_ACCESS(object::access_type::list_add);
			if (value_prop_type::ref_ == nullptr){
				EWIN_PROP_REQUIRE_CALLBACK;
				EWIN_PROP_WRITE_VALUE((value_type)target, object::access_type::list_add);
			}
			else{//Update ref
				EWIN_SET(*value_prop_type::ref_, (value_type)target);
				EWIN_PROP_SAFE_ALERT(object::access_type::list_add);
			}

			return *this;
		}

		template <typename target_type>
		flag &operator -=(const target_type &target){
			EWIN_PROP_CHECK_ACCESS(object::access_type::list_remove);
			if (value_prop_type::ref_ == nullptr){
				EWIN_PROP_REQUIRE_CALLBACK;
				EWIN_PROP_WRITE_VALUE((value_type)target, object::access_type::list_remove);
			}
			else{//Update ref
				EWIN_REMOVE(*value_prop_type::ref_, (value_type)target);
				EWIN_PROP_SAFE_ALERT(object::access_type::list_remove);
			}

			return *this;
		}

		template <typename target_type>
		value_type operator +(const target_type &target) const{
			return EWIN_SET_V((value_type)(*this), (value_type)target);
		}

		template <typename target_type>
		value_type operator -(const target_type &target) const{
			return EWIN_REMOVE_V((value_type)(*this), (value_type)target);
		}

		template <typename target_type>
		bool operator [](const target_type &target) const{
			return EWIN_IS_ANY((value_type)(*this), (value_type)target);
		}

		template <typename target_type>
		bool operator ==(const target_type &target) const{
			return EWIN_IS((value_type)(*this), (value_type)target);
		}

		template <typename target_type>
		bool operator !=(const target_type &target) const{
			return !EWIN_IS((value_type)(*this), (value_type)target);
		}

	protected:
		friend manager_type;

		flag() = default;

		explicit flag(typename value_prop_type::callback_type callback)
			: value_prop_type(callback){}

		explicit flag(value_type *ref)
			: value_prop_type(ref){}

		flag(typename value_prop_type::callback_type callback, value_type *ref)
			: value_prop_type(callback, ref){}

		flag(flag &&target)
			: value_prop_type(std::move(target)){}

		flag &operator =(flag &&target){
			value_prop_type::operator=(std::move(target));
			return *this;
		}
	};
}

#endif /* !EWIN_FLAG_PROPERTY_H */
