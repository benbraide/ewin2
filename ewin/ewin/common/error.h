#pragma once

#ifndef EWIN_ERROR_H
#define EWIN_ERROR_H

namespace ewin::common{
	enum class error{
		nil,
		property_not_initialized,
		property_access_violation,
		property_forbidden,
		local_error,
		status_error,
		change_rejected,
	};
}

#endif /* !EWIN_ERROR_H */
