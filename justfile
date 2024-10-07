build_folder := "./build"
build_script := build_folder / "build.sh"
bin_folder := build_folder / "bin"

default:
  just --list

@fix-perms:
  chmod +x {{build_script}}

@compile target="both" args="": fix-perms
  {{build_script}} -c {{target}} {args}}

@link type="bin" name="": fix-perms
  {{build_script}} -l {{type}} {{name}} 

@build type="bin" target="both" name="":
  just compile {{target}}
  just link {{type}} {{name}}

@run args="" name="":
	just build
	{{bin_folder}}/{{name}} {{args}}

@debug args="":
  just compile --debug
  just link
  valgrind ./build/bin/cav {{args}}
