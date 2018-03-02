#pragma once

#ifndef EWIN_VALUE_PROPERTY_H
#define EWIN_VALUE_PROPERTY_H

#include "property_object.h"

#define EWIN_VAL_PROP_TMPL(n, ...) n<manager_type, unused_type, __VA_ARGS__>
#define EWIN_VAL_PROP_ENABLE_GENERIC(t, r) std::enable_if_t<(t), r>
#define EWIN_VAL_PROP_CHECK_UNIQUE(n, ...) (!std::is_same_v<target_type, EWIN_VAL_PROP_TMPL(n, __VA_ARGS__)>)

#define EWIN_VAL_PROP_OP(t, r, op)\
template <typename target_type, typename unused_type = value_type>\
EWIN_VAL_PROP_ENABLE_GENERIC((t), r) operator op(const target_type &target) const{\
	return (operator value_type() op static_cast<value_type>(target));\
}

#define EWIN_VAL_PROP_ARITHMETIC_OP(t, r, op)\
EWIN_VAL_PROP_OP((t), value_type, op)\
template <typename target_type, typename unused_type = value_type>\
EWIN_VAL_PROP_ENABLE_GENERIC((t), r &) operator op ## =(const target_type &target){\
	return (*this = (*this op target));\
}

#define EWIN_VAL_PROP_FRIEND_OP(t, n, r, op, ...)\
template <typename target_type, typename unused_type = value_type>\
friend EWIN_VAL_PROP_ENABLE_GENERIC(((t) && EWIN_VAL_PROP_CHECK_UNIQUE(n, __VA_ARGS__)), r)\
operator op(const target_type &left, const EWIN_VAL_PROP_TMPL(n, __VA_ARGS__) &right){\
	return (static_cast<value_type>(left) op static_cast<value_type>(right));\
}

#define EWIN_VAL_PROP_RELATIONAL_OPERATORS \
EWIN_VAL_PROP_OP(std::is_arithmetic_v<unused_type>, bool, <)\
EWIN_VAL_PROP_OP(std::is_arithmetic_v<unused_type>, bool, <=)\
EWIN_VAL_PROP_OP(std::is_arithmetic_v<unused_type>, bool, ==)\
EWIN_VAL_PROP_OP(std::is_arithmetic_v<unused_type>, bool, !=)\
EWIN_VAL_PROP_OP(std::is_arithmetic_v<unused_type>, bool, >=)\
EWIN_VAL_PROP_OP(std::is_arithmetic_v<unused_type>, bool, >)

#define EWIN_VAL_PROP_NUMERIC_OPERATORS(r)\
EWIN_VAL_PROP_ARITHMETIC_OP(std::is_arithmetic_v<unused_type>, r, +)\
EWIN_VAL_PROP_ARITHMETIC_OP(std::is_arithmetic_v<unused_type>, r, -)\
EWIN_VAL_PROP_ARITHMETIC_OP(std::is_arithmetic_v<unused_type>, r, *)\
EWIN_VAL_PROP_ARITHMETIC_OP(std::is_arithmetic_v<unused_type>, r, /)\
\
EWIN_VAL_PROP_ARITHMETIC_OP(std::is_integral_v<unused_type>, r, %)\
EWIN_VAL_PROP_ARITHMETIC_OP(std::is_integral_v<unused_type>, r, <<)\
EWIN_VAL_PROP_ARITHMETIC_OP(std::is_integral_v<unused_type>, r, >>)\
EWIN_VAL_PROP_ARITHMETIC_OP(std::is_integral_v<unused_type>, r, &)\
EWIN_VAL_PROP_ARITHMETIC_OP(std::is_integral_v<unused_type>, r, |)\
EWIN_VAL_PROP_ARITHMETIC_OP(std::is_integral_v<unused_type>, r, ^)

