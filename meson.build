project(
  'dotf',
  'cpp',
  version: '3.0.0', # update in main.cpp too 
  default_options: [
        'c_std=c++17',
  ]
)

fmt_dep = dependency('fmt')
argparse_dep = dependency('argparse')
tomlplusplus_dep = dependency('tomlplusplus')

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
