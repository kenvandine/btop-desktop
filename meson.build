project('btop-desktop', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

i18n = import('i18n')

executable('btop-desktop',
           'src/btop-desktop.c',
           dependencies : [
              dependency('vte-2.91-gtk4'),
              dependency('gtk4')],
           install : true)

bin_name = 'btop-desktop'
prefix = get_option('prefix')
bindir = prefix / get_option('bindir')
datadir = prefix / get_option('datadir')
conf = configuration_data()
conf.set('BIN_DIR', prefix / bindir)
conf.set('BIN_NAME', bin_name)
subdir('data')
