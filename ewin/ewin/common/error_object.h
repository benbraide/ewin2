#pragma once

#ifndef EWIN_ERROR_OBJECT_H
#define EWIN_ERROR_OBJECT_H

#include <string>
#include <variant>

#include "../property/variadic_property.h"

#include "windows_headers.h"
#include "error.h"

namespace ewin::common{
	class error_object{
	public:
		enum class error_throw_policy_type{
			throw_always,
			throw_once,
			no_throw_once,
			no_throw,
		};

		typedef property::variadic<
			error_object,
			property::object::access_type::nil,
			error,
			DWORD,
			HRESULT,
			error_throw_policy_type,
			std::wstring,
			bool
		> property_type;

		typedef std::variant<
			error,
			DWORD,
			HRESULT
		> value_type;

		error_object()
			: value_(error::nil), policy_(error_throw_policy_type::throw_always){
			init_();
		}

		error_object(const error_object &) = delete;

		error_object &operator =(const error_object &target) = delete;

		error_object(error_object &&target)
			: value_(std::move(target.value_)), policy_(target.policy_), text_(std::move(target.text_)), prop_(std::move(target.prop_)){
			init_();
		}

		error_object &operator =(error_object &&target){
			value_ = std::move(target.value_);
			policy_ = target.policy_;

			text_ = std::move(target.text_);
			prop_ = std::move(target.prop_);

			return *this;
		}

		template <typename target_type>
		operator target_type() const{
			return prop_.operator target_type();
		}

		template <typename target_type>
		error_object &operator =(const target_type &target){
			prop_.operator=(target);
			return *this;
		}

	private:
		void init_(){
			prop_.set_ref_(&policy_);
			prop_.set_ref_(&text_);
			prop_.set_manager_([this](void *prop, void *arg, property::object::access_type access){
				if (EWIN_IS(access, property::object::access_type::validate))
					return;

				auto info = static_cast<property::object::indexed_target_info_type *>(arg);
				if (EWIN_IS(access, property::object::access_type::write)){
					if (EWIN_IS(access, property::object::access_type::alert))
						return;

					text_.clear();
					switch (info->index){
					case property_type::variadic_type_list_type::index<error>:
						value_ = *static_cast<error *>(info->target);
						break;
					case property_type::variadic_type_list_type::index<DWORD>:
						value_ = *static_cast<DWORD *>(info->target);
						break;
					case property_type::variadic_type_list_type::index<HRESULT>:
						value_ = *static_cast<HRESULT *>(info->target);
						break;
					default:
						break;
					}

					throw_(get_error_());
				}
				else if (EWIN_IS(access, property::object::access_type::read)){
					if (EWIN_IS(access, property::object::access_type::alert)){
						if (prop_.current_ == property_type::variadic_type_list_type::index<std::wstring>)
							error_to_string_();
						return;
					}

					switch (info->index){
					case property_type::variadic_type_list_type::index<error>:
						*static_cast<error *>(info->target) = get_error_();
						break;
					case property_type::variadic_type_list_type::index<DWORD>:
						*static_cast<DWORD *>(info->target) = get_local_error_();
						break;
					case property_type::variadic_type_list_type::index<HRESULT>:
						*static_cast<HRESULT *>(info->target) = get_status_error_();
						break;
					case property_type::variadic_type_list_type::index<bool>:
						*static_cast<bool *>(info->target) = (get_error_() != error::nil);
						break;
					default:
						break;
					}
				}
			});
		}

		error get_error_() const{
			if (std::holds_alternative<error>(value_))
				return std::get<error>(value_);

			if (std::holds_alternative<DWORD>(value_))
				return ((std::get<DWORD>(value_) == 0u) ? error::nil : error::local_error);

			if (std::holds_alternative<HRESULT>(value_))
				return ((std::get<HRESULT>(value_) == S_OK) ? error::nil : error::status_error);

			return error::nil;
		}

		DWORD get_local_error_() const{
			if (std::holds_alternative<DWORD>(value_))
				return std::get<DWORD>(value_);

			if (std::holds_alternative<HRESULT>(value_))
				return HRESULT_CODE(std::get<HRESULT>(value_));

			return 0u;
		}

		HRESULT get_status_error_() const{
			if (std::holds_alternative<HRESULT>(value_))
				return std::get<HRESULT>(value_);

			if (std::holds_alternative<DWORD>(value_))
				return static_cast<HRESULT>(std::get<DWORD>(value_));

			return S_OK;
		}

		void error_to_string_(){
			switch (get_error_()){
			case error::nil:
				text_ = L"Success";
				break;
			case error::property_not_initialized:
				text_ = L"Property not initialized";
				break;
			case error::property_access_violation:
				text_ = L"Property access violation";
				break;
			case error::property_forbidden:
				text_ = L"Property forbidden";
				break;
			case error::local_error:
				local_error_to_string_();
				break;
			case error::status_error:
				status_error_to_string_();
				break;
			default:
				break;
			}
		}

		void local_error_to_string_(){
			code_to_string_(std::get<DWORD>(value_));
		}

		void status_error_to_string_(){
			code_to_string_(HRESULT_CODE(std::get<HRESULT>(value_)));
		}

		void code_to_string_(DWORD code){
			void *allocated_buffer = nullptr;
			auto count = FormatMessageW(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr,
				code,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				reinterpret_cast<wchar_t *>(&allocated_buffer),
				0u,
				nullptr
			);

			if (allocated_buffer != nullptr){
				if (count > 0u)
					text_.assign(reinterpret_cast<wchar_t *>(allocated_buffer), count);
				LocalFree(allocated_buffer);
			}
		}

		void throw_(error e){
			if (e == error::nil || policy_ == error_throw_policy_type::no_throw)
				return;

			if (policy_ == error_throw_policy_type::no_throw_once){
				policy_ = error_throw_policy_type::throw_always;
				return;
			}

			if (policy_ == error_throw_policy_type::throw_once)
				policy_ = error_throw_policy_type::no_throw;

			throw this;
		}

		value_type value_;
		error_throw_policy_type policy_;
		std::wstring text_;
		property_type prop_;
	};
}

#endif /* !EWIN_ERROR_OBJECT_H */
