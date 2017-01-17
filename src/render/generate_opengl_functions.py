with open('glcorearb.h', 'r') as file:
	for line in file:
		if line.find('APIENTRY gl') != -1:
			start = line.index('APIENTRY gl') + 9
			end = line.index(' ', start)
			function = line[start:end]
			function_prototype = 'PFN' + function.upper() + 'PROC'
			#print('extern ' + function_prototype + ' ' + function + ';')
			# print(function_prototype + ' ' + function + ';')
			print(function + ' = (' + function_prototype + ')SDL_GL_GetProcAddress("' + function + '");')