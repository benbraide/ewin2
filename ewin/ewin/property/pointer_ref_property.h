#pragma once

#ifndef EWIN_POINTER_REF_PROPERTY_H
#define EWIN_POINTER_REF_PROPERTY_H

#include "value_property.h"

namespace ewin::property{
	template <class manager_type, class value_type, object::access_type access = object::access_type::nil>
	class pointer_ref : public value<manager_type, value_type **, access>{
	public:
		typedef value_type value_type;
		typedef value<manager_type, value_type **, access> value_prop_type;

		value_type *operator()() const{
			return operator value_type();
		}

		template <typename target_type>
		pointer_ref &operator()(const target_type &target){
			return operator=(target);
		}

		operator value_type *() const{
			EWIN_PROP_CHECK_ACCESS(object::access_type::read);
			if (value_prop_type::ref_ == nullptr)
				throw common::error::property_not_initialized;

			EWIN_PROP_SAFE_ALERT(object::access_type::read);
			return *value_prop_type::ref_;
		}

		template <typename target_type>
		pointer_ref &operator =(const target_type &target){
			return operator=(&target);
		}

		pointer_ref &operator =(const value_type *target){
			EWIN_PROP_CHECK_ACCESS(object::access_type::write);
			if (value_prop_type::ref_ == nullptr)
				throw common::error::property_not_initialized;

			*value_prop_type::ref_ = const_cast<value_type *>(target);
			EWIN_PROP_SAFE_ALERT(object::access_type::write);

			return *this;
		}

		template <typename unused_type = value_type>
		EWIN_VAL_PROP_ENABLE_GENERIC(!std::is_fundamental_v<unused_type>, value_type) *operator ->() const{
			return operator value_type *();
		}

		value_type &operator *() const{
			return *operator value_type *();
		}

		EWIN_VAL_PROP_RELATIONAL_OPERATORS;
		EWIN_VAL_PROP_NUMERIC_OPERATORS(pointer_ref);

		EWIN_VAL_PROP_FRIENDL_OPERATORS(pointer_ref, access);

	protected:
		friend manager_type;

		pointer_ref() = default;

		explicit pointer_ref(typename value_prop_type::callback_type callback)
			: value_prop_type(callback){}

		explicit pointer_ref(value_type **ref)
			: value_prop_type(ref){}

		pointer_ref(typename value_prop_type::callback_type callback, value_type **ref)
			: value_prop_type(callback, ref){}

		pointer_ref(pointer_ref &&target)
			: value_prop_type(std::move(target)){}

		pointer_ref &operator =(pointer_ref &&target){
			value_prop_type::operator=(std::move(target));
			return *this;
		}
	};
}

#endif /* !EWIN_POINTER_REF_PROPERTY_H */
