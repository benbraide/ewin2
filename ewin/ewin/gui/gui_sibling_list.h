#pragma once

#ifndef EWIN_GUI_SIBLING_LIST_H
#define EWIN_GUI_SIBLING_LIST_H

namespace ewin::gui{
	template <class object_type>
	class sibling_iterator{
	public:
		typedef object_type object_type;

		sibling_iterator() = default;

		sibling_iterator(const sibling_iterator &) = default;

		sibling_iterator(sibling_iterator &&) = default;

		sibling_iterator(object_type *object, object_type *target)
			: object_(object), target_(target){}

		sibling_iterator &operator =(const sibling_iterator &) = default;

		sibling_iterator &operator =(sibling_iterator &&) = default;

		const object_type &operator *() const{
			return *object_;
		}

		object_type &operator *(){
			return *object_;
		}

		object_type *operator ->() const{
			return object_;
		}

		sibling_iterator &operator --(){
			if ((object_ = object_->previous_sibling) == target_)
				object_ = object_->previous_sibling;
			return *this;
		}

		sibling_iterator operator --(int){
			sibling_iterator copy(*this);
			--*this;
			return copy;
		}

		sibling_iterator &operator ++(){
			if ((object_ = object_->next_sibling) == target_)
				object_ = object_->next_sibling;
			return *this;
		}

		sibling_iterator operator ++(int){
			sibling_iterator copy(*this);
			++*this;
			return copy;
		}

		bool operator ==(const sibling_iterator &target) const{
			return (object_ == target.object_);
		}

		bool operator !=(const sibling_iterator &target) const{
			return !(*this == target);
		}

	private:
		object_type *object_ = nullptr;
		object_type *target_ = nullptr;
	};

	template <class object_type>
	class sibling_list{
	public:
		typedef object_type object_type;
		typedef sibling_iterator<object_type> iterator;
	};
}

#endif /* !EWIN_GUI_SIBLING_LIST_H */
