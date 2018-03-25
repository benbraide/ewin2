#pragma once

#ifndef EWIN_GUI_OBJECT_H
#define EWIN_GUI_OBJECT_H

#include <list>

#include "../property/list_property.h"

#include "../common/error_object.h"

#include "gui_ancestor_list.h"
#include "gui_sibling_list.h"

namespace ewin::gui{
	class tree;

	class object{
	public:
		typedef common::error error_type;
		typedef common::error_object<object> error_object_type;

		typedef ancestor_list<object> ancestor_list_type;
		typedef sibling_list<object> sibling_list_type;

		property::value<object, tree *> parent;
		property::value<object, std::size_t> index;

		property::value<object, object *> previous_sibling;
		property::value<object, object *> next_sibling;

		property::list<object, ancestor_list_type, tree> ancestors;
		property::list<object, sibling_list_type, object> siblings;

		error_object_type error;

	protected:
		friend class tree;

		virtual void handle_property_(void *prop, void *arg, property::object::access_type access);

		virtual void set_parent_(tree *value);

		virtual bool parent_changing_(tree *value);

		virtual void parent_changed_();

		virtual bool index_changing_(std::size_t value);

		virtual void index_changed_();

		tree *parent_ = nullptr;
		std::size_t index_ = -1;
	};
}

#endif /* !EWIN_GUI_OBJECT_H */
