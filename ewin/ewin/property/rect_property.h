#pragma once

#ifndef EWIN_RECT_PROPERTY_H
#define EWIN_RECT_PROPERTY_H

#include "value_property.h"

namespace ewin::property{
	template <class manager_type, class value_type, class field_type, object::access_type access = object::access_type::nil>
	class rect : public value<manager_type, value_type, access>{
	public:
		typedef value_type value_type;
		typedef field_type field_type;
		typedef value<manager_type, value_type, access> value_prop_type;

		struct info_type{
			field_type left;
			field_type top;
			field_type right;
			field_type bottom;
		};

		value_type operator()() const{
			return operator value_type();
		}

		template <typename target_type>
		rect &operator()(const target_type &target){
			return operator=(target);
		}

		operator value_type() const{
			recent_ = nullptr;
			EWIN_PROP_CHECK_ACCESS(object::access_type::read);
			if (value_prop_type::ref_ != nullptr){//Return ref
				EWIN_PROP_SAFE_ALERT(object::access_type::read);
				return *value_prop_type::ref_;
			}

			EWIN_PROP_REQUIRE_CALLBACK;
			EWIN_PROP_RETURN_SUB_VALUE_UNNAMED_DEF(value_type, nullptr);
		}

		template <typename target_type>
		rect &operator =(const target_type &target){
			recent_ = nullptr;
			EWIN_PROP_CHECK_ACCESS(object::access_type::write);
			if (value_prop_type::ref_ == nullptr){
				EWIN_PROP_REQUIRE_CALLBACK;
				EWIN_PROP_WRITE_SUB_VALUE_DEF(nullptr, static_cast<const value_type &>(target));
			}
			else{//Update ref
				*value_prop_type::ref_ = static_cast<const value_type &>(target);
				EWIN_PROP_SAFE_ALERT(object::access_type::write);
			}

			return *this;
		}

		rect &operator =(const rect &target){
			return ((&target == this) ? *this : operator=(static_cast<const value_type &>(target)));
		}

		EWIN_VAL_PROP_REL_OPERATORS(rect, const value_type &, compare, field_type, access);
		EWIN_VAL_PROP_FRIEND_REL_OPERATORS(rect, const value_type &, compare);

		static int compare(const value_type &left, const value_type &right){
			auto left_info = reinterpret_cast<const info_type *>(&left), right_info = reinterpret_cast<const info_type *>(&right);
			return (
				(left_info->left == right_info->left) &&
				(left_info->top == right_info->top) &&
				(left_info->right == right_info->right) &&
				(left_info->bottom == right_info->bottom)
				? 0 : -1);
		}

		value<rect, field_type, access> left;
		value<rect, field_type, access> top;
		value<rect, field_type, access> right;
		value<rect, field_type, access> bottom;

	protected:
		friend manager_type;

		rect() = default;

		explicit rect(typename value_prop_type::callback_type callback)
			: value_prop_type(callback){
			init_();
		}

		explicit rect(value_type *ref)
			: value_prop_type(ref){
			init_();
		}

		rect(typename value_prop_type::callback_type callback, value_type *ref)
			: value_prop_type(callback, ref){
			init_();
		}

		rect(rect &&target)
			: value_prop_type(std::move(target)){}

		rect &operator =(rect &&target){
			value_prop_type::operator=(std::move(target));
			return *this;
		}

		void set_ref_(value_type *ref){
			value_prop_type::set_ref_(ref);
			init_();
		}

		void set_manager_(typename value_prop_type::managed_type::callback_type callback){
			value_prop_type::set_manager_(callback);
			init_();
		}

		void set_manager_(typename value_prop_type::managed_type::callback_type callback, value_type *ref){
			set_manager_(callback);
			set_ref_(ref);
			init_();
		}

		void init_(){
			auto handler = EWIN_PROP_HANDLER_DEF(rect);
			if (value_prop_type::callback_ == nullptr){
				left.set_manager_(nullptr);
				top.set_manager_(nullptr);
				right.set_manager_(nullptr);
				bottom.set_manager_(nullptr);
			}
			else{//Bind callbacks
				left.set_manager_(handler);
				top.set_manager_(handler);
				right.set_manager_(handler);
				bottom.set_manager_(handler);
			}

			if (value_prop_type::ref_ == nullptr){
				left.set_ref_(nullptr);
				top.set_ref_(nullptr);
				right.set_ref_(nullptr);
				bottom.set_ref_(nullptr);
			}
			else{//Bind refs
				auto info = reinterpret_cast<info_type *>(value_prop_type::ref_);
				left.set_ref_(&info->left);
				top.set_ref_(&info->top);
				right.set_ref_(&info->right);
				bottom.set_ref_(&info->bottom);
			}
		}

		void handle_property_(void *prop, void *arg, object::access_type access){
			recent_ = prop;
			if (EWIN_IS(access, object::access_type::validate)){
				EWIN_PROP_SAFE_VALIDATE(access);
				return;
			}

			if (EWIN_IS(access, object::access_type::alert) || value_prop_type::ref_ != nullptr){
				EWIN_PROP_SAFE_ALERT(access);
				return;
			}

			EWIN_PROP_REQUIRE_CALLBACK;
			if (EWIN_IS(access, object::access_type::read)){
				EWIN_PROP_READ_SUB_VALUE_DEF(val, field_type, prop);
				*static_cast<field_type *>(arg) = val;
			}
			else if (EWIN_IS(access, object::access_type::write))
				EWIN_PROP_WRITE_SUB_VALUE_DEF(prop, *static_cast<field_type *>(arg));
		}

		mutable void *recent_ = nullptr;
	};
}

#endif /* !EWIN_RECT_PROPERTY_H */
