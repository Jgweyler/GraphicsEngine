solution "AG"
	configurations {"debug", "release"}
	language("c++")
	platforms{"x64"}
	location("build")
	debugdir("build")
	targetdir("bin")
	flags{"ExtraWarnings"}
project ("OpenGLProject")
	kind ("ConsoleApp")
	includedirs{"include", "src/deps/glfw", "src/deps/glad"}
	files {"tests/*.cpp",
		   "src/*.cpp",
		   "src/*.h",
		   "include/**.h",
		   "src/deps/glfw/context.c",
		   "src/deps/glfw/init.c",
		   "src/deps/glfw/input.c",
		   "src/deps/glfw/monitor.c",
		   "src/deps/glfw/window.c",
		   "src/deps/glfw/vulkan.c",
		   "src/deps/glad/glad.c"}
	defines {"_GLFW_USE_OPENGL=1"}
	vpaths { ["Headers"] = "include/*.h",
			 ["Source"]  =  "src/*.cpp",
			 ["Deps Headers"] = {"include/GLFW/*.h",
			                    "include/glad/**",
			                	"include/KHR/**",
			                	"include/glm/**"},
			 ["Deps Src"] = "src/deps/**",
			 ["Tests"] = "tests/*.cpp"}
	configuration {"debug"}
		flags {"Symbols"}
		targetsuffix("_d")
	configuration {"release"}
		flags {"Optimize"}
		targetsuffix("_r")
	configuration {"windows"}
		files{"src/deps/glfw/egl_context.c",
			  "src/deps/glfw/win32*",
			  "src/deps/glfw/wgl_*",
			  "src/deps/glfw/winmm_*"}
		links{"OpenGL32"}
		defines{"_GLFW_WIN32", "_GLFW_WGL"}
		flags{"NoEditAndContinue"}
		windowstargetplatformversion "10.0.16299.0"