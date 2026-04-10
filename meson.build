project(
  'dotf',
  'cpp',
  version: '3.0.0', # update in main.cpp too 
  default_options: [
        'c_std=c++17',
  ]
)

fmt = subproject('fmt')
argparse = subproject('argparse')
tomlplusplus = subproject('tomlplusplus')

fmt_dep = fmt.get_variable('fmt_dep')
argparse_dep = argparse.get_variable('argparse_dep')
tomlplusplus_dep = tomlplusplus.get_variable('tomlplusplus_dep')

sources = files(
  'src/main.cpp',
  'src/config.cpp',
  'src/eval.cpp',
  'src/remove.cpp',
  'src/status.cpp',
  'src/sync.cpp',
)

out = executable(
  'dotf',
  sources,
  dependencies: [fmt_dep, argparse_dep, tomlplusplus_dep],
  install: true
)
