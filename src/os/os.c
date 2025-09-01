#include "os/os.h"

#include "os/linux/os.c"
#include "os/win32/os.c"

OS_Handle os_handle_zero(void) {
	OS_Handle h = {0};
	return h;
}

bool os_handle_match(OS_Handle a, OS_Handle b) {
	return a.value == b.value;
}

bool os_handle_is_zero(OS_Handle a) {
	return a.value == 0;
}

NORETURN void os_abort_with_message(S32 exit_code, String message) {
	#warning Implement Message Box
	// os_show_message_box(OS_MessageBoxIcon_Error, str8_lit("Error Occurred!"), message);
	os_abort(exit_code);
}

Rng1DU64 os_path_basename(String path) {
	Rng1DU64 basename = {
		.min = 0,
		.max = path.size - 1
	};

	for (S64 i = path.size - 1; i >= 0; i--) {
		if (path.data[i] == '/' || path.data[i] == '\\') {
			basename.max = i - 1;
		} else {
			break;
		}
	}

	for EachIndex(i, basename.max) {
		if (path.data[i] == '/' || path.data[i] == '\\') {
			basename.min = i + 1;
		}
	}

	return basename;
}

Rng1DU64 os_path_extension(String path) {
	Rng1DU64 ext = {0};

	for (S64 i = path.size - 1; i >= 0; i--) {
		if (path.data[i] == '/' || path.data[i] == '\\') {
			break;
		} else if (path.data[i] == '.') {
			ext.min = i;
			ext.max = path.size - 1;
		}
	}

	return ext;
}
