#pragma once

#ifndef EWIN_PROPERTY_OBJECT_H
#define EWIN_PROPERTY_OBJECT_H

#include <functional>

#include "../common/macro.h"
#include "../common/error.h"

#define EWIN_PROP_REF_PTR(v) const_cast<void *>(static_cast<const void *>(&v))
#define EWIN_PROP_PTR_PTR(v) const_cast<void *>(static_cast<const void *>(v))

#define EWIN_PROP_HAS_CALLBACK (object::callback_ != nullptr)
#define EWIN_PROP_REQUIRE_CALLBACK if (!EWIN_PROP_HAS_CALLBACK) throw common::error::property_not_initialized;

#define EWIN_PROP_CHECK_ACCESS(a) if (access != object::access_type::nil && !EWIN_IS(access, a)) throw common::error::property_access_violation;

#define EWIN_PROP_REF_CALL(v, a) object::callback_(EWIN_PROP_PTR_PTR(this), EWIN_PROP_REF_PTR(v), a)
#define EWIN_PROP_PTR_CALL(v, a) object::callback_(EWIN_PROP_PTR_PTR(this), EWIN_PROP_PTR_PTR(v), a)

#define EWIN_PROP_ALERT(a) EWIN_PROP_PTR_CALL(nullptr, a);
#define EWIN_PROP_SAFE_ALERT(a) if (EWIN_PROP_HAS_CALLBACK) EWIN_PROP_ALERT(a);

#define EWIN_PROP_WRITE_VALUE(v, a)\
	std::remove_cv_t<decltype(v)> EWIN_PROP_WRITE_VALUE_VAR((v));\
	EWIN_PROP_REF_CALL(EWIN_PROP_WRITE_VALUE_VAR, a);

#define EWIN_PROP_WRITE_VALUE_DEF(v) EWIN_PROP_WRITE_VALUE((v), object::access_type::write)

#define EWIN_PROP_READ_VALUE(n, t, a)\
	std::remove_cv_t<t> n;\
	EWIN_PROP_REF_CALL(n, a);

#define EWIN_PROP_READ_VALUE_DEF(n, t) EWIN_PROP_READ_VALUE(n, t, object::access_type::read)

#define EWIN_PROP_RETURN_VALUE(n, t, a)\
	EWIN_PROP_READ_VALUE(n, t, a)\
	return n;

#define EWIN_PROP_RETURN_VALUE_DEF(n, t) EWIN_PROP_RETURN_VALUE(n, t, object::access_type::read)

#define EWIN_PROP_RETURN_VALUE_UNNAMED(t, a) EWIN_PROP_RETURN_VALUE(EWIN_PROP_RETURN_VALUE_UNNAMED_VAR, t, a)
#define EWIN_PROP_RETURN_VALUE_UNNAMED_DEF(t) EWIN_PROP_RETURN_VALUE_UNNAMED(t, object::access_type::read)

#define EWIN_PROP_SEQ_REF_IDX(t, v, i) (reinterpret_cast<t *>(const_cast<std::remove_cv<decltype(&v)>>(&v)) + i)
#define EWIN_PROP_SEQ_CREF_IDX(t, v, i) (reinterpret_cast<const t *>(&v) + i)

#define EWIN_PROP_SEQ_PTR_IDX(t, v, i) (reinterpret_cast<t *>(const_cast<std::remove_cv<decltype(v)>>(v)) + i)
#define EWIN_PROP_SEQ_CPTR_IDX(t, v, i) (reinterpret_cast<const t *>(v) + i)

namespace ewin::property{
	class object{
	public:
		enum class access_type : unsigned int{
			nil					= (0 << 0x0000),
			read				= (1 << 0x0000),
			write				= (1 << 0x0001),
			list_add			= (1 << 0x0002),
			list_remove			= (1 << 0x0003),
			list_remove_index	= (1 << 0x0004),
			list_at				= (1 << 0x0005),
			list_find			= (1 << 0x0006),
			list_begin			= (1 << 0x0007),
			list_end			= (1 << 0x0008),
			list_size			= (1 << 0x0009),
			write_alt			= (1 << 0x000A),
			increment			= (1 << 0x000B),
			decrement			= (1 << 0x000C),
		};

		typedef std::function<void(void *, void *, access_type)> callback_type;

		object() = default;

		object(const object &) = delete;

		object(object &&) = delete;
		
		object &operator =(const object &) = delete;

		object &operator =(object &&) = delete;

	protected:
		callback_type callback_ = nullptr;
	};

	template <class manager_type>
	class managed : public object{
	public:
		typedef manager_type manager_type;

	protected:
		friend manager_type;

		managed() = default;

		explicit managed(callback_type callback)
			: callback_(callback){}

		managed(managed &&target)
			: callback_(target.callback_){
			target.callback_ = nullptr;
		}

		managed &operator =(managed &&target){
			callback_ = target.callback_;
			target.callback_ = nullptr;
			return *this;
		}

		void set_manager_(callback_type callback){
			callback_ = callback;
		}

		struct group{
			template <typename arg_type>
			static void set_manager(callback_type callback){}

			template <typename arg_type>
			static void set_manager(callback_type callback, arg_type &arg){
				arg.set_manager_(callback);
			}

			template <typename arg_type, typename... args_types>
			static void set_manager(callback_type callback, arg_type &arg1, arg_type &arg2, args_types &... args){
				arg1.set_manager_(callback);
				arg2.set_manager_(callback);
				set_manager(callback, std::forward<args_types>(args)...);
			}
		};
	};

	EWIN_MAKE_OPERATORS(object::access_type)
}

#endif /* !EWIN_PROPERTY_OBJECT_H */