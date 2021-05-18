project('traffic', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3'])

source = []
subdir('engine')
subdir('game')

exe = executable('traffic', source,
  install : true)

test('basic', exe)