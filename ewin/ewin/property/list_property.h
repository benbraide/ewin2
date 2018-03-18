#pragma once

#ifndef EWIN_LIST_PROPERTY_H
#define EWIN_LIST_PROPERTY_H

#include "value_property.h"

namespace ewin::property{
	template <class manager_type, class value_type, class field_type, class index_type = std::size_t, object::access_type access = object::access_type::nil>
	class list : public managed<manager_type>{
	public:
		typedef value_type value_type;
		typedef field_type field_type;
		typedef index_type index_type;

		typedef managed<manager_type> managed_type;
		typedef std::conditional_t<(std::is_array_v<value_type> || std::is_pointer_v<value_type>),
			field_type *, typename value_type::iterator> iterator_type;

		template <typename target_type>
		index_type operator ()(index_type index, const target_type &target){
			return operator()(index, static_cast<const value_type &>(target));
		}

		index_type operator ()(index_type index, const field_type &target){
			EWIN_PROP_CHECK_ACCESS(object::access_type::list_add);
			EWIN_PROP_REQUIRE_CALLBACK;

			object::indexed_value_type<index_type, const field_type *> info{ index, &target };
			EWIN_PROP_REF_CALL(info, (object::access_type::list_add | object::access_type::alternate));

			return info.index;
		}

		template <typename target_type>
		index_type operator +=(const target_type &target){
			return operator+=(static_cast<const field_type &>(target));
		}

		index_type operator +=(const field_type &target){
			return operator ()(index_type(), target);
		}

		list &operator -=(const field_type &target){
			EWIN_PROP_CHECK_ACCESS(object::access_type::list_remove);

			EWIN_PROP_REQUIRE_CALLBACK;
			EWIN_PROP_REF_CALL(target, object::access_type::list_remove);

			return *this;
		}

		template <typename unused = field_type>
		std::enable_if_t<!std::is_same_v<unused, index_type>, list> &operator -=(const index_type &index){
			EWIN_PROP_CHECK_ACCESS(object::access_type::list_remove);

			EWIN_PROP_REQUIRE_CALLBACK;
			EWIN_PROP_REF_CALL(index, object::access_type::list_remove_index);

			return *this;
		}

		field_type *operator [](const index_type &index) const{
			EWIN_PROP_CHECK_ACCESS(object::access_type::list_find);
			EWIN_PROP_REQUIRE_CALLBACK;

			object::indexed_value_type<index_type, const field_type *> info{ index };
			EWIN_PROP_REF_CALL(info, object::access_type::list_find);

			return info.index;
		}

		template <typename target_type, typename unused = field_type>
		std::enable_if_t<!std::is_same_v<unused, index_type>, index_type> operator [](const target_type &target) const{
			return operator[](static_cast<const field_type &>(target));
		}

		template <typename unused = field_type>
		std::enable_if_t<!std::is_same_v<unused, index_type>, index_type> operator [](const field_type &target) const{
			EWIN_PROP_CHECK_ACCESS(object::access_type::list_find);
			EWIN_PROP_REQUIRE_CALLBACK;

			object::indexed_value_type<index_type, const field_type *> info{ index_type(), &target };
			EWIN_PROP_REF_CALL(info, object::access_type::list_find);

			return info.index;
		}

		value<list, std::size_t, (EWIN_IS(access, object::access_type::list_size) ? object::access_type::read : object::access_type::none)> size;

		value<list, field_type *, (EWIN_IS(access, object::access_type::list_at) ? object::access_type::read : object::access_type::none)> first_element;
		value<list, field_type *, (EWIN_IS(access, object::access_type::list_at) ? object::access_type::read : object::access_type::none)> last_element;

		value<list, iterator_type, (EWIN_IS(access, object::access_type::list_begin) ? object::access_type::read : object::access_type::none)> begin;
		value<list, iterator_type, (EWIN_IS(access, object::access_type::list_end) ? object::access_type::read : object::access_type::none)> end;

		static const object::access_type access = access;

	protected:
		friend manager_type;

		list(){
			init_();
		}

		explicit list(typename managed_type::callback_type callback)
			: managed_type(callback){
			init_();
		}

		list(list &&target) noexcept
			: managed_type(std::move(target)){
			init_();
		}

		list &operator =(list &&target) noexcept{
			managed_type::operator=(std::move(target));
			return *this;
		}

		void init_(){
			auto handler = EWIN_PROP_HANDLER_DEF(list);
			size.set_manager_(handler);

			first_element.set_manager_(handler);
			last_element.set_manager_(handler);

			begin.set_manager_(handler);
			end.set_manager_(handler);
		}

		void handle_property_(void *prop, void *arg, object::access_type access) const{
			if (EWIN_IS_ANY(access, (object::access_type::validate | object::access_type::alert)))
				return;

			if (prop == &size)
				access = object::access_type::list_size;
			else if (prop == &first_element)
				access = (object::access_type::list_begin | object::access_type::alternate);
			else if (prop == &last_element)
				access = (object::access_type::list_end | object::access_type::alternate);
			else if (prop == &begin)
				access = object::access_type::list_begin;
			else
				access = object::access_type::list_end;

			EWIN_PROP_REQUIRE_CALLBACK;
			EWIN_PROP_PTR_CALL(arg, access);
		}
	};
}

#endif /* EWIN_LIST_PROPERTY_H */
