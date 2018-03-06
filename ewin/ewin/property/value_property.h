#pragma once

#ifndef EWIN_VALUE_PROPERTY_H
#define EWIN_VALUE_PROPERTY_H

#include "property_object.h"

#define EWIN_VAL_PROP_TMPL(n, ...) n<manager_type, __VA_ARGS__>
#define EWIN_VAL_PROP_ENABLE_GENERIC(t, r) std::enable_if_t<(t), r>
#define EWIN_VAL_PROP_CHECK_UNIQUE(n, ...) (!std::is_same_v<target_type, EWIN_VAL_PROP_TMPL(n, __VA_ARGS__)>)

#define EWIN_VAL_PROP_OP(n, vt, r, op, ...)\
template <typename target_type, typename unused_type = value_type>\
EWIN_VAL_PROP_ENABLE_GENERIC(EWIN_VAL_PROP_CHECK_UNIQUE(n, unused_type, __VA_ARGS__), r)\
operator op(const target_type &target) const{\
	return (operator vt() op static_cast<vt>(target));\
}

#define EWIN_VAL_PROP_ARITHMETIC_OP(n, vt, r, op, ...)\
EWIN_VAL_PROP_OP(n, vt, r, op, __VA_ARGS__)\
template <typename target_type, typename unused_type = value_type>\
EWIN_VAL_PROP_ENABLE_GENERIC(EWIN_VAL_PROP_CHECK_UNIQUE(n, unused_type, __VA_ARGS__), n) &\
operator op ## =(const target_type &target){\
	return (*this = (*this op target));\
}

#define EWIN_VAL_PROP_REL_OP(n, vt, op, c, w, ...)\
template <typename target_type, typename unused_type = value_type>\
EWIN_VAL_PROP_ENABLE_GENERIC(EWIN_VAL_PROP_CHECK_UNIQUE(n, unused_type, __VA_ARGS__), bool)\
operator op(const target_type &target) const{\
	return (c(operator vt(), static_cast<vt>(target)) == w);\
}

#define EWIN_VAL_PROP_REL_OP2(n, vt, op, c, w, x, ...)\
template <typename target_type, typename unused_type = value_type>\
EWIN_VAL_PROP_ENABLE_GENERIC(EWIN_VAL_PROP_CHECK_UNIQUE(n, unused_type, __VA_ARGS__), bool)\
operator op(const target_type &target) const{\
	auto v = c(operator vt(), static_cast<vt>(target));\
	return (v == w || v == x);\
}

#define EWIN_VAL_PROP_FRIEND_OP(n, vt, r, op)\
template <typename target_type>\
friend r operator op(const target_type &left, const n &right){\
	return (static_cast<vt>(left) op static_cast<vt>(right));\
}

#define EWIN_VAL_PROP_FRIEND_REL_OP(n, vt, op, c, w)\
template <typename target_type>\
friend bool operator op(const target_type &left, const n &right){\
	return (c(static_cast<vt>(left), static_cast<vt>(right)) == w);\
}

#define EWIN_VAL_PROP_FRIEND_REL_OP2(n, vt, op, c, w, x)\
template <typename target_type>\
friend bool operator op(const target_type &left, const n &right){\
	auto v = c(static_cast<vt>(left), static_cast<vt>(right));\
	return (v == w || v == x);\
}

#define EWIN_VAL_PROP_RELATIONAL_OPERATORS(n, vt, ...)\
EWIN_VAL_PROP_OP(n, vt, bool, <, __VA_ARGS__)\
EWIN_VAL_PROP_OP(n, vt, bool, <=, __VA_ARGS__)\
EWIN_VAL_PROP_OP(n, vt, bool, ==, __VA_ARGS__)\
EWIN_VAL_PROP_OP(n, vt, bool, !=, __VA_ARGS__)\
EWIN_VAL_PROP_OP(n, vt, bool, >=, __VA_ARGS__)\
EWIN_VAL_PROP_OP(n, vt, bool, >, __VA_ARGS__)

#define EWIN_VAL_PROP_FRIEND_RELATIONAL_OPERATORS(n, vt)\
EWIN_VAL_PROP_FRIEND_OP(n, vt, bool, <)\
EWIN_VAL_PROP_FRIEND_OP(n, vt, bool, <=)\
EWIN_VAL_PROP_FRIEND_OP(n, vt, bool, ==)\
EWIN_VAL_PROP_FRIEND_OP(n, vt, bool, !=)\
EWIN_VAL_PROP_FRIEND_OP(n, vt, bool, >=)\
EWIN_VAL_PROP_FRIEND_OP(n, vt, bool, >)

