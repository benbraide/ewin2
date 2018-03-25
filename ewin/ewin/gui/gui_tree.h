#pragma once

#ifndef EWIN_GUI_TREE_H
#define EWIN_GUI_TREE_H

#include "gui_object.h"

namespace ewin::gui{
	class tree : public object{
	public:

	protected:
		friend class object;

		virtual void add_child_(object &target, std::size_t index = -1);

		virtual void remove_child_(object &target);

		virtual void update_index_(object &target, std::size_t value);

		virtual object *find_(std::size_t index) const;

		virtual std::size_t find_(object &target) const;
	};
}

#endif /* !EWIN_GUI_TREE_H */
