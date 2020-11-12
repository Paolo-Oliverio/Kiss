let p = new Project('Entt');

p.addIncludeDir('box2d/include');
//p.addIncludeDir('box2d/src/**');
p.addFile('box2d/src/**');
p.addExcludes(
	'box2d/include/box2d/b2_draw.h',
	'box2d/box2d/src/common/b2_draw.cpp'
)
p.addDefine("KISS_BOX2D");
p.cppstd = 11;
resolve(p);