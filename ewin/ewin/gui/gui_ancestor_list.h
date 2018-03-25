#pragma once

#ifndef EWIN_GUI_ANCESTOR_LIST_H
#define EWIN_GUI_ANCESTOR_LIST_H

namespace ewin::gui{
	template <class object_type>
	class ancestor_iterator{
	public:
		typedef object_type object_type;

		ancestor_iterator() = default;

		ancestor_iterator(const ancestor_iterator &) = default;

		ancestor_iterator(ancestor_iterator &&) = default;

		explicit ancestor_iterator(object_type *object)
			: object_(object){}

		ancestor_iterator &operator =(const ancestor_iterator &) = default;

		ancestor_iterator &operator =(ancestor_iterator &&) = default;

		const object_type &operator *() const{
			return *object_;
		}

		object_type &operator *(){
			return *object_;
		}

		object_type *operator ->() const{
			return object_;
		}

		ancestor_iterator &operator ++(){
			object_ = object_->parent;
			return *this;
		}

		ancestor_iterator operator ++(int){
			ancestor_iterator copy(*this);
			++*this;
			return copy;
		}

		bool operator ==(const ancestor_iterator &target) const{
			return (object_ == target.object_);
		}

		bool operator !=(const ancestor_iterator &target) const{
			return !(*this == target);
		}

	private:
		object_type *object_ = nullptr;
	};

	template <class object_type>
	class ancestor_list{
	public:
		typedef object_type object_type;
		typedef ancestor_iterator<object_type> iterator;
	};
}

#endif /* !EWIN_GUI_ANCESTOR_LIST_H */
