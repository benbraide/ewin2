#pragma once

#ifndef EWIN_TRANSFORM_PROPERTY_H
#define EWIN_TRANSFORM_PROPERTY_H

#include "../common/variadic_type_list.h"

#include "value_property.h"

namespace ewin::property{
	template <class manager_type, object::access_type access, class... type_list>
	class transform : public managed<manager_type>{
	public:
		typedef managed<manager_type> managed_type;
		typedef common::variadic_type_pair_list<type_list...> variadic_type_list_type;

		template <typename target_type>
		using second_type = typename std::tuple_element_t<variadic_type_list_type::template index_of_first<target_type>, std::tuple<type_list...>>::second_type;

		template <typename return_type, typename target_type>
		struct return_value{
			static constexpr return_type value(const target_type &target){
				return target;
			}
		};

		template <typename target_type>
		struct return_value<void, target_type>{
			static void value(const target_type &/*target*/){}
		};

		template <typename target_type>
		target_type operator()() const{
			return operator target_type();
		}

		template <typename target_type>
		second_type<target_type> operator[](const target_type &target) const{
			typedef std::conditional_t<std::is_void_v<second_type<target_type>>, char, second_type<target_type>> return_type;
			typedef object::query_return_info_type<const target_type *, return_type> qr_info_type;

			current_ = variadic_type_list_type::template index_of_first<target_type>;
			EWIN_PROP_CHECK_ACCESS(object::access_type::read);

			qr_info_type query_return_info{ &target, return_type() };
			typename managed_type::indexed_target_info_type info{ current_, &query_return_info };

			EWIN_PROP_REQUIRE_CALLBACK;
			EWIN_PROP_REF_CALL(info, object::access_type::read);

			return return_value<second_type<target_type>, return_type>::value(query_return_info.return_value);
		}

		static const object::access_type access = access;

	protected:
		friend manager_type;

		transform() = default;

		explicit transform(typename managed_type::callback_type callback)
			: managed_type(callback){}

		transform(transform &&target) noexcept
			: managed_type(std::move(target)){}

		transform &operator =(transform &&target) noexcept{
			managed_type::operator=(std::move(target));
			return *this;
		}

		void set_manager_(typename managed_type::callback_type callback){
			managed_type::set_manager_(callback);
		}

		mutable std::size_t current_ = static_cast<std::size_t>(-1);
	};

	template <class manager_type, class query_type, class return_type, object::access_type access = object::access_type::nil>
	using single_transform = transform<manager_type, access, common::variadic_type_pair<query_type, return_type>>;
}

#endif /* EWIN_TRANSFORM_PROPERTY_H */
