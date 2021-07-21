# Include target platform's entry point
platform = host_machine.system()

# source += files([platform + '_main.cpp'])
if platform == 'linux'
	source = files([
		'linux-main.cpp',
		'linux-opengl.cpp'
	])
elif platform == 'windows'
	source = files([
		'windows-main.cpp',
		'windows-opengl.cpp',
		'windows-shared.cpp'
	])
endif

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
