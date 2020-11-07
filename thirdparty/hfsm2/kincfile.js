let project = new Project('HFSM2');
project.addDefine("KISS_HFSM2");
project.addIncludeDir('HFSM2/include/');
project.addFile('HFSM2/include/hfsm2/*');
resolve(project);