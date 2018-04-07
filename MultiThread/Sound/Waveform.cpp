#include "Waveform.h"

//Waveform Constructor
Waveform::Waveform()
{
	mlpWaveform = NULL;
	mucWaveformData = NULL;
	mdwWaveformLength = NULL;
}

//Waveform Destructor
Waveform::~Waveform()
{
	CleanWaveform();
}

//Releases memory
void Waveform::CleanWaveform()
{
	if(mucWaveformData)
	{
		delete mlpWaveform;
		mlpWaveform = NULL;
		delete mucWaveformData;
		mucWaveformData = NULL;
		mdwWaveformLength = NULL;
	}
}

//Loads specified .wav file
void Waveform::LoadWaveform(char *cWavfile)
{
	//Removes obselete data
	CleanWaveform();

	//Stores current file position
	char cWavDataBuffer[5];
	ZeroMemory(cWavDataBuffer, sizeof(cWavDataBuffer));

	//////////////////////////
	//read length
	DWORD dwNumRead;

	//length variable
	DWORD dwLength;

	//data buffer
	unsigned char* ucTemp;

	//Opens the .wav file and determines size
	HANDLE hWavfileStream = CreateFile(cWavfile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//Reads first 4 bytes, should contain "RIFF"
	ReadFile(hWavfileStream, cWavDataBuffer, 4, &dwNumRead, NULL);
	//Reads next 4 bytes, contains file size minus 8 bytes for file header
	ReadFile(hWavfileStream, &dwLength, sizeof(dwLength), &dwNumRead,NULL);
	//Reads next 4 bytes, should contain "WAVE"
	ReadFile(hWavfileStream, cWavDataBuffer, 4, &dwNumRead, NULL);


	//Reads next 4 bytes, should contain "fmt "
	ReadFile(hWavfileStream, cWavDataBuffer, 4, &dwNumRead, NULL);
	//Reads next 4 bytes, contains the size of the .wav file format data block
	ReadFile(hWavfileStream, &dwLength, sizeof(dwLength), &dwNumRead, NULL);

	///////////////////////////
	ucTemp = new unsigned char[dwLength];

	ReadFile(hWavfileStream, ucTemp, dwLength, &dwNumRead, NULL);


	if(strcmp("fmt ", cWavDataBuffer) == 0)
	{
		//Stores .wav file format information
		mlpWaveform = new WAVEFORMATEX;
		ZeroMemory(mlpWaveform, sizeof(WAVEFORMATEX));
		memcpy(mlpWaveform, ucTemp, dwLength);
	}

	//delete ucTemp;

	//Reads next 4 bytes, should contain "data"
	ReadFile(hWavfileStream, cWavDataBuffer, 4, &dwNumRead, NULL);
	//Reads next 4 bytes, contains size of .wav file raw sound data block
	ReadFile(hWavfileStream, &dwLength, sizeof(dwLength), &dwNumRead, NULL);

	ucTemp = new unsigned char[dwLength];

	ReadFile(hWavfileStream, ucTemp, dwLength, &dwNumRead, NULL);

	if(strcmp("data", cWavDataBuffer) == 0)
	{
		//data chunk

		//allocate data buffer
		mucWaveformData = new UCHAR[dwLength];

		//copy length
		mdwWaveformLength = dwLength;
		
		//copy buffer
		memcpy(mucWaveformData, ucTemp, mdwWaveformLength);
	}

	delete ucTemp;

	//Closes .wav file
	CloseHandle(hWavfileStream);
}

//Returns length of wav file
DWORD Waveform::GetWaveformLength()
{
	return mdwWaveformLength;
}

//Returns the data contained in wav file
unsigned char* Waveform::GetWaveformData()
{
	return mucWaveformData;
}

//Returns the Waveform
LPWAVEFORMATEX Waveform::GetWaveform()
{
	return mlpWaveform;
}