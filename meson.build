project('traffic', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3'])

source = []
dependencies = []
subdir('engine')
subdir('game')

exe = executable('traffic', source, dependencies: dependencies,
  install : true)

test('basic', exe)
