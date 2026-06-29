project(
    'wombat-api-template',
    'cpp',
    version: '0.1.0',
    meson_version: '>=1.1',
    default_options: [
        'cpp_std=c++23',
        'warning_level=2',
    ],
)

# Pulls in headers + link flags for the Wombat API. Transitively provides
# urc-common (logging, units) via the pkg-config 'Requires:' field, so a single
# dependency is enough. The dev package liburc-wombat-api-dev brings the headers.
wombat_api_dep = dependency('urc-wombat-api')

executable(
    get_option('program_name'),
    'src/main.cpp',
    include_directories: include_directories('include'),  # your own headers
    dependencies: [wombat_api_dep],
    install: false,
)