#define EWIN_VAL_PROP_FRIENDL_OPERATORS(n, ...)\
EWIN_VAL_PROP_FRIEND_OP(std::is_arithmetic_v<unused_type>, n, bool, <, __VA_ARGS__)\
EWIN_VAL_PROP_FRIEND_OP(std::is_arithmetic_v<unused_type>, n, bool, <=, __VA_ARGS__)\
EWIN_VAL_PROP_FRIEND_OP(std::is_arithmetic_v<unused_type>, n, bool, ==, __VA_ARGS__)\
EWIN_VAL_PROP_FRIEND_OP(std::is_arithmetic_v<unused_type>, n, bool, !=, __VA_ARGS__)\
EWIN_VAL_PROP_FRIEND_OP(std::is_arithmetic_v<unused_type>, n, bool, >=, __VA_ARGS__)\
EWIN_VAL_PROP_FRIEND_OP(std::is_arithmetic_v<unused_type>, n, bool, >, __VA_ARGS__)\
\
EWIN_VAL_PROP_FRIEND_OP(std::is_arithmetic_v<unused_type>, n, value_type, +, __VA_ARGS__)\
EWIN_VAL_PROP_FRIEND_OP(std::is_arithmetic_v<unused_type>, n, value_type, -, __VA_ARGS__)\
EWIN_VAL_PROP_FRIEND_OP(std::is_arithmetic_v<unused_type>, n, value_type, *, __VA_ARGS__)\
EWIN_VAL_PROP_FRIEND_OP(std::is_arithmetic_v<unused_type>, n, value_type, /, __VA_ARGS__)\
\
EWIN_VAL_PROP_FRIEND_OP(std::is_integral_v<unused_type>, n, value_type, %, __VA_ARGS__)\
EWIN_VAL_PROP_FRIEND_OP(std::is_integral_v<unused_type>, n, value_type, <<, __VA_ARGS__)\
EWIN_VAL_PROP_FRIEND_OP(std::is_integral_v<unused_type>, n, value_type, >>, __VA_ARGS__)\
EWIN_VAL_PROP_FRIEND_OP(std::is_integral_v<unused_type>, n, value_type, &, __VA_ARGS__)\
EWIN_VAL_PROP_FRIEND_OP(std::is_integral_v<unused_type>, n, value_type, |, __VA_ARGS__)\
EWIN_VAL_PROP_FRIEND_OP(std::is_integral_v<unused_type>, n, value_type, ^, __VA_ARGS__)

namespace ewin::property{
	template <class manager_type, class value_type, object::access_type access = object::access_type::nil>
	class value : public managed<manager_type>{
	public:
		typedef value_type value_type;
		typedef managed<manager_type> managed_type;

		value_type operator()() const{
			return operator value_type();
		}

		template <typename target_type>
		value &operator()(const target_type &target){
			return operator=(target);
		}

		operator value_type() const{
			EWIN_PROP_CHECK_ACCESS(object::access_type::read);
			if (ref_ != nullptr){//Return ref
				EWIN_PROP_SAFE_ALERT(object::access_type::read);
				return *ref_;
			}
			
			EWIN_PROP_REQUIRE_CALLBACK;
			EWIN_PROP_RETURN_VALUE_UNNAMED_DEF(value_type);
		}

		template <typename target_type>
		value &operator =(const target_type &target){
			EWIN_PROP_CHECK_ACCESS(object::access_type::write);
			if (ref_ == nullptr){
				EWIN_PROP_REQUIRE_CALLBACK;
				EWIN_PROP_WRITE_VALUE_DEF(static_cast<value_type>(target));
			}
			else{//Update ref
				EWIN_PROP_SAFE_ALERT(object::access_type::write);
				*ref_ = static_cast<value_type>(target);
			}

			return *this;
		}

		EWIN_VAL_PROP_RELATIONAL_OPERATORS;
		EWIN_VAL_PROP_NUMERIC_OPERATORS(value);

		EWIN_VAL_PROP_FRIENDL_OPERATORS(value, access);

		static const object::access_type access = access;

	protected:
		friend manager_type;

		value() = default;

		explicit value(managed_type::callback_type callback)
			: managed_type(callback){}

		explicit value(value_type *ref)
			: ref_(ref){}

		value(managed_type::callback_type callback, value_type *ref)
			: managed_type(callback), ref_(ref){}

		value(value &&target)
			: managed_type(std::move(target)), ref_(target.ref_){
			target.ref_ = nullptr;
		}

		value &operator =(value &&target){
			managed_type::operator=(std::move(target));
			ref_ = target.ref_;
			target.ref_ = nullptr;
			return *this;
		}

		void set_ref_(value_type *ref){
			ref_ = ref;
		}

		void set_manager_(managed_type::callback_type callback, value_type *ref){
			managed_type::set_manager_(callback);
			set_ref_(ref);
		}

		value_type *ref_;
	};
}

#endif /* !EWIN_VALUE_PROPERTY_H */
