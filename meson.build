project('tonic', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3'])

inc_dir = include_directories('engine')

source = []
dependencies = []
subdir('engine')
subdir('game')

exe = executable('tonic', source,
	dependencies: dependencies,
	include_directories: inc_dir,
	install : true)

test('basic', exe)