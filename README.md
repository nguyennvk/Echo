# Echo
This program will add echo to a wav file. Keep in mind that the wav file has to be mono.
The program can custom the delay and the volume of the echo.
#NAME
   	addecho -  add echo effect to mono audio file

#SYNOPSIS
   	addecho [OPTION]...[FILENAME]...[FILENAME]

#DESCRIPTION
	Using an input audio file, addecho generates a new file as output with additional echo. The delay option dictates the timing of this echo. The volume of 	the echo is also open for modification –this is done through the volume option. Wav files are stored as a short sequence representing the air pressure. Wav 	files can have multiple sampling rates: 11025 (voice quality), 22050, and 44100 (CD quality) with different modes mono and stereo. To make sure the program 	works correctly, the input file should be mono.

	-d,  —delay=SAMPLE
	Default to 8000, number of samples delayed until echo is played. To determine the delay time divide delay by the sample rate. 
 	If SAMPLE exceeds the number of samples in the input file, the excess space will be filled with 0.
	-v, –volume=VOLUME
 	Default to 4, after “delay” number samples, the program will add the current byte with the byte that is at delay samples
  	before the current byte. Before adding to the current byte, the before byte is scaled down by VOLUME. 

	infile.wav - Name of file to be used as a source. Note that the infile.wav cannot have the same name as outfile.wav

	Outfile.wav - name of the file with new echo. Its size will be greater than infile.wav.

#EXAMPLES
	The following example creates a file with the default delay and no volume increase.
	./addecho <infile.wav> <outfile.wav>

	Another example uses a 12000-sample delay and a volume decrease by a factor of 0.5. 
	./addecho -d 12000 -v 2 <infile.wav> <outfile.wav>

#RETURN VALUES
	The addecho function returns the following values:
	0 =  The program has completed, and output or overwrote a new .wav file
	1 = An error has occurred somewhere during runtime. An error message will appear outlining its nature.

#WARNINGS
	Adding an option -d or -v without the corresponding value will not use the default values.
	<infile.wav> and <outfile.wav> are required and are different.
	Do not substitute the infile or outfile for other audio filetypes.

#BUGS
	The program only works correctly for mono wav files with the header length of 44 bytes. The input file is required to have consistent header and date i.e, 	the size in the header has to reflect the actual size of the data. Reporting bugs to <khoinguyen.nguyen@mail.utoronto.ca> or <carson.lee@mail.utoronto.ca>
