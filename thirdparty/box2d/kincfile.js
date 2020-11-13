let p = new Project('Box2D');
p.addIncludeDir('box2d/include');
//p.addIncludeDir('box2d/src/**');
p.addFile('box2d/src/**');
p.addExcludes(
	'box2d/include/box2d/b2_draw.h',
	'box2d/box2d/src/common/b2_draw.cpp'
)
p.cppstd = 11;
resolve(p);