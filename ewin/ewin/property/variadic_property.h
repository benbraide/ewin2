#pragma once

#ifndef EWIN_VARIADIC_PROPERTY_H
#define EWIN_VARIADIC_PROPERTY_H

#include <vector>

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

		template <typename first_type, typename... target_types>
		variadic &operator()(const first_type &first, const target_types &... targets){
			if (common::variadic_type_list<first_type, target_types...>::size == 1u)
				return single_call_(first, targets...);//Single argument

			current_ = static_cast<std::size_t>(-1);
			EWIN_PROP_CHECK_ACCESS(object::access_type::write);
			std::vector<object::indexed_target_info_type> args;

			args.reserve(common::variadic_type_list<first_type, target_types...>::size);
			stack_call_arg_(args, first, targets...);

			if (args.size() < common::variadic_type_list<first_type, target_types...>::size)
				EWIN_PROP_SAFE_ALERT(object::access_type::write);//Refs updated

			if (!args.empty()){//Update rest without ref
				EWIN_PROP_REQUIRE_CALLBACK;
				EWIN_PROP_REF_CALL(args, (object::access_type::write | object::access_type::alternate));
			}

			return *this;
		}

		template <typename target_type>
		operator target_type() const{
			current_ = variadic_type_list_type::template index<target_type>;
			EWIN_PROP_CHECK_ACCESS(object::access_type::read);
			if (refs_[variadic_type_list_type::template index<target_type>] != nullptr){//Return ref
				EWIN_PROP_SAFE_ALERT(object::access_type::read);
				return *static_cast<target_type *>(refs_[variadic_type_list_type::template index<target_type>]);
			}

			EWIN_PROP_REQUIRE_CALLBACK;
			EWIN_PROP_RETURN_INDEXED_VALUE_UNNAMED_DEF(target_type, variadic_type_list_type::template index<target_type>);
		}

		template <typename target_type>
		variadic &operator =(const target_type &target){
			current_ = variadic_type_list_type::template index<target_type>;
			EWIN_PROP_CHECK_ACCESS(object::access_type::write);
			if (refs_[variadic_type_list_type::template index<target_type>] == nullptr){
				EWIN_PROP_REQUIRE_CALLBACK;
				EWIN_PROP_WRITE_INDEXED_VALUE_DEF(variadic_type_list_type::template index<target_type>, target);
			}
			else{//Update ref
				*static_cast<target_type *>(refs_[variadic_type_list_type::template index<target_type>]) = target;
				EWIN_PROP_SAFE_ALERT(object::access_type::write);
			}

			return *this;
		}

		static const object::access_type access = access;

	protected:
		friend manager_type;

		variadic(){
			init_();
		}

		explicit variadic(typename managed_type::callback_type callback)
			: managed_type(callback){
			init_();
		}

		template <typename ref_type>
		explicit variadic(ref_type *ref){
			init_();
			set_ref_(ref);
		}

		template <typename ref_type>
		variadic(typename managed_type::callback_type callback, ref_type *ref)
			: managed_type(callback){
			init_();
			set_ref_(ref);
		}

		variadic(variadic &&target) noexcept
			: managed_type(std::move(target)){
			move_(std::move(target));
		}

		variadic &operator =(variadic &&target) noexcept{
			managed_type::operator=(std::move(target));
			move_(std::move(target));
			return *this;
		}

		void init_(){
			for (std::size_t i = 0u; i < variadic_type_list_type::size; ++i)
				refs_[i] = nullptr;
		}

		void move_(variadic &&target){
			for (std::size_t i = 0u; i < variadic_type_list_type::size; ++i){
				refs_[i] = target.refs_[i];
				target.refs_[i] = nullptr;
			}
		}

		template <typename ref_type>
		void set_ref_(ref_type *ref){
			refs_[variadic_type_list_type::template index<ref_type>] = ref;
		}

		void set_manager_(typename managed_type::callback_type callback){
			managed_type::set_manager_(callback);
		}

		template <typename ref_type>
		void set_manager_(typename managed_type::callback_type callback, ref_type *ref){
			set_manager_(callback);
			set_ref_(ref);
		}

		template <typename first_type, typename... target_types>
		variadic &single_call_(const first_type &first, const target_types &... targets){
			return operator=(first);
		}

		template <typename first_type>
		void stack_call_arg_(std::vector<object::indexed_target_info_type> &args, const first_type &first){
			if (refs_[variadic_type_list_type::template index<first_type>] == nullptr){
				args.push_back(object::indexed_target_info_type{
					variadic_type_list_type::template index<first_type>,
					EWIN_PROP_REF_PTR(first)
				});
			}
			else//Has ref
				*static_cast<first_type *>(refs_[variadic_type_list_type::template index<first_type>]) = first;
		}

		template <typename first_type, typename second_type, typename... target_types>
		void stack_call_arg_(std::vector<object::indexed_target_info_type> &args, const first_type &first,
			const second_type &second, const target_types &... targets){
			stack_call_arg_(args, first);
			stack_call_arg_(args, second, targets...);
		}

		void *refs_[variadic_type_list_type::size];
		mutable std::size_t current_ = static_cast<std::size_t>(-1);
	};
}

#endif /* !EWIN_VARIADIC_PROPERTY_H */
