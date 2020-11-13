let project = new Project('HFSM2');
project.addIncludeDir('hfsm2/include/');
project.addFile('hfsm2/include/hfsm2/*');
resolve(project);