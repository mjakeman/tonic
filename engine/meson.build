# Include target platform's entry point
platform = host_machine.system()
source += files([platform + '_main.cpp'])

if platform == 'linux'
	dependencies += [
		dependency('wayland-client'),
		dependency('wayland-egl'),
		dependency('egl'),
		dependency('gl')
	]
endif
