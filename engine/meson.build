# Include target platform's entry point
platform = host_machine.system()

source += files([platform + '-main.cpp'])

subdir('data')
subdir('platform')

if platform == 'linux'
	dependencies += [
		dependency('wayland-client'),
		dependency('wayland-egl'),
		dependency('egl'),
		dependency('gl')
	]
elif platform == 'windows'
	dependencies += [
		dependency('gl')
	]
endif
