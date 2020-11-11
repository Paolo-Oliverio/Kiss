let project = new Project('DearImGui');
//project.addDefine('IMGUI_USER_CONFIG="imguiconfig.h"');
project.addIncludeDir('include');
project.addIncludeDir('sources');
project.addIncludeDir('imgui');
project.addFiles(
	'imgui/imgui.cpp',
	'imgui/imgui.h',
	'imgui/imgui_draw.cpp',
	'imgui/imgui_widgets.cpp'
);
project.addFiles('sources/**', 'include/**', 'shaders/**');
project.addDefine("KISS_IMGUI");
resolve(project);