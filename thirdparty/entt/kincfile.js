let project = new Project('Entt');
project.addIncludeDir('Entt/src');
project.addFile('Entt/src/**');
project.addDefine("KISS_ENTT");
project.addDefine("_ITERATOR_DEBUG_LEVEL#0");
project.addDefine("_HAS_ITERATOR_DEBUGGING#0");
project.addDefine("ENTT_DISABLE_ASSERT");
resolve(project);