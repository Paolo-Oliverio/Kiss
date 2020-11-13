let p = new Project('soloud');
p.addDefine("KISS_SOLOUD");
let srcPath = 'soloud/src/';
p.addIncludeDir('soloud/include');
p.addFile(srcPath + 'core/**');

{//Sources Selection
	let sources [
		//"ay",
		//"monotone",
		"noise",
		//"openmpt",
		//"sfxr2",
		//"speech2",
		//"tedsid",
		//"vic",
		//"vizsn",
		//"wav",
	];
	let sourcesPath = srcPath + 'audiosource/';
	let nSources = sources.length;
	for (i = 0; i < nSources;++i)
	{
		p.addFile(sourcesPath + sources[i] +'/**');
		p.addDefine('SOLOUD_'+ sources[i].toUpperCase());
	}
}//End Sources Selection

{//Backend Selection
	var backend = '';
	switch (platform): {
		case 'html5': 	
			backend = 'miniaudio'; 	
			break;
		case 'android': 
			backend = 'opensles';	
			break;
		case 'windows': 
			backend = 'xaudio2';	
			break;
		default: 
			backend = 'null';	
			break;
	}
	p.addFile(srcPath + 'backend/' + backend +'/**');
	p.addDefine('WITH_'+backend.toUpperCase());
}//End Backend Selection 

{//Filters Selection
	let filters [
		//"bassboost",
		//"biquadresonant",
		//"dcremoval",
		//"echo",
		//"fft",
		//"flanger",
		//"freeverb",
		//"lofi",
		//"robotize",
		//"waveshaper",
	];
	let filtersPath = srcPath + 'filter/';
	let nFilters = filters.length;
	for (i = 0; i < nFilters;++i)
	{
		p.addFile(filtersPath + 'soloud_' + filters[i] +'filter.cpp');
		p.addDefine('SOLOUD_'+ filters[i].toUpperCase());
	}
}//End Filters Selection

resolve(p);