let p = new Project('soloud');

let srcPath = 'soloud/src/';
p.addIncludeDir('soloud/include');
p.addFile(srcPath + 'core/**');

{//Sources Selection
	let sources = [
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
		let def = 'AUDIO_SOURCE_'+ sources[i].toUpperCase();
		p.addDefine(def);
		console.log('#define ' + def);
	}
}//End Sources Selection

{//Backend Selection
	var backend = '';
	switch (platform) {
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
	let def = 'WITH_'+backend.toUpperCase();
	p.addDefine(def);
	console.log('#define ' + def);
}//End Backend Selection 

{//Filters Selection
	let filters = [
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
		let def = 'SOUND_FILTER_'+ filters[i].toUpperCase();
		p.addDefine(def);
		console.log('#define ' + def);
	}
}//End Filters Selection

resolve(p);