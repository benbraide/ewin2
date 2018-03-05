#pragma once

#ifndef EWIN_VARIADIC_PROPERTY_H
#define EWIN_VARIADIC_PROPERTY_H

#include "../common/variadic_type_list.h"

#include "value_property.h"

namespace ewin::property{
	template <class manager_type, object::access_type access, class... type_list>
	class variadic : public managed<manager_type>{
	public:
		typedef managed<manager_type> managed_type;
		typedef common::variadic_type_list<type_list...> variadic_type_list_type;

		template <typename target_type>
		target_type operator()() const{
			return operator target_type();
		}

		template <typename target_type>
		variadic &operator()(const target_type &target){
			return operator=(target);
		}

		template <typename target_type>
		operator target_type() const{
			EWIN_PROP_CHECK_ACCESS(object::access_type::read);
			if (refs_[variadic_type_list_type::index<target_type>()] != nullptr){//Return ref
				EWIN_PROP_SAFE_ALERT(object::access_type::read);
				return *static_cast<target_type *>(refs_[variadic_type_list_type::index<target_type>()]);
			}

			EWIN_PROP_REQUIRE_CALLBACK;
			EWIN_PROP_RETURN_INDEXED_VALUE_UNNAMED_DEF(target_type, variadic_type_list_type::index<target_type>());
		}

		template <typename target_type>
		variadic &operator =(const target_type &target){
			EWIN_PROP_CHECK_ACCESS(object::access_type::write);
			if (refs_[variadic_type_list_type::index<target_type>()] == nullptr){
				EWIN_PROP_REQUIRE_CALLBACK;
				EWIN_PROP_WRITE_INDEXED_VALUE_DEF(variadic_type_list_type::index<target_type>(), target);
			}
			else{//Update ref
				*static_cast<target_type *>(refs_[variadic_type_list_type::index<target_type>()]) = target;
				EWIN_PROP_SAFE_ALERT(object::access_type::write);
			}

			return *this;
		}

		static const object::access_type access = access;

	protected:
		friend manager_type;

		variadic() = default;

		explicit variadic(typename managed_type::callback_type callback)
			: managed_type(callback){}

		template <typename ref_type>
		explicit variadic(ref_type *ref){
			set_ref_(ref);
		}

		template <typename ref_type>
		variadic(typename managed_type::callback_type callback, ref_type *ref)
			: managed_type(callback){
			set_ref_(ref);
		}

		variadic(variadic &&target)
			: managed_type(std::move(target)){
			move_(target);
		}

		variadic &operator =(variadic &&target){
			managed_type::operator=(std::move(target));
			move_(target);
			return *this;
		}

		void move_(variadic &&target){
			for (std::size_t i = 0u; i < variadic_type_list_type::size; ++i){
				refs_[i] = target.refs_[i];
				target.refs_[i] = nullptr;
			}
		}

		template <typename ref_type>
		void set_ref_(ref_type *ref){
			refs_[variadic_type_list_type::index<ref_type>()] = ref;
		}

		void set_manager_(typename managed_type::callback_type callback){
			managed_type::set_manager_(callback);
		}

		template <typename ref_type>
		void set_manager_(typename managed_type::callback_type callback, ref_type *ref){
			set_manager_(callback);
			set_ref_(ref);
		}

		void *refs_[variadic_type_list_type::size];
	};
}

#endif /* !EWIN_VARIADIC_PROPERTY_H */
