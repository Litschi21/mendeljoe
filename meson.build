project(
    'wombat-test',
    'cpp',
    version: '0.1.0',
    default_options: [
        'cpp_std=c++23',
        'warning_level=2',
    ],
)

wombat_api_dep = dependency('urc-wombat-api')

executable(
    meson.project_name(),
    'src/main.cpp',
    'src/wombat_demo.cpp',
    include_directories: include_directories('include'),
    dependencies: [wombat_api_dep],
    install: false,
)