#define EWIN_VAL_PROP_NUMERIC_OPERATORS(n, vt, r, ...)\
EWIN_VAL_PROP_ARITHMETIC_OP(n, vt, r, +, __VA_ARGS__)\
EWIN_VAL_PROP_ARITHMETIC_OP(n, vt, r, -, __VA_ARGS__)\
EWIN_VAL_PROP_ARITHMETIC_OP(n, vt, r, *, __VA_ARGS__)\
EWIN_VAL_PROP_ARITHMETIC_OP(n, vt, r, /, __VA_ARGS__)\
\
EWIN_VAL_PROP_ARITHMETIC_OP(n, vt, r, %, __VA_ARGS__)\
EWIN_VAL_PROP_ARITHMETIC_OP(n, vt, r, <<, __VA_ARGS__)\
EWIN_VAL_PROP_ARITHMETIC_OP(n, vt, r, >>, __VA_ARGS__)\
EWIN_VAL_PROP_ARITHMETIC_OP(n, vt, r, &, __VA_ARGS__)\
EWIN_VAL_PROP_ARITHMETIC_OP(n, vt, r, |, __VA_ARGS__)\
EWIN_VAL_PROP_ARITHMETIC_OP(n, vt, r, ^, __VA_ARGS__)

#define EWIN_VAL_PROP_FRIEND_NUMERIC_OPERATORS(n, vt, r, ...)\
EWIN_VAL_PROP_FRIEND_OP(n, vt, r, +)\
EWIN_VAL_PROP_FRIEND_OP(n, vt, r, -)\
EWIN_VAL_PROP_FRIEND_OP(n, vt, r, *)\
EWIN_VAL_PROP_FRIEND_OP(n, vt, r, /)\
\
EWIN_VAL_PROP_FRIEND_OP(n, vt, r, %)\
EWIN_VAL_PROP_FRIEND_OP(n, vt, r, <<)\
EWIN_VAL_PROP_FRIEND_OP(n, vt, r, >>)\
EWIN_VAL_PROP_FRIEND_OP(n, vt, r, &)\
EWIN_VAL_PROP_FRIEND_OP(n, vt, r, |)\
EWIN_VAL_PROP_FRIEND_OP(n, vt, r, ^)

#define EWIN_VAL_PROP_REL_OPERATORS(n, vt, c, ...)\
EWIN_VAL_PROP_REL_OP(n, vt, <, c, -1, __VA_ARGS__)\
EWIN_VAL_PROP_REL_OP2(n, vt, <=, c, -1, 0, __VA_ARGS__)\
EWIN_VAL_PROP_REL_OP(n, vt, ==, c, 0, __VA_ARGS__)\
EWIN_VAL_PROP_REL_OP2(n, vt, !=, c, -1, 1, __VA_ARGS__)\
EWIN_VAL_PROP_REL_OP2(n, vt, >=, c, 0, 1, __VA_ARGS__)\
EWIN_VAL_PROP_REL_OP(n, vt, >, c, 1, __VA_ARGS__)

#define EWIN_VAL_PROP_FRIEND_REL_OPERATORS(n, vt, c)\
EWIN_VAL_PROP_FRIEND_REL_OP(n, vt, <, c, -1)\
EWIN_VAL_PROP_FRIEND_REL_OP2(n, vt, <=, c, -1, 0)\
EWIN_VAL_PROP_FRIEND_REL_OP(n, vt, ==, c, 0)\
EWIN_VAL_PROP_FRIEND_REL_OP2(n, vt, !=, c, -1, 1)\
EWIN_VAL_PROP_FRIEND_REL_OP2(n, vt, >=, c, 0, 1)\
EWIN_VAL_PROP_FRIEND_REL_OP(n, vt, >, c, 1)

#define EWIN_VAL_PROP_OPERATORS(n, vt, r, ...)\
EWIN_VAL_PROP_RELATIONAL_OPERATORS(n, vt, __VA_ARGS__)\
EWIN_VAL_PROP_NUMERIC_OPERATORS(n, vt, r, __VA_ARGS__)

#define EWIN_VAL_PROP_FRIEND_OPERATORS(n, vt, r)\
EWIN_VAL_PROP_FRIEND_RELATIONAL_OPERATORS(n, vt)\
EWIN_VAL_PROP_FRIEND_NUMERIC_OPERATORS(n, vt, r)

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
				EWIN_PROP_WRITE_VALUE_DEF(static_cast<const value_type &>(target));
			}
			else{//Update ref
				*ref_ = static_cast<value_type>(target);
				EWIN_PROP_SAFE_ALERT(object::access_type::write);
			}

			return *this;
		}

		value &operator =(const value &target){
			return ((&target == this) ? *this : operator=(static_cast<const value_type &>(*this)));
		}

		value_type operator ->() const{
			return operator value_type();
		}

		std::remove_pointer_t<value_type> &operator *() const{
			return *operator value_type();
		}

		EWIN_VAL_PROP_OPERATORS(value, value_type, value_type, access);
		EWIN_VAL_PROP_FRIEND_OPERATORS(value, value_type, value_type);

		static const object::access_type access = access;

	protected:
		friend manager_type;

		value() = default;

		explicit value(typename managed_type::callback_type callback)
			: managed_type(callback){}

		explicit value(value_type *ref)
			: ref_(ref){}

		value(typename managed_type::callback_type callback, value_type *ref)
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

		void set_manager_(typename managed_type::callback_type callback){
			managed_type::set_manager_(callback);
		}

		void set_manager_(typename managed_type::callback_type callback, value_type *ref){
			set_manager_(callback);
			set_ref_(ref);
		}

		value_type *ref_;
	};
}

#endif /* !EWIN_VALUE_PROPERTY_H */
