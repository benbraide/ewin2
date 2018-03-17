#pragma once

#ifndef EWIN_VARIADIC_TYPE_LIST_H
#define EWIN_VARIADIC_TYPE_LIST_H

#include "cstddef"

namespace ewin::common{
	template <class target_type, class... type_list>
	struct variadic_type_index;

	template <class target_type, class... type_list>
	struct variadic_type_index<target_type, target_type, type_list...> : std::integral_constant<std::size_t, 0>{};

	template <class target_type, class other_type, class... type_list>
	struct variadic_type_index<target_type, other_type, type_list...>
		: std::integral_constant<std::size_t, 1 + variadic_type_index<target_type, type_list...>::value>{};

	template <class first_type, class second_type>
	struct variadic_type_pair{
		typedef first_type first_type;
		typedef second_type second_type;
	};

	template <class... type_list>
	struct variadic_type_list{
		static constexpr std::size_t size = sizeof...(type_list);

		template <typename target_type>
		static constexpr std::size_t index = variadic_type_index<target_type, type_list...>::value;
	};

	template <class... type_list>
	struct variadic_type_pair_list : variadic_type_list<type_list...>{
		template <typename target_type>
		static constexpr std::size_t index_of_first = variadic_type_index<target_type, typename type_list::first_type...>::value;

		template <typename target_type>
		static constexpr std::size_t index_of_second = variadic_type_index<target_type, typename type_list::second_type...>::value;
	};
}

#endif /* !EWIN_VARIADIC_TYPE_LIST_H */
