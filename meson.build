project(
  'dotf',
  'cpp',
  version: '3.0.0',
  default_options: [
        'cpp_std=c++17',
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
  cpp_args: ['-DDOTF_VERSION="@0@"'.format(meson.project_version())],
  dependencies: [fmt_dep, argparse_dep, tomlplusplus_dep],
  install: true
)
