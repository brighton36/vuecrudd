import os

def DirectoryOfThisScript():
  return os.path.dirname( os.path.abspath( __file__ ) )

def Settings( **kwargs ):
  return {
    'flags': [
        "-std=c++17",
        '-x', 'c++',
        "-I/usr/include/mysql",
        "-I/usr/include",
        "-Iinclude",
        "-Imodels",
        "-Icontrollers",
        "-Ibuild/_deps/prails-src/include",
        "-Ibuild/_deps/prails-src/thirdparty/include",
        "-Ibuild/_deps/pistache-src/include",
        "-Ibuild/_deps/pistache-src/tests",
        "-Ibuild/_deps/googletest-src/googletest/include",
        "-Ibuild/_deps/spdlog-src/include" ],
    'include_paths_relative_to_dir': DirectoryOfThisScript()
  }
