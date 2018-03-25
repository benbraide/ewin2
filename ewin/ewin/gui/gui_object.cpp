#include "gui_tree.h"

void ewin::gui::object::handle_property_(void *prop, void *arg, property::object::access_type access){
	typedef property::object::access_type access_type;

	if (EWIN_IS(access, access_type::validate)){
		if ((prop == &ancestors || prop == &siblings) && EWIN_IS_ANY(access, (access_type::list_add |
			access_type::list_remove | access_type::list_remove_index))){
			error = error_type::property_forbidden;
		}
		else if (parent_ == nullptr && (prop == &previous_sibling || prop == &next_sibling || prop == &siblings))
			error = error_type::property_forbidden;

		return;
	}

	if (EWIN_IS(access, access_type::alert))
		return;

	if (prop == &parent){
		if (EWIN_IS(access, access_type::write)){
			auto value = *static_cast<tree **>(arg);
			if (value == parent_)
				return;//No changes

			if (value == nullptr)
				parent_->remove_child_(*this);
			else//Switch
				value->add_child_(*this, index_);
		}
		else//Read
			*static_cast<tree **>(arg) = parent_;
	}
	else if (prop == &index){
		if (EWIN_IS(access, access_type::write)){
			auto value = *static_cast<std::size_t *>(arg);
			if (value == index_)
				return;//No changes

			if (parent_ == nullptr)
				index_ = value;
			else//Update parent
				parent_->update_index_(*this, value);
		}
		else//Read
			*static_cast<std::size_t *>(arg) = ((parent_ == nullptr) ? index_ : parent_->find_(*this));
	}
	else if (prop == &previous_sibling){
		auto index = parent_->find_(*this);
		if (EWIN_IS(access, access_type::write))
			parent_->add_child_(**static_cast<object **>(arg), index);
		else//Read
			*static_cast<object **>(arg) = ((index == 0u) ? nullptr : parent_->find_(index - 1));
	}
	else if (prop == &next_sibling){
		auto index = parent_->find_(*this);
		if (EWIN_IS(access, access_type::write))
			parent_->add_child_(**static_cast<object **>(arg), (index + 1));
		else//Read
			*static_cast<object **>(arg) = parent_->find_(index + 1);
	}
}

void ewin::gui::object::set_parent_(tree *value){
	if (parent_changing_(value)){
		parent_ = value;
		parent_changed_();
	}
	else//Rejected
		error = error_type::change_rejected;
}

bool ewin::gui::object::parent_changing_(tree *value){
	return true;
}

void ewin::gui::object::parent_changed_(){}

bool ewin::gui::object::index_changing_(std::size_t value){
	return true;
}

void ewin::gui::object::index_changed_(){}
