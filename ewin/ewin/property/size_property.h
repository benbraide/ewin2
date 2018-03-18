#pragma once

#ifndef EWIN_SIZE_PROPERTY_H
#define EWIN_SIZE_PROPERTY_H

#include "value_property.h"

namespace ewin::property{
	template <class manager_type, class value_type, class field_type, object::access_type access = object::access_type::nil>
	class size : public value<manager_type, value_type, access>{
	public:
		typedef value_type value_type;
		typedef field_type field_type;
		typedef value<manager_type, value_type, access> value_prop_type;

		struct info_type{
			field_type width;
			field_type height;
		};

		value_type operator()() const{
			return operator value_type();
		}

		template <typename target_type>
		size &operator()(const target_type &target){
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
		size &operator =(const target_type &target){
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

		size &operator =(const size &target){
			return ((&target == this) ? *this : operator=(static_cast<const value_type &>(target)));
		}

		EWIN_VAL_PROP_REL_OPERATORS(size, const value_type &, compare, field_type, access);
		EWIN_VAL_PROP_FRIEND_REL_OPERATORS(size, const value_type &, compare);

		static int compare(const value_type &left, const value_type &right){
			auto left_info = reinterpret_cast<const info_type *>(&left), right_info = reinterpret_cast<const info_type *>(&right);
			if (left_info->height != right_info->height)
				return ((left_info->height < right_info->height) ? -1 : 1);

			if (left_info->width != right_info->width)
				return ((left_info->width < right_info->width) ? -1 : 1);

			return 0;
		}

		value<size, field_type, access> width;
		value<size, field_type, access> height;

	protected:
		friend manager_type;

		size() = default;

		explicit size(typename value_prop_type::callback_type callback)
			: value_prop_type(callback){
			init_();
		}

		explicit size(value_type *ref)
			: value_prop_type(ref){
			init_();
		}

		size(typename value_prop_type::callback_type callback, value_type *ref)
			: value_prop_type(callback, ref){
			init_();
		}

		size(size &&target)
			: value_prop_type(std::move(target)){}

		size &operator =(size &&target){
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
			auto handler = EWIN_PROP_HANDLER_DEF(size);
			if (value_prop_type::callback_ == nullptr){
				width.set_manager_(nullptr);
				height.set_manager_(nullptr);
			}
			else{//Bind callbacks
				width.set_manager_(handler);
				height.set_manager_(handler);
			}

			if (value_prop_type::ref_ == nullptr){
				width.set_ref_(nullptr);
				height.set_ref_(nullptr);
			}
			else{//Bind refs
				auto info = reinterpret_cast<info_type *>(value_prop_type::ref_);
				width.set_ref_(&info->width);
				height.set_ref_(&info->height);
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

#endif /* !EWIN_SIZE_PROPERTY_H */
