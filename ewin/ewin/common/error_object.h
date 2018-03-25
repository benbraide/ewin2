#pragma once

#ifndef EWIN_ERROR_OBJECT_H
#define EWIN_ERROR_OBJECT_H

#include <string>
#include <variant>

#include "windows_headers.h"
#include "error.h"

namespace ewin::common{
	template <class owner_type>
	class error_object{
	public:
		typedef owner_type owner_type;

		enum class error_throw_policy_type{
			throw_always,
			throw_once,
			no_throw_once,
			no_throw,
		};

		typedef std::variant<
			error,
			DWORD,
			HRESULT
		> value_type;

		error_object()
			: value_(error::nil), policy_(error_throw_policy_type::throw_always){}

		error_object(const error_object &) = delete;

		error_object &operator =(const error_object &target) = delete;

		error_object(error_object &&target)
			: value_(std::move(target.value_)), policy_(target.policy_), text_(std::move(target.text_)){}

		error_object &operator =(error_object &&target){
			value_ = std::move(target.value_);

			policy_ = target.policy_;
			text_ = std::move(target.text_);

			return *this;
		}

		operator error() const{
			return get_error_();
		}

		operator DWORD() const{
			return get_local_error_();
		}

		operator HRESULT() const{
			return get_status_error_();
		}

		operator error_throw_policy_type() const{
			return policy_;
		}

		operator const std::wstring &() const{
			if (text_.empty())
				error_to_string_();
			return text_;
		}

		operator bool() const{
			return (get_error_() != error::nil);
		}

		error_object &operator =(error value){
			return assign_(value);
		}

		error_object &operator =(DWORD value){
			return assign_(value);
		}

		error_object &operator =(HRESULT value){
			return assign_(value);
		}

		error_object &operator =(error_throw_policy_type value){
			policy_ = value;
			return *this;
		}

	private:
		friend owner_type;

		template <typename target_type>
		void silent_assign_(target_type target){
			value_ = target;
			text_.clear();
		}

		template <typename target_type>
		error_object &assign_(target_type target){
			silent_assign_(target);
			throw_(get_error_());
			return *this;
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

		void error_to_string_() const{
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
			case error::change_rejected:
				text_ = L"Change rejected";
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

		void local_error_to_string_() const{
			code_to_string_(std::get<DWORD>(value_));
		}

		void status_error_to_string_() const{
			code_to_string_(HRESULT_CODE(std::get<HRESULT>(value_)));
		}

		void code_to_string_(DWORD code) const{
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
		mutable std::wstring text_;
	};
}

#endif /* !EWIN_ERROR_OBJECT_H */
