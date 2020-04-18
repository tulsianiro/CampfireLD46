#pragma comment(lib,"ole32.lib")
#pragma comment(lib, "xaudio2.lib")

#ifdef _XBOX //Big-Endian If on XBOX
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian if on Windows
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

enum Audio
{
    TEST_AUDIO,
    LAST_AUDIO
};

struct win32_audio
{
    XAUDIO2_BUFFER Buffer;
    IXAudio2SourceVoice *SourceVoice;
};

global win32_audio audio_cache[LAST_AUDIO];

struct win32_xaudio
{
    IXAudio2 *XAudio;
    IXAudio2MasteringVoice *MasterVoice;
};

void InitSound(char *FileName, u32 audio_cache_id, bool Loop);

internal win32_xaudio StaticXAudio;
void audio_init()
{
    bool XAudioInitFailure = FAILED(XAudio2Create(&StaticXAudio.XAudio, 0, XAUDIO2_DEFAULT_PROCESSOR));
    if(XAudioInitFailure) { /* TODO(Rohan): Logging */ }
    XAudioInitFailure = FAILED(StaticXAudio.XAudio->CreateMasteringVoice(&StaticXAudio.MasterVoice));
    if (XAudioInitFailure) {/* TODO(Rohan): Logging */ }

    InitSound("tune2.wav", TEST_AUDIO, true);
}

internal HRESULT
FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
    HRESULT hr = S_OK;
    if(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD bytesRead = 0;
    DWORD dwOffset = 0;

    while (hr == S_OK)
    {
        DWORD dwRead;
        if( 0 == ReadFile( hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL ) )
            hr = HRESULT_FROM_WIN32( GetLastError() );

        if( 0 == ReadFile( hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL ) )
            hr = HRESULT_FROM_WIN32( GetLastError() );

        switch (dwChunkType)
        {
            case fourccRIFF:
                dwRIFFDataSize = dwChunkDataSize;
                dwChunkDataSize = 4;
                if( 0 == ReadFile( hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL ) )
                    hr = HRESULT_FROM_WIN32( GetLastError() );
                break;

            default:
                if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, dwChunkDataSize, NULL, FILE_CURRENT ) )
                    return HRESULT_FROM_WIN32( GetLastError() );            
        }

        dwOffset += sizeof(DWORD) * 2;

        if (dwChunkType == fourcc)
        {
            dwChunkSize = dwChunkDataSize;
            dwChunkDataPosition = dwOffset;
            return S_OK;
        }

        dwOffset += dwChunkDataSize;

        if (bytesRead >= dwRIFFDataSize) return S_FALSE;

    }

    return S_OK;
}

internal HRESULT
ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
    HRESULT hr = S_OK;
    if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, bufferoffset, NULL, FILE_BEGIN ) )
        return HRESULT_FROM_WIN32( GetLastError() );
    DWORD dwRead;
    if( 0 == ReadFile( hFile, buffer, buffersize, &dwRead, NULL ) )
        hr = HRESULT_FROM_WIN32( GetLastError() );
    return hr;
}

internal void
InitSound(char *FileName, u32 audio_cache_id, bool Loop)
{
    WAVEFORMATEXTENSIBLE wfx = {0};
    XAUDIO2_BUFFER buffer = {0};

    char sound_path[128];
	sprintf_s(sound_path, "../res/audio/%s", FileName);
    
    HANDLE hFile = CreateFileA(sound_path,
                              GENERIC_READ,
                              FILE_SHARE_READ,
                              NULL,
                              OPEN_EXISTING,
                              0,
                              NULL );
	if (INVALID_HANDLE_VALUE == hFile)
	{
		// TODO(Rohan): Logging
	}

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
	{
		// TODO(Rohan): Logging
	}

    DWORD dwChunkSize;
    DWORD dwChunkPosition;

    // check the file type, should be fourccWAVE or 'XWMA'
    FindChunk(hFile,fourccRIFF,dwChunkSize, dwChunkPosition );
    DWORD filetype;
    ReadChunkData(hFile,&filetype,sizeof(DWORD),dwChunkPosition);
	if (filetype != fourccWAVE)
	{
		// TODO(Rohan): Logging
	}

    FindChunk(hFile,fourccFMT, dwChunkSize, dwChunkPosition );
    ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition );

    //fill out the audio data buffer with the contents of the fourccDATA chunk
    FindChunk(hFile,fourccDATA,dwChunkSize, dwChunkPosition );
    BYTE * pDataBuffer = (BYTE *)malloc(sizeof(BYTE) * dwChunkSize);
    ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

    buffer.AudioBytes = dwChunkSize;  //buffer containing audio data
    buffer.pAudioData = pDataBuffer;  //size of the audio buffer in bytes
    buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

    if(Loop)
    {
        buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    }

    win32_audio audio;
    IXAudio2SourceVoice* source_voice;
    StaticXAudio.XAudio->CreateSourceVoice(&source_voice, (WAVEFORMATEX*)&wfx);
    audio.SourceVoice = source_voice;
    audio.Buffer = buffer;
    audio_cache[audio_cache_id] = audio;
}
    
void play_sound(u32 audio_cache_id)
{
    win32_audio audio = audio_cache[audio_cache_id];
    IXAudio2SourceVoice *source_voice = audio.SourceVoice;
    source_voice->SubmitSourceBuffer(&audio.Buffer);
    source_voice->Start( 0 );
}

void stop_sound(u32 audio_cache_id)
{
    win32_audio audio = audio_cache[audio_cache_id];
    IXAudio2SourceVoice *source_voice = audio.SourceVoice;
    source_voice->Stop(0);
}


