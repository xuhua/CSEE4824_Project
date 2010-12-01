#error Modules should never use kernel-headers system headers,
#error but rather headers from an appropriate kernel-source package.
#error Change -I/usr/src/linux/include (or similar) to
#error -I/lib/modules/$(uname -r)/build/include
#error to build against the currently-running kernel.
