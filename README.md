# shadertool

## usage
./shadertool <script/path\> [options...]

## script layout

	.dir1 par1 par2
	.dir2 par1 
	.dir3 par1
	...
	.exec
	ins1	a, b
	ins2	a
	ins3	a, b
	...

## directives 

.source name [path|@OPT] - defines frame source

.sink name [path|@OPT] - defines frame sink

.shader name [path|@OPT] - defines frame sink

.exec - begin executable section

## instructions

pull 	reg, source - load frame from source into reg

push 	sink, reg - store frame into sink from reg

apply 	shader, [uniform=reg]... - run shader with uniforms and return frame to r0

jmp 	offset - change pc by relative offset

jmpif 	offset, [!]cond - change pc by relative offset if cond is true

mov 	dreg, [sreg|imm|obj.param] - copy value into dreg

add 	dreg, sreg - add value of sreg to dreg, sets zero flag if dreg is 0

## source

main.cc				- main logic, loads script file and runs interpreter

cmdline/args		- cmdline argument parser

video/frame 		- structure for holding pixel data

video/frame_source 	- abstraction of media container, provides frames

video/frame_sink 	- abstraction of media container, receives frames

fs/remote_path		- wrapper for storing path data, supports runtime resolution of references to cmdline arguments

script/file			- abstraction of file contents

script/parser		- parses script files

script/directives	- implementation of directives

script/instructions - implementation of instructions

script/interpreter	- interpreter of script files, applies directives and executes instructions

script/register		- base class representing default storage type of interpreter

graphics/render_device 	- abstraction of rendering device, interfaces with OpenGL

graphics/render_target 	- abstraction of underlying OpenGL structures for receiving frames

graphics/shader			- abstraction of shader loading/uploading

graphics/shader_program - abstraction of loaded shader program

