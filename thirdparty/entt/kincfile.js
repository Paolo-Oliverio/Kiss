let project = new Project('Entt');
project.addIncludeDir('entt/src');
project.addFile('entt/src/**');
project.addDefine("KISS_ENTT");
if(platform == 'windows' || platform == 'windowsapp'){
    project.addDefine("_ITERATOR_DEBUG_LEVEL#0");
    project.addDefine("_HAS_ITERATOR_DEBUGGING#0");
}
project.addDefine("ENTT_ASSERT(x)");
project.cppstd = 17;
resolve(project);