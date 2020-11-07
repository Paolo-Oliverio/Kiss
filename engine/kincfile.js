let p = new Project('Engine');
p.addFile('src/**');
p.addFile('shaders/**');
p.addIncludeDir('src');
p.cppstd = 11;
resolve(p);