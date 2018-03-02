#pragma once

#ifndef EWIN_VALUE_REF_PROPERTY_H
#define EWIN_VALUE_REF_PROPERTY_H

#include "value_property.h"

namespace ewin::property{
	template <class manager_type, class value_type, object::access_type access = object::access_type::nil>
	class value_ref : public value<manager_type, value_type, access>{
	public:
		typedef value<manager_type, value_type, access> value_prop_type;

		value_type *operator()() const{
			return operator value_type();
		}

		template <typename target_type>
		value_ref &operator()(const target_type &target){
			return operator=(target);
		}

		operator value_type() const{
			return *operator value_type *();
		}

		operator value_type *() const{
			EWIN_PROP_CHECK_ACCESS(object::access_type::read);
			if (value_prop_type::ref_ == nullptr)
				throw common::error::property_not_initialized;

			EWIN_PROP_SAFE_ALERT(object::access_type::read);
			return value_prop_type::ref_;
		}

		template <typename target_type>
		value_ref &operator =(const target_type &target){
			EWIN_PROP_CHECK_ACCESS(object::access_type::write);
			if (value_prop_type::ref_ == nullptr)
				throw common::error::property_not_initialized;

			EWIN_PROP_SAFE_ALERT(object::access_type::write);
			*value_prop_type::ref_ = static_cast<value_type>(target);

			return *this;
		}

		template <typename unused_type = value_type>
		EWIN_VAL_PROP_ENABLE_GENERIC(!std::is_fundamental_v<unused_type>, value_type) *operator ->() const{
			return operator value_type *();
		}

		EWIN_VAL_PROP_RELATIONAL_OPERATORS;
		EWIN_VAL_PROP_NUMERIC_OPERATORS(value_ref);

		EWIN_VAL_PROP_FRIENDL_OPERATORS(value_ref, access);

	protected:
		friend manager_type;

		value_ref() = default;

		explicit value_ref(typename value_prop_type::callback_type callback)
			: value_prop_type(callback){}

		explicit value_ref(value_type *ref)
			: value_prop_type(ref){}

		value_ref(typename value_prop_type::callback_type callback, value_type *ref)
			: value_prop_type(callback, ref){}

		value_ref(value_ref &&target)
			: value_prop_type(std::move(target)){}

		value_ref &operator =(value_ref &&target){
			value_prop_type::operator=(std::move(target));
			return *this;
		}
	};
}

#endif /* !EWIN_VALUE_REF_PROPERTY_H */
