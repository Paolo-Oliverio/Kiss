let project = new Project('DearImGui');

/*
if (platform == 'html5') {
	project.addDefine('IMGUI_USER_CONFIG="\\"imguiconfig.h\\""');
} else if (platform == 'android') {
	project.addDefine('IMGUI_USER_CONFIG=<imguiconfig.h>');
}else{
	project.addDefine('IMGUI_USER_CONFIG="imguiconfig.h"');
}
*/

project.addIncludeDir('include');
project.addIncludeDir('sources');
project.addIncludeDir('imgui');
project.addFiles(
	'imgui/imgui.cpp',
	'imgui/imgui.h',
	'imgui/imgui_draw.cpp',
	'imgui/imgui_widgets.cpp'
);
project.addFiles('sources/**', 'include/**');

resolve(project);