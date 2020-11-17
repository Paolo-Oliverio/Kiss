let p = new Project('imgui');

p.addIncludeDir('sources');
p.addIncludeDir('imgui');
p.addFiles(
	'imgui/imgui.cpp',
	'imgui/imgui.h',
	'imgui/imgui_draw.cpp',
	'imgui/imgui_widgets.cpp',
	'imgui/imgui_demo.cpp'
);

if(!Options.imgui_DemoEnabled){
	console.log("DemoDisabled");
	p.addDefine('IMGUI_DISABLE_DEMO_WINDOWS');
}

p.addFiles('sources/**');
p.addDefine('IMGUI_DISABLE_OBSOLETE_FUNCTIONS');
p.addDefine('IMGUI_DISABLE_WIN32_FUNCTIONS');
//p.addDefine('IMGUI_DISABLE_DEFAULT_FILE_FUNCTIONS');// Don't implement ImFileOpen/ImFileClose/ImFileRead/ImFileWrite so you can implement them yourself if you don't want to link with fopen/fclose/fread/fwrite. This will also disable the LogToTTY() function.

resolve(p);